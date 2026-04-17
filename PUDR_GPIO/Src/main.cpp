#include<cstdint>
#include"stm32f407.h"
class GPIO
{
	uint32_t pin;
public:
	GPIO(uint32_t p)noexcept
	{
		//if(p>15)
			//throw out_of_range("GPIO");

		pin=p;
		RCC->AHB1ENR|=(1<<3);  //enable GPIOD clock
		RCC->AHB1ENR|=(1<<0);   // enable GPIOA clock
		GPIOD->MODER&=~(3<<pin*2);  // clear the GPIOD_Moder pin
		GPIOD->MODER|=(1<<pin*2);  // set GPIOD_MODER pin
		GPIOA->MODER&=~(3<<0);    //clear GPIOA pin
		GPIOA->PUPDR&=~(3<<pin*2);  // clear PUPDR pin
		GPIOA->PUPDR|=(1<<pin);   //pull-up register
	}
	void status()
	{
		if(GPIOA->IDR&1)   // check whether switch pressed or not
		{
			GPIOD->ODR|=(1<<pin);
		}
		else
		{
			GPIOD->ODR|=(1<<pin);
		}
	}
};
int main()
{
	GPIO g(13);

	while(1)
	{
		g.status();
		for(uint32_t i=0;i<100000;i++);
	}
}
