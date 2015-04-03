
/*-----------------------------------------------------------------------------
 *  this part is to process the original digital frame using some dsp algorithm
 *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *
 *  All the rights of this shit code belong to mightyvoice 
 *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *
 *-----------------------------------------------------------------------------*/

#include "includes.h"

Data_Type   hamming_window[Max_Frame_Len];
/* hamming_window factor array */

Data_Type   frame_after_pre_proc[Max_Frame_Len];
/* frame after preprcocess */

Data_Type   frame_after_window[Max_Frame_Len];
/* frame after add hamming window */

Data_Type   frame_energy[Max_Queue_Len];
/* energy of every frame */

Data_Type   frame_over_zero[Max_Queue_Len];
/* the over-zero rate of every frame */

void hamming_init()
{
    /* 
     * ===  FUNCTION  ===================================
     *         Name:  hamming_init
     *  Description:  the initialition of hamming_window
     * ==================================================
     */
    int i;
    Data_Type tmp = 2.0 * PI / ( Max_Frame_Len - 1);
    for( i = 0; i < Max_Frame_Len; i++)
    {
        hamming_window[i] = 0.54 - 0.46 * cos( i * tmp);
    }
}

void add_window()
{
    /* 
     * ===  FUNCTION  ================================
     *         Name:  add_window
     *  Description:  add a hamming_window to a frame
     * ================================================
     */
    int i;
    for( i = 0; i < Max_Frame_Len; i++)
    {
        frame_after_window[i] = frame_after_pre_proc[i] * hamming_window[i];
    }
}

void frame_pre_process( Adc_Type cur_frame[])
{
    /* 
     * ===  FUNCTION  ================================================
     *         Name:  frame_pre_process
     *  Description:  use a pre_emphasize filter to process the frame 
     * ===============================================================
     */
    int i;
    for( i = Max_Frame_Len - 1; i > 0; i--)
    {
        frame_after_pre_proc[i] = cur_frame[i] - Emphasize * cur_frame[i-1];
    }
    frame_after_pre_proc[0] = ( 1.0 - Emphasize) * cur_frame[0];
}

Data_Type frame_energy_cal( int num)
{
    /* 
     * ===  FUNCTION  ======================================================================
     *         Name:  frame_energy_cal
     *  Description:  caluculate the energy of the frame, which has been added
     *  hamming_window 
     * =====================================================================================
     */
    Data_Type   tmp = 0;
    int i;
    for( i = 0; i < Max_Frame_Len; i++)
    {
        tmp += fabs( frame_after_pre_proc[i] * hamming_window[i]);
    }
    frame_energy[i] = tmp;
    return tmp;
}

Data_Type frame_over_zero_cal( int num)
{
    /* 
     * ===  FUNCTION  ======================================================================
     *         Name:  frame_over_zero_cal
     *  Description:  caluculate the over-zero rate of the frame
     * =====================================================================================
     */
    Data_Type tmp = 0.0;
    int i, j, k;
    for( i = 1; i < Max_Frame_Len; i ++)
    {
        j = ( frame_after_pre_proc[i-1] - 0.0 >= EPS) ? 1 : 0;
        k = ( frame_after_pre_proc[i] - 0.0 >= EPS) ? 1 : 0;
        tmp +=  abs( k - j) * hamming_window[i];
    }
    frame_over_zero[num] = tmp;
    return tmp;
}










