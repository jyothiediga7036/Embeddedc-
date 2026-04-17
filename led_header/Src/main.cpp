#include"stm32f407.h"
#include<stdint.h>
int main()
{
	RCC->AHB1ENR|=(1U<<3);
	GPIOD->MODER &=~(3U<<24);
	GPIOD->MODER |=(1U<<24);
	while(1)
	{
		GPIOD->ODR|=(1U<<12);
		for(uint32_t i=0;i<300000;i++);
		GPIOD->ODR&=~(1U<<12);
		for(uint32_t i=0;i<300000;i++);
	}
}
