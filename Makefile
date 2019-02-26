
TARGET := led.bin
BUILD  := led

ENV    ?= SD
SDTOOLS := ./mk210

COBJS  += main.o
COBJS  += start.o
COBJS  += led.o
COBJS  += Uart.o
COBJS  += Key.o
CROSS_COMPILE := arm-linux-gnueabi-

CC     := $(CROSS_COMPILE)gcc
LD     := $(CROSS_COMPILE)ld
OBJCOPY := $(CROSS_COMPILE)objcopy

CFLAGS += -Wall
CFLAGS += -I./inc

LDFLAGS += -Tmap.lds
ifeq ($(ENV), SD)
LDFLAGS += -Ttext=0xD0020010
else
LDFLAGS += -Ttext=0x20000000
endif


all:$(TARGET)
ifeq ($(ENV), RAM)
$(TARGET):$(BUILD)
	$(OBJCOPY) -O binary $^ $@
	cp $@ ./binary_produced -f
else
$(TARGET):$(BUILD)
	$(OBJCOPY) -O binary $^ $@.TMP
	$(SDTOOLS) $@.TMP $@
	cp $@ ./binary_produced -f
endif

$(BUILD):$(COBJS)
	$(LD) $(LDFLAGS) -o $@ $^

%.o:%.c
	$(CC) $(CFLAGS) -c -o $@ $^
%.o:%.S
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	rm -f $(TARGET) $(BUILD) *.o *.TMP
