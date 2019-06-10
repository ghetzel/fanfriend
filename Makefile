# Board Configuration
# --------------------------------------------------------------------------------------------------
# If you dont have this: https://www.adafruit.com/product/296, you're probably
# changing these values.
#
ARCH       ?= AVR8
BOARD      ?= ADAFRUITU4
MCU        ?= atmega32u4
F_CPU      ?= 16000000
F_USB      ?= $(F_CPU)
# --------------------------------------------------------------------------------------------------

PORT       ?= /dev/ttyACM0
CC         ?= avr-gcc
CC_FLAGS   += -DUSE_LUFA_CONFIG_HEADER
CPP_FLAGS  +=
SRC        += $(wildcard *.c)
SRC        += $(wildcard *.cpp)
SRC        += $(LUFA_SRC_USB)
SRC        += $(LUFA_SRC_USBCLASS)

LUFA_PATH  = $(PWD)/lib/LUFA
POLLITER   ?= 240
BUILDDIR   ?= build
TARGET     ?= $(BUILDDIR)/fanfriend
POLLDELAY  := 0.125

all:

# $(LUFA_PATH):
# 	test -d "$(dir $(LUFA_PATH))" || mkdir -p "$(dir $(LUFA_PATH))"
# 	wget -O "$(dir $(LUFA_PATH))/lufa-master.zip" https://github.com/abcminiuser/lufa/archive/master.zip
# 	cd "$(dir $(LUFA_PATH))" && unzip lufa-master.zip
# 	mv $(LUFA_PATH)/LUFA .

# Include LUFA-specific DMBS extension modules
DMBS_LUFA_PATH ?= $(LUFA_PATH)/Build/LUFA
include $(DMBS_LUFA_PATH)/lufa-sources.mk
include $(DMBS_LUFA_PATH)/lufa-gcc.mk

# Include common DMBS build system modules
DMBS_PATH      ?= $(LUFA_PATH)/Build/DMBS/DMBS
include $(DMBS_PATH)/core.mk
include $(DMBS_PATH)/cppcheck.mk
include $(DMBS_PATH)/dfu.mk
include $(DMBS_PATH)/gcc.mk
include $(DMBS_PATH)/hid.mk
include $(DMBS_PATH)/avrdude.mk
include $(DMBS_PATH)/atprogram.mk

upload: $(IHEX)
	@echo "Waiting for $(PORT)..."
	@for i in $(shell seq $(POLLITER)); do test -c $(PORT) && break || sleep $(POLLDELAY); done
	@test -c $(PORT)
	sudo avrdude -p m32u4 -P $(PORT) -c avr109 -U "flash:w:$(TARGET).hex"

erase:
	@echo "Waiting for $(PORT)..."
	@for i in $(shell seq $(POLLITER)); do test -c $(PORT) && break || sleep $(POLLDELAY); done
	@test -c $(PORT)
	sudo avrdude -p m32u4 -P $(PORT) -c avr109 -e