#include <stdio.h>
#include "bit_ops.h"
#define MASK 0xffffffff
#define ONE 0x00000001
// Return the nth bit of x.
// Assume 0 <= n <= 31
unsigned get_bit(unsigned x,
                 unsigned n) {
    // YOUR CODE HERE
    // Returning -1 is a placeholder (it makes
    // no sense, because get_bit only returns
    // 0 or 1)
    return (x>>n) & 0x00000001;
}
// Set the nth bit of the value of x to v.
// Assume 0 <= n <= 31, and v is 0 or 1
void set_bit(unsigned * x,
             unsigned n,
             unsigned v) {
    unsigned original = *x;
    unsigned nright = (MASK<<(n+1))&original;
    unsigned nleft = (~(MASK<<n))&original;
    *x = (((v<<n)|nright)|nleft);

}
// Flip the nth bit of the value of x.
// Assume 0 <= n <= 31
void flip_bit(unsigned * x,
              unsigned n) {
    unsigned temp = *x;
    set_bit(x,n,get_bit(~temp,n));
}

