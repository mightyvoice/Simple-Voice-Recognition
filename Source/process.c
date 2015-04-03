
#include "includes.h"

void audio_indent_process_init()
{
    /* 
     * ===  FUNCTION  ======================================================================
     *         Name:  audio_indet_process_init
     *  Description:  the initialization of the audio identification 
     *  process
     * =====================================================================================
     */

    hamming_init();/* initialize hamming_window */
    mfcc_init(); /* initialize mfcc calculation */
    pattern_match_init();
    ADC_init();
    audio_start_test_init();

}

void audio_indenti_process()
{
    /* 
     * ===  FUNCTION  ======================================================================
     *         Name:  audio_indent_process
     *  Description:  the main procedure of the audio identification
     * =====================================================================================
     */

    audio_indent_process_init();

    while(1)
    {
        get_ADC_data();
        audio_start_test();
    }
    return;
}
