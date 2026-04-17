#include "stm32f407.h"
#include<stdint.h>
class LED
{
	uint32_t  pin;
	static uint32_t count;
public:
	LED(uint32_t p=0)
	{
		pin=p;
		RCC->AHB1ENR|=(1<<3)|(1<<0);
		GPIOD->MODER &=~(3<<(pin*2));  // GPIOD
		GPIOD->MODER |=(1<<(pin*2));

		GPIOA->MODER &=~(3<<0);     //GPIOA

	}
	void status()
	{
		if(GPIOA->IDR&1)
		{
			GPIOD->ODR|=(1<<pin);
			count++;
		}
		else
		{
			GPIOD->ODR&=~(1<<pin);
		}
	}
};
uint32_t LED::count = 0;
int main()
{
	LED l(12);
	while(1)
	{
		l.status();

	}
}

