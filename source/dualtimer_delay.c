/* Includes ------------------------------------------------------------------*/
#include "system.h"
#include "dualtimer_delay.h"
/* Defines -------------------------------------------------------------------*/

/* Variables -----------------------------------------------------------------*/

/* Constants -----------------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/* Functions -----------------------------------------------------------------*/

void _delay_init(void)
{
	/* Microsecond Order Uses FM3 DUAL Timer(PCLK0 = HCLK/1 =20MHz) */
	FM3_DTIM->TIMER1CONTROL_f.TIMERSIZE	 = 1;									/* 32bit Counter 	 */
	FM3_DTIM->TIMER1CONTROL_f.TIMERMODE	 = 1;									/* Cyclic Mode   	 */
	FM3_DTIM->TIMER1CONTROL_f.TIMERPRE0	 = 0;									/* PreScale PCLK0/1 */
	FM3_DTIM->TIMER1CONTROL_f.TIMERPRE1	 = 0;									/* PreScale PCLK0/1 */
	FM3_DTIM->TIMER1LOAD				 = (APB0_Clock/USEC_INTERVAL)-1;        /* PCLK0 = HCLK/2    */
}

/**************************************************************************/
/*! 
    @brief	Delay Millisecond Order (upto 16777214mSec!)
	@param	mSec: 24-bit value due to systick downcount timer
    @retval	none
*/
/**************************************************************************/
void _delay_ms(__IO uint32_t mSec)
{ 
    uint32_t usec;
    if(mSec > 4294967){
        usec = 0xffffffff;
    }else{
        usec = mSec * 1000;
    }
    _delay_us(usec);
}

/**************************************************************************/
/*! 
    @brief	Delay Microsecond Order (upto 4294967296uSec!)
	@param	uSec: 32-bit value due to dual upcount timer
    @retval	none
*/
/**************************************************************************/
void _delay_us(__IO uint32_t uSec)
{
	bFM3_DTIM_TIMER1CONTROL_TIMEREN = 1;
	while(uSec--) {
		while(!bFM3_DTIM_TIMER1MIS_TIMER1MIS);
		FM3_DTIM->TIMER1INTCLR =0;
	}
	bFM3_DTIM_TIMER1CONTROL_TIMEREN = 0;
}

/* End Of File ---------------------------------------------------------------*/

