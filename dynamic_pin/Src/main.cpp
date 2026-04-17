#include<cstdint>
#include"stm32f407.h"

enum class GPIO : uint32_t {
GPIOA_EN =0,
GPIOB_EN =1,
GPIOC_EN =2,
GPIOD_EN =3
};

class GPIO_port
{
	uint32_t*pin;
public:
	GPIO_port(uint32_t p)
	{
		pin=new uint32_t(p);

		RCC->AHB1ENR|=(1<<static_cast<uint32_t>(GPIO::GPIOD_EN));
		RCC->AHB1ENR|=(1<<static_cast<uint32_t>(GPIO::GPIOA_EN));
		GPIOD->MODER&=~(3<<(*pin*2));
		GPIOD->MODER|=(1<<(*pin*2));
		GPIOA->MODER&=~(3<<0);

	}
	~GPIO_port()
	{
		delete pin;
		RCC->AHB1ENR&=~(1<<static_cast<uint32_t>(GPIO::GPIOD_EN));
		RCC->AHB1ENR&=~(1<<static_cast<uint32_t>(GPIO::GPIOA_EN));
	}
	void status()
	{
			GPIOD->BSRR|=(1<<*pin);
			delay();

			GPIOD->BSRR|=(1<<(*pin+16));
			delay();
	}
	void delay()
	{
		for(uint32_t i=0;i<300000;i++);
	}
};
int main()
{
	GPIO_port p(13);
	while(1)
	{
		if(GPIOA->IDR&1)
		{
			break;
		}
		p.status();
	}
}
