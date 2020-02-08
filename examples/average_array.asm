# Program that uses a function to calculate sum of array and then calculates the average
# By: Abhishek Kumar, 181CO201
# Date: 17 Jan 2020

  .data
N: .word 5
A: .word 10, 20, 30, 40, 50

  .text
  .globl main

# a0 - Base address of array
# a1 - Number of elements
# t0 - A[i]
# v0 - Sum of array
sum_list:
  li $v0, 0

  loop:
    beq  $a1, $zero, done
    addi $a1, $a1, -1

    lw   $t0, 0($a0)
    add  $v0, $v0, $t0
    addi $a0, $a0, 4

    j loop

  done:
    jr $ra

# a0 - Base address of array
# a1 - Number of elements
main:
  la $a0, A
  lw $a1, N

  # Stores sum in $v0
  jal sum_list

  # Re-load N since it becomes zero at the end of loop
  lw $a1, N
  # Calculate average
  div $v0, $a1

  li $v0, 10
  syscall

  .end
