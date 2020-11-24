/*Write a program to execute an SVC instruction from thread mode, implement
 * the SVC handler to print the SVC number used.
 * Also, increment the SVC number by 4 and return it to the thread mode code and print it.
 * Hints :
 * 1) Write a main() function where you should execute the SXC instruction with an argument.
 * assume SVC #0x05
 * 2) Implement the SVC handler.
 * 3) In the SVC handler extract the SVC number and print it using printf().
 * 4) Increment the SVC number by 4 and return it to the thread mode code.
 */

#include <stdio.h>
#include <stdint.h>

void SVC_Handler_c(uint32_t *pBaseOfStackFrame);

int main(void)
{
    __asm volatile ("SVC #24");

//5. Receiving the incremented SVC number.
	uint8_t data;

	//Method 1(Recommended)
	__asm volatile ("MOV %0,r0":"=r"(data)::);

/*	//Method 2(Not recommended)
	register uint8_t data __asm("r0");
*/

    printf("SVC number after increment : %d\n",data);

    for(;;);
}

__attribute__((naked)) void SVC_Handler(void)
{
	__asm volatile ("MRS r0,MSP");
	__asm volatile ("B SVC_Handler_c");
}

//1. Get the value of MSP.
void SVC_Handler_c(uint32_t *pBaseOfStackFrame)
{
	printf("in SVC handler\n");

	uint8_t *pReturn_addr = (uint8_t*)pBaseOfStackFrame[6];

	//2. Decrement the return address by 2 to point to
	//opcode of the SVC instruction in the program memory.
	pReturn_addr -= 2;

	//3. Extract the SVC number from the LSB of the opcode.
	uint8_t SVC_number = *pReturn_addr;

	printf("SVC number is : %d\n", SVC_number);

	//4. Increment the SVC number by 4.
	SVC_number += 4;

	pBaseOfStackFrame[0] = SVC_number;
}
