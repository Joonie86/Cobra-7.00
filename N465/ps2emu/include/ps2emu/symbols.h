/* Keep this file language agnostic. Only preprocessor here. */

#ifndef __PS2EMU_SYMBOLS_H_S__
#define __PS2EMU_SYMBOLS_H_S__

#if defined(FIRMWARE_3_41)

#if defined(PS2HWEMU)

#define TOC					0x4E9A28

#define cdvd_send_atapi_command_symbol		0x21FC4

#define ufs_open_symbol				0x4DF14
#define ufs_close_symbol			0x4E658
#define ufs_read_symbol				0x4DA00
#define ufs_write_symbol			0x4DAF4
#define ufs_fstat_symbol			0x4E95C

#define zeroalloc_symbol			0xFDDE8
#define malloc_symbol				0xFDD00
#define free_symbol				0xFDE40
#define memcpy_symbol				0xDFAC
#define memset_symbol				0xD488
#define strcpy_symbol				0xED04
#define strncpy_symbol				0xED30
#define strcat_symbol				0xEED0
#define strlen_symbol				0xECD8
#define strcmp_symbol				0xED94
#define strncmp_symbol				0xEDE8
#define strchr_symbol				0xEE50
#define strrchr_symbol				0xEE8C

#define ps2_disc_auth_symbol			0x2053C

#define overwritten_symbol			0x23BF8

#elif defined(PS2GXEMU)

#define TOC					0x676460

#define cdvd_read_symbol			0x8E33C
#define cdvd_send_atapi_command_symbol		0x8D910
#define cdvd_send_device_command_symbol		0x8D824

#define ufs_open_symbol				0x24EA48
#define ufs_close_symbol			0x24F1B0
#define ufs_read_symbol				0x24E758
#define ufs_write_symbol			0x24E8D0
#define ufs_fstat_symbol			0x24E598

#define printf_symbol				0x1E4854

#define zeroalloc_symbol			0x1E47A4
#define malloc_symbol				0x1E4744
#define free_symbol				0x1E47F0
#define memcpy_symbol				0x514BC
#define memset_symbol				0x51528
#define snprintf_symbol				0x52360
#define strcpy_symbol				0x51600
#define strncpy_symbol				0x99AD0
#define strcat_symbol				0x51634
#define strlen_symbol				0x515C8
#define strcmp_symbol				0x99AFC
#define strchr_symbol				0x99A5C
#define strrchr_symbol				0x99A90

#define vuart_read_symbol			0x49544
#define vuart_write_symbol			0x494A4

#define ps2_disc_auth_symbol			0x8FF8C

#define overwritten_symbol			0x8c968

/* Calls */
#define reboot_parameters_vuart_call		0x58C0C

/* Vars */
#define arguments_symbol			0x6E5198

#elif defined(PS2SOFTEMU)

#define TOC					0x5C8C00

#define cdvd_send_atapi_command_symbol		0x88C88
#define cdvd_send_device_command_symbol		0x88B9C

#define ufs_open_symbol				0x1AABBC
#define ufs_close_symbol			0x1AB334
#define ufs_read_symbol				0x1AA89C
#define ufs_write_symbol			0x1AAA2C
#define ufs_fstat_symbol			0x1AA6E8

#define zeroalloc_symbol			0x1447A0
#define malloc_symbol				0x144740
#define free_symbol				0x1447EC
#define memcpy_symbol				0x4AF8C
#define memset_symbol				0x4AFF8
#define snprintf_symbol				0x4BE40
#define strcpy_symbol				0x4B0D0
#define strcat_symbol				0x4B104
#define strlen_symbol				0x4B098
#define strcmp_symbol				0x14316C
#define strrchr_symbol				0x143100

#define ps2_disc_auth_symbol			0x8B2BC

#define overwritten_symbol			0x87BE0

/* Vars */
#define arguments_symbol			0x636E18

#endif /* PS2EMU type */

#elif defined(FIRMWARE_3_55)

#if defined(PS2HWEMU)

#define TOC					0x4E99F8

#define cdvd_send_atapi_command_symbol		0x21FC4

#define ufs_open_symbol				0x4DF14
#define ufs_close_symbol			0x4E658
#define ufs_read_symbol				0x4DA00
#define ufs_write_symbol			0x4DAF4
#define ufs_fstat_symbol			0x4E95C

#define printf_symbol				0x3BC4

#define zeroalloc_symbol			0xFDDE8
#define malloc_symbol				0xFDD00
#define free_symbol				0xFDE40
#define memcpy_symbol				0xDFAC
#define memset_symbol				0xD488
#define strcpy_symbol				0xED04
#define strncpy_symbol				0xED30
#define strcat_symbol				0xEED0
#define strlen_symbol				0xECD8
#define strcmp_symbol				0xED94
#define strncmp_symbol				0xEDE8
#define strchr_symbol				0xEE50
#define strrchr_symbol				0xEE8C

#define vuart_read_symbol			0x1E338
#define vuart_write_symbol			0x1E288

#define ps2_disc_auth_symbol			0x2053C
#define ps2_disc_auth_caller_symbol		0x5111C

#define overwritten_symbol			0x23BF8

#define arguments_symbol			0x4FCE68

#elif defined(PS2GXEMU)

#define TOC					0x6765F8

#define cdvd_read_symbol			0x8E33C
#define cdvd_send_atapi_command_symbol		0x8D910
#define cdvd_send_device_command_symbol		0x8D824

#define ufs_open_symbol				0x24EA08
#define ufs_close_symbol			0x24F170
#define ufs_read_symbol				0x24E718
#define ufs_write_symbol			0x24E890
#define ufs_fstat_symbol			0x24E558

#define log_printf_symbol			0x1E4814

#define zeroalloc_symbol			0x1E4764
#define malloc_symbol				0x1E4704
#define free_symbol				0x1E47B0
#define memcpy_symbol				0x514BC
#define memset_symbol				0x51528
#define snprintf_symbol				0x52360
#define strcpy_symbol				0x51600
#define strncpy_symbol				0x99A90
#define strcat_symbol				0x51634
#define strlen_symbol				0x515C8
#define strcmp_symbol				0x99ABC
#define strchr_symbol				0x99A1C
#define strrchr_symbol				0x99A50

#define vuart_read_symbol			0x49544
#define vuart_write_symbol			0x494A4

#define ps2_disc_auth_symbol			0x8FF8C
#define ps2_disc_auth_caller_symbol		0x24F774

#define overwritten_symbol			0x8c968

/* Calls */
#define reboot_parameters_vuart_call		0x58C0C

/* Vars */
#define arguments_symbol			0x6E5298

#elif defined(PS2SOFTEMU)

#define TOC					0x5C8C00

#define cdvd_send_atapi_command_symbol		0x88C88
#define cdvd_send_device_command_symbol		0x88B9C

#define ufs_open_symbol				0x1AABBC
#define ufs_close_symbol			0x1AB334
#define ufs_read_symbol				0x1AA89C
#define ufs_write_symbol			0x1AAA2C
#define ufs_fstat_symbol			0x1AA6E8

#define zeroalloc_symbol			0x1447A0
#define malloc_symbol				0x144740
#define free_symbol				0x1447EC
#define memcpy_symbol				0x4AF8C
#define memset_symbol				0x4AFF8
#define snprintf_symbol				0x4BE40
#define strcpy_symbol				0x4B0D0
#define strcat_symbol				0x4B104
#define strlen_symbol				0x4B098
#define strcmp_symbol				0x14316C
#define strrchr_symbol				0x143100

#define ps2_disc_auth_symbol			0x8B2BC
#define ps2_disc_auth_caller_symbol		0x22130C

#define overwritten_symbol			0x87BE0

/* Vars */
#define arguments_symbol			0x636E18

#endif /* PS2EMU type */

#elif defined(FIRMWARE_4_30)

#if defined(PS2HWEMU)

#define TOC					0x4EB750

#define cdvd_send_atapi_command_symbol		0x21FC4

#define ufs_open_symbol				0x4DF34
#define ufs_close_symbol			0x4E6C0
#define ufs_read_symbol				0x4DA04
#define ufs_write_symbol			0x4DAF8
#define ufs_fstat_symbol			0x4E9C4

#define printf_symbol				0x3BC4

#define zeroalloc_symbol			0xFDE70
#define malloc_symbol				0xFDD88
#define free_symbol				0xFDEC8
#define memcpy_symbol				0xDFAC
#define memset_symbol				0xD488
#define strcpy_symbol				0xED04
#define strncpy_symbol				0xED30
#define strcat_symbol				0xEED0
#define strlen_symbol				0xECD8
#define strcmp_symbol				0xED94
#define strncmp_symbol				0xEDE8
#define strchr_symbol				0xEE50
#define strrchr_symbol				0xEE8C

#define vuart_read_symbol			0x1E338
#define vuart_write_symbol			0x1E288

#define ps2_disc_auth_symbol			0x2053C
#define ps2_disc_auth_caller_symbol		0x51184

#define overwritten_symbol			0x23BF8

#define arguments_symbol			0x4FEBE8

#elif defined(PS2GXEMU)

#define TOC					0x678448

#define cdvd_read_symbol			0x8E364
#define cdvd_send_atapi_command_symbol		0x8D938
#define cdvd_send_device_command_symbol		0x8D84C

#define ufs_open_symbol				0x24EA70
#define ufs_close_symbol			0x24F218
#define ufs_read_symbol				0x24E780
#define ufs_write_symbol			0x24E8F8
#define ufs_fstat_symbol			0x24E5C0

#define log_printf_symbol			0x1E485C

#define zeroalloc_symbol			0x1E47AC
#define malloc_symbol				0x1E474C
#define free_symbol				0x1E47F8
#define memcpy_symbol				0x514BC
#define memset_symbol				0x51528
#define snprintf_symbol				0x52360
#define strcpy_symbol				0x51600
#define strncpy_symbol				0x99AB8
#define strcat_symbol				0x51634
#define strlen_symbol				0x515C8
#define strcmp_symbol				0x99AE4
#define strchr_symbol				0x99A44
#define strrchr_symbol				0x99A78

#define vuart_read_symbol			0x49544
#define vuart_write_symbol			0x494A4

#define ps2_disc_auth_symbol			0x8FFB4
#define ps2_disc_auth_caller_symbol		0x24F81C

#define overwritten_symbol			0x8C990

/* Calls */
#define reboot_parameters_vuart_call		0x58C0C

/* Vars */
#define arguments_symbol			0x6e7198

#endif /* type */

#elif defined(FIRMWARE_4_65)

#if defined(PS2HWEMU)

#define TOC					0x4EB840 //done

#define cdvd_send_atapi_command_symbol		0x220B8 //done

#define ufs_open_symbol				0x4E028 //done
#define ufs_close_symbol			0x4E7B4 //done
#define ufs_read_symbol				0x4DAF8 //done
#define ufs_write_symbol			0x4DBEC
#define ufs_fstat_symbol			0x4EAB8

#define printf_symbol				0x3BC4

#define zeroalloc_symbol			0xFDF60
#define malloc_symbol				0xFDE78
#define free_symbol				0xFDFB8
#define memcpy_symbol				0xDFAC
#define memset_symbol				0xD488
#define strcpy_symbol				0xED04
#define strncpy_symbol				0xED30
#define strcat_symbol				0xEED0
#define strlen_symbol				0xECD8
#define strcmp_symbol				0xED94
#define strncmp_symbol				0xEDE8
#define strchr_symbol				0xEE50
#define strrchr_symbol				0xEE8C

#define vuart_read_symbol			0x1E42C
#define vuart_write_symbol			0x1E37C

#define ps2_disc_auth_symbol			0x20630
#define ps2_disc_auth_caller_symbol		0x51278

#define overwritten_symbol			0x23CEC

#define arguments_symbol			0x4FED68

#elif defined(PS2GXEMU)

#define TOC					0x678548

#define cdvd_read_symbol			0x8E46C
#define cdvd_send_atapi_command_symbol		0x8DA40
#define cdvd_send_device_command_symbol		0x8D954

#define ufs_open_symbol				0x24EB70
#define ufs_close_symbol			0x24F318
#define ufs_read_symbol				0x24E880
#define ufs_write_symbol			0x24E9F8
#define ufs_fstat_symbol			0x24E6C0

#define log_printf_symbol			0x1E495C

#define zeroalloc_symbol			0x1E48AC
#define malloc_symbol				0x1E484C
#define free_symbol				0x1E48F8
#define memcpy_symbol				0x514BC
#define memset_symbol				0x51528
#define snprintf_symbol				0x52360
#define strcpy_symbol				0x51600
#define strncpy_symbol				0x99BC0
#define strcat_symbol				0x51634
#define strlen_symbol				0x515C8
#define strcmp_symbol				0x99BEC
#define strchr_symbol				0x99B4C
#define strrchr_symbol				0x99B80

#define vuart_read_symbol			0x49544
#define vuart_write_symbol			0x494A4

#define ps2_disc_auth_symbol			0x900BC
#define ps2_disc_auth_caller_symbol		0x24F91C

#define overwritten_symbol			0x8CA98

/* Calls */
#define reboot_parameters_vuart_call		0x58C0C

/* Vars */
#define arguments_symbol			0x6E7298

#elif defined(PS2NETEMU)

#define	cdvd_read_symbol			0x137744 // Found by Joonie

#define TOC							0x751280 //

#define ufs_open_symbol				0x1ECBDC // bytes matched: 0x60  7D800026F821FF31FB010090FB4100A0FB6100A8FBC100C07C7B1B78EB42BD08
#define ufs_close_symbol			0x1ECA90 // bytes matched: 0x38  F821FF717C0802A6FB810070FBC10080FBE100887C7E1B78EB82BD10EBE2BD08
#define ufs_read_symbol				0x1ED54C // bytes matched: 0x50  F821FF517C0802A6FB210078FB410080FB610088FB810090FBC100A0FBE100A8
#define ufs_write_symbol			0x1ED3D8 // Found by Joonie
#define ufs_fstat_symbol			0x1EC6E0 // bytes matched: 0x40  F821FF617C0802A6FB610078FB810080FBC10090FBE100987C9C2378EB62BD10

#define log_printf_symbol			0xB8E30  // bytes matched: 0x80  386000004E8000207C8323784E800020F821FF817C0802A6FBE10078F8010090

#define memcpy_symbol				0x118A10 // bytes matched: 0x80  2F8500007C0802A6F821FF91F8010080419E002C2FA3000078A500207C691B78
#define memset_symbol				0x1189BC // bytes matched: 0x80  2F8500007C0802A6F821FF91F8010080419E00282FA3000078A500207C691B78
#define memcmp_symbol				0x1187E0 // bytes matched: 0x80  38A500017C6B1B787CA903A66000000042400028892B0000396B000188040000
#define memchr_symbol				0x1E6D58 // bytes matched: 0x80  2C2500007CA903A6418200285484063E880300007F8020004D9E002042400014
#define snprintf_symbol				0x1196A4 // bytes matched: 0x80  F821FF817C0802A6F8C100C8F8010090380100C8788400207C060378F8010070
#define strcpy_symbol				0x11885C // bytes matched: 0x80  880400007C691B782F800000419E001C60000000980900008C04000139290001
#define strncpy_symbol				0x1EB5C0 // bytes matched: 0x80  38A500017C691B787CA903A66000000088040000388400012F80000098090000
#define strcat_symbol				0x118894 // bytes matched: 0x80  880300007C691B782F800000419E0014600000008C0900012F800000409EFFF8
#define strlen_symbol				0x118824 // bytes matched: 0x80  7C691B7838600000880900002F800000419E0020392900017D234B7888030000
#define strcmp_symbol				0x1EB5F0 // bytes matched: 0x80  880300007C691B782F800000419E0040886400002F830000419E004C7F830000
#define strncmp_symbol				0x1EB674 // bytes matched: 0x80  38A500017C6B1B787CA903A6892B0000396B00012F090000409A001088040000
#define strchr_symbol				0x1EB548 // bytes matched: 0x80  880300002F800000419E00247F802000409E000C4E8000204D9A00208C030001
#define strrchr_symbol				0x1EB57C // bytes matched: 0x80  88030000392000002F800000409E00144800002C8C0300012F800000419E0020


#endif /* type */

#endif  /* FIRMWARE */

#endif /* __PS2EMU_SYMBOLS_H_S__ */
