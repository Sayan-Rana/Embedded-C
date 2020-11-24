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

#include <stdio.h>
#include <stdint.h>
#include "main.h"
#include "led.h"



void task1_handler(void);  //This is task_1
void task2_handler(void);  //This is task_2
void task3_handler(void);  //This is task_3
void task4_handler(void);  //This is task_4

void init_systic_timer(uint32_t tick_hz);
__attribute ((naked)) void init_scheduler_stack(uint32_t sched_top_of_stack);
void init_task_stack(void);
void enable_precessor_faults(void);
uint32_t get_psp_value(void);
__attribute ((naked)) void switch_sp_to_psp(void);



uint8_t current_task = 0; //Task1 is running.


typedef struct
{
	uint32_t psp_value;
	uint32_t block_count;
	uint8_t  current_state;
	void (*task_handler)(void);
}TCB_t;

TCB_t user_task[MAX_TASK];



int main(void)
{
	enable_precessor_faults();

	init_scheduler_stack(SCHED_STACK_START);

	init_task_stack(); //Creating dummy stack frame.

	led_init_all();

	init_systic_timer(TICK_HZ);

	switch_sp_to_psp();

	task1_handler();

	for(;;);
}


void task1_handler(void)
{
	while(1)
	{
		led_on(LED_GREEN);
		delay(DELAY_COUNT_1S);
		led_off(LED_GREEN);
		delay(DELAY_COUNT_1S);
	}
}


void task2_handler(void)
{
	while(1)
	{
		led_on(LED_ORANGE);
		delay(DELAY_COUNT_500MS);
		led_off(LED_ORANGE);
		delay(DELAY_COUNT_500MS);
	}
}


void task3_handler(void)
{
	while(1)
	{
		led_on(LED_BLUE);
		delay(DELAY_COUNT_250MS);
		led_off(LED_BLUE);
		delay(DELAY_COUNT_250MS);
	}
}


void task4_handler(void)
{
	while(1)
	{
		led_on(LED_RED);
		delay(DELAY_COUNT_125MS);
		led_off(LED_RED);
		delay(DELAY_COUNT_125MS);
	}
}


void init_systic_timer(uint32_t tick_hz)
{
	//Address of SYSTICK reload value resister.
	uint32_t *pSYST_RVR = (uint32_t*)0xE000E014;
	//SYSTICk control and status register.
	uint32_t *pSYST_CSR = (uint32_t*)0xE000E010;

	//Reload value calculation.
	uint32_t count_value = (SYSTICK_TIM_CLK/tick_hz);

	//Clear the value of SYST_RVR(reload value resister).
	*pSYST_RVR &= ~(0x00FFFFFF);
	//Load the value into SYST_RVR(reload value resister).
	*pSYST_RVR |= (count_value -1); //-1 is required due to SYSTICK exception will triggered after copying the value from RVR to CVR.

	//Do some settings
	*pSYST_CSR |= (0x1 << 1);  //Enable SYSTICK exception request.
	*pSYST_CSR |= (0x1 << 2);  //Indicate the clock source, processor clock source.

	//Enable the SYSTICK
	*pSYST_CSR |= (0x1 << 0);  // Enable the SYSTICK counter.

}


__attribute ((naked)) void init_scheduler_stack(uint32_t sched_top_of_stack)
{
	//Using general purpose register to move the value in MSP.
	//__asm volatile ("MSR MSP,R0");

	//Using GCC inline assembly syntax using C variable.
	__asm volatile ("MSR MSP,%0"::"r"(sched_top_of_stack): );

	//Loading the value of LR into PC because this is a naked function and we must go back to main after executing this function.
	//Naked function does not have any epilogue and prologue sequence.
	__asm volatile ("BX LR");
}




void init_task_stack(void)
{
	user_task[0].current_state = TASK_RUNNING_STATE;
	user_task[1].current_state = TASK_RUNNING_STATE;
	user_task[2].current_state = TASK_RUNNING_STATE;
	user_task[3].current_state = TASK_RUNNING_STATE;

	user_task[0].psp_value = T1_STACK_START;
	user_task[1].psp_value = T2_STACK_START;
	user_task[2].psp_value = T3_STACK_START;
	user_task[3].psp_value = T4_STACK_START;

	user_task[0].task_handler = task1_handler;
	user_task[1].task_handler = task2_handler;
	user_task[2].task_handler = task3_handler;
	user_task[3].task_handler = task4_handler;

	uint32_t *pPSP;
	for(int i = 0; i < MAX_TASK; i++)
	{
		pPSP = (uint32_t*)user_task[i].psp_value;

		pPSP--;
		*pPSP = DUMMY_xPSR; //value should be 0x01000000.

		pPSP--;
		*pPSP = (uint32_t)user_task[i].task_handler;  //PC value.

		pPSP--;
		*pPSP = 0xFFFFFFFD;  //LR value.

		for(int j = 0; j < 13; j++)
		{
			pPSP--;
			*pPSP = 0;
		}
		//Saving the address of the stack pointer(in this case PSP).
		user_task[i].psp_value = (uint32_t)pPSP;
	}
}


uint32_t get_psp_value(void)
{
	return user_task[current_task].psp_value;
}


__attribute ((naked)) void switch_sp_to_psp(void)
{
	//1. Initialize the PSP with Task1 stack start address.

	//Get the value of the PSP of the current task.
	__asm volatile ("PUSH {LR}"); //Preserve LR into stack, which connects back to main()
	__asm volatile ("BL get_psp_value");
	__asm volatile ("MSR PSP,R0"); //Initialize PSP.
	__asm volatile ("POP {LR}"); // Pops back LR value.

	//2. Change SP to PSP using Control register.
	__asm volatile ("MOV R0,#0x02");
	__asm volatile("MSR CONTROL,R0");
	__asm volatile ("BX LR");
}


void enable_precessor_faults(void)
{
    //1. Enable all configurable fault exceptions like usage fault, mem manage fault and bus fault.
	uint32_t *pSHCSR = (uint32_t*)0xE000ED24;
	*pSHCSR |= (1<<16); //Activate mem manage fault.
	*pSHCSR |= (1<<17); //Activate bus fault.
	*pSHCSR |= (1<<18); //Activate usage fault.
}


void save_psp_value(uint32_t current_psp_value)
{
	user_task[current_task].psp_value = current_psp_value;
}


void update_next_task(void)
{
	current_task++;
	current_task %= MAX_TASK;
}


__attribute ((naked)) void SysTick_Handler(void)
{
	/*Save the context of the current task*/

	//1. Get current running task's PSP value.
	__asm volatile ("MRS R0,PSP");
	//2. Using that PSP value store SF2(R4 to R11).
	__asm volatile ("STMDB R0!,{R4-R11}");

	//First we have to save the LR value of the calling function before call any function.
	__asm volatile ("PUSH {LR}");

	//3. Save the current current value of the PSP.
	__asm volatile ("BL save_psp_value");

	/*Retrieve the context of the next task*/

	//1. Decide next task to run.
	__asm volatile ("BL update_next_task");

	//2. Get its past PSP value.
	__asm volatile ("BL get_psp_value");

	//3. Using that PSP value retrieve SF2(R4 to R11).
	__asm volatile ("LDMIA R0!,{R4-R11}");

	//4. Update PSP and exit.
	__asm volatile ("MSR PSP,R0");

	//After function call we have to POP back the LR value.
	__asm volatile ("POP {LR}");

	__asm volatile ("BX LR");
}


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
	printf("Fault : UsageFault_Handler\n");
	while(1);
}

