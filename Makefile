# Board Configuration
# --------------------------------------------------------------------------------------------------
# If you dont have this: https://www.adafruit.com/product/296, you're probably
# changing these values.
#
BOARD      ?= LEONARDO
MCU        ?= atmega32u4
F_CPU      ?= 16000000
F_USB      ?= $(F_CPU)
# --------------------------------------------------------------------------------------------------

CC         ?= avr-gcc
CC_FLAGS   += -DUSE_LUFA_CONFIG_HEADER -IConfig/
CPP_FLAGS  +=
SRC        += $(wildcard *.c)
SRC        += $(wildcard *.h)
SRC        += $(wildcard *.cpp)

POLLITER   ?= 240
BUILDDIR   ?= $(PWD)/build
POLLDELAY  := 0.125
IHEX       ?= build/$(SKETCH).with_bootloader.hex
FILES      += $(SKETCH)

all: $(IHEX) upload

clean:
	-rm -rf build

$(IHEX): $(FILES)
	@test -d "$(BUILDDIR)" || mkdir -p "$(BUILDDIR)"
	$(ARDUINODIR)/$(ARDUINOCMD) \
		--board adafruit:avr:adafruit32u4 \
		--pref build.path="$(BUILDDIR)" \
		--verify "$(SKETCH)"

build: $(IHEX)

upload: $(IHEX)
	@echo "Waiting for $(PORT)..."
	@for i in $(shell seq $(POLLITER)); do test -c $(PORT) && break || sleep $(POLLDELAY); done
	@test -c $(PORT)
	avrdude -D -p m32u4 -P $(PORT) -c avr109 -U "flash:w:$(IHEX)"

erase:
# 	@echo "Waiting for $(PORT)..."
# 	@for i in $(shell seq $(POLLITER)); do test -c $(PORT) && break || sleep $(POLLDELAY); done
# 	@test -c $(PORT)
	avrdude -p m32u4 -P $(PORT) -c avr109 -e