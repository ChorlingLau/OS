path	:= /OSLAB/compiler/usr
GCC		:= $(path)/bin/mips_4KC-gcc
LD		:= $(path)/bin/mips_4KC-ld
OBJDUMP	:= $(path)/mips-linux/bin/objdump
all: sample.c
	$(GCC) -E sample.c -I ./ >result_pre.txt
	$(GCC) -c sample.c -o sample.o
	$(GCC) -c add.c -o add.o
	$(OBJDUMP) -DS sample.o >result_cp.txt
	$(LD) sample.o add.o -o sample
	$(OBJDUMP) -DS sample >result_link.txt
clean:
	rm *.txt
