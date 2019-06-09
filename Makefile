SKETCH     ?= fanfriend.ino
PORT       ?= /dev/ttyACM0
PROGRAMMER ?= USBtinyISP
ARDUINODIR ?= ~/lib/arduino-1.8.9
ARDUINOCMD ?= arduino
POLLITER   ?= 240
BUILDDIR   ?= $(PWD)/build
POLLDELAY  := 0.125
IHEX       ?= build/$(SKETCH).with_bootloader.hex
FILES      += $(SKETCH)
FILES      += $(wildcard *.c)
FILES      += $(wildcard *.h)
FILES      += $(wildcard *.cpp)

all: $(IHEX) upload

clean:
	-rm -rf build

$(IHEX): $(FILES)
	@test -d "$(BUILDDIR)" || mkdir -p "$(BUILDDIR)"
#	Build the .ino into a big pile of Arduino, producting a
#	bootloader+payload Intel Hex (.hex) file that avrdude can upload.
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