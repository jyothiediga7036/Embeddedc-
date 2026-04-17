#include<stdint.h>
#include"stm32f407.h"
class Transformer
{
	uint32_t voltage;
	uint32_t current;
	uint32_t power;
public:
	Transformer(uint32_t v=0,uint32_t i=0,uint32_t p=0)
	{
		voltage=v;
		current=i;
		power=p;
	}
	uint32_t power_calculation()
	{
		power=(voltage*current);
		uint32_t p=(power*100)/200;
		if(p>100)
			return 0;

		return p;
	}
	void reset_power()
	{
		power = 0;
		voltage = 0;
		current = 0;
		power = 0;
	}

};
class Performance
{
	uint32_t green;
	uint32_t orange;
	uint32_t red;
	uint32_t blue;
public:
	Performance(uint32_t g,uint32_t o,uint32_t r,uint32_t b)
	{
		green=g;
		orange=o;
		red=r;
		blue=b;
		RCC->AHB1ENR |= (1U<<3) | (1U<<0);
		GPIOD->MODER &= ~((3U<<(green*2)) | (3U<<(orange*2)) | (3U<<(red*2)) | (3U<<(blue*2)));
		GPIOD->MODER |=  ((1U<<(green*2)) | (1U<<(orange*2)) | (1U<<(red*2)) | (1U<<(blue*2)));
		GPIOA->MODER &= ~(3U<<0);
	}
	void Transformer_status(Transformer& obj)
	{
		if(GPIOA->IDR & 1)
		{
			uint32_t p = obj.power_calculation();
			if(p==0)
			{
				all_off();
				    on(red);
				return;
			}

			if(p<=25)
			{
				on(blue);
				for(uint32_t i=0;i<50000;i++);
			}
			else if(p>25&&p<=50)
			{
				on(green);
				for(uint32_t i=0;i<50000;i++);
			}
			else if(p>50&&p<=75)
			{
				on(orange);
				for(uint32_t i=0;i<50000;i++);
			}
			else
			{
				on(red);
				for(volatile uint32_t i=0; i<10000; i++);
				off(red);
				obj.reset_power();
			}
		}
		else
		{
			all_off();
		}
	}
	void on(uint32_t pin)
	{
		GPIOD->ODR|=(1<<pin);
	}
	void off(uint32_t pin)
	{
		GPIOD->ODR&=~(1<<pin);
	}
	void all_off()
	{
		GPIOD->ODR&=~(1<<green);
		GPIOD->ODR&=~(1<<orange);
		GPIOD->ODR&=~(1<<red);
		GPIOD->ODR&=~(1<<blue);
	}
};
int main()
{
	Transformer t(12,5);     // 60w ->30%
	Performance p(12,13,14,15);
	Transformer t1(10, 4);   // p= 40W->20%
	Transformer t2(10, 10);  // 100W -> 50%
	Transformer t3(12, 7);   // 84W  -> 42%
	Transformer t4(20, 8);   // 160W -> 80%

	while(1)
	{
		p.Transformer_status(t);
		p.Transformer_status(t1);
		p.Transformer_status(t2);
		p.Transformer_status(t3);
		p.Transformer_status(t4);

	}
}
