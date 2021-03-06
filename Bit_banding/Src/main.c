/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

#include <stdint.h>
#define ALIAS_BASE     0X22000000U
#define BIT_BAND_BASE  0X20000000U

int main(void)
{
	uint8_t *ptr = (uint8_t*) 0X20000200;

	*ptr = 0XFF;
	//Clearing the 7th bit position by using NORMAL method
	*ptr &= ~(1 << 7);

	*ptr = 0XFF;

	uint8_t *alias_addr = (uint8_t*) (ALIAS_BASE+(32*(0X20000200 - BIT_BAND_BASE)) + (7*4));

	//Clearing 7th bit using Bit Banding method
	*alias_addr = 0;

    /* Loop forever */
	for(;;);
}
