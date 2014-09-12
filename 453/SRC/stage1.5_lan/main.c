#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <lv2/lv2.h>
#include <lv2/libc.h>
#include <lv2/memory.h>
#include <lv2/patch.h>

#include <debug.h>

#include <lv1/lv1.h>
#include "gelic.h"

// WARNING: increase if you exceed this value in payload
#define MAX_PAYLOAD_SIZE			(128000)

#ifdef DEBUG
#define DPRINTF _debug_printf
#else
#define DPRINTF(...)
#endif



int main(void)
{
	u8 *payload, *stage2;
	int payload_size, result;
		
#ifdef DEBUG
	debug_init();	
#endif

	DPRINTF("Stage 1.5 lan hello.\n");	
	
	result = gelic_init();
	if (result != 0)
		goto error;
	
	payload = (void *)MKA(0x700000);//alloc(MAX_PAYLOAD_SIZE, 0x27);
	if (!payload)
		goto error;
	
	payload_size = gelic_recv_data(payload, MAX_PAYLOAD_SIZE);
	if (payload_size <= 0)
		goto error;	
		
	DPRINTF("Receive data: %d\n", payload_size);
	
	stage2 = alloc(payload_size, 0x27);
	if (!stage2)
		goto error;
	
	memcpy(stage2, payload, payload_size);
	clear_icache(stage2, payload_size);
	memset(payload, 0, payload_size);
	
	//dealloc(payload, 0x27);

	result = gelic_deinit();
	if (result != 0)
		goto error;

	/*result = mm_deinit();
	if (result != 0)
		goto error;*/

	f_desc_t desc;	
	desc.addr = stage2;
	
	DPRINTF("Calling stage2...\n");
	debug_end();
	void (* stage2_func)(void) = (void *)&desc;
	stage2_func();	
	
	return 0;

error:

	lv1_panic(0);
	return -1;
}
