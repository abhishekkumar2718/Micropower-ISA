# Program to convert an ASCII number string containing positive and negative numbers to an integer
# By: Abhishek Kumar, 181CO201
# Date: 23 Jan 2020

        .data
number: .asciiz "24"

.text
.globl main

non_digit_char:
  li $v0, -1

  li $a0, 10
  syscall

parse:
  li  $t0, 0
  li  $v0, 0

loop:
  # Load a character
  lb $t0, 0($a0)

  # Done when cursor is at '/0'
  beq $t0, $zero, done
  
  # Convert ASCII to equivalent decimal
  addi $t0, $t0, -48

  # If non digit character
  bltz $t0, non_digit_char

  # Multiply previous sum by 10
  li    $t1, 10
  mult  $v0, $t1
  mflo  $v0

  # Add current digit
  add $v0, $v0, $t0

  # Move cursor one character forward
  addi $a0, $a0, 1

  # If non digit character
  addi $t0, $t0, -9
  bgtz $t0, non_digit_char

  j loop

parse_negative:
  # Move a character ahead
  addi  $a0, 1

  # Parse treating as positive
  jal parse

  # Mutliply by -1
  li   $t0, -1
  mult $v0, $t0
  mflo $v0

  j done

main:
  # Load address at $a0
  la $a0, number

  # If first character is '-', parse it as a negative number
  lb  $t0, 0($a0)
  beq $t0, 45, parse_negative

  # Otherwise parse as positive number
  jal parse

done:
  # Store integer in $v1 because $v0 is used for syscalls
  addi $v1, $v0, 0

  li $v0, 10
  syscall

.end
