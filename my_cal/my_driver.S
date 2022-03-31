#include <asm/regdef.h>
#include <asm/cp0regdef.h>
#include <asm/asm.h>


            .section .data.stk
KERNEL_STACK:
            .space 0x8000


            .text
LEAF(_my_getchar) /*LEAF is defined in asm.h and LEAF functions don't call other functions*/

    .set    mips2      /*.set is used to instruct how the assembler works and control the order of instructions */
    .set    reorder
while_1:
	lb t0, 0x10000000(zero)
	slt t7, t0, zero
	beqz t7, while_1
	nop
	sb t0, 0x10000000(zero)
	move v0, t0
END(_my_getchar)

LEAF(_my_putchar)
	sb a0, 0x10000000(zero)
END(_my_putchar)

LEAF(_my_exit)
	sw t0, 0x10000010(zero)
END(_my_exit)
