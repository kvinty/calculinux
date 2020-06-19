CC=musl-gcc
CFLAGS=-ffreestanding -std=c11 -Wall -Wextra -Wpedantic -O3
LDFLAGS=-lc -lm -static -Wl,--gc-sections,--strip-all

SYSLINUX_VERSION=6.03
SYSLINUX_URL=https://www.kernel.org/pub/linux/utils/boot/syslinux/syslinux-$(SYSLINUX_VERSION).tar.xz

LINUX_VERSION=5.7.2
LINUX_URL=https://www.kernel.org/pub/linux/kernel/v5.x/linux-$(LINUX_VERSION).tar.xz

.PHONY: all linux_preconfig linux_config linux_compile linux_copy build test

all: linux_compile linux_copy build test

syslinux-$(SYSLINUX_VERSION).tar.xz:
	wget $(SYSLINUX_URL)

syslinux: syslinux-$(SYSLINUX_VERSION).tar.xz
	rm --recursive --force syslinux-$(SYSLINUX_VERSION) syslinux
	tar --extract --file syslinux-$(SYSLINUX_VERSION).tar.xz
	mv syslinux-$(SYSLINUX_VERSION) syslinux

linux-$(LINUX_VERSION).tar.xz:
	wget $(LINUX_URL)

linux: linux-$(LINUX_VERSION).tar.xz
	rm --recursive --force linux-$(LINUX_VERSION) linux
	tar --extract --file linux-$(LINUX_VERSION).tar.xz
	mv linux-$(LINUX_VERSION) linux

linux_preconfig:
	cp linux.cfg linux/.config
	touch linux/.config

linux_config:
	make --directory linux menuconfig

linux_compile:
	make --directory linux --jobs `nproc` bzImage

linux_copy:
	cp linux/.config linux.cfg
	cp linux/arch/x86/boot/bzImage vmlinuz

build: syslinux
	rm --recursive --force init iso/
	mkdir --parents iso/isolinux/
	cp syslinux/bios/core/isolinux.bin iso/isolinux/
	cp syslinux/bios/com32/elflink/ldlinux/ldlinux.c32 iso/isolinux/
	cp isolinux.cfg iso/isolinux/
	cp vmlinuz iso/
	$(CC) $(CFLAGS) -o init userspace.c $(LDFLAGS)
	echo init | cpio --create --format newc | xz --check crc32 >iso/initrd.xz
	genisoimage -input-charset utf8 -full-iso9660-filenames -joliet -rock -volid calculinux -nobak -no-emul-boot -boot-load-size 4 -boot-info-table -eltorito-boot isolinux/isolinux.bin -eltorito-catalog isolinux/boot.cat -output calculinux.iso iso
	isohybrid calculinux.iso

test:
	x-terminal-emulator -e "qemu-system-x86_64 -display curses -drive format=raw,file=calculinux.iso"
