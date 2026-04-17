#include "stm32f407.h"
#include <stdio.h>

void USART2_Init(void)
{
    // Enable clocks
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    // PA2 -> TX (Alternate Function)
    GPIOA->MODER &= ~(3 << (2 * 2));
    GPIOA->MODER |=  (2 << (2 * 2));

    // Select AF7 (USART2)
    GPIOA->AFR[0] &= ~(0xF << (2 * 4));
    GPIOA->AFR[0] |=  (7 << (2 * 4));

    // High speed
    GPIOA->OSPEEDR |= (3 << (2 * 2));

    // Baud rate (Assuming APB1 = 42 MHz)
    USART2->BRR = 0x1117;   // 9600 baud

    // Enable USART TX and USART
    USART2->CR1 |= USART_CR1_TE;
    USART2->CR1 |= USART_CR1_UE;
}

int main(void)
{
    USART2_Init();

    printf("Hello World via USART!\r\n");

    while (1)
    {
        printf("Looping...\r\n");

        for (volatile int i = 0; i < 1000000; i++); // delay
    }
}
