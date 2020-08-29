BUILD:=../build
LIB:=./lib
INCLUDE:=./include
KERNEL:=./kernel
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

# bootloader
$(BUILD)/boot.bin: $(BOOT)/boot.asm
	$(ASM) $(ASM_BIN_FLAGS) -o $@ $<

$(BUILD)/loader.bin: $(BOOT)/loader.asm
	$(ASM) $(ASM_BIN_FLAGS) -o $@ $<

# kernel start
$(BUILD)/kernel.a: $(KERNEL)/kernel.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(BUILD)/kernel.o: $(KERNEL)/kernel.c
	$(CC) $(CFLAGS) $< -o $@

$(BUILD)/io.a: $(KERNEL)/io.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(BUILD)/mode.a: $(KERNEL)/mode.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(BUILD)/mode.o: $(KERNEL)/mode.c
	$(CC) $(CFLAGS) $< -o $@

$(BUILD)/exception.a: $(KERNEL)/exception.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(BUILD)/exception.o: $(KERNEL)/exception.c
	$(CC) $(CFLAGS) $< -o $@

$(BUILD)/interrupt.a: $(KERNEL)/interrupt.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(BUILD)/interrupt.o: $(KERNEL)/interrupt.c
	$(CC) $(CFLAGS) $< -o $@

$(BUILD)/string.o: $(KERNEL)/string.c
	$(CC) $(CFLAGS) $< -o $@

$(BUILD)/console.o: $(KERNEL)/console.c
	$(CC) $(CFLAGS) $< -o $@

$(BUILD)/stdlib.o: $(KERNEL)/stdlib.c
	$(CC) $(CFLAGS) $< -o $@

$(BUILD)/stdio.o: $(KERNEL)/stdio.c
	$(CC) $(CFLAGS) $< -o $@

$(BUILD)/time.o: $(KERNEL)/time.c
	$(CC) $(CFLAGS) $< -o $@

$(BUILD)/memory.o: $(KERNEL)/memory.c
	$(CC) $(CFLAGS) $< -o $@

$(BUILD)/test.o: $(KERNEL)/test.c
	$(CC) $(CFLAGS) $< -o $@

$(BUILD)/kernel.bin: $(BUILD)/kernel.a $(BUILD)/kernel.o $(BUILD)/io.a \
	$(BUILD)/mode.a $(BUILD)/mode.o $(BUILD)/string.o $(BUILD)/console.o \
	$(BUILD)/stdlib.o $(BUILD)/stdio.o $(BUILD)/time.o $(BUILD)/test.o $(BUILD)/memory.o \
	$(BUILD)/exception.a $(BUILD)/exception.o  $(BUILD)/interrupt.a $(BUILD)/interrupt.o 
	$(LD) $(LDFLAGS) $^ -o $@

$(BUILD)/onix.img: $(BUILD)/boot.bin $(BUILD)/loader.bin $(BUILD)/kernel.bin
	dd if=/dev/zero of=$@ bs=512 count=2880
	dd if=$< of=$(BUILD)/$@ bs=512 count=1 conv=notrunc
	sudo mount -o loop $(BUILD)/$@ /mnt/floppy
	sudo cp $(BUILD)/loader.bin /mnt/floppy -v
	sudo cp $(BUILD)/kernel.bin /mnt/floppy -v
	sudo umount /mnt/floppy

# kernel end

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
