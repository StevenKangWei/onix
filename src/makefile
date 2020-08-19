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

ASMFLAGS= -I $(INCLUDE) -f elf
BOOTFLAGS= -I $(BOOT_INC) -f bin
CFLAGS = -I $(INCLUDE) -c -fno-builtin -m32 -fno-stack-protector 
LDFLAGS = -m elf_i386 -s 

.PHONY: image clean qemu bochs

$(BUILD)/boot.bin: $(BOOT)/boot.asm
	$(ASM) $(ASM_BIN_FLAGS) -o $@ $<

$(BUILD)/loader.bin: $(BOOT)/loader.asm
	$(ASM) $(ASM_BIN_FLAGS) -o $@ $<

$(BUILD)/kernel.a: $(KERNEL)/kernel.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(BUILD)/kernel.o: $(KERNEL)/kernel.c
	$(CC) $(CFLAGS) $< -o $@

$(BUILD)/sys.o: $(LIB)/sys.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(BUILD)/io.o: $(KERNEL)/io.c
	$(CC) $(CFLAGS) $< -o $@

$(BUILD)/stdio.o: $(LIB)/stdio.c
	$(CC) $(CFLAGS) $< -o $@

$(BUILD)/kernel.bin: \
	$(BUILD)/kernel.a $(BUILD)/kernel.o $(BUILD)/io.o $(BUILD)/sys.o \
	$(BUILD)/stdio.o 
	$(LD) $(LDFLAGS) $^ -o $@

$(BUILD)/onix.img: $(BUILD)/boot.bin $(BUILD)/loader.bin $(BUILD)/kernel.bin
	dd if=/dev/zero of=$@ bs=512 count=2880
	dd if=$< of=$(BUILD)/$@ bs=512 count=1 conv=notrunc
	sudo mount -o loop $(BUILD)/$@ /mnt/floppy
	sudo cp $(BUILD)/loader.bin /mnt/floppy -v
	sudo cp $(BUILD)/kernel.bin /mnt/floppy -v
	sudo umount /mnt/floppy

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