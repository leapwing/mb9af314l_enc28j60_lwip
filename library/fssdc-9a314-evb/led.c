/*----------------------------------------------------------------------------
 * Name:    LED.c
 * Purpose: low level LED functions
 * Note(s):
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2011 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include "mcu.h"
#include "LED.h"

volatile unsigned int* led_array[3] = {&bFM3_GPIO_PDOR2_P3, &bFM3_GPIO_PDOR1_P9, &bFM3_GPIO_PDOR1_P8};
/*----------------------------------------------------------------------------
  initialize LED Pins
 *----------------------------------------------------------------------------*/
void LED_Init (void) {

  /* Set ADE of P19 and P18 */
  bFM3_GPIO_ADE_AN9 = 0;
  bFM3_GPIO_ADE_AN8 = 0;
  /* Set function to GPIO */
  bFM3_GPIO_PFR2_P3 = 0;
  bFM3_GPIO_PFR1_P9 = 0;
  bFM3_GPIO_PFR1_P8 = 0;
  /* Set direction as output */
  bFM3_GPIO_DDR2_P3 = 1;
  bFM3_GPIO_DDR1_P9 = 1;
  bFM3_GPIO_DDR1_P8	= 1;
  /* Set Level */
  bFM3_GPIO_PDOR2_P3 = 1;
  bFM3_GPIO_PDOR1_P9 = 1;
  bFM3_GPIO_PDOR1_P8 = 1;
}

/*----------------------------------------------------------------------------
  Function that turns on requested LED
 *----------------------------------------------------------------------------*/
void LED_On (unsigned int num) {

  *led_array[num] = 0;
}

/*----------------------------------------------------------------------------
  Function that turns off requested LED
 *----------------------------------------------------------------------------*/
void LED_Off (unsigned int num) {

  *led_array[num] = 1;
}

/*----------------------------------------------------------------------------
  Function that outputs value to LEDs
 *----------------------------------------------------------------------------*/
void LED_Out(unsigned int value) {
  int i;

  for (i = 0; i < LED_NUM; i++) {
    if (value & (1<<i)) {
      LED_On (i);
    } else {
      LED_Off(i);
    }
  }
}
