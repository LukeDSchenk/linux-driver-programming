cmd_/home/luke/Projects/linux-driver-programming/chapter-2/hello-world/hello-2.ko := ld -r -m elf_x86_64  -z max-page-size=0x200000  --build-id  -T ./scripts/module-common.lds -o /home/luke/Projects/linux-driver-programming/chapter-2/hello-world/hello-2.ko /home/luke/Projects/linux-driver-programming/chapter-2/hello-world/hello-2.o /home/luke/Projects/linux-driver-programming/chapter-2/hello-world/hello-2.mod.o;  true
