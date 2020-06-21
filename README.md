## About
* This project is a simple standalone calculator
* The idea is: linux kernel, isolinux bootloader and a small initramfs containing only the control program `/init` running with PID 1

## Files overview
* `linux.cfg` - linux configuration file
* `isolinux.cfg` - isolinux configuration file
* `userspace.c` - control program implementing the calculator
* `Makefile` - make script for building the distro

## How to build
* Make sure packages `libc6-dev`, `musl-tools`, `syslinux-utils`, `genisoimage`, `qemu-system-x86` (or their alternatives) are installed
* Run `make`
* This will download `linux` and `syslinux` sources, build them and generate the image

## How to use
* To test the oprating system, run `make test`
* The resulting image `calculinux.iso` can also be booted in VirtualBox

## Customization
* There is a lot of space for modifications and customizations
* Run `make linux_config`, `make linux_compile` and `make linux_copy` to customize the kernel
* To rebuild the image, run `make` once again

## ToDo
* Bare metal hardware support
* Loading via univesal bootloaders like refind
* EFI support
* Bash-like history using readline

## References
* [Minimal Linux: Kernel](https://web.archive.org/web/20120531230827/http://blog.nasirabed.com/2012/01/minimal-linux-kernel.html)
* [How to Build A Custom Linux Kernel For Qemu (2015 Edition)](https://mgalgs.github.io/2015/05/16/how-to-build-a-custom-linux-kernel-for-qemu-2015-edition.html)
* [Remastering TC](https://web.archive.org/web/20191011060753/http://wiki.tinycorelinux.net/wiki:remastering)
* [ISOLINUX](https://wiki.syslinux.org/wiki/index.php?title=ISOLINUX)
* [Emacs standing alone on a Linux Kernel](http://informatimago.com/linux/emacs-on-user-mode-linux.html)
* [Statically (cross) compiled vim for x86, x86-64 and mipsel](https://raymii.org/s/blog/Statically_cross_compiled_vim_for_x86_x86-64_and_mips.html)
* [How to Build A Custom Linux Kernel For Qemu](https://mudongliang.github.io/2017/09/12/how-to-build-a-custom-linux-kernel-for-qemu.html)
