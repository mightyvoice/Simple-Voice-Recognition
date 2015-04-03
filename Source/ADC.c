
/*-----------------------------------------------------------------------------
 * this part is used to get the original digital audio signal from the OS
 * I set the sample rate and quolify bits size then use the read system call
 * to read the sampled digital signal from the voice-card I opened
 *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *
 *  All the rights of this shit code belong to mightyvoice 2011-5-1
 *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *
 *-----------------------------------------------------------------------------*/

#include "includes.h"

int voice_device_fd;
/* the id of voice card */

int queue_h, queue_t;
/* the head and tail of the ADC data queue */

Frame   adc_data_queue[Max_Queue_Len+2];
/* the circular queue of the ADC data */

char                can_point;
/* if it's time to do the point test */

pthread_mutex_t     can_point_lock;
/* the lock of the flag variant can_match */


void ADC_init()
{
    /* 
     * ===  FUNCTION  ======================================================================
     *         Name:  ADC_init
     *  Description:  ADC initialization
     * =====================================================================================
     */
    int arg;
    int status;

    /* open the audio device */
    voice_device_fd = open("/dev/dsp", O_RDONLY);
    if( voice_device_fd < 0)
    {
        perror("open of /dev/dsp failed"); 
        exit(1);
    }

    /* set the sample size*/
    arg = Sample_Size;
    status = ioctl(voice_device_fd, SOUND_PCM_WRITE_BITS, &arg);
    if (status == -1)
        perror("SOUND_PCM_WRITE_BITS ioctl failed");
    if (arg != Sample_Size)
        perror("unable to set sample size");

    /* set number of channels*/
    arg = Channels; 
    status = ioctl(voice_device_fd, SOUND_PCM_WRITE_CHANNELS, &arg);
    if (status == -1)
        perror("SOUND_PCM_WRITE_CHANNELS ioctl failed");
    if (arg != Channels)
        perror("unable to set number of channels");

    /* set the frequency of sample */
    arg = Rate;
    status = ioctl(voice_device_fd, SOUND_PCM_WRITE_RATE, &arg);
    if (status == -1)
        perror("SOUND_PCM_WRITE_WRITE ioctl failed");

    //initialize the data queue
    memset(adc_data_queue, 0, sizeof(adc_data_queue));
    queue_h = queue_t = 0;
}

void  get_ADC_data()
{
    /* 
     * ===  FUNCTION  ======================================================================
     *         Name:  get_ADC_data
     *  Description:  get the ADC results and put it into the buffer
     * =====================================================================================
     */
    
    int status;
    int num = 0;
    Adc_Type buffer1[Max_Frame_Len];
    while( num < 5)
    {
        pthread_mutex_unlock( &can_point_lock);
        status = read( voice_device_fd, adc_data_queue[queue_t].buffer, sizeof(
                     adc_data_queue[queue_t].buffer));
         if (status != sizeof(buffer1))
             perror("read wrong number of bytes\n");
         queue_t ++;
         if( queue_t > Max_Queue_Len - 1) 
         {
             queue_t = 0;
         }
//             printf("queue_t = %d\n", queue_t);
         num ++;
    }
}

