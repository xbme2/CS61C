.globl read_matrix

.text
# ==============================================================================
# FUNCTION: Allocates memory and reads in a binary file as a matrix of integers
#   If any file operation fails or doesn't read the proper number of bytes,
#   exit the program with exit code 1.
# FILE FORMAT:
#   The first 8 bytes are two 4 byte ints representing the # of rows and columns
#   in the matrix. Every 4 bytes afterwards is an element of the matrix in
#   row-major order.
# Arguments:
#   a0 (char*) is the pointer to string representing the filename
#   a1 (int*)  is a pointer to an integer, we will set it to the number of rows
#   a2 (int*)  is a pointer to an integer, we will set it to the number of columns
# Returns:
#   a0 (int*)  is the pointer to the matrix in memory
#
# If you receive an fopen error or eof, 
# this function exits with error code 50.
# If you receive an fread error or eof,
# this function exits with error code 51.
# If you receive an fclose error or eof,
# this function exits with error code 52.
# ==============================================================================
read_matrix:

    # Prologue

    # Save registers
    addi sp, sp, -32
    sw s0, 0(sp)
    sw s1, 4(sp)
    sw s2, 8(sp)
    sw s3, 12(sp)
    sw s4, 16(sp)
    sw s5, 20(sp)
    sw s6, 24(sp)
    sw ra, 28(sp)

    mv s0, a0
    mv s1, a1
    mv s2, a2
	
call_fopen:
    mv a1, s0
    li a2, 0
    jal fopen
    
    li t0, -1
    beq a0, t0, 50_exit
    mv s3, a0   # s3 stores filehandler

read_row_col:
    mv a1, s3
    mv a2, s1
    li a3, 4
    jal  fread
    li t0, 4
    bne a0, t0, 51_exit
    mv a1, s3
    mv a2, s2
    li a3, 4
    jal fread
    li t0, 4
    bne a0, t0, 51_exit

loop_start:
    lw t1, 0(s1)    
    lw t2, 0(s2)    # s5 stores col
    mul s4, t1, t2  # s4 stores row * col
    li s5, 0        # s5 stores index
    addi t0, s4, 1
    slli t0, t0, 2
    mv a0, t0
    jal  malloc  # malloc for matrix
    beq a0, x0, 48_exit
    mv s6, a0       # s6 stores pointer to the matrix

loop_read:
    mv a1, s3
    slli t5, s5, 2
    add a2, s6, t5
    mv s2, s6
    li a3, 4
    jal fread
    li t0, 4
    bne a0, t0, 51_exit
    addi s5, s5, 1
    blt s5, s4, loop_read

loop_end:
    mv a1, s3
    jal fclose
    bne a0, x0, 52_exit
    mv a0, s6
    # Epilogue
    lw s0, 0(sp)
    lw s1, 4(sp)
    lw s2, 8(sp)
    lw s3, 12(sp)
    lw s4, 16(sp)
    lw s5, 20(sp)
    lw s6, 24(sp)
    lw ra, 28(sp)
    addi sp, sp, 32

    ret

48_exit:
    li a1, 48
    j exit2
50_exit:
    li a1, 50
    j exit2
51_exit:
    li a1, 51
    j exit2
52_exit:
    li a1, 52
    j exit2
