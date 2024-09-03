.import read_matrix.s
.import write_matrix.s
.import matmul.s
.import dot.s
.import relu.s
.import argmax.s
.import utils.s
.import classify.s

.globl main

# This is a dummy main function which imports and calls the classify function.
# While it just exits right after, it could always call classify again.
main:
    mv a2, x0
    jal classify
    jal exit

# Hint:
# Not all inputs will classify properly. 
# A neural network will practically never have 100% accuracy in its predictions. 
# In our test cases specifically, mnist_input2 and mnist_input7 will be misclassified to 9 and 8 respectively. 
# All other test cases should classify correctly. 
# !!!
