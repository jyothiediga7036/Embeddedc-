#include<stdio.h>
int main(void)
{
	printf("hello world\n");
	for(;;);
}
/*#include<cstdint>
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
	}
	~print()
	{
		RCC->AHB1ENR&=~(1<<3);
	}
	void status()
	{
		GPIOD->ODR^=(1<<pin);
	}

};
void delay()
{
	for(volatile uint32_t i=0;i<500000;i++);
}
int main()
{
	print<12>p;
	while(1)
	{
		p.status();
		delay();
	}
}*/
