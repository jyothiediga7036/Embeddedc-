#include <cstdint>
#include "stm32f407.h"

char read_ch;

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
    GPIOD->MODER &= ~(3U << 26);
    GPIOD->MODER |=  (1U << 26);

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

void USART3_write_str(const char* str)
{
    while (*str)
    {
        USART3_write(*str++);
    }
}

uint8_t USART3_read(void)
{
    while (!(USART3->SR & (1U << 5)));   // Wait until RXNE = 1
    return (uint8_t)USART3->DR;
}

void USART3_read_str(char *buffer)
{
    int i = 0;

    while (1)
    {
        read_ch = USART3_read();

        if (read_ch == 'o' || read_ch == 'O')
        {
            GPIOD->ODR |= (1U << 13);      // LED ON
        }
        else if (read_ch == 'f' || read_ch == 'F')
        {
            GPIOD->ODR &= ~(1U << 13);     // LED OFF
        }

        if (read_ch == '\r')   // Enter pressed
        {
            buffer[i] = '\0';
            break;
        }

        buffer[i++] = read_ch;
    }
}

int main()
{
    USART3_init();

    char str[50];

    while (1)
    {
        USART3_write_str("Entor name: ");
        USART3_read_str(str);

        USART3_write_str("\r\nYou typed: ");
        USART3_write_str(str);
        USART3_write_str("\r\n");
    }
}
