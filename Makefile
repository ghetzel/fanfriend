SKETCH     ?= fanfriend.ino
PORT       ?= /dev/ttyACM0
PROGRAMMER ?= USBtinyISP
ARDUINODIR ?= ~/lib/arduino-1.8.9
ARDUINOCMD ?= arduino
POLLITER   ?= 240
BUILDDIR   ?= $(PWD)/build
POLLDELAY  := 0.125
IHEX       := build/$(SKETCH).with_bootloader.hex

all: $(IHEX) upload

$(IHEX): $(SKETCH)
	@test -d "$(BUILDDIR)" || mkdir -p "$(BUILDDIR)"
#	Build the .ino into a big pile of Arduino, producting a
#	bootloader+payload Intel Hex (.hex) file that avrdude can upload.
	$(ARDUINODIR)/$(ARDUINOCMD) \
		--verbose \
		--board adafruit:avr:adafruit32u4 \
		--port "$(PORT)" \
		--pref build.path="$(BUILDDIR)" \
		--useprogrammer $(PROGRAMMER) \
		--verify "$(SKETCH)"

upload: $(IHEX)
	@echo "Waiting for $(PORT)..."
	@for i in $(shell seq $(POLLITER)); do test -c "$(PORT)" && break || sleep $(POLLDELAY); done
	@test -c "$(PORT)"
	avrdude -p m32u4 -P "$(PORT)" -c avr109 -U "flash:w:$(IHEX)"

erase:
	@echo "Waiting for $(PORT)..."
	@for i in $(shell seq $(POLLITER)); do test -c "$(PORT)" && break || sleep $(POLLDELAY); done
	@test -c "$(PORT)"
	avrdude -p m32u4 -P "$(PORT)" -c avr109 -e