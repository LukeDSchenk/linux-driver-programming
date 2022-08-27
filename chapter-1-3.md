# Chapter 1: Introduction

## What is a Kernel Module?

* Kernel modules are pieces of code which can be loaded and unloaded into the kernel on demand
  * They extend kernel functionality without the need for a reboot
* Without kernel modules, all changes to the kernel would have to be built directly into the kernel image, requiring a rebuild and reboot

## 1.4 Kernel module package

* modprobe, insmod, and depmod can be installed on Linux Mint through the `kmod` package

## 1.5 What Modules are in my Kernel?

* `lsmod` can be used to show modules currently loaded in your kernel
* Modules are stored within `/proc/modules`, so you can also see them by reading that file

## Before We Begin

There are a couple of things to keep in mind when compiling kernel modules for your system:

1. **Modversioning:** A module compiled for one kernel will not load when booting a different kernel unless you enable `CONFIG_MODVERSIONS` in the kernel
  * Most stock Linux distributions come with Modversioning turned on
2. It is highly recommended to extract, compile and load the examples in this guide from a console and not from X Window System
  * Modules cannot print to the screen like printf(), but they can log info and warnings (these logs are only printed to the screen in a console)
  * *I personally recommend just opening an extra terminal window and running `tail -f /var/log/kern.log` to see live updates of your modules' logs* 🥴

# Chapter 2: Headers

Before beginning, your Kernel header files must be installed (if they are not already):

```bash
sudo apt-get update
apt-cache search linux-headers-`uname -r`
```

The above command tells you which kernel header files are available, then they can be installed with apt.

# Chapter 3: Examples

* All examples from this doc are available in the examples subdirectory
* If there are compile errors, you may have a more recent kernel version or need to install the proper kernel header files
