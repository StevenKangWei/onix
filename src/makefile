BUILD:=../build
LIB:=./lib
INCLUDE:=./include
KERNEL:=./kernel
DRIVERS:=./drivers
BOOT:=./boot
BOOT_INC:=./boot/include
FLOPPY:=/mnt/floppy/

ASM=nasm
CC=gcc
LD=ld

ASM_BIN_FLAGS= -I boot/include/
ASM_KERNEL_FLAGS= -I include/ -f elf

ASMFLAGS= -I $(INCLUDE) -f elf -g 
BOOTFLAGS= -I $(BOOT_INC) -f bin
CFLAGS = -I $(INCLUDE) -c -fno-builtin -m32 -fno-stack-protector \
		-nostdlib -nostdinc -nostartfiles -nodefaultlibs -Wall -Wextra 
		# -Werror 
ENTRYPOINT = 0x30400

LDFLAGS = -m elf_i386 -s -Ttext $(ENTRYPOINT)

.PHONY: image clean qemu bochs

$(BUILD)/boot.bin: $(BOOT)/boot.asm
	$(ASM) $(ASM_BIN_FLAGS) -o $@ $<

$(BUILD)/loader.bin: $(BOOT)/loader.asm
	$(ASM) $(ASM_BIN_FLAGS) -o $@ $<

$(BUILD)/kernel.a: $(KERNEL)/kernel.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(BUILD)/kernel.o: $(KERNEL)/kernel.c
	$(CC) $(CFLAGS) $< -o $@

$(BUILD)/mode.a: $(KERNEL)/mode.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(BUILD)/mode.o: $(KERNEL)/mode.c
	$(CC) $(CFLAGS) $< -o $@

$(BUILD)/io.o: $(KERNEL)/io.c
	$(CC) $(CFLAGS) $< -o $@

$(BUILD)/io.a: $(KERNEL)/io.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(BUILD)/interrupt.a: $(KERNEL)/interrupt.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(BUILD)/interrupt.o: $(KERNEL)/interrupt.c
	$(CC) $(CFLAGS) $< -o $@

$(BUILD)/keyboard.o: $(DRIVERS)/keyboard.c
	$(CC) $(CFLAGS) $< -o $@

$(BUILD)/clock.o: $(KERNEL)/clock.c
	$(CC) $(CFLAGS) $< -o $@

$(BUILD)/process.a: $(KERNEL)/process.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(BUILD)/process.o: $(KERNEL)/process.c
	$(CC) $(CFLAGS) $< -o $@

$(BUILD)/test.o: $(KERNEL)/test.c
	$(CC) $(CFLAGS) $< -o $@

$(BUILD)/syscall.a: $(KERNEL)/syscall.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(BUILD)/syscall.o: $(KERNEL)/syscall.c
	$(CC) $(CFLAGS) $< -o $@

$(BUILD)/memory.o: $(KERNEL)/memory.c
	$(CC) $(CFLAGS) $< -o $@

$(BUILD)/time.o: $(LIB)/time.c
	$(CC) $(CFLAGS) $< -o $@

$(BUILD)/audio.o: $(LIB)/audio.c
	$(CC) $(CFLAGS) $< -o $@

$(BUILD)/stdio.o: $(LIB)/stdio.c
	$(CC) $(CFLAGS) $< -o $@

$(BUILD)/string.o: $(LIB)/string.c
	$(CC) $(CFLAGS) $< -o $@

$(BUILD)/stdlib.o: $(LIB)/stdlib.c
	$(CC) $(CFLAGS) $< -o $@

$(BUILD)/kernel.bin: \
	$(BUILD)/kernel.a $(BUILD)/kernel.o $(BUILD)/io.o $(BUILD)/io.a $(BUILD)/memory.o \
	$(BUILD)/stdio.o $(BUILD)/time.o $(BUILD)/audio.o $(BUILD)/string.o $(BUILD)/clock.o\
	$(BUILD)/stdlib.o $(BUILD)/mode.a $(BUILD)/mode.o $(BUILD)/interrupt.o $(BUILD)/interrupt.a \
	$(BUILD)/process.a $(BUILD)/process.o $(BUILD)/syscall.o $(BUILD)/syscall.a $(BUILD)/test.o \
	$(BUILD)/keyboard.o 
	$(LD) $(LDFLAGS) $^ -o $@

$(BUILD)/onix.img: $(BUILD)/boot.bin $(BUILD)/loader.bin $(BUILD)/kernel.bin
	dd if=/dev/zero of=$@ bs=512 count=2880
	dd if=$< of=$(BUILD)/$@ bs=512 count=1 conv=notrunc
	sudo mount -o loop $(BUILD)/$@ /mnt/floppy
	sudo cp $(BUILD)/loader.bin /mnt/floppy -v
	sudo cp $(BUILD)/kernel.bin /mnt/floppy -v
	sudo umount /mnt/floppy

debug:CFLAGS = -I $(INCLUDE) -c -m32 -g -fno-stack-protector -DONIX_DEBUG 
debug:ASMFLAGS = -I $(INCLUDE) -f elf -F dwarf -g 
debug:LDFLAGS = -m elf_i386 -s 
debug: \
	$(BUILD)/kernel.o $(BUILD)/io.o $(BUILD)/syscall.o $(BUILD)/syscall.a \
	$(BUILD)/stdio.o $(BUILD)/time.o $(BUILD)/audio.o $(BUILD)/string.o \
	$(BUILD)/stdlib.o 
	$(CC) -DONIX_DEBUG -m32 $^ -o $(BUILD)/$@ 
	-

image: $(BUILD)/onix.img
	-

qemu: $(BUILD)/onix.img
	qemu-system-x86_64 $<

bochs: $(BUILD)/onix.img
	cd $(BUILD) && bochs -q

clean:
	rm -rf $(BUILD)/onix.img
	rm -rf $(BUILD)/*.bin
	rm -rf $(BUILD)/*.a
	rm -rf $(BUILD)/*.o
	rm -rf $(BUILD)/debug
