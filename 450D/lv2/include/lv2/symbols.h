#elif defined(FIRMWARE_4_50DEX)

#define TOC						0x36EC40 //done

#define create_kernel_object_symbol			0x12658 //done
#define destroy_kernel_object_symbol			0x11FBC //done
#define destroy_shared_kernel_object_symbol		0x11930 //done
#define open_kernel_object_symbol			0x12CA8 //done
#define open_shared_kernel_object_symbol		0x12AB8 //done
#define close_kernel_object_handle_symbol		0x120D8 //done

#define alloc_symbol					0x66944 //done	
#define dealloc_symbol					0x66D80 //done
#define copy_to_user_symbol				0xFEA0 //done
#define copy_from_user_symbol				0x100BC //done
#define copy_to_process_symbol				0xFF58 //done
#define copy_from_process_symbol			0xFD68 //done
#define page_allocate_symbol				0x624B4 //done
#define page_free_symbol				0x61F18 //done
#define page_export_to_proc_symbol			0x62650 //done
#define page_unexport_from_proc_symbol			0x61E0C //done
#define kernel_ea_to_lpar_addr_symbol			0x723B8 //done
#define process_ea_to_lpar_addr_ex_symbol		0x79F58 //done
#define set_pte_symbol					0x604B0 //done
#define map_process_memory_symbol			0x79A64 //done
#define panic_symbol					0x27D944 //done

#define memcpy_symbol					0x81124 //done
#define memset_symbol					0x50E38 //done
#define memcmp_symbol					0x50148 //done
#define memchr_symbol					0x500F8 //done
#define printf_symbol					0x280E08 //done
#define printfnull_symbol				0x28588C //done
#define sprintf_symbol					0x52260 //done
#define snprintf_symbol					0x521CC //done
#define strcpy_symbol					0x50FE4 //done
#define strncpy_symbol					0x510AC //done
#define strlen_symbol					0x5100C //done
#define strcat_symbol					0x50F14 //done
#define strcmp_symbol					0x50F90 //done
#define strncmp_symbol					0x51038 //done
#define strchr_symbol					0x50F4C //done
#define strrchr_symbol					0x5111C //done

#define spin_lock_irqsave_ex_symbol			0x27DB14 //done
#define spin_unlock_irqrestore_ex_symbol		0x27DAE8 //done

#define create_process_common_symbol			0x27A928 //done
#define create_process_base_symbol			0x279FCC //done
#define load_process_symbol				0x5004 //done
#define process_kill_symbol				0x27A728 //done

#define ppu_thread_create_symbol			0x1465C //done
#define ppu_thread_exit_symbol				0x14714 //done
#define ppu_thread_join_symbol				0x14768 //done
#define ppu_thread_delay_symbol				0x2A50C //done
#define create_kernel_thread_symbol			0x265FC //done
#define create_user_thread_symbol			0x26D38 //done
#define create_user_thread2_symbol			0x26B5C //done
#define start_thread_symbol				0x25694 //done
#define run_thread_symbol				0x24E58 /* temp name */ //done
#define register_thread_symbol				0x27673C //done
#define allocate_user_stack_symbol			0x276F24 //done
#define deallocate_user_stack_symbol			0x276E8C //done

#define mutex_create_symbol				0x13CEC //done
#define mutex_destroy_symbol				0x13C84 //done
#define mutex_lock_symbol				0x13C7C //done
#define mutex_lock_ex_symbol				0x1F438 //done
#define mutex_trylock_symbol				0x13C78 //done
#define mutex_unlock_symbol				0x13C74 //done

#define cond_create_symbol				0x13E58 //done
#define cond_destroy_symbol				0x13E08 //done
#define cond_wait_symbol				0x13E00 //done
#define cond_wait_ex_symbol				0x20A24 //done
#define cond_signal_symbol				0x13DDC //done
#define cond_signal_all_symbol				0x13DB8 //done

#define semaphore_initialize_symbol			0x34398 //done
#define semaphore_wait_ex_symbol			0x340A0 //done
#define semaphore_trywait_symbol			0x33C88 //done
#define semaphore_post_ex_symbol			0x33DD4 //done

#define event_port_create_symbol			0x13728 //done
#define event_port_destroy_symbol			0x13B90 //done
#define event_port_connect_symbol			0x13C08 //done
#define event_port_disconnect_symbol			0x13B34 //done
#define event_port_send_symbol				0x13720 //done
#define event_port_send_ex_symbol			0x2D0EC //done

#define event_queue_create_symbol			0x13A30 //done
#define event_queue_destroy_symbol			0x139B8 //done
#define event_queue_receive_symbol			0x137FC //done
#define event_queue_tryreceive_symbol			0x138C8 //done

#define cellFsOpen_symbol				0x2B84B0 //done
#define cellFsClose_symbol				0x2B8318 //done
#define cellFsRead_symbol				0x2B8454 //done
#define cellFsWrite_symbol				0x2B83C0 //done
#define cellFsLseek_symbol				0x2B7C14 //done
#define cellFsStat_symbol				0x2B7CCC //done
#define cellFsUtime_symbol				0x2B963C //done ???
#define cellFsUnlink_internal_symbol			0x1A2330 //done

#define cellFsUtilMount_symbol				0x2B7988 //done
#define cellFsUtilUmount_symbol				0x2B795C //done
#define cellFsUtilNewfs_symbol				0x2B92D4 //done

#define pathdup_from_user_symbol			0x1A858C //done
#define	open_path_symbol				0x2B81E8 //done
#define open_fs_object_symbol				0x190A68 //done
#define close_fs_object_symbol				0x18F9A4 //done

#define storage_get_device_info_symbol			0x294E8C //done ???
#define storage_get_device_config_symbol		0x293518 //done
#define storage_open_symbol				0x2950B4 //done
#define storage_close_symbol				0x2938A0 //done
#define storage_read_symbol				0x29281C //FIXED!!!!!!!!!!!!!!!!!
#define storage_write_symbol				0x2926EC //done
#define storage_send_device_command_symbol		0x29233C //done
#define storage_map_io_memory_symbol			0x294D38 //done
#define storage_unmap_io_memory_symbol			0x294BF4 //done


#define new_medium_listener_object_symbol		0x9B25C //done
#define delete_medium_listener_object_symbol		0x9CA94 //done
#define set_medium_event_callbacks_symbol		0x9CDF8 //done

#define cellUsbdRegisterLdd_symbol			0x26EC2C //done
#define cellUsbdUnregisterLdd_symbol			0x26EBDC //done
#define cellUsbdScanStaticDescriptor_symbol		0x26FE2C //done ???
#define cellUsbdOpenPipe_symbol				0x26FEDC //done ???
#define cellUsbdClosePipe_symbol			0x26FE8C //done ???
#define cellUsbdControlTransfer_symbol			0x26FD90 //done
#define cellUsbdBulkTransfer_symbol			0x26FD10 //done

#define decrypt_func_symbol				0x380B4 //done
#define lv1_call_99_wrapper_symbol			0x52674 //done
#define modules_verification_symbol			0x5C1F4 //done
#define authenticate_program_segment_symbol		0x5D630 //done

#define prx_load_module_symbol				0x8B934 //done
#define prx_start_module_symbol				0x8A600 //done
#define prx_stop_module_symbol				0x8B9D8 //done
#define prx_unload_module_symbol			0x8A334 //done
#define prx_get_module_info_symbol			0x89D2C //done
#define prx_get_module_id_by_address_symbol		0x89C3C //done
#define prx_get_module_id_by_name_symbol		0x89C8C //done
#define prx_get_module_list_symbol			0x89DAC //done
#define load_module_by_fd_symbol			0x8AF64 //done
#define parse_sprx_symbol				0x88C58 //done
#define open_prx_object_symbol				0x81CE0 //done
#define close_prx_object_symbol				0x825F0 //done
#define lock_prx_mutex_symbol				0x89BE4 //done
#define unlock_prx_mutex_symbol				0x89BF0 //done

#define extend_kstack_symbol				0x72310 //done

#define get_pseudo_random_number_symbol			0x236E0C //done
#define md5_reset_symbol				0x167F24 //done
#define md5_update_symbol				0x1689C4 //done
#define md5_final_symbol				0x168B44 //done
#define ss_get_open_psid_symbol				0x2392B4 //done
#define update_mgr_read_eeprom_symbol			0x232900 //done
#define update_mgr_write_eeprom_symbol			0x232834 //done

#define ss_params_get_update_status_symbol		0x54280 //done		

#define syscall_table_symbol				0x383658 //done
#define syscall_call_offset				0x286370 //done??

#define read_bdvd0_symbol				0x1B34FC //done
#define read_bdvd1_symbol				0x1B5128 //done
#define read_bdvd2_symbol				0x1C22B0 //done

#define storage_internal_get_device_object_symbol	0x291DF4 //done

#define hid_mgr_read_usb_symbol				0x107F8C //done
#define hid_mgr_read_bt_symbol				0x101E64 //done

#define bt_set_controller_info_internal_symbol		0xF5ED0 /* just a name... */

/* Calls, jumps */
#define device_event_port_send_call			0x29F088 //done

#define ss_pid_call_1					0x222110 //done

#define process_map_caller_call				0x4D24 //done

#define read_module_header_call				0x81BD4 //done
#define read_module_body_call				0x671C //done
#define load_module_by_fd_call1				0x8B7D0 //done

#define shutdown_copy_params_call			0xAB3C //done

#define fsloop_open_call				0x2B8648 //done ???
#define fsloop_close_call				0x2B8698 //done ???
#define fsloop_read_call				0x2B86D8 //done ???

/* Patches */
#define shutdown_patch_offset				0xAB28 //done
#define module_sdk_version_patch_offset			0x275D64 //done
#define module_add_parameter_to_parse_sprxpatch_offset	0x8B040 //done		

#define user_thread_prio_patch				0x21CD8 //done
#define user_thread_prio_patch2				0x21CE4 //done

/* Rtoc entries */

#define io_rtoc_entry_1					-0xB8 //done
#define io_sub_rtoc_entry_1				-0x7EA0 //done

#define decrypt_rtoc_entry_2 				-0x65A8 //done
#define decrypter_data_entry				-0x7F10 //done

#define storage_rtoc_entry_1				0x22A0 //done

#define device_event_rtoc_entry_1			0x2628 //done

#define time_rtoc_entry_1				-0x75E0 //done
#define time_rtoc_entry_2				-0x75E8 //done

#define thread_rtoc_entry_1				-0x7660 //done

#define process_rtoc_entry_1				-0x77A0 //done

#define bt_rtoc_entry_1					-0x3548 //done

/* Permissions */
#define permissions_func_symbol				0x3560  /* before it was patch_func5+patch_func5_offset */ //done
#define permissions_exception1 				0x26BDC /* before it was patch_func6+patch6_func6_offset */ //done
#define permissions_exception2				0xC8CDC	/* before it was patch_func7+patch7_func7_offset */	//done
#define permissions_exception3				0x21DAC /* Added in v 3.0 */ //done

/* Legacy patches with no names yet */
/* Kernel offsets */
#define patch_data1_offset				0x402AC0 //done
#define patch_func2 					0x5CBAC //done
#define patch_func2_offset 				0x2C
#define patch_func8 					0x5990C //lv2open update patch //done
#define patch_func8_offset1 				0xA4 //lv2open update patch
#define patch_func8_offset2 				0x208 //lv2open update patch
#define patch_func9 					0x5D068 // must upgrade error //done
#define patch_func9_offset 				0x470
#define mem_base2					0x3D90 //done

/* vars */
// TODO: #define open_psid_buf_symbol			0x45218C
#define thread_info_symbol				0x39DAB0 //done

#endif /* FIRMWARE */

#endif /* __FIRMWARE_SYMBOLS_H_S__ */
