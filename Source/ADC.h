
#ifndef     GET_ADC_DATA_H
#define     GET_ADC_DATA_H

#include    "type.h"

extern int voice_device_fd;

extern Frame   adc_data_queue[];
/* the circular queue of the ADC data */

extern int queue_h, queue_t;
/* the head and tail of the ADC data queue */

void  ADC_init();
/* ADC initializition */

void   get_ADC_data();
/* get the ADC results and put the data into adc_data_queue */

#endif
