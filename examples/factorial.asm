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
   #sub $sp, $sp, 8
   #sw  $ra, 0($sp)
   #sw  $a0, 4($sp)

   # Call factorial for N - 1
   addi R4, R4, -1

   bl factorial

   # Multiply (N - 1)! with N
   #lw   $a0, 4($sp)
   #mult $a0, $v0
   #mflo $v0

   # Cleanup stack and return
   #lw   $ra, 0($sp)
   #addi $sp, $sp, 8
   #jr   $ra

# s0 - Number for which factorial is calculated
# s1 - Factorial by recursion
# s2 - Factorial by loop
main:
  li 3,5

  # Copy value as argument
  addi R4,R3,0

  bl factorial

  addi R5, R6, 0

  # Copy values for factorial by loop
  li 7,1

  addi R8,R3,0

loop:
  #mult $s2, $t1
  #mflo $s2
  addi R8,R8,1
  #bne  $t1, $zero, loop

  li R0,10
  sc

  .end
