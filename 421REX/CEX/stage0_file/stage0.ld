OUTPUT_FORMAT("elf64-powerpc")
OUTPUT_ARCH(powerpc:common64)

__base = 0x800000000029B9FC;

ENTRY(_start)

PHDRS {
	stage1		PT_LOAD FLAGS(7);
}

SECTIONS {
	. = __base;

	.start : { *(.start) } :stage1 = 0

	. = ALIGN(32);
	.text : {
		*(.text)
		*(.text.*)
	}	
}


