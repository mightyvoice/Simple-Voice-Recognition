
#ifndef     AUDIO_START_TEST_H
#define     AUDIO_START_TEST_H

#include    "type.h"

extern Data_Type   none_audio_energy;
/* the energy of none_audio */

extern Data_Type   none_audio_over_zero;
/* the over_zero rate of none_audio */

extern int         audio_start;
/* the start frame of current audio */

extern int         audio_end;
/* the end frame of current audio */

void audio_start_test_init();


void   audio_start_test();
/* the process of the audio's start-point test */


#endif

