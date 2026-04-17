
#include<iostream>
#include"stm32f407.h"
using namespace std;
char dataa;
template<uint32_t pin>
class singleton{
	singleton(){
		USART_init();
	}
	void USART_init(){
		RCC->AHB1ENR|=(1<<1);
		RCC->APB1ENR|=(1<<18);
		//TX
		GPIOB->MODER&=~(3<<20);
		GPIOB->MODER|=(1<<21);
		//select AFR
		GPIOB->AFR[1]&=~(0xF<<8);
		GPIOB->AFR[1]|=(7<<8);

		//RX
		GPIOB->MODER&=~(3<<22);
		GPIOB->MODER|=(1<<23);
		//select AFR
		GPIOB->AFR[1]&=~(0xF<<12);
		GPIOB->AFR[1]|=(7<<12);
		// pull up and down
		//GPIOB->PUPDR&=~(3<<22);

		//buard rate
		USART3->BRR=0x683;
		USART3->CR1|=(1<<3);
		USART3->CR1|=(1<<2);
		USART3->CR1|=(1<<13);

		RCC->AHB1ENR|=(1<<3);//GPIOD
		GPIOD->MODER&=~(3<<26);//green
		GPIOD->MODER|=(1<<26);//output
		}
	public:
	static  singleton& get_instance(){
		static  singleton instance;
		return instance;
	}
	singleton(const singleton&obj)=delete;
	singleton& operator=(const singleton&obj)=delete;
	singleton(const singleton&&obj)=delete;
	singleton& operator=(const singleton&&obj)=delete;
	void USART_write(char c){
		while(!(USART3->SR&(1<<7)));
		USART3->DR=c;
		while(!(USART3->SR&(1<<6)));
	}
	char USART_READ(){
		while(!(USART3->SR&(1<<5)));
		return USART3->DR;
	}
};
/*class LED{
	public:
	LED(){
		RCC->AHB1ENR|=(1<<3);//GPIOD
		GPIOD->MODER&=~(3<<12);//green
		GPIOD->MODER|=(1<<12);//output
	}
};*/
void delay()
{
	for(volatile uint32_t i=0;i<500000;i++);
}
int main(){
	auto& sll=singleton<12>::get_instance();
	while(1){
		sll.USART_write('A');
		GPIOD->ODR|=(1<<13);
		delay();
		GPIOD->ODR&=~(1<<13);
		dataa=sll.USART_READ();
	}

	//dataa=sll.USART_READ();
}


