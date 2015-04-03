
#ifndef     MFCC_H
#define     MFCC_H

#include    "type.h"

extern Data_Type       mfcc_factor[Max_audio_frame_num + 1][Mel_factor_num+1];
/* the mfcc factor array, using to do the pattern match process */

extern int             mfcc_factor_top;
/* current position in mfcc_factor */

void mfcc_init();

void mfcc_factor_cal( Adc_Type data[], char last);

#endif
