GCCPARAMS = -m32 -Iinclude -nostdlib -nostdinc -fno-builtin -fno-exceptions -fno-leading-underscore
ASPARAMS = -f elf32
LDPARAMS = -melf_i386
objects = output/loader.o output/descriptors/gdt.o \
	output/descriptors/idt.o output/interrupts/isr.o \
	output/interrupts/irq.o output/hw/port.o output/hw/timer.o \
	output/driver/keyboard.o output/std/memory.o output/std/string.o \
	output/std/screen.o output/kernel.o

run: kernel.iso
	(killall VirtualBox && sleep 1) || true
	VirtualBox --startvm 'OS' &

output/%.o: code/%.c
	mkdir -p $(@D)
	gcc $(GCCPARAMS) -c -o $@ $<
	
output/%.o: code/%.s
	mkdir -p $(@D)
	nasm $(ASPARAMS) -o $@ $<
	
kernel.bin: linker.ld $(objects)
	ld $(LDPARAMS) -T $< -o $@ $(objects)

kernel.iso: kernel.bin
	mkdir -p iso/boot/grub
	cp    kernel.bin iso/kernel.bin
	#echo 'set timeout=15'                      > iso/boot/grub/grub.cfg
	echo 'set default=0'                      > iso/boot/grub/grub.cfg
	echo ''                                  >> iso/boot/grub/grub.cfg
	echo 'menuentry "OS" {'			 >> iso/boot/grub/grub.cfg
	echo '  multiboot /kernel.bin'   	 >> iso/boot/grub/grub.cfg
	echo '  boot'                            >> iso/boot/grub/grub.cfg
	echo '}'                                 >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=kernel.iso iso
	#sudo rm -rf iso

install: kernel.bin
	sudo cp $< /boot/kernel.bin
	
.PHONY: clean
clean:
	rm -rf iso output kernel.bin kernel.iso