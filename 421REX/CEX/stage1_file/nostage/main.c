#include <stddef.h>

#include <lv2/lv2.h>
#include <debug.h>
#include <lv2/memory.h>
#include <lv2/io.h>
#include <lv2/libc.h>
#include <lv2/thread.h>
#include <lv2/patch.h>

#include <lv1/lv1.h>


#ifdef DEBUG
#define DPRINTF		_debug_printf
#define DPRINT_HEX	debug_print_hex
#else
#define DPRINTF(...)
#define DPRINT_HEX(a, b)
#endif

#define STAGE2_FILE_DEX	"/dev_flash/sys/stage2.dex"
#define STAGE2_FILE_CEX	"/dev_flash/sys/stage2.cex"
#define VSH_SIZE_DEX		0x2FF850
#define VSH_SIZE_CEX		0x303F08

void flash_mount_clone(void);

int main(void)
{
	void *stage2 = NULL;
	
	f_desc_t f;
	int (* func)(void);	
	int ret;
	
#ifdef DEBUG		
	debug_init();
	DPRINTF("Stage1 hello.\n");	
#endif
	f.addr = flash_mount_clone;
	f.toc = (void *)MKA(TOC);
	func = (void *)&f;
	
	ret = func();
	
	if (ret != 0 && ret != 1)
	{
		DPRINTF("Flash mount failed!\n");		
	}
	else
	{
		
		DPRINTF("Flash mounted\n");
		
		CellFsStat stat;
		
		cellFsStat("/dev_flash/vsh/module/vsh.self", &stat); //get active vsh's size

		if (stat.st_size == VSH_SIZE_DEX) // vsh.self/vsh.self.swp size = debug xmb
		{
				
			CellFsStat stat;
			
			if (cellFsStat(STAGE2_FILE_DEX, &stat) == 0)
			{
				int fd;
				
				if (cellFsOpen(STAGE2_FILE_DEX, CELL_FS_O_RDONLY, &fd, 0, NULL, 0) == 0)
				{
					uint32_t psize = stat.st_size;
					
					DPRINTF("Payload size = %d\n", psize);
					
					stage2 = alloc(psize, 0x27);
					if (stage2)
					{
						uint64_t rs;
						
						if (cellFsRead(fd, stage2, psize, &rs) != 0)
						{
							DPRINTF("DEX stage2 read fail.\n");
							dealloc(stage2, 0x27);
							stage2 = NULL;
						}
					}
					else
					{
						DPRINTF("Cannot allocate DEX stage2\n");
					}
					
					cellFsClose(fd);
				}
			}
			else
			{
				DPRINTF("There is no DEX stage2, booting system.\n");
			}
		}
		else if (stat.st_size == VSH_SIZE_CEX) // vsh.self.cexsp size = retail xmb if active
		{
				
			CellFsStat stat;
			
			if (cellFsStat(STAGE2_FILE_CEX, &stat) == 0)
			{
				int fd;
				
				if (cellFsOpen(STAGE2_FILE_CEX, CELL_FS_O_RDONLY, &fd, 0, NULL, 0) == 0)
				{
					uint32_t psize = stat.st_size;
					
					DPRINTF("Payload size = %d\n", psize);
					
					stage2 = alloc(psize, 0x27);
					if (stage2)
					{
						uint64_t rs;
						
						if (cellFsRead(fd, stage2, psize, &rs) != 0)
						{
							DPRINTF("CEX stage2 read fail.\n");
							dealloc(stage2, 0x27);
							stage2 = NULL;
						}
					}
					else
					{
						DPRINTF("Cannot allocate CEX stage2\n");
					}
					
					cellFsClose(fd);
				}
			}
			else
			{
				DPRINTF("There is no CEX stage2, booting system.\n");
			}
		}
		else if (stat.st_size == VSH_SIZE_CEX) // vsh.self.cexsp size = retail xmb if active
		{
			else
			{
				DPRINTF("There is no CEX stage2, booting system.\n");
			}
		}
	}
	
	if (stage2)
	{
		f.addr = stage2;			
		func = (void *)&f;	
		DPRINTF("Calling stage2...\n");
		func();
	}
	
	return ret;
}
