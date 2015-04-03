
/*-----------------------------------------------------------------------------
 *  This part is to calculate the mel-factor of the current audio frame which
 *  must be used in the pattern-match process.
 *  All of the processes are according to the paper I read
 *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *
 *  All the rights of this shit code belong to mightyvoice 2011-5-1
 *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *
 *-----------------------------------------------------------------------------*/

#include "includes.h"

complex_num     fft_result[FFT_Len + 2];
/* reserve the fft result of current frame */

Data_Type       mel_filter_factor[Mel_filter_num + 2][Max_Frame_Len + 2];
/* the array of mel_filter's factor */

Data_Type       mid_freq[Mel_filter_num + 2];
/* reserve the middle frequency of all mel_filter */

Data_Type       dct_factor[Mel_factor_num + 1][Mel_filter_num + 2];
/* reserve all the DCT's factor */

Data_Type       signal_after_filter[Mel_factor_num + 2];
/* reserve the energy value of the signal after the filter */

Data_Type       mfcc_factor[Max_audio_frame_num + 1][Mel_factor_num + 1];
/* the mfcc factor array, using to do the pattern match process */

Data_Type       digital_signal_freq[Max_Frame_Len << 1];
/* the frequency distribution of a frame digital signal */

int             in_which_filter[Max_Frame_Len << 1];
/* which number of filter every point in */

Data_Type       every_point_gain[Max_Frame_Len << 1];
/* the gain of every point which comes from fft */

int             mfcc_factor_top;
/* current position in mfcc_factor */

void digital_signal_freq_init()
{
    /* 
     * ===  FUNCTION  ======================================================================
     *         Name:  digital_signal_freq_init
     *  Description:  initialize the frequency of every point which
     *  comes from the fft algorithm
     * =====================================================================================
     */
    int i;
    for( i = 0; i < Max_Frame_Len; i++)
    {
        digital_signal_freq[i] = Rate * ( 1.0 * i) / ( Max_Frame_Len << 1);
    } 
}

void dct_factor_init()
{
    /* 
     * ===  FUNCTION  ==============================
     *         Name:  dct_factor_init
     *  Description:  using the formula in paper to
     *  initialize the dct_factor array
     * =============================================
     */
    int i, j;
    /* according to the DCT formula */
    for( i = 0; i < Mel_factor_num; i++)
    {
        for( j = 0; j < Mel_filter_num; j++)
        {
            dct_factor[i][j] = cos( (j + 0.5) * PI * i / Mel_filter_num);
        }
    }
}

void mid_freq_init()
{
    /* 
     * ===  FUNCTION  ==================================
     *         Name:  mid_freq_init
     *  Description:  using the formula in the paper to
     *  initialize the mid_freq array
     * =================================================
     */
    Data_Type mel_dis;
    Data_Type mel_start, mel_end;
    int i;

    /* according to the formula:
     * Mel(f) = 2595*log10(1+f/700) and
     * Mel(f[i+1]) - Mel(f[i]) = Mel(f[i]) - Mel(f[i-1])
     * then we can first the distance between tow adjecent
     * Mel frequency then we can use the distance to get
     * all the other frequency, eventually we can get the
     * phsical frequency as to: Mel(f) = 2595*log10(1+f/700)
     */
    mel_start = 2595.0 * log10( 1.0 + Start_freq / 700.0);
    mel_end = 2595.0 * log10( 1.0 + End_freq / 700.0);
    mel_dis = ( mel_end - mel_start) / ( Mel_filter_num + 1);

    /* when there are n mel filters then 
     * there must be n+2 mid_freq */
    mid_freq[0] = mel_start;
    mid_freq[Mel_filter_num + 1] = mel_end;
    for( i = 1; i < Mel_filter_num + 1; i++)
    {
        mid_freq[i] = mid_freq[i - 1] + mel_dis;
    }

    for( i = 0; i < Mel_filter_num + 2; i++)
    {
        mid_freq[i] = (pow( 10.0, mid_freq[i] / 2595.0) - 1.0) * 700.0;
    }
    /*
    for( i = 0; i < Mel_filter_num + 2; i++)
    {
        printf("%.5f ", mid_freq[i]);
    }
    printf ( "\n" );
    */
}

void mel_filter_factor_init()
{
    /* 
     * ===  FUNCTION  ==================================
     *         Name:  mel_filter_factor_init
     *  Description:  using the formula in the paper to
     *  initialize the mel_filter_factor
     * ==================================================
     */
    int i, j, n;
    Data_Type   k;
    /* k is Max_Frame_Len points in the interval between
     * mid_freq[i] to mid_freq[i+2], adjecent k have the
     * same dis */
    Data_Type   dis;
    
    /* initialize the mel_filter_factor array
     * this array is used to do the leach process
     * we sample Max_Frame_Len points in each 
     * triangle filter to get the H(k) */
    n = Max_Frame_Len / 2;
    for( i = 0; i < Mel_filter_num; i++)
    {
        dis = ( mid_freq[i+2] - mid_freq[i]) / ( Max_Frame_Len - 1);
        k = mid_freq[i];

        /* calculate the first half of points as to formula */
        mel_filter_factor[i][0] = 0.0;
        mel_filter_factor[i][Max_Frame_Len-1] = 0.0;
        for( j = 1; j < n; j++)
        {
            k += dis;
            mel_filter_factor[i][j] = 2.0 * ( k - mid_freq[i]) / 
            ( ( mid_freq[i+2] - mid_freq[i]) * ( mid_freq[i+1] - mid_freq[i]));
        }

        /* calculate the second half of points as to formula */
        for( j = n; j < Max_Frame_Len - 1; j++)
        {
            k += dis;
            mel_filter_factor[i][j] = 2.0 * ( mid_freq[i+2] - k) / 
            ( ( mid_freq[i+2] - mid_freq[i]) * ( mid_freq[i+2] - mid_freq[i+1]));
        }
    }
    /*
    for( i = 0; i< Mel_filter_num; i++)
    {
        for( j = 0; j < Max_Frame_Len; j++)
        {
            printf("%7f ", mel_filter_factor[i][j]);
        }
        printf ( "\n" );
    }
    printf ( "\n" );
    */
}

void in_which_filter_init()
{
    int i,j;
    for( i = 0; i < Max_Frame_Len; i++)
    {
        for( j = 0; j < Mel_filter_num; j++)
        {
            if( digital_signal_freq[i] - EPS >= mid_freq[j] &&
                mid_freq[j+2] - digital_signal_freq[i] >= EPS)
            {
                in_which_filter[i] = j;
                break;
            }
        }
//        printf("%d ", in_which_filter[i]);
    }
}

void every_point_gain_init()
{
    int i, j;
    Data_Type tmp1;
    for( i = 0; i < Max_Frame_Len; i++)
    {
        j = in_which_filter[i];
        tmp1 = digital_signal_freq[i];
        if( mid_freq[j+1] - tmp1 >= EPS)
        {
             every_point_gain[i] = 2.0 * ( tmp1 - mid_freq[j]) / ( mid_freq[j+1] - mid_freq[j]);
        }
        else
        {
             every_point_gain[i] = 2.0 * ( mid_freq[j+2] - tmp1) /  ( mid_freq[j+2] - mid_freq[j+1]);
        }
//        printf("%.3f ", every_point_gain[i]);
    }
//    printf ( "\n" );
}

void mfcc_init()
{
    /* 
     * ===  FUNCTION  ============================
     *         Name:  mfcc_init
     *  Description:  the whole initializiton of 
     *  the mfcc_factor_cal process
     * ===========================================
     */
    digital_signal_freq_init();
    dct_factor_init();
    mid_freq_init();
    in_which_filter_init();
    every_point_gain_init();
    mel_filter_factor_init();
    mfcc_factor_top = -1;
    memset( mfcc_factor, 0, sizeof(mfcc_factor));
}

void mfcc_factor_cal( Adc_Type data[], char last)
{
    /* 
     * ===  FUNCTION  =======================================
     *         Name:  mfcc_factor_cal
     *  Description:  the main process of the mfcc_factor's
     *  calculation, all according to the paper
     * ======================================================
     */
    complex_num  tmp[FFT_Len + 2];
    int i, j;
    Data_Type sum;

    frame_pre_process( data);
    add_window();

    /* get the array to FFT */
    for( i = 0; i < Max_Frame_Len; i++)
    {
        tmp[i].real = frame_after_window[i];
        tmp[i].imaginary = 0;
    }
    for( i = Max_Frame_Len; i < Max_Frame_Len + 2; i++)
    {
        tmp[i].real = 0;
        tmp[i].imaginary = 0;
    }
    FFT( tmp, fft_result, Max_Frame_Len + 2);

    /* get the energy value of signal after it
     * has been through the filter */
    memset( signal_after_filter, 0, sizeof( signal_after_filter));
    for( i = 0; i < Max_Frame_Len; i++)
    {
//       print_complex(fft_result[i]);
       fft_result[i].imaginary *= every_point_gain[i];
       fft_result[i].real *= every_point_gain[i];
       signal_after_filter[i] += fft_result[i].imaginary * fft_result[i].imaginary 
                               + fft_result[i].real * fft_result[i].real;
       if( 1 - signal_after_filter[i] >= EPS)
       {
           signal_after_filter[i] = 1;
       }
//       printf("i = %d, gain = %.3f, energy = %.3f\n",i, every_point_gain[i], signal_after_filter[i]);
    }
    for( i = 0; i < Max_Frame_Len; i++)
    {
        signal_after_filter[i] = log(signal_after_filter[i]);
    }
    /*
    for( i = 0; i < Mel_filter_num; i++)
    {
        printf("%.5f ", signal_after_filter[i]);
    } 
    printf ( "\n" );
    */
    mfcc_factor_top++; /* get current frame's position */
    
    /* get the mfcc_factor according to the formula
     * mentioned in the paper
     */
    for( i = 0; i < Mel_factor_num; i++)
    {
        sum = 0;
        for( j = 0; j < Mel_filter_num; j++)
        {
            sum += signal_after_filter[j] * dct_factor[i][j];
        }
        mfcc_factor[mfcc_factor_top][i] = sum;
    }

    /* if current fram was the last frame of th audio
     * then we shoud set the top of mfcc_factor to -1
     */
    if( last)
    {
//        top = -1;
    }
}

    


