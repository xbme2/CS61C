.globl relu
.globl wrong_exit
.text
# ==============================================================================
# FUNCTION: Performs an inplace element-wise ReLU on an array of ints
# Arguments:
# 	a0 (int*) is the pointer to the array
#	a1 (int)  is the # of elements in the array
# Returns:
#	None
#
# If the length of the vector is less than 1, 
# this function exits with error code 8.
# ==============================================================================
relu:
    # Prologue

    beq a1, x0 , wrong_exit # If the length of the vector is less than 1, wrong exit; 

loop_start:
    
loop_continue:
    lw t0, 0(a0)
    blt x0,t0, max
    sw x0, 0(a0)
max:
    addi a0, a0, 4
    addi a1, a1, -1
    bne a1, x0, loop_continue
loop_end:
    # Epilogue
	ret

wrong_exit:
    addi a0, x0, 10
    addi a1, x0, 8
    ecall  #Exit 8


#ecall
    #exit   a0:10   a1:exitcode
    #print  a0:11   a1:content
    #malloc a0:9    a1:size