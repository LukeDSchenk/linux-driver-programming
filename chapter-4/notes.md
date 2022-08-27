# Chapter 4: Hello, World!

## 4.1 The Simplest Module

#### Some info about `Make` commands

* `make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules`
  * `-C` means change into that directory before running the make command (that directory appears to be a build dir for kernel modules?)
  * `M=` is actually just setting a variable for the build process; in this case it appears to be setting M to the current directory to be returned to after finishing

* You can use `modinfo <module_name>.ko` to retrieve information about a compiled module
* `lsmod` will show you the modules currently installed in the kernel
* Use `insmod <mod_name>.ko` to install a module in the running kernel
* Use 'rmmod <mod_name>' to remove a mod from the running kernel
* Kernel module logs can be viewed in the Systemd journal or in the file `/var/log/kern.log`

* Using Make for Kernel development uses the **kbuild** system
* In kbuild, obj-m refers to modules to be compiled, and obj-y refers to objects being compiled directly into the Kernel (quick thought, what if you recompiled someone's kernel but added your own additional modules to be built in to the kernel?)

* That crazy custom kernel menu you use when compiling a kernel for Gentoo is actually just setting different configurable parameters to be passed to the Makefile when compiling theh Linux kernel and all of its modules; the more you know!

#### Kernel module functions

* Kernel modules must each have a "start" function called `init_module()` and an "end" function called `cleanup_module()`
  * You can also use macros to specify functions with any name as start and end functions
* **"Typically, init_module() either registers a handler for something with the
kernel, or it replaces one of the kernel functions with its own code (usually code
to do something and then call the original function)"** - from the text

## 4.3 The __init and __exit Macros

* The __init macro makes the init function get discarded and its memory freed once the function is finished, but only for built in drivers
  * This does not apply for loadable modules
  * __initdata does the same thing as __init, but for variables instead of functions
* The __exit macro causes the omission of the function when the module is built in to the kernel (makes perfect sense because the module is never unloaded if built in)
* These macros are found in `include/linux/init.h` and are used to free up memory for the kernel
  * If you see a message like "*Freeing unused kernel memory*" while booting your kernel, that is precisely what is being freed

## 4.4 Licensing and Module Documentation

* Loading proprietary modules without license information will create a warning message similar to: *loading out-of-tree module taints kernel. module license 'unspecified' taints kernel.*
* There are a number of macros that can be used to document license, authorship, and module info

## 4.5 Passing Command Line Arguments to a Module

Modules can take command line arguments, but not with argc/argv like normal C programs.

* Pass CLI args by declaring the variables that will take the CLI values globally, and then use the `module_param()` macro (defined in *include/linux/moduleparam.h*)
  * At runtime, insmod will fill the variables with any command line arguments that are given: `insmod mymodule.ko myvariable=5`
* module_param() takes 3 args: name of the variable, its type, and permissions for the corresponding file in sysfs
* For arrays or string arguments, use module_param_array() and module_param_string()
