FILES ?= servotest.c servoctl.c
PART ?= 90usb1286
PROG ?= program
F_CPU ?= 16000000
CFLAGS ?= 
include ./avr-common.mk
