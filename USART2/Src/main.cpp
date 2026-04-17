#include <cstdint>
#include "stm32f407.h"
char ch;
void USART2_init(void);
void USART2_write(uint8_t ch);
void delayMs(int);
void USART_string(const char* s);
int main(void)
{
    USART2_init();

    while (1)
    {
        USART_string("Jyothi\r\n");
        delayMs(500);
    }
}

void USART2_init(void)
{
    RCC->AHB1ENR |= (1U << 0);    // GPIOA clock enable
    RCC->APB1ENR |= (1U << 17);   // USART2 clock enable
    RCC->AHB1ENR|=(1<<3);

    // PA2 -> USART2_TX
    GPIOA->MODER &= ~(3U << 4);   //white cable
    GPIOA->MODER |=  (2U << 4);   // alternate function mode

    GPIOA->AFR[0] &= ~(0xFU << 8);
    GPIOA->AFR[0] |=  (7U << 8);  // AF7

    // PA3 -> USART2_RX
    GPIOA->MODER &= ~(3U << 6);    // green cable
    GPIOA->MODER |=  (2U << 6);   // alternate function mode

    GPIOA->AFR[0] &= ~(0xFU << 12);
    GPIOA->AFR[0] |=  (7U << 12); // AF7

    GPIOD->MODER&=~(3<<26);
    GPIOD->MODER|=(1<<26);

    USART2->BRR = 0x0683;         // 9600 baud @ 16 MHz
    USART2->CR1 |= (1U << 3);     // TE
    USART2->CR1 |= (1U << 13);    // UE
}

void USART2_write(uint8_t ch)
{
    while (!(USART2->SR & (1U << 7)));   // TXE
    USART2->DR = ch;
    while (!(USART2->SR & (1U << 6)));   // TC
}

void USART_string(const char* s)
{
    while (*s)
    {
        USART2_write(*s);
        		s++;
        		ch=*s;
        		if(ch=='o')
        		{
        			GPIOD->ODR|=(1<<13);
        			delayMs(500);
        		}

    }

    while (!(USART2->SR & (1U << 6)));   // wait for complete transmission
}

void delayMs(int n)
{
    for (; n > 0; n--)
        for (volatile int i = 0; i < 2000; i++);
}
