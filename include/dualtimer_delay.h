#ifndef __DT_DELAY_H
#define __DT_DELAY_H

/* General Inclusion */
#include "mcu.h"

/* Timer Interval MicroSecond order (in Hz) */
#define USEC_INTERVAL 	1000000UL

/* Inline Static Function */

/* Externals */
extern void _delay_init(void);
extern void _delay_ms(__IO uint32_t mSec);
extern void _delay_us(__IO uint32_t uSec);

#endif /* __DT_DELAY_H */
