CC		:= ppu-gcc
CXX		:= ppu-g++
LD		:= ppu-ld 
OBJCOPY		:= ppu-objcopy
OBJDUMP		:= ppu-objdump
AR 		:= ppu-ar
STRIP		:= ppu-strip
INCLUDE		= ../lv2/include ../lv1/include ../debug/include
LIBSDIR         =
LIBS            =
BUILD_TYPE      = debug

CFLAGS = -ffunction-sections -fdata-sections -Os -m64 -fno-builtin -fsigned-char  -fno-exceptions \
			   -Os -Wall -Wno-strict-aliasing $(foreach dir,$(INCLUDE),-I$(dir)) \
			   -DLV2 -DFIRMWARE_4_46 -DCFW -ffreestanding 
CFLAGS += --std=gnu99

ifeq ($(BUILD_TYPE), debug)
CFLAGS += -DDEBUG
endif

LDFLAGS= -T stage2.ld -nostartfiles -nostdlib -nodefaultlibs -Wl,-static -Wl,-s -L. $(foreach dir,$(LIBSDIR),-L$(dir)) $(LIBS) \
	-Wl,--gc-sections -Wl,-Map=stage1_5.map

OBJS = start.o main.o gelic.o ../lv2/src/libc.o ../lv2/src/memory.o ../lv2/src/patch.o ../lv1/src/mm.o ../lv1/src/hvcall.o ../lv1/src/device.o  
	

ifeq ($(BUILD_TYPE), debug)
OBJS += ../lv2/src/interrupt.o ../debug/src/debug.o ../debug/src/printf.o 
endif

all: stage2.bin

objdump: stage2.bin
	$(OBJDUMP) -D -EB -b binary -m powerpc:common64 stage2.bin

%.bin: %.elf
	$(OBJCOPY) -O binary $< $@	

stage2.elf: $(OBJS) stage2.ld
	$(CC) $(LDFLAGS) -o $@ $(OBJS)

%.o: %.S
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o *.elf *.bin *.map ../lv2/src/*.o ../lv1/src/*.o ../debug/src/*.o
