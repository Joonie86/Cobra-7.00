#include <sdk_version.h>
#include <cellstatus.h>
#include <cell/cell_fs.h>

#include <sys/prx.h>
#include <sys/ppu_thread.h>
#include <sys/event.h>
#include <sys/syscall.h>
#include <sys/memory.h>


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "common.h"
#include "storage.h"
#include "syscall8.h"
#include "scsi.h"

SYS_MODULE_INFO(RAWSECISO, 0, 1, 0);
SYS_MODULE_START(rawseciso_start);
SYS_MODULE_STOP(rawseciso_stop);

#ifdef DEBUG
#define THREAD_NAME "rawseciso_thread"
#define STOP_THREAD_NAME "rawseciso_stop_thread"
#else
#define THREAD_NAME ""
#define STOP_THREAD_NAME ""
#endif

#define MIN(a, b)	((a) <= (b) ? (a) : (b))
#define ABS(a)		(((a) < 0) ? -(a) : (a))

#define CD_CACHE_SIZE			(64)

enum EMU_TYPE
{
	EMU_OFF = 0,
	EMU_PS3,
	EMU_PS2_DVD,
	EMU_PS2_CD,
	EMU_PSX,
	EMU_BD,
	EMU_DVD,
	EMU_MAX,
};

enum STORAGE_COMMAND
{
	CMD_READ_ISO,
	CMD_READ_DISC,	
	CMD_READ_CD_ISO_2352,
	CMD_FAKE_STORAGE_EVENT,
	CMD_GET_PSX_VIDEO_MODE
};

typedef struct
{
	uint64_t device;
        uint32_t emu_mode;
        uint32_t num_sections;
        uint32_t num_tracks;
        // sections after
        // sizes after
        // tracks after
} __attribute__((packed)) rawseciso_args;

static sys_ppu_thread_t thread_id = -1;
static int running = 1;

sys_device_handle_t handle = -1;
static sys_event_queue_t command_queue = -1;

static uint32_t *sections, *sections_size;
static uint32_t num_sections;

static uint64_t discsize;
static int is_cd2352;
static uint8_t *cd_cache;
static uint32_t cached_cd_sector=0x80000000;

int rawseciso_start(uint64_t arg);
int rawseciso_stop(void);

static inline void _sys_ppu_thread_exit(uint64_t val)
{
	system_call_1(41, val);
}

static inline sys_prx_id_t prx_get_module_id_by_address(void *addr)
{
	system_call_1(461, (uint64_t)(uint32_t)addr);
	return (int)p1;
}

static int fake_eject_event(void)
{
	sys_storage_ext_fake_storage_event(4, 0, BDVD_DRIVE);
	return sys_storage_ext_fake_storage_event(8, 0, BDVD_DRIVE);	
}

static int fake_insert_event(uint64_t disctype)
{
	uint64_t param = (uint64_t)(disctype) << 32ULL;	
	sys_storage_ext_fake_storage_event(7, 0, BDVD_DRIVE);
	return sys_storage_ext_fake_storage_event(3, param, BDVD_DRIVE);
}

static inline void get_next_read(uint64_t discoffset, uint64_t bufsize, uint64_t *offset, uint64_t *readsize, int *idx)
{
	uint64_t base = 0;
	*idx = -1;
	*readsize = bufsize;
	*offset = 0;
        
	for (uint32_t i = 0; i < num_sections; i++)
	{
		uint64_t last = base + ((uint64_t)sections_size[i] * 512);
                
		if (discoffset >= base && discoffset < last)
		{
			uint64_t maxfileread = last-discoffset;
                        
			if (bufsize > maxfileread)
				*readsize = maxfileread;
			else
				*readsize = bufsize;
                        
			*idx = i;
			*offset = discoffset-base; 
			return;
		}
                
		base += ((uint64_t)sections_size[i] * 512);
	}
        
	// We can be here on video blu-ray
	DPRINTF("Offset or size out of range  %lx%08lx   %lx!!!!!!!!\n", discoffset>>32, discoffset, bufsize);
}

static int process_read_iso_cmd(uint8_t *buf, uint64_t offset, uint64_t size)
{
	uint64_t remaining;
	
	//DPRINTF("read iso: %p %lx %lx\n", buf, offset, size);	
	remaining = size;
        
	while (remaining > 0)
	{
		uint64_t pos, readsize;
		int idx;
		int ret;
		uint8_t tmp[512];
		uint32_t sector;   
		uint32_t r;
                
		get_next_read(offset, remaining, &pos, &readsize, &idx);
                
		if (idx == -1 || sections[idx] == 0xFFFFFFFF)
		{
			memset(buf, 0, readsize);
			buf += readsize;
			offset += readsize;
			remaining -= readsize;
			continue;
		}
                
		if (pos & 511)
		{
			uint64_t csize;                        
                        
			sector = sections[idx] + pos/512;                        
			ret = sys_storage_read(handle, 0, sector, 1, tmp, &r, 0);
			if (ret != 0 || r != 1)
			{
				DPRINTF("sys_storage_read failed: %x 1 -> %x\n", sector, ret);
				return -1;
			}
                        
			csize = 512-(pos&511);
                        
			if (csize > readsize)
				csize = readsize;
                        
			memcpy(buf, tmp+(pos&511), csize);
			buf += csize;
			offset += csize;
			pos += csize;
			remaining -= csize;
			readsize -= csize;
		}
                
		if (readsize > 0)
		{
			uint32_t n = readsize / 512;
			
			if (n > 0)
			{
				uint64_t s;
				
				sector = sections[idx] + pos/512;				
				ret = sys_storage_read(handle, 0, sector, n, buf, &r, 0);
				if (ret != 0 || r != n)
				{
					DPRINTF("sys_storage_read failed: %x %x -> %x\n", sector, n, ret);
					return -1;
				}
				
				s = n * 512;
				buf += s;
				offset += s;
				pos += s;
				remaining -= s;
				readsize -= s;			
			}
			
			if (readsize > 0)
			{
				sector = sections[idx] + pos/512;
				ret = sys_storage_read(handle, 0, sector, 1, tmp, &r, 0);
				if (ret != 0 || r != 1)
				{
					DPRINTF("sys_storage_read failed: %x 1 -> %x\n", sector, ret);
					return -1;
				}
				
				memcpy(buf, tmp, readsize);
				buf += readsize;
				offset += readsize;
				remaining -= readsize;
			}
		}
	}
        
	return 0;
}

static inline void my_memcpy(uint8_t *dst, uint8_t *src, int size)
{
	for (int i = 0; i < size; i++)
		dst[i] = src[i];
}

static int process_read_cd_2048_cmd(uint8_t *buf, uint32_t start_sector, uint32_t sector_count)
{
	uint32_t capacity = sector_count*2048;
	uint32_t fit = capacity/2352;
	uint32_t rem = (sector_count-fit);
	uint32_t i;
	uint8_t *in = buf;
	uint8_t *out = buf;
	
	if (fit > 0)
	{
		process_read_iso_cmd(buf, start_sector*2352, fit*2352);
			
		for (i = 0; i < fit; i++)
		{
			my_memcpy(out, in+24, 2048);
			in += 2352;
			out += 2048;
			start_sector++;
		}
	}
		
	for (i = 0; i < rem; i++)
	{
		process_read_iso_cmd(out, (start_sector*2352)+24, 2048);
		out += 2048;
		start_sector++;
	}
	
	return 0;
}

static int process_read_cd_2352_cmd(uint8_t *buf, uint32_t sector, uint32_t remaining)
{
	int cache = 0;
	
	if (remaining <= CD_CACHE_SIZE)
	{
		int dif = (int)cached_cd_sector-sector;
		
		if (ABS(dif) < CD_CACHE_SIZE)
		{
			uint8_t *copy_ptr = NULL;
			uint32_t copy_offset = 0;
			uint32_t copy_size = 0;	
						
			if (dif > 0)
			{
				if (dif < (int)remaining)
				{
					copy_ptr = cd_cache;
					copy_offset = dif;
					copy_size = remaining-dif;						
				}
			}
			else
			{
							
				copy_ptr = cd_cache+((-dif)*2352);
				copy_size = MIN((int)remaining, CD_CACHE_SIZE+dif);				
			}
			
			if (copy_ptr)
			{
				memcpy(buf+(copy_offset*2352), copy_ptr, copy_size*2352);
								
				if (remaining == copy_size)
				{
					return 0;
				}
				
				remaining -= copy_size;
				
				if (dif <= 0)
				{
					uint32_t newsector = cached_cd_sector + CD_CACHE_SIZE;	
					buf += ((newsector-sector)*2352);
					sector = newsector;
				}
			}
		}
		
		cache = 1;		
	}
	
	if (!cache)
	{
		return process_read_iso_cmd(buf, sector*2352, remaining*2352);
	}
	
	if (!cd_cache)
	{
		sys_addr_t addr;
				
		int ret = sys_memory_allocate(192*1024, SYS_MEMORY_PAGE_SIZE_64K, &addr);
		if (ret != 0)
		{
			DPRINTF("sys_memory_allocate failed: %x\n", ret);
			return ret;
		}
		
		cd_cache = (uint8_t *)addr;		
	}
	
	if (process_read_iso_cmd(cd_cache, sector*2352, CD_CACHE_SIZE*2352) != 0)
		return -1;
	
	memcpy(buf, cd_cache, remaining*2352);	
	cached_cd_sector = sector;
	return 0;
}

static void rawseciso_thread(uint64_t arg)
{
	rawseciso_args *args;
	sys_event_port_t result_port;
	sys_event_queue_attribute_t queue_attr;
	unsigned int real_disctype;
	int ret;
	ScsiTrackDescriptor *tracks;
	int emu_mode, num_tracks;
	
	args = (rawseciso_args *)(uint32_t)arg;
		
	DPRINTF("Hello VSH\n");
	
	num_sections = args->num_sections;
	sections = (uint32_t *)(args+1);
	sections_size = sections + num_sections;
               
	discsize = 0;
        
	for (uint32_t i = 0; i < num_sections; i++)
	{
		discsize += sections_size[i];
	}
	
	discsize = discsize * 512;
	
	emu_mode = args->emu_mode;      
	if (emu_mode == EMU_PSX)
        {
		num_tracks = args->num_tracks;
		tracks = (ScsiTrackDescriptor *)(sections_size + num_sections);
		is_cd2352 = 1;
	}
	else
        {
		num_tracks = 0;
		tracks = NULL;
	}
        
	if (is_cd2352)
	{
		if (discsize % 2352)
		{
			discsize = discsize - (discsize % 2352);			
		}
	}
        
	DPRINTF("discsize = %lx%08lx\n", discsize>>32, discsize);
        
	ret = sys_storage_open(args->device, 0, &handle, 0);
	if (ret != 0)
	{
		DPRINTF("sys_storage_open failed: %x\n", ret);
		sys_memory_free((sys_addr_t)args);
		sys_ppu_thread_exit(ret);
	}
	
	ret = sys_event_port_create(&result_port, 1, SYS_EVENT_PORT_NO_NAME);
	if (ret != 0)
	{
		DPRINTF("sys_event_port_create failed: %x\n", ret);
                sys_storage_close(handle);
		sys_memory_free((sys_addr_t)args);
		sys_ppu_thread_exit(ret);
	}
	
	sys_event_queue_attribute_initialize(queue_attr);
	ret = sys_event_queue_create(&command_queue, &queue_attr, 0, 1);
	if (ret != 0)
	{
		DPRINTF("sys_event_queue_create failed: %x\n", ret);
		sys_event_port_destroy(result_port);
		sys_storage_close(handle);
		sys_memory_free((sys_addr_t)args);
		sys_ppu_thread_exit(ret);
	}
	
	sys_storage_ext_get_disc_type(&real_disctype, NULL, NULL);
	
	if (real_disctype != 0)
	{
		fake_eject_event();
	}
	
	ret = sys_storage_ext_mount_discfile_proxy(result_port, command_queue, emu_mode, discsize, 256*1024, num_tracks, tracks);
	DPRINTF("mount = %x\n", ret);
	
	fake_insert_event(real_disctype);
	
	if (ret != 0)
	{
		sys_event_port_disconnect(result_port);
		// Queue destroyed in stop thread sys_event_queue_destroy(command_queue);
		sys_event_port_destroy(result_port);
		sys_storage_close(handle);
		sys_memory_free((sys_addr_t)args);
		sys_ppu_thread_exit(0);
	}
	
	while (1)
	{
		sys_event_t event;
				
		ret = sys_event_queue_receive(command_queue, &event, 0);	
		if (ret != 0)
		{
			//DPRINTF("sys_event_queue_receive failed: %x\n", ret);
			break;
		}
		
		if (!running)
                        break;
		
		void *buf = (void *)(uint32_t)(event.data3>>32ULL);
		uint64_t offset = event.data2;
		uint32_t size = event.data3&0xFFFFFFFF;
		
		switch (event.data1)
		{
			case CMD_READ_ISO:
			{
				if (is_cd2352)
				{
					ret = process_read_cd_2048_cmd(buf, offset/2048, size/2048);
				}
				else
				{				
					ret = process_read_iso_cmd(buf, offset, size);
				}
			}
			break;
			
			case CMD_READ_CD_ISO_2352:
			{
				ret = process_read_cd_2352_cmd(buf, offset/2352, size/2352);
			}
			break;
		}
		
		ret = sys_event_port_send(result_port, ret, 0, 0);
		if (ret != 0)
		{
			DPRINTF("sys_event_port_send failed: %x\n", ret);
			break;
		}
	}
	
	sys_memory_free((sys_addr_t)args);
	
	sys_storage_ext_get_disc_type(&real_disctype, NULL, NULL);
	fake_eject_event();	
	sys_storage_ext_umount_discfile();	
	
	if (real_disctype != 0)
	{
		fake_insert_event(real_disctype);
	}
	
	if (cd_cache)
	{
		sys_memory_free((sys_addr_t)cd_cache);
	}
	
	sys_storage_close(handle);
	
	sys_event_port_disconnect(result_port);
	if (sys_event_port_destroy(result_port) != 0)
	{
		DPRINTF("Error destroyng result_port\n");
	}
	
	// queue destroyed in stop thread
	
	DPRINTF("Exiting main thread!\n");	
	sys_ppu_thread_exit(0);
}

int rawseciso_start(uint64_t arg)
{
	if (arg != 0)
	{
		void *argp = (void *)(uint32_t)arg;
		sys_addr_t addr;
		
		if (sys_memory_allocate(64*1024, SYS_MEMORY_PAGE_SIZE_64K, &addr) == 0)
		{
			memcpy((void *)addr, argp, 64*1024);
			sys_ppu_thread_create(&thread_id, rawseciso_thread, (uint64_t)addr, -0x1d8, 0x2000, SYS_PPU_THREAD_CREATE_JOINABLE, THREAD_NAME);
		}
	}	
	// Exit thread using directly the syscall and not the user mode library or we will crash
	_sys_ppu_thread_exit(0);	
	return SYS_PRX_RESIDENT;
}

static void rawseciso_stop_thread(uint64_t arg)
{
	uint64_t exit_code;
	
	DPRINTF("rawseciso_stop_thread\n");
	
	running = 0;
	
	if (command_queue != (sys_event_queue_t)-1)
	{
		if (sys_event_queue_destroy(command_queue, SYS_EVENT_QUEUE_DESTROY_FORCE) != 0)
		{
			DPRINTF("Failed in destroying command_queue\n");
		}
	}
	
	if (thread_id != (sys_ppu_thread_t)-1)
	{
		sys_ppu_thread_join(thread_id, &exit_code);
	}
	
	DPRINTF("Exiting stop thread!\n");
	sys_ppu_thread_exit(0);
}

static void finalize_module(void)
{
	uint64_t meminfo[5];
	
	sys_prx_id_t prx = prx_get_module_id_by_address(finalize_module);
	
	meminfo[0] = 0x28;
	meminfo[1] = 2;
	meminfo[3] = 0;
	
	system_call_3(482, prx, 0, (uint64_t)(uint32_t)meminfo);		
}

int rawseciso_stop(void)
{
	sys_ppu_thread_t t;
	uint64_t exit_code;
	
	sys_ppu_thread_create(&t, rawseciso_stop_thread, 0, 0, 0x2000, SYS_PPU_THREAD_CREATE_JOINABLE, STOP_THREAD_NAME);
	sys_ppu_thread_join(t, &exit_code);	
	
	finalize_module();
	_sys_ppu_thread_exit(0);
	return SYS_PRX_STOP_OK;
}
