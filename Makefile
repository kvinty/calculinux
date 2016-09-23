LINUX_VERSION=4.7.4
SYSLINUX_VERSION=6.03

.PHONY: all syslinux linux build clean

all: syslinux build clean

syslinux-$(SYSLINUX_VERSION).tar.xz:
	wget https://www.kernel.org/pub/linux/utils/boot/syslinux/syslinux-$(SYSLINUX_VERSION).tar.xz

syslinux: syslinux-$(SYSLINUX_VERSION).tar.xz
	rm -rf syslinux-$(SYSLINUX_VERSION)
	tar xf syslinux-$(SYSLINUX_VERSION).tar.xz
	mv syslinux-$(SYSLINUX_VERSION) syslinux

linux-$(LINUX_VERSION).tar.xz:
	wget https://www.kernel.org/pub/linux/kernel/v4.x/linux-$(LINUX_VERSION).tar.xz

linux: linux-$(LINUX_VERSION).tar.xz
	rm -rf linux-$(LINUX_VERSION)
	tar xf linux-$(LINUX_VERSION).tar.xz
	mv linux-$(LINUX_VERSION) linux

build:
	mkdir -p iso/isolinux/
	cp syslinux/bios/core/isolinux.bin iso/isolinux/
	cp syslinux/bios/com32/elflink/ldlinux/ldlinux.c32 iso/isolinux/
	cp isolinux.cfg iso/isolinux/
	cp vmlinuz iso/
	gcc -pipe -std=c11 -m32 -ffreestanding -funsigned-char -fno-asynchronous-unwind-tables -O3 -fno-ident -o init userspace.c -lc -lm -static -Wl,--gc-sections,--strip-all,--build-id=none
	strip -R .comment -R .note.ABI-tag -R .gnu.version -R .hash -R .eh_frame -R .eh_frame_hdr -R .got -R .jcr init
	echo init | cpio -o --format=newc | gzip > iso/initrd.gz
	genisoimage -input-charset utf8 -l -J -R -V calculinux -nobak -no-emul-boot -boot-load-size 4 -boot-info-table -b isolinux/isolinux.bin -c isolinux/boot.cat -o calculinux.iso iso
	isohybrid calculinux.iso

clean:
	rm -rf init iso/ syslinux/
