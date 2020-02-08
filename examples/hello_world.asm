# Program to print "Hello World"
# By: Abhishek Kumar, 181CO201
# Date: 

.data
str: .asciiz "Hello World"

.text
.globl main
main:
  # Load string into a0
  la $a0, str

  li $v0, 4
  syscall

  li $v0, 10
  syscall

.end
