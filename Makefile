ASM  = nasm

CC   = i686-elf-gcc
LD   = i686-elf-ld
OBJCOPY = objcopy

OBJFILES = build/kernel/kernel.o build/kernel/vid.o build/kernel/kb.o build/kernel/power.o build/kernel/sh.o build/kernel/mem.o build/kernel/fs.o


CFLAGS = -w -m16 -mgeneral-regs-only -c -ffreestanding -nostdlib -fno-pic

QEMU = qemu-system-x86_64

OS = build\os\os.img

all:
	@echo 1) Build bootloader
	$(ASM) -f bin boot/boot.asm -o build/boot/boot.bin

	@echo 2) Build kernel
	$(CC) $(CFLAGS) kernel/kernel.c -o build/kernel/kernel.o
	$(CC) $(CFLAGS) kernel/drivers/video/vid.c -o build/kernel/vid.o
	$(CC) $(CFLAGS) kernel/drivers/keyboard/kb.c -o build/kernel/kb.o
	$(CC) $(CFLAGS) kernel/drivers/power/power.c -o build/kernel/power.o
	$(CC) $(CFLAGS) kernel/shell/sh.c -o build/kernel/sh.o
	$(CC) $(CFLAGS) kernel/memory/mem.c -o build/kernel/mem.o
	$(CC) $(CFLAGS) kernel/fs/fs.c -o build/kernel/fs.o
	$(LD) -Ttext=0x10000 --oformat=binary -o build/kernel/kernel.bin $(OBJFILES)

	@echo 3) Build img
	copy /b build\boot\boot.bin + build\kernel\kernel.bin $(OS)
	fsutil file createnew build\padding 1474560
	copy /b build\os\os.img + build\padding $(OS)

run:
	@echo 4) Run
	$(QEMU) -hda $(OS)

clean:
	del /q build\*.* build\boot\*.* build\kernel\*.* build\os\*.*

disasm:
	gcc -S kernel/kernel.c 
