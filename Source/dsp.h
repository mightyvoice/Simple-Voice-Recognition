
#ifndef     DSP_H
#define     DSP_H

#include    "type.h"

#define     Emphasize   0.95

extern Data_Type   hamming_window[Max_Frame_Len];
/* hamming_window factor array */

extern Data_Type   frame_after_pre_proc[Max_Frame_Len];
/* frame after preprcocess */

extern Data_Type   frame_after_window[Max_Frame_Len];
/* frame after add hamming window */

extern Data_Type   frame_energy[Max_Queue_Len];
/* energy of every frame */

extern Data_Type   frame_over_zero[Max_Queue_Len];
/* the over-zero rate of every frame */


void hamming_init();
/* the initialition of hamming_window */


void add_window(); 
/* add a hamming_window to a frame */


void frame_pre_process( Adc_Type cur_frame[]);
/* use a pre_emphasize filter to process the frame */


Data_Type frame_energy_cal(int num);
/* caluculate the energy of the frame */


Data_Type frame_over_zero_cal(int num);
/* caluculate the over-zero rate of the frame */


#endif
