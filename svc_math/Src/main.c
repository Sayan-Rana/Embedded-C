/*Write a program to add, subtract, multiply and divide 2 operands using
 * SVC handler and return the result to the thread mode code and print the
 * result. Thread mode code should pass 2 operands via the stack frame.
 *
 * 36 for addition
 * 37 for subtraction
 * 38 for multiplication
 * 39 for division
 */

#include <stdio.h>
#include <stdint.h>

void SVC_Handler_c(uint32_t *pBaseOfStack);

int32_t add_numbers(uint32_t x, uint32_t y)
{
	int32_t res;
	__asm volatile ("SVC #36");
	__asm volatile ("MOV %0,R0":"=r"(res)::);
	return res;
}

int32_t sub_numbers(uint32_t x, uint32_t y)
{
	int32_t res;
	__asm volatile ("SVC #37");
	__asm volatile ("MOV %0,R0":"=r"(res)::);
	return res;
}

int32_t mul_numbers(uint32_t x, uint32_t y)
{
	int32_t res;
	__asm volatile ("SVC #38");
	__asm volatile ("MOV %0,R0":"=r"(res)::);
	return res;
}

int32_t div_numbers(uint32_t x, uint32_t y)
{
	int32_t res;
	__asm volatile ("SVC #39");
	__asm volatile ("MOV %0,R0":"=r"(res)::);
	return res;
}

int main(void)
{
    int32_t res;

    res = add_numbers(40, -90);
    printf("Add result : %ld\n",res);

    res = sub_numbers(25, 150);
    printf("Subtraction result : %ld\n",res);

    res = mul_numbers(374, 890);
    printf("Multiplication result : %ld\n",res);

    res = div_numbers(67, -3);
    printf("Division result : %ld\n",res);

	for(;;);
}

__attribute__((naked)) void SVC_Handler(void)
{
	__asm volatile ("MRS r0,MSP");
	__asm volatile ("B SVC_Handler_c");
}

void SVC_Handler_c(uint32_t *pBaseOfStack)
{
	printf("In SVC handler\n");

	int32_t arg0, arg1, res;

	uint8_t *pReturn_addr = (uint8_t*)pBaseOfStack[6];

	pReturn_addr -= 2;

	uint8_t SVC_number = *pReturn_addr;

	printf("SVC number is : %d\n",SVC_number);

	arg0 = pBaseOfStack[0];
	arg1 = pBaseOfStack[1];

	switch(SVC_number)
	{
		case 36:
			res = arg0 + arg1;
			break;
		case 37:
			res = arg0 - arg1;
			break;
		case 38:
			res = arg0 * arg1;
			break;
		case 39:
			res = arg0 / arg1;
			break;
		default:
			printf("Invalid SVC number\n");
	}
	pBaseOfStack[0] = res;
}
