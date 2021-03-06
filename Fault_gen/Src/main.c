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

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

#include <stdio.h>
#include <stdint.h>

int main(void)
{
    //1. Enable all configurable fault exceptions like usage fault, mem manage fault and bus fault.
	uint32_t *pSHCSR = (uint32_t*)0xE000ED24;
	*pSHCSR |= (1<<16); //Activate mem manage fault.
	*pSHCSR |= (1<<17); //Activate bus fault.
	*pSHCSR |= (1<<18); //Activate usage fault.

	//2. Lets force the processor to execute some undefined instruction.
	uint32_t *pSRAM = (uint32_t*)0x20010000;
	*pSRAM = 0xFFFFFFFF;
	void (*some_address)(void);
	some_address = (void*)0x20010001; //Bit 0 is changed to 1 due to T-bit. Instead of 0x20010000 we write 0x20010001.
	some_address();

	//4. Analyze the fault.

	for(;;);
}

//3. Implement the fault handlers.

	void HardFault_Handler(void)
	{
		printf("Fault : HardFault_Handler\n");
		while(1);
	}

	void MemManage_Handler(void)
	{
		printf("Fault : MemManage_Handler\n");
		while(1);
	}

	void BusFault_Handler(void)
	{
		printf("Fault : BusFault_Handler\n");
		while(1);
	}

	void UsageFault_Handler(void)
	{
		//Checking the details of the Usage Fault by exploring the UFSR(Usage Fault Status Register)
		uint16_t *pUFSR = (uint16_t*)0xE000ED2A;
		printf("UFSR = %X\n",( (*pUFSR) & 0xFFFF) );

		printf("Fault : UsageFault_Handler\n");
		while(1);
	}
