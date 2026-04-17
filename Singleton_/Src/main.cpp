#include<cstdint>
#include"stm32f407.h"
class singleton_USART3
{

	Singleton_USART3()
	{
		 RCC->AHB1ENR |= (1U << 2);   // GPIOC
		    RCC->AHB1ENR |= (1U << 3);   // GPIOD
		    RCC->APB1ENR |= (1U << 18);  // USART3

		    // PD12 and PD14 as output (optional LEDs)
		    GPIOD->MODER &= ~(3U << 24);
		    GPIOD->MODER |=  (1U << 24);

		    GPIOD->MODER &= ~(3U << 28);
		    GPIOD->MODER |=  (1U << 28);

		    // PC10 -> USART3_TX
		    GPIOC->MODER &= ~(3U << 20);
		    GPIOC->MODER |=  (2U << 20);
		    GPIOC->AFR[1] &= ~(0xFU << 8);
		    GPIOC->AFR[1] |=  (7U << 8);

		    // PC11 -> USART3_RX
		    GPIOC->MODER &= ~(3U << 22);
		    GPIOC->MODER |=  (2U << 22);
		    GPIOC->AFR[1] &= ~(0xFU << 12);
		    GPIOC->AFR[1] |=  (7U << 12);

		    // USART3 settings
		    USART3->BRR = 0x0683;      // 9600 baud @ 16 MHz
		    USART3->CR1 |= (1U << 3);  // TE
		    USART3->CR1 |= (1U << 2);  // RE
		    USART3->CR1 |= (1U << 13); // U
	}
	~Singleton_USART3()
		{
			RCC->AHB1ENR|=(1<<3);
			RCC->AHB1ENR|=(1<<0);
		}

public:
	static Singleton_USART3&  func()
	{
		static  Singleton_USART3 s;
		return s;
	}
};
