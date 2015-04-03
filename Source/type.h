
#ifndef     TYPE_H
#define     TYPE_H

#define     u_short         unsigned short

#define     u_int           unsigned int

#define     u_long          unsigned long

#define     u_char          unsigned char

#define     Adc_Type        u_char
/*the data type of the ADC results */

#define     Data_Type       double
/*the data type of the float number */

#define     PI                      3.141592653589793

#define     EPS                     1e-9
/* the min precision */

#define     oo                      1e10

#define     MAX_FFT_N               (1<<8) + 2
/* the max length of FFT */

#define     Max_Queue_Len           500   
/*the length of the quque */

#define     Max_Frame_Len           256
/*The number of data in a frame */

#define     Rate                    8000 
/* digital signal sample rate */

#define     Sample_Size             8
/* quantify bits */

#define     Channels                1 
/* channle number of sample */

#define     Energy_low_thre         50
/* audio energy's low threshold */

#define     Energy_high_thre        800
/* audio energy's high threshold */

#define     Over_zero_thre          8
/* audio's over_zero rate threshold */

#define     Min_audio_frame_num     10
/* the min audio length */

#define     Max_audio_frame_num     55
/* the max audio length */

#define     Init_test_num           10
/* the frame use to find the none_audio value */

#define     Interval_num            5
/* every Interval_num frames do one audio-start test */

#define     FFT_Len                 512
/* fft algorithm's points number */

#define     Mel_filter_num          23
/* the number of Mell filter */

#define     Mel_factor_num          50
/* calculate how many mfcc factor a frame */

#define     Start_freq              0
/* Mell filter's start frequency */

#define     End_freq                (Rate / 2.0)
/* Mell filter's end frequency */

#define     Mould_num               70


typedef struct complex_num{
    Data_Type   real;
    Data_Type   imaginary;
}complex_num;
/*complex number definition */


typedef struct {
    Adc_Type    buffer[Max_Frame_Len];
    //int         top;
}Frame;
/* the definition of the frame */


#endif

