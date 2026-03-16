ASM  = nasm

CC   = i686-elf-gcc
LD   = i686-elf-ld
OBJCOPY = objcopy

OBJFILES = build/kernel/kernel.o build/kernel/vid.o build/kernel/kb.o build/kernel/power.o build/kernel/sh.o build/kernel/mem.o build/kernel/fs.o build/kernel/files.o build/kernel/sectors.o build/kernel/exec.o build/kernel/interrupt.o build/kernel/program.o build/kernel/regs.o build/kernel/safety.o build/kernel/time.o build/kernel/date.o build/kernel/port.o


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
	$(CC) $(CFLAGS) kernel/fs/sectors.c -o build/kernel/sectors.o
	$(CC) $(CFLAGS) kernel/fs/files.c -o build/kernel/files.o
	$(CC) $(CFLAGS) kernel/exec/exec.c -o build/kernel/exec.o
	$(CC) $(CFLAGS) kernel/exec/interrupt.c -o build/kernel/interrupt.o
	$(CC) $(CFLAGS) kernel/exec/program.c -o build/kernel/program.o
	$(CC) $(CFLAGS) kernel/exec/regs.c -o build/kernel/regs.o
	$(CC) $(CFLAGS) kernel/safety/safety.c -o build/kernel/safety.o
	$(CC) $(CFLAGS) kernel/drivers/time/time.c -o build/kernel/time.o
	$(CC) $(CFLAGS) kernel/drivers/time/date.c -o build/kernel/date.o
	$(CC) $(CFLAGS) kernel/drivers/ports/port.c -o build/kernel/port.o
	$(LD) -Ttext=0x10000 --oformat=binary -o build/kernel/kernel.bin $(OBJFILES)

	@echo 3) Build img
	copy /b build\boot\boot.bin + build\kernel\kernel.bin $(OS)
	fsutil file createnew build\padding 1474560
	copy /b build\os\os.img + build\padding $(OS)

run:
	@echo 4) Run
	$(QEMU) -hda $(OS)

debug:
	@echo 4) Debug
	qemu-system-i386 -hda build/os/os.img -monitor stdio

clean:
	del /q build\*.* build\boot\*.* build\kernel\*.* build\os\*.*

disasm:
	gcc -S kernel/kernel.c 
