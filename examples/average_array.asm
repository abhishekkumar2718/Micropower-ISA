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
  li R8,0

  loop:
     li R6,0
     bc R4,R6,done

     addi R4,R4,-1

     lwz R7,0(R3)

     add R8, R8, R7

     addi R3, R3, 4

    j loop

  done:
    jr $ra

# a0 - Base address of array
# a1 - Number of elements
main:
  la R3, A

  lwz R4, N

  # Stores sum in $v0
  bl sum_list

  # Re-load N since it becomes zero at the end of loop
  lwz R4, N

  # Calculate average
  divw R9, R8, R4



  li R0,10
  sc

  .end
