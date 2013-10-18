# for avrdude: [mt]\d+, gcc: at(mega|tiny)\d+
# i.e. m -> atmega, t -> attiny
AVRDUDEPART := $(PART)
GCCPART := $(subst m,mega,$(PART))
GCCPART := $(PART:m%=mega%)
GCCPART := $(GCCPART:t%=tiny%)
GCCPART := at$(GCCPART)
# use with bad hardware or slow chips
AVRDUDEDELAY ?= 1
# port is not needed with usb devices, but would work like this:
#PORT ?= /dev/parport0
#AVRDUDE_OPTS = -cbsd -p$(AVRDUDEPART) -P$(PORT) -i$(AVRDUDEDELAY) -E noreset
# (note also that you can put some variables in ${HOME}/.avrduderc)
# usb programmer style:
AVRDUDE_OPTS = -cusbtiny -p$(AVRDUDEPART) -B$(AVRDUDEDELAY)

CFLAGS := -DF_CPU=$(F_CPU) -mmcu=$(GCCPART) -g -Os -Wall -Wextra -std=c99 -o $(PROG).out -fshort-enums $(CFLAGS) $(LDFLAGS)

# Here be dragons! Watch out of some of these.
# -ffunction-sections, --gc-sections: each function in its own section and remove unneeded stuff individually
# (remember __attribute__((used)) and -Wl,--undefined and KEEP(*(.vectors)))
# --relax: short jumps (call -> rcall, jmp -> rjmp) if applicable
# -mcall-prologues: big prologues -> make them subroutines
# -combine -fwhole-program: treat all compile units as one: treat all functions as static
# -ffreestanding: dont care about main being void etc, but also assume that we provide memcpy, strlen etc builtins, use with caution
#CFLAGS := -ffunction-sections -fdata-sections -Wl,--print-gc-sections -Wl,--gc-sections -Wl,--relax \
#-mcall-prologues -combine -fwhole-program -Wdisabled-optimization $(CFLAGS)

# With bootloader: -Wl,-Ttext=0x3800 (check addr in datasheet)
# keep in mind also -Wl,--undefined=_jumptable (faq)


all: $(PROG).hex

flash: $(PROG).hex
	avrdude $(AVRDUDE_OPTS) -U flash:w:$(PROG).hex:i

$(PROG).hex: $(PROG).out
	avr-objcopy -j .text -j .data -O ihex $(PROG).out $(PROG).hex

asmcheck: $(PROG).lst
	vim $(PROG).lst

dumpcheck: $(PROG).dump.lst
	vim $(PROG).dump.lst
	
$(PROG).dump.lst: $(PROG).out
	avr-objdump -S $(PROG).out > $(PROG).dump.lst

$(PROG).lst: $(PROG).out

$(PROG).out: $(FILES)
	avr-gcc $(CFLAGS) -Wa,-adhlmns=$(PROG).lst $(FILES)
	avr-size -d $(PROG).out

clean:
	rm -f $(PROG).out $(PROG).hex $(PROG).lst $(PROG).dump.lst

read:
	avrdude $(AVRDUDE_OPTS) -U flash:r:out.hex:i

readfuses:
	avrdude $(AVRDUDE_OPTS) -U hfuse:r:-:h -U lfuse:r:-:h -U efuse:r:-:h || true

writehighfuse:
	avrdude $(AVRDUDE_OPTS) -U hfuse:w:$(shell read -p "High fuse (e.g. 0xzz): "; echo $$REPLY):m

writelowfuse:
	avrdude $(AVRDUDE_OPTS) -U lfuse:w:$(shell read -p "Low fuse (e.g. 0xzz): "; echo $$REPLY):m

writeextfuse:
	avrdude $(AVRDUDE_OPTS) -U efuse:w:$(shell read -p "Extended fuse (e.g. 0xzz): "; echo $$REPLY):m

readeeprom:
	avrdude $(AVRDUDE_OPTS) -U eeprom:r:eeprom.bin:r

readeepromhex:
	avrdude $(AVRDUDE_OPTS) -U eeprom:r:eeprom.hex:h
	
writeeeprom:
	avrdude $(AVRDUDE_OPTS) -U eeprom:w:eeprom.bin:r

help:
	@echo all flash asmcheck dumpcheck clean read readfuses writehighfuse writelowfuse readeeprom readeepromhex writeeeprom help
	
.PHONY: all flash asmcheck dumpcheck clean read readfuses writehighfuse writelowfuse readeeprom readeepromhex writeeeprom help
