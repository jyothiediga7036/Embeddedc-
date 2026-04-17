#include<cstdint>
#include"stm32f407.h"
void timer_init()
{
RCC->AHB1ENR|=(1<<3);
GPIOD->MODER&=~(3<<24);
GPIOD->MODER|=(1<<24);

RCC->APB1ENR|=(1<<0);
TIM2->PSC=1600-1;
TIM2->ARR=10000-1;
TIM2->CNT=0;
TIM2->CR1=(1<<0);

}
int main()
{
timer_init();
while(1)
{
while(!(TIM2->SR&(1<<0)));
{
TIM2->SR&=(1<<0);
GPIOD->ODR^=(1<<12);
//for(volatile uint32_t i=0;i<500000;i++);
//GPIOD->ODR&=~(1<<12);
}
}
}
