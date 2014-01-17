/******************************************************************************/
/* System Level #define Macros                                                */
/******************************************************************************/

#include "mcu.h"
/* Macros */
#define countof(a)   (sizeof(a) / sizeof(*(a)))

#define DEFAULT_INTERRUPT_LEVEL 0x0F

#define NVIC_PRIORITYGROUP0 0
#define NVIC_PRIORITYGROUP1 4
#define NVIC_PRIORITYGROUP2 5
#define NVIC_PRIORITYGROUP3 6
#define NVIC_PRIORITYGROUP4 7

#if defined(NVIC_PRIORITYGROUP0)
#define NVIV_PREEMPTPRIORITY_MAX 0x0F
#define NVIV_SUBPRIORITY_MAX 0x00
#elif defined(NVIC_PRIORITYGROUP1)
#define NVIV_PREEMPTPRIORITY_MAX 0x01
#define NVIV_SUBPRIORITY_MAX 0x07
#elif defined(NVIC_PRIORITYGROUP2)
#define NVIV_PREEMPTPRIORITY_MAX 0x03
#define NVIV_SUBPRIORITY_MAX 0x03
#elif defined(NVIC_PRIORITYGROUP3)
#define NVIV_PREEMPTPRIORITY_MAX 0x07
#define NVIV_SUBPRIORITY_MAX 0x01
#elif defined(NVIC_PRIORITYGROUP4)
#define NVIV_PREEMPTPRIORITY_MAX 0x00
#define NVIV_SUBPRIORITY_MAX 0x0F
#endif

/******************************************************************************/
/* System Function Prototypes                                                 */
/******************************************************************************/
#define APB0_Clock (SystemCoreClock / 2)
#define APB1_Clock (SystemCoreClock / 2)
#define APB2_Clock (SystemCoreClock / 2)
/* Custom oscillator configuration funtions, reset source evaluation
functions, and other non-peripheral microcontroller initialization functions
go here. */
#define SYS_CODE "FM3-TEST-20140106"
#define __AP_NAME__    "ETHERNET APP"
#define __AP_VERSION__    "1.0"
#define MAIN_VERSION    1
#define SUB_VERSION    0
/*
AP version
v0.1		- first draft version
v0.2		- adjust program
v1.0		- first release version
v1.1		- 
v1.2		- 
*/

#define FLASH_SIZE_MIN 0
#define FLASH_SIZE_MAX 0x20000
#define RAM_SIZE_MIN 0x1FFFE000
#define RAM_SIZE_MAX 0x20002000

