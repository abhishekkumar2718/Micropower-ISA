# Program to convert an ASCII number string containing positive and negative numbers to an integer
# By: Abhishek Kumar, 181CO201
# Date: 23 Jan 2020

        .data
number: .asciiz "24"

.text
.globl main

non_digit_char:
  li 7,-1

  li R0,10
  sc

parse:
  li 4,0

  li 7,0

loop:
  # Load a character
   lbz R4, 0(R3)


  # Done when cursor is at '/0'
  li 8,0
  bc 4,8,done
  
  # Convert ASCII to equivalent decimal
  addi R4, R4, -48

  # If non digit character
  #bltz $t0, non_digit_char


  # Multiply previous sum by 10
  li 8,10

  mult  $v0, $t1
  mflo  $v0

  # Add current digit
  add $v0, $v0, $t0

  # Move cursor one character forward
  addi $a0, $a0, 1

  # If non digit character
  addi R4,R4, -9

  bgtz $t0, non_digit_char
  # 

  j loop
  #

parse_negative:
  # Move a character ahead
  addi 3,1

  # Parse treating as positive
  bl parse

  # Mutliply by -1
  li 4,-1

  #mult $v0, $t0 

  #mflo $v0
  #
  #j done

main:
  # Load address at $a0
  la R3, number

  # If first character is '-', parse it as a negative number
  lbz R4, 0(R3)

  bc 4,5,parse_negative

  # Otherwise parse as positive number
  bl parse

done:
  # Store integer in $v1 because $v0 is used for syscalls
  addi R6,R7,0

  li R0,10
  sc

.end