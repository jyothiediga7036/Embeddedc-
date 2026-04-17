#include"stm32f407.h"
#include<cstdint>
template<std::uint32_t pin>
class GPIO
{
	GPIO()
	{
		RCC->AHB1ENR|=(1<<3);
		RCC->AHB1ENR|=(1<<0);
	}
	~GPIO()
	{
		RCC->AHB1ENR&=~(1<<3);
		RCC->AHB1ENR&=~(1<<0);
	}
	GPIO(const GPIO&)=delete;
	GPIO& operator=(const GPIO&)=delete;

public:
	static GPIO& instant()
	{
		static GPIO g;
		return g;
	}
	void enable_moder()
	{
		GPIOD->MODER&=~(3<<pin*2);
		GPIOD->MODER|=(1<<pin*2);
		GPIOA->MODER&=~(3<<0);
	}
	void on()
	{
		GPIOD->ODR|=(1<<pin);
	}
	void off()
	{
		GPIOD->ODR&=~(1<<pin);
	}

};
int main()
{
	auto& p=GPIO<13>::instant();
	auto &p1=GPIO<13>::instant();
	auto &p2=GPIO<14>::instant();

	p.enable_moder();
	p2.enable_moder();
	while(1)
	{
		p.on();
		for(uint32_t i=0;i<100000;i++);

		p1.off();
		for(uint32_t i=0;i<100000;i++);

		p2.on();
		for(uint32_t i=0;i<100000;i++);

		p2.off();
		for(uint32_t i=0;i<100000;i++);

	}
}
