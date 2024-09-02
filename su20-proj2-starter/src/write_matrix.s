.globl write_matrix

.text
# ==============================================================================
# FUNCTION: Writes a matrix of integers into a binary file
#   If any file operation fails or doesn't write the proper number of bytes,
#   exit the program with exit code 1.
# FILE FORMAT:
#   The first 8 bytes of the file will be two 4 byte ints representing the
#   numbers of rows and columns respectively. Every 4 bytes thereafter is an
#   element of the matrix in row-major order.
# Arguments:
#   a0 (char*) is the pointer to string representing the filename
#   a1 (int*)  is the pointer to the start of the matrix in memory
#   a2 (int)   is the number of rows in the matrix
#   a3 (int)   is the number of columns in the matrix
# Returns:
#   None
#
# If you receive an fopen error or eof, 
# this function exits with error code 53.
# If you receive an fwrite error or eof,
# this function exits with error code 54.
# If you receive an fclose error or eof,
# this function exits with error code 55.
# ==============================================================================
write_matrix:

    # Prologue

    addi sp, sp, -24
    sw s0, 0(sp)
    sw s1, 4(sp)
    sw s2, 8(sp)
    sw s3, 12(sp)
    sw s4, 16(sp)
    sw ra, 20(sp)

    mv s0, a0       # s0 stores pointer to the outputfile
    mv s1, a1       # s1 stores pointer to the matrix
    mv s2, a2       # s2 stores row
    mv s3, a3       # s3 stores col

call_fopen:
    mv a1, s0
    li a2, 1
    jal fopen
    
    li t0, -1
    beq a0, t0, 53_exit
    mv s4, a0   # s4 stores filehandler

write_row_col:
    li a0, 8
    jal malloc
    sw s2, 0(a0)
    sw s3, 4(a0)
    mv a1, s4
    mv a2, a0
    li a3, 2
    li a4, 4
    jal fwrite
    li t0, 2
    bne a0, t0, 54_exit
write_content:
    mv a1, s4
    mv a2, s1
    mul t0, s2, s3
    mv a3, t0
    li a4, 4
    jal fwrite
    mul t0, s2, s3
    bne a0, t0, 54_exit

end:
    mv a1, s4
    jal fclose
    bne a0, x0, 55_exit
    # Epilogue
    lw s0, 0(sp)
    lw s1, 4(sp)
    lw s2, 8(sp)
    lw s3, 12(sp)
    lw s4, 16(sp)
    lw ra, 20(sp)
    addi sp, sp, 24


    ret

53_exit:
    li a1, 53
    j exit2
54_exit:
    li a1, 54
    j exit2
55_exit:
    li a1, 55
    j exit2