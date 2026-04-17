#include<cstdint>
#include<cstdio>
#include"stm32f407.h"
template<uint32_t pin>
class print
{
public:
	print()
	{
		RCC->AHB1ENR|=(1<<3);
		GPIOD->MODER&=~(3<<(pin*2));
		GPIOD->MODER|=(1<<(pin*2));

		RCC->AHB1ENR|=(1<<0);
		GPIOA->MODER&=~(3<<0);

	}
	~print()
	{
		RCC->AHB1ENR&=~(1<<3);
	}
	void status()
	{
		if(GPIOA->IDR&1)
		{
			GPIOD->ODR|=(1<<pin);
			printf(" button is pressed\n");
			delay();
		}
		else
		{
			GPIOD->ODR&=~(1<<pin);
			printf(" button is not  pressed\n");
			delay();
		}
	}
	void delay()
	{
		for(volatile uint32_t i=0;i<500000;i++);
	}

};
int main()
{
	print<12>p;
	while(1)
	{
		p.status();
		p.delay();
	}
}
