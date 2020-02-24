# Program to print "Hello World"
# By: Abhishek Kumar, 181CO201
# Date: 

.data
str: .asciiz "Hello World"

.text
.globl main
main:
  # Load string into a0
  la R3, str

  li R0,4
  sc

  li R0,10
  sc

.end
