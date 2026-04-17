#include<stdint.h>
#include<stm32f407.h>
uint8_t ch;
void delay()
{
	for(volatile uint32_t i=0;i<500000;i++);
}
void init()
{
	RCC->APB1ENR|=(1<<17); //USART2
	RCC->APB1ENR|=(1<<18); //USART3

	RCC->AHB1ENR|=(1<<2); //port c
	RCC->AHB1ENR|=(1<<3); //port d


	GPIOC->MODER&=~(3<<20); //PC10
	GPIOC->MODER|=(2<<20);
	GPIOC->AFR[1]&=~(0XF<<8);
	GPIOC->AFR[1]|=(0X7<<8);

	GPIOC->MODER&=~(3<<22); //PC11
	GPIOC->MODER|=(2<<22);
	GPIOC->AFR[1]&=~(0XF<<12);
	GPIOC->AFR[1]|=(0X7<<12);

    GPIOD->MODER&=~(3<<24);
    GPIOD->MODER|=(1<<24);
	GPIOD->MODER&=~(3<<10); //PD5
	GPIOD->MODER|=(2<<10);
	GPIOD->AFR[0]&=~(0XF<<20);
	GPIOD->AFR[0]|=(0X7<<20);

	GPIOD->MODER&=~(3<<12); //PD6
    GPIOD->MODER|=(2<<12);
	GPIOD->AFR[0]&=~(0XF<<24);
	GPIOD->AFR[0]|=(0X7<<24);

	USART3->BRR=0X683; //baud rate setting
	USART2->BRR=0X683;

	USART3->CR1|=(1<<13); //usart enable
	USART2->CR1|=(1<<13);

	USART3->CR1|=(1<<3);//usartE3 TE enable
	USART3->CR1|=(1<<2);//usart2 RE enable
	USART2->CR1|=(1<<3);//usartE3 TE enable
	USART2->CR1|=(1<<2);

	GPIOD->MODER&=~(3<<26);
	GPIOD->MODER|=(1<<26);
}
uint8_t read_usart2()
{
	while(!(USART2->SR>>5&1));
	GPIOD->ODR&=~(1<<13);
	delay();
	return USART2->DR;
}
void write_usart3(uint8_t c)
{
	while(!(USART3->SR>>7&1));
	GPIOD->ODR|=(1<<13);
	delay();
	USART3->DR=c;
}
/*void on()
{
	GPIOD->ODR|=(1<<12);
	for(uint32_t i=0;i<300000;i++);
}
void off()
{
	GPIOD->ODR&=~(1<<12);
	for(uint32_t i=0;i<300000;i++);
}*/

int main()
{
	init();
	while(1)
	{
		write_usart3('A');
		ch=read_usart2();
		delay();
	}
}
