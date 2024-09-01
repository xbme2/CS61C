.globl argmax

.text
# =================================================================
# FUNCTION: Given a int vector, return the index of the largest
#	element. If there are multiple, return the one
#	with the smallest index.
# Arguments:
# 	a0 (int*) is the pointer to the start of the vector
#	a1 (int)  is the # of elements in the vector
# Returns:
#	a0 (int)  is the first index of the largest element
#
# If the length of the vector is less than 1, 
# this function exits with error code 7.
# =================================================================
argmax:

    # Prologue

    beq a1, x0, wrong_exit

loop_start:

    lw t0, 0(a0)    # t0 stores max value
    add t1, x0, x0  # t1 stores max index
    addi t3, x0, 1
    addi a0, a0, 4

loop_continue:
    lw t2, 0(a0)
    bge t0, t2, no_change
    add t0, x0, t2
    add t1, x0, t3
no_change:
    addi a0, a0, 4
    addi t3, t3, 1
    bne t3, a1, loop_continue
loop_end:
    add a0, x0, t1

    # Epilogue


    ret


wrong_exit:
    addi a0, x0, 10
    addi a1, x0, 7
    ecall  #Exit 7