# Program loads 32b constants on $t0, $t1. Adds them and stores result in $t2
# By: Abhishek Kumar, 181CO201
# Date: 17 Jan 2020

.text
.globl main
main:
  li R3, 100 
  li R4, -5

  add R5,R3,R4

  li R0,10
  sc
.end