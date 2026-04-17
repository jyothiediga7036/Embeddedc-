#include"Performance.h"
Performance::Performance(uint32_t g,uint32_t o,uint32_t r,uint32_t b)
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
void Performance::Transformer_status(Transformer& obj)
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
			else if(p<=50)
			{
				on(green);
				for(uint32_t i=0;i<50000;i++);
			}
			else if(p<=75)
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
void Performance::on(uint32_t pin)
{
		GPIOD->ODR|=(1<<pin);
}
void Performance::off(uint32_t pin)
{
		GPIOD->ODR&=~(1<<pin);
}
void Performance::all_off()
{
		GPIOD->ODR&=~(1<<green);
		GPIOD->ODR&=~(1<<orange);
		GPIOD->ODR&=~(1<<red);
		GPIOD->ODR&=~(1<<blue);
}



