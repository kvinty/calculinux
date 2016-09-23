## About
This project is a simple calculator standing alone on the linux kernel. The idea is: linux kernel, isolinux bootloader and small initramfs containing just one file /init, the control program with PID 1.

## Files overview
* linux.cfg - .config used for compiling the linux kernel
* isolinux.cfg - isolinux config
* userspace.c - C program implementing the calculator
* Makefile - make script for building the distro
* vmlinuz - precompiled kernel
* calculinux.iso - ready-to-use image

## How to use
To test the oprating system, run `qemu-system-i386 calculinux.iso` or burn it on a CD/DVD/USB and launch on bare metal. System requirements: 686-compatible processor + ~20MiB RAM.

## Customization
You can use any version/architecture/bootloader/kernel you want.
To rebuild the image, do the following:
* Install gcc, syslinux-utils, genisoimage
* Compile a kernel (optionally, see below for details), or use the attached vmlinuz
* Read Makefile and modily it properly, update linux version etc.
* Run `make`
* Have fun!

## How to compile the kernel
* `make linux`
* `cd linux`
* `make allnoconfig`
* `make menuconfig`
* Configure the options (see below for details)
* `make -j4 bzImage`
* `cp arch/x86/boot/bzImage ../vmlinuz`
* `cd ..`

## How to configure the kernel
Enable at least these options to make the kernel work:
```
-> General setup  
  [*] Initial RAM filesystem and RAM disk (initramfs/initrd) support
    [*] Support initial ramdisks compressed using gzip
  [*] Embedded system
  -> Choose SLAB allocator
    [*] SLOB (Simple Allocator)

-> Processor type and features
  [ ] CPU microcode loading support
  -> High Memory Support
    [*] off

-> Executable file formats / Emulations
  [*] Kernel support for ELF binaries

-> Device Drivers
  -> Character devices
    [*] Enable TTY
      [*] Virtual terminal
        [*] Support for console on virtual terminal
    -> Serial drivers
      [*] 8250/16550 and compatible serial support
      [*] Console on 8250/16550 and compatible serial port

-> Kernel hacking
  -> printk and dmesg options
    (1) Default message log level (1-7)
```

## Kernel log levels
0. KERN_EMERG  
The system is unusable.
1. KERN_ALERT  
Actions that must be taken care of immediately.
2. KERN_CRIT  
Critical conditions.
3. KERN_ERR  
Non-critical error conditions.
4. KERN_WARNING  
Warning conditions that should be taken care of.
5. KERN_NOTICE  
Normal, but significant events.
6. KERN_INFO  
Informational messages that require no action.
7. KERN_DEBUG  
Kernel debugging messages, output by the kernel
if the developer enabled debugging at compile time.

## ToDo
* Fix `shutdown` (all the manuals state it should work, but it doesn't)
* Add bash-style history feature

## Useful links
* http://web.archive.org/web/20120531230827/http://blog.nasirabed.com/2012/01/minimal-linux-kernel.html
* http://mgalgs.github.io/2015/05/16/how-to-build-a-custom-linux-kernel-for-qemu-2015-edition.html
* http://wiki.tinycorelinux.net/wiki:remastering
* http://syslinux.org/wiki/index.php/ISOLINUX
* http://informatimago.com/linux/emacs-on-user-mode-linux.html
* http://raymii.org/s/blog/Statically_cross_compiled_vim_for_x86_x86-64_and_mips.html
