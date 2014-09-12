#include <stdio.h>
#include <string.h>
#include "ntfs.h"
#include "cobra.h"
#include "scsi.h"

#define USB_MASS_STORAGE_1(n)	(0x10300000000000AULL+(n)) /* For 0-5 */
#define USB_MASS_STORAGE_2(n)	(0x10300000000001FULL+((n)-6)) /* For 6-127 */
#define USB_MASS_STORAGE(n)	(((n) < 6) ? USB_MASS_STORAGE_1(n) : USB_MASS_STORAGE_2(n))

#define MAX_SECTIONS	((0x10000-sizeof(rawseciso_args))/8)

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


uint8_t plugin_args[0x10000];
uint8_t cue_buf[0x10000];

char path[0x420];
char sprx[0x420];

uint32_t sections[MAX_SECTIONS], sections_size[MAX_SECTIONS];
ntfs_md *mounts;
int mountCount;

void exit1(void)
{
	cobra_lib_finalize();
}

void exit2(void)
{
	for (int i = 0; i < mountCount; i++)
	{
		printf("Unmounting %s\n", mounts[i].name);
		ntfsUnmount(mounts[i].name, 1);
	}
}

int main(int argc, const char* argv[])
{
        int i, j, parts, max;
	unsigned int num_tracks;
	char *lt, *sprx_rel;
	char *iso, *cue, *emu_mode_str;
	int emu_mode;
	
	TrackDef tracks[100];
	ScsiTrackDescriptor *scsi_tracks;
	rawseciso_args *p_args;
		
	if (cobra_lib_init() != 0)
	{
		printf("cobra_lib_init failed (not in Cobra?)\n");
		return 0;
	}
	
	atexit(exit1);
	
	for (i = 0; i < argc; i++)
        {
                printf("argv[%d]=%s\n", i, argv[i]);
        }
	
	if (argc < 3)
	{
		printf("Invalid usage\n");
		printf("Usage: %s iso_file emu_mode [cue_file]\n", argc > 0 ? argv[0] : "SELF_PATH");
		printf("emu_mode ->  EMU_PS3, EMU_DVD, EMU_BD or EMU_PSX\n");
		printf("iso_file is relative path in NTFS volume, e.g. \"/PS3ISO/tequen6.iso\"\n");
		printf("cue_file also relative path and must be in same NTFS volume, e.g. \"/PSXISO/Last Fantasy VII.cue\"\n");
		return 0;
	}
        
        iso = (char *)argv[1];
	emu_mode_str = (char *)argv[2];
	cue = (argc >= 4) ? (char *)argv[3] : NULL;
	
	if (strcmp(emu_mode_str, "EMU_PS3") == 0)
	{
		emu_mode = EMU_PS3;
	}
	else if (strcmp(emu_mode_str, "EMU_DVD") == 0)
	{
		emu_mode = EMU_DVD;
	}
	else if (strcmp(emu_mode_str, "EMU_BD") == 0)
	{
		emu_mode = EMU_BD;
	}
	else if (strcmp(emu_mode_str, "EMU_PSX") == 0)
	{
		emu_mode = EMU_PSX;
	}
	else
	{
		printf("Invalid emu_mode!\n");
		return 0;
	}
        
        sprx_rel = "/rawseciso.sprx";
        strncpy(sprx, argv[0], sizeof(sprx)-strlen(sprx_rel)-1);
	sprx[sizeof(sprx)-strlen(sprx_rel)-1] = 0;
	
	lt = strrchr(sprx, '/');
	if (!lt)
	{
		printf("Invalid argv[0]\n");
		return 0;
	}
	
	*lt = 0;
	strcat(sprx, sprx_rel); 
        
        mountCount = ntfsMountAll(&mounts, NTFS_DEFAULT | NTFS_RECOVER | NTFS_READ_ONLY);
        if (mountCount <= 0)
        {
                printf("No NTFS/EXT volumes found or error (%d)\n", mountCount);
		return 0;
        }
       
        printf("Found %d volumes\n", mountCount);
	atexit(exit2);
    
        for (i = 0; i < mountCount; i++)
        {
                printf("Name %d %s, startSector: %08X, USB port: %c\n", i, mounts[i].name, mounts[i].startSector, (char)mounts[i].interface->ioType & 0xff);
		
		if (strncmp(mounts[i].name, "ntfs", 4) == 0)
		{
			snprintf(path, sizeof(path), "%s:%s", mounts[i].name, iso);
			printf("Testing file: %s\n", path);
			parts = ps3ntfs_file_to_sectors(path, sections, sections_size, MAX_SECTIONS, 1);
			
			if (parts == MAX_SECTIONS)
			{
				printf("File found but limit of parts reached\n");
				return 0;
			}
			else if (parts > 0)
			{
				printf("Number of parts = %d\n", parts);
				 
				uint64_t total_size = 0;
    
				for (j = 0; j < parts; j++)
				{
					printf("%08X - %08X\n", sections[j], sections_size[j]);
					total_size += sections_size[j];
				}
    
				// Note that total_size is not the size of the file, but the size of the file aligned to the next 512
				// In the case of 2048 isos, since they are supossed to be a multiple of 2048, they are also multiple of 512,
				// what makes total_size = size of file
				// Even if the file were corrupt and wasn't 512 bytes aligned, it is not much of a problem, rawseciso is prepared
				// to zero data beyond disc size
				// For 2352 isos, rawseciso is prepared to handle that and calculate the correct size of the file.
				printf("total_size = 0x%lx sectors (%ld bytes)\n", total_size, total_size*512);
				
				num_tracks = 1;
				
				if (emu_mode == EMU_PSX && cue)
				{
					int fd;
					
					snprintf(path, sizeof(path), "%s:%s", mounts[i].name, cue);
					
					fd = ps3ntfs_open(path, O_RDONLY, 0);
					if (fd >= 0)
					{
						int r = ps3ntfs_read(fd, (char *)cue_buf, sizeof(cue_buf));
						ps3ntfs_close(fd);
						
						if (r > 0)
						{
							char dummy[64];
							
							if (cobra_parse_cue(cue_buf, r, tracks, 100, &num_tracks, dummy, sizeof(dummy)-1) != 0)
							{
								num_tracks = 1;
								cue = NULL;
							}
						}
						else
						{
							cue = NULL;
						}
					}
					else
					{
						cue = NULL;
					}
				}
				
				p_args = (rawseciso_args *)plugin_args;
				p_args->device = USB_MASS_STORAGE((mounts[i].interface->ioType & 0xff) - '0');
				p_args->emu_mode = emu_mode;
				p_args->num_sections = parts;
		
				memcpy(plugin_args+sizeof(rawseciso_args), sections, parts*sizeof(uint32_t));
				memcpy(plugin_args+sizeof(rawseciso_args)+(parts*sizeof(uint32_t)), sections_size, parts*sizeof(uint32_t));
				
				if (emu_mode == EMU_PSX)
				{
					max = MAX_SECTIONS - ((num_tracks*sizeof(ScsiTrackDescriptor)) / 8);
					
					if (parts == max)
					{
						printf("Max parts for psx reached, sorry\n");
						return 0;
					}
					
					p_args->num_tracks = num_tracks;
					scsi_tracks = (ScsiTrackDescriptor *)(plugin_args+sizeof(rawseciso_args)+(2*parts*sizeof(uint32_t)));	
					
					if (!cue)
					{
						scsi_tracks[0].adr_control = 0x14;
						scsi_tracks[0].track_number = 1;
						scsi_tracks[0].track_start_addr = 0;
					}
					else
					{
						for (j = 0; j < num_tracks; j++)
						{
							scsi_tracks[j].adr_control = (tracks[j].is_audio) ? 0x10 : 0x14;
							scsi_tracks[j].track_number = j+1;
							scsi_tracks[j].track_start_addr = tracks[j].lba;
						}
					}
				}
				
				cobra_unload_vsh_plugin(0);
	
				if (cobra_load_vsh_plugin(0, sprx, plugin_args, sizeof(plugin_args)) != 0)
				{
					printf("cobra_load_vsh_plugin failed\n");
					return 0;
				}
	
				printf("Completed\n");
				return 0;
			}
		}
	}
        
	return 0;
}
