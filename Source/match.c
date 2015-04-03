
/*-----------------------------------------------------------------------------
 *  this part use dynamic programming to solve the pattern_match, use the dp
 *  algorithm to get the minimun distance between the input data and every
 *  mould, the mould which has the minimun value is the answer, the dp algorithm
 *  here is simple according to the paper
 *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *
 *  All the rights of this shit code belong to mightyvoice 2011-5-2 17:24:16
 *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *
 *-----------------------------------------------------------------------------*/

#include "includes.h"

Data_Type   audio_mould[Mould_num][Max_audio_frame_num][Mel_factor_num];
/* the 0-9 digit audio's mfcc factor */

int         mould_len[Mould_num+2];
/* every mould have how many frame number */

Data_Type   dp[Max_audio_frame_num][Max_audio_frame_num];
/* the array for the dynamic programming, dp[n][m] means:
 * the minimun accumulated distance between the input data
 * and mould through a path ended with point (n, m) */

char        vis[Max_audio_frame_num][Max_audio_frame_num];

char    dir[3][2] = {{0,-1},{-1,-1},{-1,0}};
/* the three adjecent grid of current grid */

int     n_len, m_len;
/* current length of mould and the input data, used in dfs procedure */

char    mould_path[][3] = 
{
    "00", "01", "02", "03", "04", "05", "06",
    "10", "11", "12", "13", "14", "15", "16",
    "20", "21", "22", "23", "24", "25", "26",
    "30", "31", "32", "33", "34", "35", "36",
    "40", "41", "42", "43", "44", "45", "46",
    "50", "51", "52", "53", "54", "55", "56",
    "60", "61", "62", "63", "64", "65", "66",
    "70", "71", "72", "73", "74", "75", "76",
    "80", "81", "82", "83", "84", "85", "86",
    "90", "91", "92", "93", "94", "95", "96",
}; 

void pattern_match_init()
{
    /* 
     * ===  FUNCTION  ======================================================================
     *         Name:  pattern_match_init
     *  Description:  read in the mould in 10 files
     * =====================================================================================
     */
    FILE    *fd;
    int     i;
    int     j;
    int     k;
    int     l; 
    for( i = 0; i < Mould_num; i++)
    {
        fd = fopen( mould_path[i], "rd");
        fscanf( fd, "%d", &k);
        mould_len[i] = k;
        for( j = 0; j < k; j++)
        {
            for( l = 0; l < Mel_factor_num; l++)
            {
                fscanf( fd, "%lf", &audio_mould[i][j][l]);
            }
        }
        fclose(fd);
    }
    /* 
    for( i = 0; i < Mould_num; i++)
    {
        printf("%d\n", mould_len[i]);
        for( j = 0; j < mould_len[i]; j++)
        {
            for( k = 0; k < Mel_factor_num; k++)
            {
                printf("%.5f ", audio_mould[i][j][k]);
            }
            printf ( "\n" );
        }
        printf ( "\n\n\n" );
    }
    */
    
}

Data_Type   dfs( int x, int n, int m)
{
    /* 
     * ===  FUNCTION  ======================================================================
     *         Name:  dfs
     *  Description:  do the memorized search to get the result of dynamic programming
     *  this is a very simple dp problem
     * =====================================================================================
     */
    int i;
    int dx;
    int dy;
    Data_Type min = oo;
    Data_Type t;
    if( vis[n][m])            /* if the value has been got */
    {
        return( dp[n][m]);
    }
    for( i = 0; i < 3; i++)                     /* get the min value of three direction */
    {
        dx = n + dir[i][0];
        dy = m + dir[i][1];
        //printf ( "dx = %d  dy = %d\n", dx, dy );
        if( dx >= 0 && dx < n_len && dy >= 0 && dy < m_len)
        { 
            t = dfs( x, dx, dy);
            //printf ( "t = %.5f\n", t );
            if( t < min)
            {
                min = t;
            }
        }
    }
    //printf ( "min = %.5f\n",min );
    t = 0;
    for( i = 0; i < Mel_factor_num; i++)        /* get current distance of n and m */
    {
        t += fabs( audio_mould[x][m][i] - mfcc_factor[n][i]);
    }
    dp[n][m] = t + min;
    vis[n][m] = 1;
    return ( t+min);
}

char signal_pattern_match(int n)
{
    /* 
     * ===  FUNCTION  ======================================================================
     *         Name:  signal_pattern_match
     *  Description:  the main procedure of pattern_match return the match result
     *  n is the input data's length
     * =====================================================================================
     */
    int i, j, l;
    int k = 0;
    int m;
    Data_Type dis = oo;
    Data_Type sum;
    n_len = n;
    for( i = 0; i < Mould_num; i++)
    {
        m = mould_len[i];
        m_len = m;
        sum = 0;
        /* if the two arrays of data have the same length
         * the we can calculate the distance between them
         * directly, otherwise we use dp algorithm */
        if( m == n)
        {
            for( j = 0; j < m; j++)
            {
                for( l = 0; l < Mel_factor_num; l++)
                {
                    sum += fabs( audio_mould[i][j][l] - mfcc_factor[j][l]);
                }
            }
//            printf("The distance with mould %s = %.5f\n",mould_path[i],  sum);
            if( sum < dis)            /* if current value is smaller*/
            {
                dis = sum;
                k = i;
            }  
        }
        else
        {
            memset( vis, 0, sizeof(vis));
            for( j = 0; j < Mel_factor_num; j++) /* get the start value of dp array */
            {
                sum += fabs( audio_mould[i][0][j] - mfcc_factor[0][j]);
            }
            dp[0][0] = sum;
            vis[0][0] = 1;
            sum = dfs( i, n-1, m-1);            /* get the value of dp[n-1][m-1] */
//            printf("The distance with mould %s = %.5f\n",mould_path[i],  sum);
            if( sum < dis)
            {
                dis = sum;
                k = i;
            }
        }
    }
    printf("The distance with mould %s is minimun and the ", mould_path[k]);
    return( ( k / 7));
}

