BOOT:= ./boot
BUILD:= ../build

ASM=nasm

.PHONY: clean

$(BUILD)/boot.bin: $(BOOT)/boot.asm
	$(ASM) $(BOOT)/boot.asm -o $(BUILD)/boot.bin

$(BUILD)/onix.img: $(BUILD)/boot.bin $(BUILD)/loader.bin $(BUILD)/kernel.bin
	dd if=/dev/zero of=$@ bs=512 count=2880
	dd if=$< of=$(BUILD)/$@ bs=512 count=1 conv=notrunc

bochs: $(BUILD)/onix.img
	-
	cd $(BUILD) && bochs -q

clean:
	rm -rf $(BUILD)/onix.img
	rm -rf $(BUILD)/boot.bin
	rm -rf $(BUILD)/*.o
	rm -rf $(BUILD)/*.a
