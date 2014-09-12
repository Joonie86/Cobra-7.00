BUILD_TYPE = debug

OUTPUT := netiso_test
OBJS=main.o 
CFLAGS=-Wall -I. -std=gnu99 -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64
LDFLAGS=-L. 
LIBS = -lpthread

ifeq ($(BUILD_TYPE), debug)
CFLAGS += -DDEBUG
endif

all: $(OUTPUT)

clean:
	rm -f $(OUTPUT) *.o

$(OUTPUT): $(OBJS)
	$(LINK.c) $(LDFLAGS) -o $@ $^ $(LIBS)
