.globl dot

.text
# =======================================================
# FUNCTION: Dot product of 2 int vectors
# Arguments:
#   a0 (int*) is the pointer to the start of v0
#   a1 (int*) is the pointer to the start of v1
#   a2 (int)  is the length of the vectors
#   a3 (int)  is the stride of v0
#   a4 (int)  is the stride of v1
# Returns:
#   a0 (int)  is the dot product of v0 and v1
#
# If the length of the vector is less than 1, 
# this function exits with error code 5.
# If the stride of either vector is less than 1,
# this function exits with error code 6.
# =======================================================
dot:

    # Prologue

    addi t0, x0, 1
    blt a2, t0, 5_exit
    blt a3, t0, 6_exit
    blt a4, t0, 6_exit

loop_start:
    addi t0, x0, 0  # t0 stores v0 index
    addi t6, x0, 0  # t6 stores v1 index
    addi t1, x0, 0  # t1 stores sum
    addi t2, x0, 4   
    mul t2, t2, a3  # t2 stores 4* v0 stride
    addi t3, x0, 4
    mul t3, t3, a4  # t3 stores 4* v1 stride

loop_continue:
    lw t4, 0(a0)
    lw t5, 0(a1)
    mul t4, t4, t5  #temp = ()*()
    add t1, t1, t4  #sum += temp
stride_forward:
    add a0, a0, t2
    add a1, a1, t3
    add t0, t0, a3
    blt t0, a2, loop_continue

loop_end:
    addi a0, t1, 0
    # Epilogue

    
    ret

5_exit:
    addi a0, x0, 10
    addi a1, x0, 5
    ecall
6_exit:
    addi a0, x0, 10
    addi a1, x0, 6
    ecall