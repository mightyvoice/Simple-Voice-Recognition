
#ifndef  PROCESS_H
#define  PROCESS_H

#include    "type.h"


extern int queue_h, queue_t;
/* the head and tail of the ADC data queue */


void audio_indenti_process_init();
/* the initialization of the audio identification process */


void audio_indenti_process();
/* the main procedure of the audio identification */


#endif
