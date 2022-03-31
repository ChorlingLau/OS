.data 
array1: .space 256
array2: .space 256
space: .asciiz " "
enter: .asciiz "\n"
.macro addr(%dst, %i, %j)	# dst = i * 8 + j
	sll %dst, %i, 3
	addu %dst, %dst, %j
	sll %dst, %dst, 2
.end_macro
.text
input:	li $v0, 5
	syscall
	move $t0, $v0	# t0 is n
	
		li $t1, 0	# $t1 is i
for_1_begin:	slt $t7, $t1, $t0
		beqz $t7, for_1_end
		nop
		li $t2, 0	# $t2 is j
for_2_begin:	slt $t7, $t2, $t0
		beqz $t7, for_2_end
		nop
		
		li $v0, 5
		syscall
		
		addr($t4, $t1, $t2)
		sw $v0, array1($t4)
		
		addi $t2, $t2, 1	# j++
		j for_2_begin
		nop		
for_2_end:	addi $t1, $t1, 1	# i++
		j for_1_begin
		nop
for_1_end:	li $t1, 0	# $t1 is i
for_3_begin:	slt $t7, $t1, $t0
		beqz $t7, for_3_end
		nop
		li $t2, 0	# $t2 is j
for_4_begin:	slt $t7, $t2, $t0
		beqz $t7, for_4_end
		nop
		
		li $v0, 5
		syscall
		
		addr($t4, $t1, $t2)
		sw $v0, array2($t4)
		
		addi $t2, $t2, 1	# j++
		j for_4_begin
		nop		
for_4_end:	addi $t1, $t1, 1	# i++
		j for_3_begin
		nop		
for_3_end:	li $t1, 0	# $t1 is i		
for_5_begin:	slt $t7, $t1, $t0
		beqz $t7, for_5_end
		nop
		
		li $t2, 0	# $t2 is j
for_6_begin:	slt $t7, $t2, $t0
		beqz $t7, for_6_end
		nop
		
		li $t3, 0	# $t3 is k
		li $a0, 0	# result = 0		
for_7_begin:	slt $t7, $t3, $t0
		beqz $t7, for_7_end
		nop		

		addr($t4, $t1, $t3)
		lw $t5, array1($t4)	# $t5 is array1[i][k]
		addr($t4, $t3, $t2)
		lw $t6, array2($t4)	# $t6 is array2[k][j]
		mult $t5, $t6
		mflo $t7		# $t7 = array1[i][k]*array2[k][j]
		add $a0, $a0, $t7	# result += array1[i][k]*array2[k][j]
		
		addi $t3, $t3, 1	# k++
		j for_7_begin
		nop		

for_7_end:	li $v0, 1
		syscall
		
		la $a0, space
		li $v0, 4
		syscall
		
		addi $t2, $t2, 1	# j++
		j for_6_begin
		nop					
		
for_6_end:	la $a0, enter
		li $v0, 4
		syscall
		
		addi $t1, $t1, 1	# i++
		j for_5_begin
		nop	
for_5_end:	li $v0, 10
		syscall			
		
		
		
		
		