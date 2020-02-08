# Program to calculate factorial using recursion and loop
# By: Abhishek Kumar, 181CO201
# Date: 17 Jan 2020

  .text
  .globl main

factorial:
  move $v0, $a0
  bne  $a0, 1, recurse
  jr   $ra

recurse:
   # Store current state into stack
   sub $sp, $sp, 8
   sw  $ra, 0($sp)
   sw  $a0, 4($sp)

   # Call factorial for N - 1
   addi $a0, $a0, -1
   jal  factorial

   # Multiply (N - 1)! with N
   lw   $a0, 4($sp)
   mult $a0, $v0
   mflo $v0

   # Cleanup stack and return
   lw   $ra, 0($sp)
   addi $sp, $sp, 8
   jr   $ra

# s0 - Number for which factorial is calculated
# s1 - Factorial by recursion
# s2 - Factorial by loop
main:
  li $s0, 5

  # Copy value as argument
  addi $a0, $s0, 0

  jal factorial
  addi $s1, $v0, 0

  # Copy values for factorial by loop
  li   $s2, 1
  addi $t1, $s0, 0

loop:
  mult $s2, $t1
  mflo $s2
  addi $t1, $t1  , -1
  bne  $t1, $zero, loop

  li $v0, 10
  syscall

  .end
