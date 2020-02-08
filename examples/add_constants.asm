# Program loads 32b constants on $t0, $t1. Adds them and stores result in $t2
# By: Abhishek Kumar, 181CO201
# Date: 17 Jan 2020

.text
.globl main
main:
  li $t0, 100
  li $t1, -5

  add $t2, $t0, $t1

  li $v0, 10
  syscall
.end
