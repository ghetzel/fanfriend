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
LUFA       ?= false
ROOT        = $(PWD)
PORT       ?= /dev/ttyACM0
CC_FLAGS   += -DUSE_LUFA_CONFIG_HEADER -Isrc
CPP_FLAGS  +=

ifeq ($(LUFA),true)
CC         ?= avr-gcc
SRC        += $(wildcard src/*.c)
SRC        += $(wildcard src/*.cpp)
SRC        += $(LUFA_SRC_USB)
SRC        += $(LUFA_SRC_USBCLASS)
else
SRC        += $(wildcard src/*.ino)
IHEX       ?= $(ROOT)/build/$(notdir $(SRC)).adafruit.avr.adafruit32u4.hex
TARGET     := $(IHEX)
endif

LIBS       += $(wildcard lib/*)
LUFA_PATH  = lib/LUFA
LUFA_URL   ?= https://github.com/abcminiuser/lufa.git
LUFA_REF   ?= 5ba628d10b54d58d445896290ba9799bd76a73b3
POLLITER   ?= 240
BUILDDIR   ?= $(ROOT)/build
TARGET     ?= $(ROOT)/$(BUILDDIR)/fanfriend
POLLDELAY  := 0.125

all: arduino

clean:
	-rm -rf build obj

arduino: $(IHEX)

$(IHEX): $(SRC) $(wildcard src/*.h)
	@test -d "$(BUILDDIR)" || mkdir -p "$(BUILDDIR)"
	cd src && arduino-cli compile \
		--fqbn adafruit:avr:adafruit32u4 \
		--build-path "$(BUILDDIR)" \
		$(notdir $(SRC))
	mv src/*.hex src/*.elf $(BUILDDIR)/

ifeq ($(LUFA),true)
update-libs:
	@-rm -rf vcs
	@test -d vcs || mkdir vcs
	@echo "  [GIT]\tCloning LUFA ($(LUFA_URL))..."
	@git clone --quiet $(LUFA_URL) vcs/lufa
	@echo "  [REF]\tUsing LUFA revision $(LUFA_REF)..."
	@cd vcs/lufa && git checkout --quiet $(LUFA_REF)
	@echo "  [CP]\tCopying LUFA to vendor directory..."
	@rsync -a --delete-after vcs/lufa/LUFA/ lib/LUFA/
	@rm -rf vcs

$(LUFA_PATH):
	-rm -rf lib/LUFA lib/lufa-*
	@test -d "$(dir $(LUFA_PATH))" || mkdir -p "$(dir $(LUFA_PATH))"
	@wget -O "$(dir $(LUFA_PATH))/lufa-master.zip" https://github.com/abcminiuser/lufa/archive/master.zip
	@cd "$(dir $(LUFA_PATH))" && unzip lufa-master.zip
	@mv lib/lufa-master/LUFA lib/
	@rm -rf lib/lufa-master*

ifneq ($(wildcard $(LUFA_PATH)/*),)
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

endif
endif

upload: $(IHEX)
	@echo "Waiting for $(PORT)..."
	@for i in $(shell seq $(POLLITER)); do test -c $(PORT) && break || sleep $(POLLDELAY); done
	@test -c $(PORT)
	sudo avrdude -p m32u4 -P $(PORT) -c avr109 -U "flash:w:$(TARGET)"

erase:
	@echo "Waiting for $(PORT)..."
	@for i in $(shell seq $(POLLITER)); do test -c $(PORT) && break || sleep $(POLLDELAY); done
	@test -c $(PORT)
	sudo avrdude -p m32u4 -P $(PORT) -c avr109 -e