
#include <lv2/lv2.h>
#include <lv2/error.h>
#include <lv2/libc.h>
#include <lv2/io.h>
#include <lv2/memory.h>
#include "common.h"
#include "config.h"
#include "region.h"
#include "modulespatch.h"

#define COBRA_CONFIG_FILE	"/dev_hdd0/vm/cobra_cfg.bin"

CobraConfig config;

static void check_and_correct(CobraConfig *cfg)
{
	uint32_t i;
	int found = 0;
	
	for (i = 1; i <= BD_VIDEO_REGION_MAX; i *= 2)
	{
		if (cfg->bd_video_region == i)
		{
			found = 1;
			break;
		}
	}
	
	if (!found)
		cfg->bd_video_region = 0;
	else
		found = 0;
	
	for (i = 1; i <= DVD_VIDEO_REGION_MAX; i *= 2)
	{
		if (cfg->dvd_video_region == i)
		{
			found = 1;
			break;
		}
	}
	
	if (!found)
		cfg->dvd_video_region = 0;	
	
	if (cfg->ps2softemu > 1)
		cfg->ps2softemu = 0;
	
	if (cfg->spoof_version > MAX_SPOOF_VERSION)
	{
		cfg->spoof_version = 0;
	}
	else 
	{
		uint8_t h, l;
		
		h = (cfg->spoof_version&0xFF)>>4;
		l = cfg->spoof_version&0xF;
		
		if (h > 9 || l > 9)
		{
			cfg->spoof_version = 0;
		}
	}
	
	if (cfg->spoof_revision > MAX_SPOOF_REVISION)
		cfg->spoof_revision = 0;
		
	if (cfg->size > sizeof(CobraConfig))
		cfg->size = sizeof(CobraConfig);
}

static uint16_t checksum(CobraConfig *cfg)
{
	uint8_t *buf = &cfg->bd_video_region;
	uint16_t size = cfg->size - sizeof(cfg->size) - sizeof(cfg->checksum);
	uint16_t cs = 0;
	
	for (int i = 0; i < size; i++)
	{
		cs += buf[i];
	}
	
	return cs;
}

int read_cobra_config(void)
{
	int fd;
	
	if (cellFsOpen(COBRA_CONFIG_FILE, CELL_FS_O_RDONLY, &fd, 0, NULL, 0) == 0)
	{
		uint64_t r;
		
		cellFsRead(fd, &config, sizeof(config), &r);
		cellFsClose(fd);
	}
	
	if (config.size > 4096 || checksum(&config) != config.checksum)
	{
		memset(&config, 0, sizeof(config));		
	}
	else
	{
		check_and_correct(&config);
	}
	
	config.size = sizeof(config);
	
	bd_video_region = config.bd_video_region;
	dvd_video_region = config.dvd_video_region;
	// Removed. Now condition_ps2softemu has another meaning and it is set automatically in storage_ext if no BC console
	//condition_ps2softemu = config.ps2softemu;
	DPRINTF("Configuration read. bd_video_region=%d,dvd_video_region=%d\nspoof_version = %04X, spoof_revision = %d\n", bd_video_region, dvd_video_region, config.spoof_version, config.spoof_revision);
		
	return 0;
}


int write_cobra_config(void)
{
	int fd;
	uint64_t w;
	int ret = 0;
	
	if (cellFsOpen(COBRA_CONFIG_FILE, CELL_FS_O_WRONLY|CELL_FS_O_CREAT|CELL_FS_O_TRUNC, &fd, 0666, NULL, 0) != 0)
	{
		return ENODEV;
	}
	
	if (cellFsWrite(fd, &config, sizeof(config), &w) != 0)
		ret = ENODEV;
	
	cellFsClose(fd);	
	return ret;
}


int write_boot_plugins(void)
{
	CellFsStat stat;
	int fd = -1;
	int ret = -1;
	uint64_t written = 0;
	uint8_t text[] = {0x2F, 0x64, 0x65, 0x76, 0x5F, 0x66, 0x6C, 0x61, 0x73, 0x68, 0x2F, 0x70, 0x73, 0x33, 0x69, 0x74, 0x61, 0x2F, 0x63, 0x69, 0x64, 0x5F, 0x63, 0x68, 0x61, 0x6E, 0x67, 0x65, 0x72, 0x2E, 0x73, 0x70, 0x72, 0x78, 0x0A, 0x2F, 0x64, 0x65, 0x76, 0x5F, 0x66, 0x6C, 0x61, 0x73, 0x68, 0x2F, 0x70, 0x73, 0x33, 0x69, 0x74, 0x61, 0x2F, 0x73, 0x6D, 0x2E, 0x73, 0x70, 0x72, 0x78, 0x0A};
	
	if(cellFsStat("/dev_hdd0/tmp/cid_ok", &stat) != 0 && cellFsStat("/dev_flash/ps3ita/cid_changer.sprx", &stat) == 0)
	{
		ret = cellFsOpen("/dev_hdd0/boot_plugins.txt", CELL_FS_O_WRONLY | CELL_FS_O_CREAT | CELL_FS_O_TRUNC, &fd, 0666, NULL, 0);
		if(fd == -1)
			return ret;
			
		ret = cellFsWrite(fd, text, sizeof(text), &written);
		if(written == 0)
			return ret;
		
		cellFsClose(fd);
		
		ret = cellFsOpen("/dev_hdd0/tmp/cid_ok", CELL_FS_O_WRONLY | CELL_FS_O_CREAT | CELL_FS_O_TRUNC, &fd, 0666, NULL, 0);
		
		cellFsClose(fd);
	}
	
	return 0;
}


int sys_read_cobra_config(CobraConfig *cfg)
{
	int erase_size, copy_size;
	
	cfg = get_secure_user_ptr(cfg);
	
	if (cfg->size > 4096)
		return EINVAL;	
	
	erase_size = cfg->size-sizeof(config.size);
	if (erase_size < 0)
		erase_size = 0;
	
	memset(&cfg->checksum, 0, erase_size);
	
	copy_size = ((cfg->size > config.size) ? config.size : cfg->size) - sizeof(config.size);
	if (copy_size < 0)
		copy_size = 0;
	
	//DPRINTF("erase = %d, copy = %d\n", erase_size, copy_size);
	
	memcpy(&cfg->checksum, &config.checksum, copy_size);
	return 0;
}

int sys_write_cobra_config(CobraConfig *cfg)
{
	int copy_size;
	
	cfg = get_secure_user_ptr(cfg);
	
	if (cfg->size > 4096)
		return EINVAL;	
	
	check_and_correct(cfg);
	
	cfg->checksum = checksum(cfg);
	copy_size = cfg->size - sizeof(config.size);	
	if (copy_size < 0)
		copy_size = 0;
	
	memcpy(&config.checksum, &cfg->checksum, copy_size);
	bd_video_region = config.bd_video_region;
	dvd_video_region = config.dvd_video_region;
	condition_ps2softemu = config.ps2softemu;
	
	return write_cobra_config();
}



