.data
array: .space 4000
front_b: .asciiz "["
back_b: .asciiz "]"
comma: .asciiz ","

.text
input:	li $v0, 5
	syscall
	move $t0, $v0	# $t0 is n
	
		li $t1, 0		# $t1 is i
while_1:	slt $t7, $t1, $t0	# $t7 = (i < n)
		beqz $t7, while_1_done	# while(i<n)
		nop
		
		li $v0, 5
		syscall
		move $t2, $v0
		sll $t7, $t1, 2		# $t7 = 4 * i
		sw $t2, array($t7)	# store array[i]
		
		addi $t1, $t1, 1	# i++
		j while_1
		nop
		
while_1_done:	li $a0, 0	# $a0 is low
		subi $t7, $t0, 1
		move $a1, $t7	# $a1 is high
		jal QuickSort
		nop
		
output:		la $a0, front_b
		li $v0, 4
		syscall
		
		li $t1, 0		# $t1 is i
		subi $t0, $t0, 1	# $t0 is n - 1
while_2:	slt $t7, $t1, $t0	# $t7 = (i < n-1)
		beqz $t7, while_2_done	# while(i<n)
		nop
		
		sll $t7, $t1, 2		# $t7 = 4 * i
		lw $a0, array($t7)	# load array[i]
		li $v0, 1
		syscall
		
		la $a0, comma
		li $v0, 4
		syscall
		
		addi $t1, $t1, 1	# i++
		j while_2
		nop

while_2_done:	sll $t7, $t1, 2		# $t7 = 4 * i
		lw $a0, array($t7)	# load array[i]
		li $v0, 1
		syscall
		
		la $a0, back_b
		li $v0, 4
		syscall	
		
		li $v0, 10
		syscall	

QuickSort:	slt $t7, $a0, $a1	# $a0 is low, $a1 is high
		beqz $t7, if_1_end	# if(low<high)
		nop
		
		move $t4, $a0		# $t4 is low
		move $t5, $a1		# $t5 is high
		sw $ra, 0($sp)		# push last address
		subi $sp, $sp, 4
		sw $t4, 0($sp)		# push last low
		subi $sp, $sp, 4
		sw $t5, 0($sp)		# push last high
		subi $sp, $sp, 4
		sw $s0, 0($sp)		# push last standard
		subi $sp, $sp, 4
		
		jal getStandard
		nop
		
		addi $sp, $sp, 4
		lw $s0, 0($sp)		# pop the last standard
		addi $sp, $sp, 4
		lw $a1, 0($sp)		# pop the last high
		addi $sp, $sp, 4
		lw $a0, 0($sp)		# pop the last low
		addi $sp, $sp, 4
		lw $ra, 0($sp)		# pop the last address
		
		
		move $s0, $v0	# $s0 is standard, $v0 is the return of getStandard
		
		j work_1
a:		nop

		j work_2
c:		nop
		
if_1_end:	jr $ra

work_1:	move $t4, $a0		# $t4 is low
	move $t5, $a1		# $t5 is high
	sw $ra, 0($sp)		# push last address
	subi $sp, $sp, 4
	sw $t4, 0($sp)		# push last low
	subi $sp, $sp, 4
	sw $t5, 0($sp)		# push last high
	subi $sp, $sp, 4
	sw $s0, 0($sp)		# push last standard
	subi $sp, $sp, 4
		
	subi $t7, $s0, 1	# $t7 = standard - 1
	move $a1, $t7		# QuickSort(low,standard-1)
	jal QuickSort
	nop
		
	addi $sp, $sp, 4
	lw $s0, 0($sp)		# pop the last standard
	addi $sp, $sp, 4
	lw $a1, 0($sp)		# pop the last high
	addi $sp, $sp, 4
	lw $a0, 0($sp)		# pop the last low
	addi $sp, $sp, 4
	lw $ra, 0($sp)		# pop the last address
	
	j a
	nop
	
work_2:	move $t4, $a0		# $t4 is low
	move $t5, $a1		# $t5 is high
	sw $ra, 0($sp)		# push last address
	subi $sp, $sp, 4
	sw $t4, 0($sp)		# push last low
	subi $sp, $sp, 4
	sw $t5, 0($sp)		# push last high
	subi $sp, $sp, 4
	sw $s0, 0($sp)		# push last standard
	subi $sp, $sp, 4
		
	addi $t7, $s0, 1	# $t7 = standard + 1
	move $a0, $t7		# QuickSort(standard+1,high)
	jal QuickSort
	nop
		
	addi $sp, $sp, 4
	lw $s0, 0($sp)		# pop the last standard
	addi $sp, $sp, 4
	lw $a1, 0($sp)		# pop the last high
	addi $sp, $sp, 4
	lw $a0, 0($sp)		# pop the last low
	addi $sp, $sp, 4
	lw $ra, 0($sp)		# pop the last address
	
	j c
	nop
	
getStandard:	sll $t7, $a0, 2
		lw $s1, array($t7)	# $s1 is array[i]
		sll $t7, $a1, 2
		lw $s2, array($t7)	# $s2 is array[j]
		move $s3, $s1		# key = array[i]
		
while_3:	slt $t7, $a0, $a1
		beqz $t7, while_3_done
		nop
		
while_4:	slt $t6, $s2, $s3	# $t6 = (array[j]<key)
		slt $t7, $a0, $a1
		beqz $t7, while_4_done
		nop
		bnez $t6, while_4_done
		nop
		
		subi $a1, $a1, 1
		sll $t7, $a1, 2
		lw $s2, array($t7)	# $s2 is updated array[j]
		j while_4
		nop
		
while_4_done:	slt $t6, $a0, $a1
		beqz $t6, if_2_end
		nop
		
		move $s1, $s2		#array[i]=array[j]
		sll $t7, $a0, 2
		sw $s1, array($t7)
		
if_2_end:	
while_5:	slt $t6, $s3, $s1	# $t6 = (key<array[i])
		slt $t7, $a0, $a1
		beqz $t7, while_5_done
		nop
		bnez $t6, while_5_done
		nop
		
		addi $a0, $a0, 1
		sll $t7, $a0, 2
		lw $s1, array($t7)	# $s1 is updated array[i]
		j while_5
		nop

while_5_done:	slt $t6, $a0, $a1
		beqz $t6, if_3_end
		nop
		
		move $s2, $s1		#array[j]=array[i]
		sll $t7, $a1, 2
		sw $s2, array($t7)
		
if_3_end:	j while_3
		nop
		
while_3_done:	sll $t7, $a0, 2
		sw $s3, array($t7)	# array[i]=key
		move $v0, $a0		# return i
		jr $ra








