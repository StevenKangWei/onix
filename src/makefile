BUILD:=../build
BOOT:=./boot

ASM=nasm
CC=gcc
LD=ld
BXIMAGE=bximage

ASM_BIN_FLAGS= -I boot/include/
ASM_KERNEL_FLAGS= -I include/ -f elf

.PHONY: image clean qemu bochs

$(BUILD)/boot.bin: $(BOOT)/boot.asm
	$(ASM) $(ASM_BIN_FLAGS) -o $@ $<

$(BUILD)/loader.bin: $(BOOT)/loader.asm
	$(ASM) $(ASM_BIN_FLAGS) -o $@ $<

$(BUILD)/onix.img: $(BUILD)/boot.bin $(BUILD)/loader.bin
	dd if=/dev/zero of=$@ bs=512 count=2880
	dd if=$< of=$(BUILD)/$@ bs=512 count=1 conv=notrunc
	sudo mount -o loop $(BUILD)/$@ /mnt/floppy
	sudo cp $(BUILD)/loader.bin /mnt/floppy -v
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