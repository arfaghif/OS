dd if=/dev/zero of=system.img bs=512 count=2880
nasm bootloader.asm -o bootloader
dd if=bootloader of=system.img bs=512 count=1 conv=notrunc
dd if=map.img of=system.img bs=512 count=1 seek=256 conv=notrunc
dd if=dir.img of=system.img bs=512 count=2 seek=257 conv=notrunc
dd if=arfas.img of=system.img bs=512 count=1 seek=22 conv=notrunc
dd if=sectors.img of=system.img bs=512 count=1 seek=259 conv=notrunc  
bcc -ansi -c -o matematika.o matematika.c
bcc -ansi -c -o teks.o teks.c
bcc -ansi -c -o fileIO.o fileIO.c
bcc -ansi -c -o folderIO.o folderIO.c
bcc -ansi -c -o kernel.o kernel.c
nasm -f as86 kernel.asm -o kernel_asm.o
ld86 -o kernel -d kernel.o kernel_asm.o folderIO.o fileIO.o matematika.o teks.o
dd if=kernel of=system.img bs=512 conv=notrunc seek=1