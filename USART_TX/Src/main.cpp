#include <cstdint>
#include "stm32f407.h"

//char read_ch;

void delayMs(int n)
{
    for (; n > 0; n--)
        for (volatile int i = 0; i < 2000; i++);
}

void USART3_init()
{
    // Enable clock for GPIOC, GPIOD and USART3
    RCC->AHB1ENR |= (1U << 2);   // GPIOC
    RCC->AHB1ENR |= (1U << 3);   // GPIOD
    RCC->APB1ENR |= (1U << 18);  // USART3

    // PC10 -> USART3_TX
    GPIOC->MODER &= ~(3U << 20);
    GPIOC->MODER |=  (2U << 20);     // Alternate function mode
    GPIOC->AFR[1] &= ~(0xFU << 8);
    GPIOC->AFR[1] |=  (7U << 8);     // AF7

    // PC11 -> USART3_RX
    GPIOC->MODER &= ~(3U << 22);
    GPIOC->MODER |=  (2U << 22);     // Alternate function mode
    GPIOC->AFR[1] &= ~(0xFU << 12);
    GPIOC->AFR[1] |=  (7U << 12);    // AF7

    // PD13 as output
    //GPIOD->MODER &= ~(3U << 26);
    //GPIOD->MODER |=  (1U << 26);

    // USART3 settings
    USART3->BRR = 0x0683;            // 9600 baud @ 16 MHz
    USART3->CR1 |= (1U << 3);        // TE
    USART3->CR1 |= (1U << 2);        // RE
    USART3->CR1 |= (1U << 13);       // UE
}

void USART3_write(uint8_t ch)
{
    while (!(USART3->SR & (1U << 7)));   // Wait until TXE = 1
    USART3->DR = ch;
    while (!(USART3->SR & (1U << 6)));   // Wait until TC = 1
}
int main()
{
    USART3_init();  //USART3
    while (1)
    {
        USART3_write('1');
        delayMs(500);
        USART3_write('2');
        delayMs(500);
        USART3_write('3');
        delayMs(500);
        USART3_write('4');
        delayMs(500);


    }
}
