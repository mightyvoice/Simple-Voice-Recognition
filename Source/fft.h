
#ifndef     G_MATH_H
#define     G_MATH_H

#include    "type.h"

void print_complex(complex_num x);/*print a complex number*/

int get_min_power(int x);

/* reverse the first mbits of unsigned int x*/
u_int reverse_bit(u_int x, int m);


/*add two complex number x and y*/
complex_num complex_num_add(complex_num x, complex_num y);


/*minus two complex number x and y*/
complex_num complex_num_minus(complex_num x, complex_num y);


/*multiple two complex number x and y*/
complex_num complex_num_mult(complex_num x, complex_num y);


/*divide two complex number x and y*/
complex_num complex_num_div(complex_num x, complex_num y);


/* use none-recursive algorithm to get the FFT 
 * results of serise a which has n point and put 
 * the results in the array ans*/
void FFT(complex_num a[], complex_num ans[], int n);


/* use none-recursive algorithm to get the IFFT 
 * results of serise a which has n point and put 
 * the results in the array ans */
void IFFT(complex_num a[], complex_num ans[], int n);


/* compute the convolution of two serise a and b, which has l1
 * and l2 points individually, and put the results into the array c
 */
void convolution(complex_num a[], complex_num b[], complex_num c[], int l1, int l2);


#endif

