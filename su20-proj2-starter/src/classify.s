.globl classify

.text
classify:
    # =====================================
    # COMMAND LINE ARGUMENTS
    # =====================================
    # Args:
    #   a0 (int)    argc
    #   a1 (char**) argv
    #   a2 (int)    print_classification, if this is zero, 
    #               you should print the classification. Otherwise,
    #               this function should not print ANYTHING.
    # Returns:
    #   a0 (int)    Classification
    # 
    # If there are an incorrect number of command line args,
    # this function returns with exit code 49.
    #
    # Usage:
    #   main.s -m -1 <M0_PATH> <M1_PATH> <INPUT_PATH> <OUTPUT_PATH>

	# =====================================
    # SAVE REGISTERS
	# =====================================
    addi sp, sp, -44
    sw s0, 0(sp)
    sw s1, 4(sp)
    sw s2, 8(sp)
    sw s3, 12(sp)
    sw s4, 16(sp)
    sw s5, 20(sp)
    sw s6, 24(sp)
    sw s7, 28(sp)
    sw s8, 32(sp)
    sw s9, 36(sp)
    sw ra, 40(sp)

	# =====================================
    # LOAD MATRICES
    # =====================================


    mv s0, a0       # s0 stores argc
    mv s1, a1       # s1 stores **argv
    mv s2, a2       # s2 stores is_print


    li t0, 5
    bne s0, t0, 49_exit

    # Load pretrained m0

    li a0, 24
    jal malloc
    beq a0, x0, 48_exit
    mv s5, a0       # s5 stores pointer to 3 rows and cols
    mv a1, s5
    addi a2, a1, 4
    lw a0, 4(s1)
    jal read_matrix
    mv s3, a0       # s3 stores pointer to m0
    

    # Load pretrained m1

    addi a1, s5, 8
    addi a2, a1, 4
    lw a0, 8(s1)
    jal read_matrix
    
    mv s4, a0       # s4 stores pointer to m1

    # Load input matrix

    addi a1, s5, 16
    addi a2, a1, 4
    lw a0, 12(s1)
    jal read_matrix
    mv s6, a0       # s6 stores pointer to input

    # =====================================
    # RUN LAYERS
    # =====================================
    # 1. LINEAR LAYER:    m0 * input
    # 2. NONLINEAR LAYER: ReLU(m0 * input)
    # 3. LINEAR LAYER:    m1 * ReLU(m0 * input)

alloc_memory_for_middle:
    lw t1, 0(s5)
    lw t2, 20(s5)
    mul a0, t1, t2
    slli a0, a0, 2
    jal malloc
    beq a0, x0, 48_exit
    mv s7, a0       # s7 stores pointer to middle

hidden_layer:
    lw a1, 0(s5)
    lw a2, 4(s5)
    lw a4, 16(s5)
    lw a5, 20(s5)
    mv a0, s3
    mv a3, s6
    mv a6, s7
    jal matmul

relu_cal:
    lw t1, 0(s5)
    lw t2, 20(s5)
    mul a1, t1, t2
    mv a0, s7
    jal relu

score:
    lw t1, 8(s5)
    lw t2, 20(s5)
    mul a0, t1, t2
    slli a0, a0, 2
    jal malloc
    beq a0, x0, 48_exit
    mv s8, a0       # s8 stores pointer to score
    lw a1, 8(s5)
    lw a2, 12(s5)
    lw a4, 0(s5)
    lw a5, 20(s5)
    mv a0, s4
    mv a3, s7
    mv a6, s8
    jal matmul

    # =====================================
    # Test:
    # mv a0, s8
    # lw a1, 8(s5)
    # lw a2, 20(s5)
    # jal print_int_array
    # li a1 '\n'    # no ','
    # jal print_char
    # =====================================

    # =====================================
    # WRITE OUTPUT
    # =====================================
    # Write output matrix

write_score:
    lw a0, 16(s1)
    mv a1, s8
    lw a2, 8(s5)
    lw a3, 20(s5)
    jal write_matrix

    # =====================================
    # CALCULATE CLASSIFICATION/LABEL
    # =====================================
    # Call argmax

    lw t1, 8(s5)
    lw t2, 20(s5)
    mul a1, t1, t2
    mv a0, s8
    jal argmax
    mv s9, a0       # s9 stores argmax answer

    # Print classification
    bne s2, x0, not_print
    mv a1, s9
    jal print_int
    
    # Print newline afterwards for clarity
    li a1 '\n'
    jal print_char

not_print:
    mv a0, s3
    jal free
    mv a0, s4
    jal free
    mv a0, s5
    jal free
    mv a0, s6
    jal free
    mv a0, s7
    jal free
    mv a0, s8
    jal free

    mv a0, s9

    lw s0, 0(sp)
    lw s1, 4(sp)
    lw s2, 8(sp)
    lw s3, 12(sp)
    lw s4, 16(sp)
    lw s5, 20(sp)
    lw s6, 24(sp)
    lw s7, 28(sp)
    lw s8, 32(sp)
    lw s9, 36(sp)
    lw ra, 40(sp)
    addi sp, sp, 44
    
    ret

48_exit:
    li a1, 48
    j exit2

49_exit:
    li a1, 49
    j exit2