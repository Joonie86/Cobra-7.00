 #ifndef __MODULESPATCH_H__
#define __MODULESPATCH_H__

#include <lv2/process.h>
#include <lv2/thread.h>

// BIG WARNING: self offsets need to add 0x10000 for address shown in IDA, but not sprxs!

#if defined(FIRMWARE_3_41)

#define VSH_HASH			0xa0556f3d002cb8fd
#define NAS_PLUGIN_HASH			0x6b70280200020017
#define EXPLORE_PLUGIN_HASH		0x8c0a948c000d99b1
#define EXPLORE_CATEGORY_GAME_HASH	0xa2bc1a5600052adc
#define BDP_DISC_CHECK_PLUGIN_HASH	0x86517b4c0000320d
#define PS1_EMU_HASH			0x7a611dec000a0505
#define GAME_EXT_PLUGIN_HASH		0x34c23b7d0001c920
#define X3_MDIMP1_HASH			0x32150f1b0000898f

/* vsh */
#define elf1_func1 			0x5f3fc0
#define elf1_func1_offset 		0x00
#define elf1_func2 			0x305354
#define elf1_func2_offset 		0x14
#define game_update_offset		0x3310f8

/* nas_plugin */
#define elf2_func1 			0x2eb7c
#define elf2_func1_offset		0x374

/* explore_plugin */
#define elf3_data_offset 		0x0022b888
#define app_home_offset			0x2367E8
#define ps2_nonbw_offset		0xD7194

/* explore_category_game */
#define elf4_data_offset 		0x000d68b8
#define ps2_nonbw_offset2		0x7522c

/* bdp_disc_check_plugin */
#define dvd_video_region_check_offset	0x1B20

/* ps1_emu */
#define get_region_offset		0x3E74		

/* game_ext_plugin */
#define sfo_check_offset		0x2029C
#define ps2_nonbw_offset3		0x114A4

#elif defined(FIRMWARE_3_55)

#define VSH_HASH			0xa05428bd002d15fb
#define VSH_REACTPSN_HASH		0xa05428bd002d15f7
#define BASIC_PLUGINS_HASH		0xdf98d6550002538b
#define NAS_PLUGIN_HASH			0xe21928ff00025c6f
#define EXPLORE_PLUGIN_HASH		0x8c0a948c000db78d
#define EXPLORE_CATEGORY_GAME_HASH	0xa2bc18fa00052c74
#define BDP_DISC_CHECK_PLUGIN_HASH	0x86517b7c0000324d
#define PS1_EMU_HASH			0x7a611fec000a0448
#define PS1_NETEMU_HASH			0x7a3451e9000c3040
#define GAME_EXT_PLUGIN_HASH		0x3bebd0440001dd6b
#define X3_MDIMP1_HASH			0x32150f1b00008951
#define PSP_EMULATOR_HASH		0x7be7aced00022a4b
#define EMULATOR_API_HASH		0xa9f5bb7a000108e1
#define PEMUCORELIB_HASH		0xf349a56300087ba4
#define EMULATOR_DRM_HASH		0x7cfa1581000037f5
#define EMULATOR_DRM_DATA_HASH		0xe7b395210000f959
#define LIBSYSUTIL_SAVEDATA_PSP_HASH	0xc7f8df5e00002fa2
#define LIBFS_EXTERNAL_HASH		0x5bc7bad800005fa4

/* vsh */
#define elf1_func1 			0x5FFEE8
#define elf1_func1_offset 		0x00
#define elf1_func2 			0x30A7C0
#define elf1_func2_offset 		0x14
#define game_update_offset		0x3365F4
#define psp_drm_patch1			0x307E74
#define psp_drm_patch2			0x3086C0
#define psp_drm_patch3			0x3082F8
#define psp_drm_patch4			0x308ACC
#define psp_drm_patchA			0x307EA8
#define psp_drm_patchB			0x308714
#define psp_drm_patchC			0x307814
#define revision_offset			0x6668B0
#define spoof_version_patch		0x190F74
#define psn_spoof_version_patch		0x26D238

/* basic_plugins */
#define pspemu_path_offset		0x559D8
#define psptrans_path_offset		0x56510

/* nas_plugin */
#define elf2_func1 			0x36EEC
#define elf2_func1_offset		0x374
#define geohot_pkg_offset		0x316C

/* explore_plugin */
#define elf3_data_offset 		0x22f688
#define app_home_offset			0x23A6C0
#define ps2_nonbw_offset		0xD7708

/* explore_category_game */
#define elf4_data_offset 		0xD6AD8
#define ps2_nonbw_offset2		0x7544C

/* bdp_disc_check_plugin */
#define dvd_video_region_check_offset	0x1B20

/* ps1_emu */
#define ps1_emu_get_region_offset	0x3E74	

/* ps1_netemu */
#define ps1_netemu_get_region_offset	0xB0154

/* game_ext_plugin */
#define sfo_check_offset		0x2345C
#define ps2_nonbw_offset3		0x14314
#define ps_region_error_offset		0x1C64

/* psp_emulator */
#define psp_set_psp_mode_offset		0x1714	

/* emulator_api */
#define psp_read			0x8710
#define psp_read_header			0x9B88
#define psp_drm_patch5			0x993c
#define psp_drm_patch6			0x9994
#define psp_drm_patch7			0x99AC
#define psp_drm_patch8			0x99B0
#define psp_drm_patch9			0x9AE0
#define psp_drm_patch10			0x9B38
#define psp_drm_patch11			0x9B38
#define psp_drm_patch12			0x9B48
#define psp_product_id_patch1		0x9CE4
#define psp_product_id_patch2		0x9D14
#define psp_product_id_patch3		0xA0E8
#define psp_product_id_patch4		0xA118

/* pemucorelib */
#define psp_eboot_dec_patch		0x5FF8C
#define psp_prx_patch			0x5919C
#define psp_savedata_bind_patch1	0x7CEE0
#define psp_savedata_bind_patch2	0x7CF38
#define psp_savedata_bind_patch3	0x7CB04
#define psp_prometheus_patch		0x10df04
#define psp_debug_patch			0xAD880

/* emulator_drm */
#define psp_drm_tag_overwrite		0x4C24
#define psp_drm_key_overwrite		(0x17900-0x7F00)

/* libsysutil_savedata_psp */
#define psp_savedata_patch1		0x4548
#define psp_savedata_patch2		0x47A0
#define psp_savedata_patch3		0x47A8
#define psp_savedata_patch4		0x47C0
#define psp_savedata_patch5		0x47D4
#define psp_savedata_patch6		0x4800
#define psp_savedata_patch7		0x4818

/* libfs (external */
#define aio_copy_root_offset		0xD37C

#elif defined(FIRMWARE_4_30)

#define VSH_HASH			0xa001e8be002df257
#define BASIC_PLUGINS_HASH		0xd72209580001f769
#define NAS_PLUGIN_HASH			0xacf4af2b00026673
#define EXPLORE_PLUGIN_HASH		0xacf4af2b000eaf86
#define EXPLORE_CATEGORY_GAME_HASH	0x9cb3ccad00056f02
#define BDP_DISC_CHECK_PLUGIN_HASH	0xb8b7a5f90000319f
#define PS1_EMU_HASH			0x7a67acd8000a1a56
#define PS1_NETEMU_HASH			0x7a376365000c4aa8
#define GAME_EXT_PLUGIN_HASH		0xe274af7b0001dd1d
#define X3_MDIMP1_HASH			0x9d05eda700009030
#define PSP_EMULATOR_HASH		0x7be646f50002371e
#define EMULATOR_API_HASH		0xa9f5b37a0001b739
#define PEMUCORELIB_HASH		0xf349a563000bfd70
#define EMULATOR_DRM_HASH		0xa324dc4a0000539c
#define EMULATOR_DRM_DATA_HASH		0x1d8d2da00001b6e4
#define LIBSYSUTIL_SAVEDATA_PSP_HASH	0x57bbc04800003207
#define LIBFS_EXTERNAL_HASH		0x5bc7bec800006430

/* vsh */
#define elf1_func1 			0x5E75E8
#define elf1_func1_offset 		0x00
#define elf1_func2 			0x23FFB0
#define elf1_func2_offset 		0x14
#define game_update_offset		0x26AA10
#define psp_drm_patch1			0x23D27C
#define psp_drm_patch2			0x23DD1C
#define psp_drm_patch3			0x23D958
#define psp_drm_patch4			0x23E164
#define psp_drm_patchA			0x23D398
#define psp_drm_patchB			0x23DC38
#define psp_drm_patchC			0x23CDD0
#define psp_drm_patchD			0x23D380
#define psp_drm_patchE			0x23D384
#define psp_drm_patchF			0x23DD50
#define revision_offset			0x6553B0
#define spoof_version_patch		0xB8F28
#define psn_spoof_version_patch		0x1A08DC

/* basic_plugins */
#define ps1emu_type_check_offset	0x1F678
#define pspemu_path_offset		0x4A4C8
#define psptrans_path_offset		0x4B0E8

/* nas_plugin */
#define elf2_func1 			0x2E4D0
#define elf2_func1_offset		0x374
#define geohot_pkg_offset		0x3174

/* explore_plugin */
#define elf3_data_offset 		0x242e78
#define app_home_offset			0x24f058
#define ps2_nonbw_offset		0xDDA04

/* explore_category_game */
#define elf4_data_offset 		0xD8E60
#define ps2_nonbw_offset2		0x69B68
#define unk_patch_offset1		0x5490 /* unknown patch from E3 cfw */
#define unk_patch_offset2		0x54B4 /* unknown patch from E3 cfw */

/* bdp_disc_check_plugin */
#define dvd_video_region_check_offset	0x1528

/* ps1_emu */
#define ps1_emu_get_region_offset	0x3E74	

/* ps1_netemu */
#define ps1_netemu_get_region_offset	0xB19CC

/* game_ext_plugin */
#define sfo_check_offset		0x2364C
#define ps2_nonbw_offset3		0x167D8
#define ps_region_error_offset		0x699C

/* psp_emulator */
#define psp_set_psp_mode_offset		0x1C10

/* emulator_api */
#define psp_read			0xFB24
#define psp_read_header			0x10AA8
#define psp_drm_patch5			0x108CC
#define psp_drm_patch6			0x108FC
#define psp_drm_patch7			0x10914
#define psp_drm_patch8			0x10918
#define psp_drm_patch9			0x10A58
#define psp_drm_patch11			0x10A5C
#define psp_drm_patch12			0x10A6C
#define psp_product_id_patch1		0x10B6C
#define psp_product_id_patch3		0x10E44

/* pemucorelib */
#define psp_eboot_dec_patch		0x5E340
#define psp_prx_patch			0x5745C
#define psp_savedata_bind_patch1	0x79810
#define psp_savedata_bind_patch2	0x79868
#define psp_savedata_bind_patch3	0x79384
#define psp_extra_savedata_patch	0x85194
#define psp_prometheus_patch		0x12E88C

/* emulator_drm */
#define psp_drm_tag_overwrite		0x4C68
#define psp_drm_key_overwrite		(0x27580-0xBE80)

/* libsysutil_savedata_psp */
#define psp_savedata_patch1		0x46D4
#define psp_savedata_patch2		0x46AC
#define psp_savedata_patch3		0x450C
#define psp_savedata_patch4		0x4544
#define psp_savedata_patch5		0x4558
#define psp_savedata_patch6		0x46C0

/* libfs (external */
#define aio_copy_root_offset		0xD5B4

#elif defined(FIRMWARE_4_53)

#define VSH_HASH			0xa002327a002e1093 //
#define VSH_ROG_HASH			0xa002327a002e1093 //  1.00, 1.01. Same for rebug vsh.self.cexsp */
#define VSH_HAB_HASH			0xa002327a002e1093 //  1.13. Same for arc A01 */
#define VSH_FER_HASH			0xa002327a002e1093 //* v3 */
#define BASIC_PLUGINS_HASH		0xb5982e3a0001f6c7 //
#define BASIC_PLUGINS_ROG_HASH		0xb5982e3a0001f6c7 // Same for hab, fer, arc */
#define NAS_PLUGIN_HASH			0xacf4af2b000270da //
#define NAS_PLUGIN_ROG_HASH		0xacf4af2b000270da // Same for hab, fer, arc */
#define EXPLORE_PLUGIN_HASH		0xacf4af2b000ec3a6 //
#define EXPLORE_PLUGIN_ROG_HASH		0xacf4af2b000ec3a6 //  Same for hab, fer, arc */
#define EXPLORE_CATEGORY_GAME_HASH	0x9cb3205600056c47 // Rog hash is same as ofw even if file is different. Same for hab, fer, arc */
#define BDP_DISC_CHECK_PLUGIN_HASH	0xb8b7a5f9000031a0 //
#define PS1_EMU_HASH			0x7a6cb94f0009a9f3 //
#define PS1_NETEMU_HASH			0x7a3017f2000bdac8 //
#define GAME_EXT_PLUGIN_HASH		0xe274af7b0001e2c8 //
#define PSP_EMULATOR_HASH		0x7be641f500023794 //
#define EMULATOR_API_HASH		0xa9f5b37a0001bb36 //
#define PEMUCORELIB_HASH		0xf349a563000c0cfc //OFFSET+10000 IS THE LOCATION OF PATCHES...
#define EMULATOR_DRM_HASH		0xa324dc4a0000539c //
#define EMULATOR_DRM_DATA_HASH		0x1d8d2da00001b6e4 //
#define LIBSYSUTIL_SAVEDATA_PSP_HASH	0x57bbc04800003207 //
#define LIBFS_EXTERNAL_HASH		0x5bc7bce800006477 //

/* vsh */
#define vsh_text_size			0x6B0000 // /* memsz of first program header aligned to 0x10000 */
#define elf1_func1 			0x5E9CD4 //
#define elf1_func1_offset 		0x00 //
#define elf1_func2 			0x243450 //
#define elf1_func2_offset 		0x14 //
#define game_update_offset		0x26E0C0 //
#define ps2tonet_patch			0xC4C5C //
#define ps2tonet_size_patch		0xC4C50 //
#define psp_drm_patch1			0x24071C //
#define psp_drm_patch2			0x2411BC //
#define psp_drm_patch3			0x240DF8 //
#define psp_drm_patch4			0x241604 //FIXED!!!
#define psp_drm_patchA			0x240838 //
#define psp_drm_patchB			0x2410D8 //
#define psp_drm_patchC			0x240270 //
#define psp_drm_patchD			0x240820 //FIXED!!!!
#define psp_drm_patchE			0x240824 //
#define psp_drm_patchF			0x2411F0 //
#define revision_offset			0x6582F0 //
#define revision_offset2		0x6FF998 /* In data section */
#define spoof_version_patch		0xB8F28
#define psn_spoof_version_patch		0x1A2840 //
#define vmode_patch_offset		0x4422FC //

/* basic_plugins *///CHECKED!!
#define ps1emu_type_check_offset	0x1F620 //
#define pspemu_path_offset		0x4A4A0 //
#define psptrans_path_offset		0x4B0C0 //

/* nas_plugin *///CHECKED!!
#define elf2_func1 			0x2E7EC //
#define elf2_func1_offset		0x374 //
#define geohot_pkg_offset		0x3174 //

/* explore_plugin *///CHECKED!!
#define app_home_offset			0x2507D8 //FIXED 12/28/13
#define ps2_nonbw_offset		0xDD754 //

/* explore_category_game *///CHECKED!!!
#define ps2_nonbw_offset2		0x68430 //
#define unk_patch_offset1		0x546C // unknown patch from E3 cfw */
#define unk_patch_offset2		0x5490 // unknown patch from E3 cfw */

/* bdp_disc_check_plugin *///CHECKED!!
#define dvd_video_region_check_offset	0x152C //

/* ps1_emu */
#define ps1_emu_get_region_offset	0x3E74	//

/* ps1_netemu */
#define ps1_netemu_get_region_offset	0xA440C //

/* game_ext_plugin *///CHECKED!!
#define sfo_check_offset		0x236C4 //
#define ps2_nonbw_offset3		0x16850 //
#define ps_region_error_offset		0x69D0 //

/* psp_emulator */
#define psp_set_psp_mode_offset		0x1C18 //

/* emulator_api */
#define psp_read			0x102D8 //FIXED!!
#define psp_read_header			0x1125C //
#define psp_drm_patch5			0x11080 //
#define psp_drm_patch6			0x110B0 //
#define psp_drm_patch7			0x110C8 //
#define psp_drm_patch8			0x110CC //
#define psp_drm_patch9			0x1120C //
#define psp_drm_patch11			0x11210 //
#define psp_drm_patch12			0x11220 //
#define psp_product_id_patch1		0x11320 //
#define psp_product_id_patch3		0x115F8 //
#define umd_mutex_offset		(0x64480+0x38C) //FIXED!!

/* pemucorelib ii */
#define psp_eboot_dec_patch		0x5E6BC //F
#define psp_prx_patch			0x577D8 //F
#define psp_savedata_bind_patch1	0x7A4BC //F
#define psp_savedata_bind_patch2	0x7A514  //F
#define psp_savedata_bind_patch3	0x7A030 //F
#define psp_extra_savedata_patch	0x8753C //F
#define psp_prometheus_patch		0x12EA28 //F
#define prx_patch_call_lr		0x5892C //F 

/* emulator_drm */
#define psp_drm_tag_overwrite		0x4C68 //
#define psp_drm_key_overwrite		(0x27580-0xBE80) //

/* libsysutil_savedata_psp */
#define psp_savedata_patch1		0x46D4 //
#define psp_savedata_patch2		0x46AC //
#define psp_savedata_patch3		0x450C //
#define psp_savedata_patch4		0x4544 //
#define psp_savedata_patch5		0x4558 //
#define psp_savedata_patch6		0x46C0 //

/* libfs (external */
#define aio_copy_root_offset		0xD658 //FIXED!!!!

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
void do_spoof_patches(void);
void load_boot_plugins(void);
int prx_load_vsh_plugin(unsigned int slot, char *path, void *arg, uint32_t arg_size);
int prx_unload_vsh_plugin(unsigned int slot);

/* Syscalls */
int sys_vsh_spoof_version(char *version_str);
int sys_prx_load_vsh_plugin(unsigned int slot, char *path, void *arg, uint32_t arg_size);
int sys_prx_unload_vsh_plugin(unsigned int slot);
int sys_thread_create_ex(sys_ppu_thread_t *thread, void *entry, uint64_t arg, int prio, uint64_t stacksize, uint64_t flags, const char *threadname);

#endif /* __MODULESPATCH_H__ */

