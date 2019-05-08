# Makefile for the simple kernel.
CC	=gcc
AS	=as
LD	=ld
OBJCOPY = objcopy
OBJDUMP = objdump
NM = nm

LDFLAGS = -m elf_i386

CFLAGS = -m32 -Wall -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -fno-stack-protector -std=gnu11

# Add debug symbol
CFLAGS += -g

CFLAGS += -I.

OBJDIR = .

CPUS ?= 8

all: boot/boot kernel/system
	dd if=/dev/zero of=$(OBJDIR)/kernel.img count=10000 2>/dev/null
	dd if=$(OBJDIR)/boot/boot of=$(OBJDIR)/kernel.img conv=notrunc 2>/dev/null
	dd if=$(OBJDIR)/kernel/system of=$(OBJDIR)/kernel.img seek=1 conv=notrunc 2>/dev/null

run:
	qemu-system-i386 -hda kernel.img -curses -smp $(CPUS)

debug:
	qemu-system-i386 -hda kernel.img -curses -s -S -smp $(CPUS)

clean:
	rm $(OBJDIR)/boot/*.o $(OBJDIR)/boot/boot.out $(OBJDIR)/boot/boot $(OBJDIR)/boot/boot.asm || true
	rm $(OBJDIR)/kernel/*.o $(OBJDIR)/kernel/system* kernel.* || true
	rm $(OBJDIR)/lib/*.o || true
	rm -rf $(OBJDIR)/user/*.o || true
	rm -rf $(OBJDIR)/user/*.asm || true

include boot/Makefile
include kernel/Makefile
