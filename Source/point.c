

/*-----------------------------------------------------------------------------
 *  this part is to realize the start point test algorithm, the algorithm is
 *  according to a correspondent paper , the main work of this part is to
 *  calculate the audio's start and it's end frame, then I can use the data
 *  between start and end to get it's characteristic to the MFCC part
 *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *
 *  All the rights of this shit code belong to mightyvoice 
 *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *
 *-----------------------------------------------------------------------------*/

#include "includes.h"
#include <sys/time.h>

Data_Type   none_audio_energy = 0.0;
/* the energy of none_audio */

Data_Type   none_audio_over_zero = 0.0;
/* the over_zero rate of none_audio */

int         p_cur;
/* the position of current point test */

int         audio_start = 0;
/* the start frame of current audio */

int         audio_end = 0;
/* the end frame of current audio */

int         cur_result;
/* current recognization result */

void audio_start_test_init()
{
    /* 
     * ===  FUNCTION  ============================================
     *         Name:  audio_start_test_init
     *  Description:  get the none_audio_energy and 
     *  none_audio_over_zero through the first Init_test_num frame
     * ===========================================================
     */
    Frame   f1[Init_test_num];
    int i;
    none_audio_energy = 0.0;
    none_audio_over_zero = 0.0;
    audio_start = -1;
    audio_end = -1;

    //get some frame 
    read( voice_device_fd, f1, sizeof(f1));

    //get the sum of every frame's energy and over_zero rate
    for( i = 0; i < Init_test_num; i++)
    {
        frame_pre_process( f1[i].buffer);
        
        none_audio_energy += frame_energy_cal( i);
        none_audio_over_zero += frame_over_zero_cal( i);
    }

    // calculate the mean value
    none_audio_energy /= ( 1.0 * Init_test_num);
    none_audio_over_zero /= ( 1.0 * Init_test_num);
//    printf("none_audio_energy = %.5f\nnone_audio_over_zero = %.5f\n", none_audio_energy, none_audio_over_zero);

    p_cur = -1;
}

void  audio_start_test()
{
    /* 
     * ===  FUNCTION  ======================================================================
     *         Name:  audio_start_test
     *  Description:  the main procedure of the audio start point'start
     * =====================================================================================
     */
    double   zero;
    int pre;
    int i;
    struct timeval t1, t2;
    double t3;
    int cur_len = 0;
    cur_result = -1;
    gettimeofday( &t1, NULL);
    p_cur += Interval_num;
    if( p_cur > Max_Queue_Len - 1)
    {
        p_cur = 0;
    }
    frame_pre_process( adc_data_queue[p_cur].buffer);
    zero = frame_over_zero_cal(p_cur);
//    printf("p_cur = %d, zero = %.3f\n", p_cur, zero);
    if( audio_start < 0 && zero - none_audio_over_zero > Over_zero_thre)
    {
        //if start hasn't been found and now
        //find it then find the real start of the audio
        pre = p_cur - Interval_num + 1;
        audio_start = p_cur;
        for( i = pre; i < p_cur; i++)
        {
            //find the real start frame number
            frame_pre_process( adc_data_queue[i].buffer);
            zero = frame_over_zero_cal(i);
            if( zero - none_audio_over_zero > Over_zero_thre)
            {
                audio_start = i;
                break;
            }
        }
    }
    if( audio_start > -1)
    {
        //if find the start then find the audio' end
        if( zero - none_audio_over_zero < Over_zero_thre)
        {
            //the audio has ended then find the real end frame
            audio_end = p_cur;
            pre = p_cur - Interval_num + 1;
            for( i = p_cur - 1; i >= pre; i--)
            {
                //find the real end frame number
                frame_pre_process( adc_data_queue[i].buffer);
                zero = frame_over_zero_cal(i);
                if( zero - none_audio_over_zero > Over_zero_thre)
                {
                    audio_end = i;
                    break;
                }
            }
        }

    }
    if( audio_start > -1 && audio_end > -1)
    {
        //find both start and end of the audio
        //then do the houxu process
//                printf("start = %d  end = %d\n", audio_start, audio_end);
//                printf("%d\n", audio_end - audio_start + 1);
        i = audio_start;
        cur_len = 0;
        do
        {
            cur_len ++;
            if( i == audio_end) break;
            mfcc_factor_cal( adc_data_queue[i].buffer, 0);
            i++;
            if( i > Max_Queue_Len - 1)
            {
                i = 0;
            }
        }while( i != audio_end);
//        printf("cur_len = %d\n", cur_len);
        if( cur_len >= Min_audio_frame_num)
        {
            cur_result = signal_pattern_match( audio_end - audio_start + 1);
            printf ("result = %d\n", cur_result);
        }
        mfcc_factor_top = -1;
        audio_start = -1;
        audio_end = -1;
        gettimeofday( &t2, NULL);
        t3 = 1000000*( t2.tv_sec-t1.tv_sec) + t2.tv_usec-t1.tv_usec;
        t3 /= 1000;
        printf("cost %.1fms\n",t3);
    }
    cur_result = -1;
}

