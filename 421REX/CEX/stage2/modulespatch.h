#ifndef __MODULESPATCH_H__
#define __MODULESPATCH_H__

#include <lv2/process.h>
#include <lv2/thread.h>

// BIG WARNING: self offsets need to add 0x10000 for address shown in IDA, but not sprxs!

#if defined(FIRMWARE_4_21)

#define VSH_HASH			0xa0026096002e22f3
#define VSH_DEX_HASH			0xa0026096002e22f3 /* REX vsh.self/vsh.self.swp */
#define VSH_CEX_HASH			0xa001ccbe002dcc13 /* REX vsh.self.cexsp */
#define BASIC_PLUGINS_HASH		0xd72209580001f7a2
#define BASIC_PLUGINS_ROG_HASH		0xd72295740001fa30 /* Same for hab, fer, arc */
#define NAS_PLUGIN_HASH			0xacf4af2b00026801
#define NAS_PLUGIN_ROG_HASH		0xacf4af2b00025d5d /* Same for hab, fer, arc */
#define EXPLORE_PLUGIN_HASH		0xacf4af2b000ec8a6
#define EXPLORE_PLUGIN_ROG_HASH		0xacf4af2b000e5c66 /* Same for hab, fer, arc */
#define EXPLORE_CATEGORY_GAME_HASH	0x9cb3c54a00057710 /* Rog hash is same as ofw even if file is different. Same for hab, fer, arc */
#define BDP_DISC_CHECK_PLUGIN_HASH	0xb8b7a5f90000319f
#define PS1_EMU_HASH			0x7a67e830000a18b6
#define PS1_NETEMU_HASH			0x7a374d45000c4955
#define GAME_EXT_PLUGIN_HASH		0xe274af7b0001d89e
#define PSP_EMULATOR_HASH		0x7be65d6d00023702
#define EMULATOR_API_HASH		0xa9f5b2320001e7c6
#define PEMUCORELIB_HASH		0xf349a563000bfcc8
#define EMULATOR_DRM_HASH		0xd2c7f3e20000538f
#define EMULATOR_DRM_DATA_HASH		0xb4f919590001b6e7
#define LIBSYSUTIL_SAVEDATA_PSP_HASH	0x57bbc3b800003210
#define LIBFS_EXTERNAL_HASH		0x5bc7bec800006430
 
// vsh dex //
#define vsh_text_size			0x6B0000 // memsz of first program header aligned to 0x10000 //
#define dex_ps2tonet_patch			0xC9A30
#define dex_ps2tonet_size_patch		0xC9A24
#define dex_psp_drm_patch1			0x244068
#define dex_psp_drm_patch2			0x244B00
#define dex_psp_drm_patch3			0x244740
#define dex_psp_drm_patch4			0x244F48
#define dex_psp_drm_patchA			0x244184
#define dex_psp_drm_patchB			0x244A1C
#define dex_psp_drm_patchC			0x243BBC
#define dex_psp_drm_patchD			0x24416C
#define dex_psp_drm_patchE			0x244170
#define dex_psp_drm_patchF			0x244B34
//#define revision_offset			0x65BBA0
//#define revision_offset2		0x6FFC1C // In data section //
//#define spoof_version_patch		0xBDBD0
//#define psn_spoof_version_patch		0x1A75AC
#define dex_vmode_patch_offset		0x446650
// vsh cex //
#define cex_ps2tonet_patch			0xC44EC
#define cex_ps2tonet_size_patch		0xC44E0
#define cex_psp_drm_patch1			0x23C85C
#define cex_psp_drm_patch2			0x23D2F4
#define cex_psp_drm_patch3			0x23CF34
#define cex_psp_drm_patch4			0x23D73C
#define cex_psp_drm_patchA			0x23C978
#define cex_psp_drm_patchB			0x23D210
#define cex_psp_drm_patchC			0x23C3B0
#define cex_psp_drm_patchD			0x23C960
#define cex_psp_drm_patchE			0x23C964
#define cex_psp_drm_patchF			0x23D328
//#define revision_offset			0x653890
//#define revision_offset2		0x6FF280 // In data section //
//#define spoof_version_patch		0xB8D78
//#define psn_spoof_version_patch		0x19FCA4
#define cex_vmode_patch_offset		0x43EA78

/* basic_plugins */
#define ps1emu_type_check_offset	0x20114
#define pspemu_path_offset		0x4AF28
#define psptrans_path_offset		0x4BB98

/* nas_plugin */
//#define elf2_func1 			0x2DCF0
//#define elf2_func1_offset		0x374
//#define geohot_pkg_offset		0x3174

/* explore_plugin */
//#define app_home_offset			0x246AE8
#define ps2_nonbw_offset		0xDAFBC

/* explore_category_game */
#define ps2_nonbw_offset2		0x75460
// #define unk_patch_offset1		0x546C /* unknown patch from E3 cfw */
// #define unk_patch_offset2		0x5490 /* unknown patch from E3 cfw */

/* bdp_disc_check_plugin */
#define dvd_video_region_check_offset	0x1528

/* ps1_emu */
#define ps1_emu_get_region_offset	0x3E74	

/* ps1_netemu */
#define ps1_netemu_get_region_offset	0xB18BC

/* game_ext_plugin */
#define sfo_check_offset		0x23054
#define ps2_nonbw_offset3		0x16788
#define ps_region_error_offset		0x6810

/* psp_emulator */
#define psp_set_psp_mode_offset		0x1BE0

/* emulator_api */
#define psp_read			0xFC64
#define psp_read_header			0x10BE8
#define psp_drm_patch5			0x10A0C
#define psp_drm_patch6			0x10A3C
#define psp_drm_patch7			0x10A54
#define psp_drm_patch8			0x10A58
#define psp_drm_patch9			0x10B98
#define psp_drm_patch11			0x10B9C
#define psp_drm_patch12			0x10BAC
#define psp_product_id_patch1		0x10CAC
#define psp_product_id_patch3		0x10F84
#define umd_mutex_offset		(0x64480+0x38C)

/* pemucorelib */
#define psp_eboot_dec_patch		0x5E35C
#define psp_prx_patch			0x57478
#define psp_savedata_bind_patch1	0x79810
#define psp_savedata_bind_patch2	0x79868
#define psp_savedata_bind_patch3	0x79384
#define psp_extra_savedata_patch	0x851A8
#define psp_prometheus_patch		0x12E870
#define prx_patch_call_lr		0x585CC

/* emulator_drm */
#define psp_drm_tag_overwrite		0x4C64
#define psp_drm_key_overwrite		(0x27580-0xBE80)

/* libsysutil_savedata_psp */
#define psp_savedata_patch1		0x46CC
#define psp_savedata_patch2		0x46A4
#define psp_savedata_patch3		0x4504
#define psp_savedata_patch4		0x453C
#define psp_savedata_patch5		0x4550
#define psp_savedata_patch6		0x46B8

/* libfs (external */
#define aio_copy_root_offset		0xD5B4

#endif /* FIRMWARE */

/* 3.72 */
#define PSP_EMULATOR372_HASH		0x7be7b71500052f98
#define EMULATOR_API372_HASH		0xa9f5b27a00041dc8
#define PEMUCORELIB372_HASH		0xf349a5630019f080

/* psp_emulator */
#define psp372_set_psp_mode_offset   	0x1860

/* emulator_api */
#define psp372_read			0x13C2C
#define psp372_read_header		0x1509C
#define psp372_drm_patch5		0x14E8C
#define psp372_drm_patch6		0x14EC0
#define psp372_drm_patch7		0x14ED8
#define psp372_drm_patch8		0x14EDC
#define psp372_drm_patch9		0x1501C
#define psp372_drm_patch10		0x15050
#define psp372_drm_patch11		0x15050
#define psp372_drm_patch12		0x15060
#define psp372_product_id_patch1	0x151F8
#define psp372_product_id_patch2	0x15228
#define psp372_product_id_patch3	0x15614

/* pemucorelib */
#define psp372_eboot_dec_patch		0x5DAA4
#define psp372_prx_patch		0x56C84
#define psp372_extra_savedata_patch	0x8366C

/* 4.00 */
#define PSP_EMULATOR400_HASH		0x7be644e500053508
#define EMULATOR_API400_HASH		0xa9f5bb7a00043158
#define PEMUCORELIB400_HASH		0xf349a5630019ee00

/* psp_emulator */
#define psp400_set_psp_mode_offset	0x19EC

/* emulator_api */
#define psp400_read			0x13E18
#define psp400_read_header		0x15288
#define psp400_drm_patch5		0x15078
#define psp400_drm_patch6		0x150AC
#define psp400_drm_patch7		0x150C4
#define psp400_drm_patch8		0x150C8
#define psp400_drm_patch9		0x15208
#define psp400_drm_patch10		0x1523C
#define psp400_drm_patch11		0x1523C
#define psp400_drm_patch12		0x1524C
#define psp400_product_id_patch1	0x153E4
#define psp400_product_id_patch2	0x15414
#define psp400_product_id_patch3	0x15800 

/* pemucorelib */
#define psp400_eboot_dec_patch		0x5DFE8
#define psp400_prx_patch		0x571C8
#define psp400_savedata_bind_patch1	0x78334
#define psp400_savedata_bind_patch2	0x7838C
#define psp400_savedata_bind_patch3	0x77EE8
#define psp400_extra_savedata_patch	0x838BC
#define psp400_prometheus_patch		0x11E3DC

typedef struct
{
	uint32_t offset;
	uint32_t data;
	uint8_t *condition;
} SprxPatch;
/* 
typedef struct
{
	uint32_t offset;
	uint32_t data;
	uint8_t *condition;
} SprxPatch2;
 */
extern uint8_t condition_ps2softemu;
extern uint8_t condition_apphome;
extern uint8_t condition_psp_iso;
extern uint8_t condition_psp_dec;
extern uint8_t condition_psp_keys;
extern uint8_t condition_psp_change_emu;
extern uint8_t condition_psp_prometheus;

extern uint8_t block_peek;

extern process_t vsh_process;
extern uint8_t safe_mode;

/* Functions for kernel */
void modules_patch_init(void);
// void do_spoof_patches(void);
void load_boot_plugins(void);
int prx_load_vsh_plugin(unsigned int slot, char *path, void *arg, uint32_t arg_size);
int prx_unload_vsh_plugin(unsigned int slot);

/* Syscalls */
// int sys_vsh_spoof_version(char *version_str);
int sys_prx_load_vsh_plugin(unsigned int slot, char *path, void *arg, uint32_t arg_size);
int sys_prx_unload_vsh_plugin(unsigned int slot);
int sys_thread_create_ex(sys_ppu_thread_t *thread, void *entry, uint64_t arg, int prio, uint64_t stacksize, uint64_t flags, const char *threadname);

#endif /* __MODULESPATCH_H__ */

