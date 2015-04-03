
/* The realization of FFT, IFFT, Convolution and other functions
 * that have declared in the header file
 */

#include "includes.h"

void print_complex(complex_num x)
{ 
    /* 
     * ===  FUNCTION  ======================
     *         Name:  print_complex
     *  Description:  print a complex number
     * =====================================
     */
    printf("  %.10f + %.10fi", x.real, x.imaginary);
}

int get_min_power(int x)
{
    /* 
     * ===  FUNCTION  =======================
     *         Name:  get_min_power
     *  Description:  get the mininum n that
	 *  2 ^ n >= x
     * ======================================
     */
    int l = 0;
    while(1)
    {
       if(x == (1 << l)) return l;
       if(x > (1 << l) && x < (1 << (l + 1))) return(l + 1);
       l ++;
    }
}

u_int reverse_bit(u_int x, int m)
{
    /* 
     * ===  FUNCTION  ====================
     *         Name:  reverse_bit
     *  Description:  reverse the first m
	 *  bits of unsigned int x
     * ===================================
     */
    //exchange the fist and second 16bits of x
    x = (x >> 16) | ((x & 0xffff) << 16); 
    
    //exchange every adjacent 8bits of x
    //tmp = 0xff | (0xff << 16);
    x = ((x & 16711935) << 8) | ((x & (16711935 << 8)) >> 8);

    //exchange every adjacent 4bits of x
    //tmp = 0xf | (0xf << 8) | (0xf << 16) | (0xf << 24);
    x = ((x & 252645135) << 4) | ((x & (252645135 << 4)) >> 4);

    //exchange every adjacent 2bits of x
    //tmp = 3 | (3 << 4) | (3 << 8) | (3 << 12) | (3 << 16)
           // | (3 << 20) | (3 << 24) | (3 << 28);
    x = ((x & 858993459) << 2) | ((x & (858993459 << 2)) >> 2);

    //exchange every adjacent 1bits of x
    //tmp = 1 | (1 << 2) | (1 << 4) | (1 << 6) | (1 << 8)
    //        | (1 << 10) | (1 << 12) | (1 << 14) | (1 << 16)
     //       | (1 << 18) | (1 << 20) | (1 << 22) | (1 << 24)
    //        | (1 << 26) | (1 << 28) | (1 << 30);
    x = ((x & 1431655765) << 1) | ((x & (1431655765 << 1)) >> 1);
    
    return( x >> (32 - m));
}

complex_num complex_num_add(complex_num x, complex_num y)
{
    /* 
     * ===  FUNCTION  ===================================
     *         Name:  complex_num_add
     *  Description:  add two comple and l2 points individually, and put the results into the array cx number x and y
     * ==================================================
     */
    complex_num ans;
    ans.imaginary = x.imaginary + y.imaginary;
    ans.real = x.real + y.real;
    return(ans);
}

complex_num complex_num_minus(complex_num x, complex_num y)
{
    /* 
     * ===  FUNCTION  =====================================
     *         Name:  complex_num_minus
     *  Description:  minus two complex number x and y
     * ====================================================
     */
    complex_num ans;
    ans.imaginary = x.imaginary - y.imaginary;
    ans.real = x.real - y.real;
    return(ans);
}

complex_num complex_num_mult(complex_num x, complex_num y)
{
    /* 
     * ===  FUNCTION  ====================================
     *         Name:  complex_num_mult
     *  Description:  multiple two complex number x and y
     * ===================================================
     */
    complex_num ans;
    ans.real = x.real * y.real - x.imaginary * y.imaginary;
    ans.imaginary = x.real * y.imaginary + x.imaginary * y.real;
    return(ans);
}

complex_num complex_num_div(complex_num x, complex_num y)
{
    /* 
     * ===  FUNCTION  ====================================
     *         Name:  complex_num_div
     *  Description:  divide two complex number x and y
     * ===================================================
     */
    complex_num ans;
    y.imaginary = - y.imaginary;
    ans = complex_num_mult(x, y);
    ans.real /= y.real * y.real + y.imaginary * y.imaginary;
    ans.imaginary /= y.real * y.real + y.imaginary * y.imaginary;
    return(ans);
}

void FFT(complex_num a[], complex_num ans[], int n)
{
    /* 
     * ===  FUNCTION  ==============================
     *         Name:  FFT
     *  Description:  use none-recursive algorithm
	 *  to get the FFT results of serise a which has
	 *  n point and put the results in the array ans
     * =============================================
     */
    int i, L, j, k, m;
    complex_num w, w_m, tmp1, tmp2;
    m = n;
    n = get_min_power(n);
    L = 1 << n;
    for(i = m; i < L; i++)
    {
        a[i].real = 0;
        a[i].imaginary = 0;
    }
    /* 
	   Get the initial sequence of the ans use the method that is mentioned in
	   the book <<Introduction To Algorithm>> chinese edition page 516, this is 
	   the first and important step of the none-recursive fft algorithm 
	*/
    for(i = 0; i < L; i++)
    {
        ans[reverse_bit( ( u_int)i, n)] = a[i]; 
    }
	/* The main procedure of the none-recursive fft algorithm according to the
	 * pseudo code in the book <<Introduction To Algorithm>> chinese edition page 
	 * 517 use the none-recursive method to realize FFT
	 */
    for(i = 1; i <= n; i++)
    {
        m = 1 << i;
        w_m.real = cos((2.0 * PI) / (1.0 * m));
        w_m.imaginary = -sin((2.0 * PI) / (1.0 * m));
        for(k = 0; k < L; k += m)
        {
            w.real = 1.0;
            w.imaginary = 0;
            for(j = 0; j < (m>>1); j++)
            {
                tmp1 = complex_num_mult(w , ans[k + j + (m>>1)]);
                tmp2 = ans[k + j];
                ans[k + j] = complex_num_add( tmp1 , tmp2);
                ans[k + j + m/2] = complex_num_minus(tmp2 , tmp1);
                w = complex_num_mult( w_m, w);
            }
        }
    }
    return;
}

void IFFT(complex_num a[], complex_num ans[], int n)
{
    /* 
     * ===  FUNCTION  ===============================
     *         Name:  IFFT
     *  Description:  use none-recursive algorithm
	 *  to get the IFFT results of serise a which has
	 *  n point and put the results in the array ans
     * ==============================================
     */
    int i, L = 1 << n, j, k, m;
    complex_num w, w_m, tmp1, tmp2, one;
    m = n;
    n = get_min_power(n);
    L = 1 << n;
    for(i = m; i < L; i++)
    {
        a[i].real = 0;
        a[i].imaginary = 0;
    }
    one.real = 1.0;
    one.imaginary = 0.0;
    /* 
	 * Get the initial sequence of the ans use the method that is mentioned in
	 * the book <<Introduction To Algorithm>> chinese edition page 516, this is 
	 * the first and important step of the none-recursive fft algorithm 
	 */
    for(i = 0; i < L; i++)
    {
        ans[reverse_bit( ( u_int)i, n)] = a[i];
    } 
	
	/* The main procedure of the none-recursive fft algorithm according to the
	 * pseudo code in the book <<Introduction To Algorithm>> chinese edition page 
	 * 517 use the none-recursive method to realize FFT, there is just a little
	 * difference of the FFT and IFFT, the differece is the w_m in the IFFT is
	 * reversion of which in the FFT described above
	 */
    for(i = 1; i <= n; i++)
    {
        m = 1 << i;
        w_m.real = cos(2.0 * PI / (1.0 * m));
        w_m.imaginary = -sin(2.0 * PI / (1.0 * m));
        w_m = complex_num_div(one, w_m);
        for(k = 0; k < L; k += m)
        {
            w = one;
            for(j = 0; j < (m>>1); j++)
            {
                tmp1 = complex_num_mult(w , ans[k + j + (m>>1)]);
                tmp2 = ans[k + j];
                ans[k + j] = complex_num_add( tmp1 , tmp2);
                ans[k + j + m/2] = complex_num_minus(tmp2 , tmp1);
                w = complex_num_mult( w, w_m);
            }
        }
    }
    for(i = 0; i < L; i++)
    {
        ans[i].imaginary /= 1.0 * L;
        ans[i].real /= 1.0 * L;
    }
    return;
}

/*To reserve the temporary fft results in the convolution computing */
complex_num     conv_fft_a[MAX_FFT_N];
complex_num     conv_fft_b[MAX_FFT_N];
complex_num     conv_fft_c[MAX_FFT_N];

void convolution(complex_num a[], complex_num b[], complex_num c[], int l1, int l2)
{
    /* 
     * ===  FUNCTION  =============================================================
     *         Name:  convolution
     *  Description:  compute the convolution of two serise a and b, which has l1
	 *  and l2 points individually, and put the results into the array c
     * ============================================================================
     */
    int l = l1 > l2 ? l1 : l2;
    int n = get_min_power(l), L = 1 << n, i; 
    L <<= 1;
    for(i = l1; i < L; i++) //append 0 in the sequence to let the length reach L
    {
        a[i].imaginary = 0;
        a[i].real = 0;
    }
    for(i = l2 ; i < L; i++)
    {
        b[i].imaginary = 0;
        b[i].real = 0;
    } 
    FFT(a, conv_fft_a, L); //do the FFT procedure
    if(a == b) //if a == b then just do one time FFT
    {
        for(i = 0; i < L; i++)
        {
            conv_fft_c[i] = complex_num_mult(conv_fft_a[i], conv_fft_a[i]);  
        }
    }
    else
    {
        FFT(b, conv_fft_b, L);// do the FFT procedure
        for(i = 0; i < L; i++)
        {
            conv_fft_c[i] = complex_num_mult(conv_fft_a[i], conv_fft_b[i]);  
        }
    }
    IFFT(conv_fft_c, c, L);
    return;
}

