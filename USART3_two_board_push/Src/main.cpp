#include <cstdint>
#include "stm32f407.h"

static uint8_t counter1 = 0;
static uint8_t counter2 = 0;
static uint8_t counter3 = 0;
static uint8_t counter4 = 0;

uint8_t status1;
uint8_t status2;
uint8_t status3;
uint8_t status4;

void delayMs(int n)
{
    for (; n > 0; n--)
        for (volatile int i = 0; i < 2000; i++);
}

void USART3_init()
{
    // Enable clocks
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
    USART3->CR1 |= (1U << 13); // UE
}

void USART3_write(uint8_t ch)
{
    while (!(USART3->SR & (1U << 7)));   // TXE
    USART3->DR = ch;
    while (!(USART3->SR & (1U << 6)));   // TC
}

void button_init()
{
    // Enable clocks for GPIOA, GPIOB, GPIOC, GPIOD
    RCC->AHB1ENR |= (1U << 0);
    RCC->AHB1ENR |= (1U << 1);
    RCC->AHB1ENR |= (1U << 2);
    RCC->AHB1ENR |= (1U << 3);

    // PA1 input
    GPIOA->MODER &= ~(3U << 2);
    GPIOA->PUPDR &= ~(3U << 2);
    GPIOA->PUPDR |=  (1U << 2);   // pull-up

    // PB1 input
    GPIOB->MODER &= ~(3U << 2);
    GPIOB->PUPDR &= ~(3U << 2);
    GPIOB->PUPDR |=  (1U << 2);   // pull-up

    // PC1 input
    GPIOC->MODER &= ~(3U << 2);
    GPIOC->PUPDR &= ~(3U << 2);
    GPIOC->PUPDR |=  (1U << 2);   // pull-up

    // PD1 input
    GPIOD->MODER &= ~(3U << 2);
    GPIOD->PUPDR &= ~(3U << 2);
    GPIOD->PUPDR |=  (1U << 2);   // pull-up
}

int main()
{
    USART3_init();
    button_init();

    uint8_t prev_status1 = 0;
    uint8_t prev_status2 = 0;
    uint8_t prev_status3 = 0;
    uint8_t prev_status4 = 0;

    while (1)
    {
        status1 = (GPIOA->IDR & (1U << 1)) ? 1 : 0;
        status2 = (GPIOB->IDR & (1U << 1)) ? 1 : 0;
        status3 = (GPIOC->IDR & (1U << 1)) ? 1 : 0;
        status4 = (GPIOD->IDR & (1U << 1)) ? 1 : 0;

        if ((status1 == 1) && (prev_status1 == 0))
        {
            delayMs(50);
            if (GPIOA->IDR & (1U << 1))
            {
                counter1++;
                USART3_write('1');
            }
        }

        if ((status2 == 1) && (prev_status2 == 0))
        {
            delayMs(50);
            if (GPIOB->IDR & (1U << 1))
            {
                counter2++;
                USART3_write('2');
            }
        }

        if ((status3 == 1) && (prev_status3 == 0))
        {
            delayMs(50);
            if (GPIOC->IDR & (1U << 1))
            {
                counter3++;
                USART3_write('3');
            }
        }

        if ((status4 == 1) && (prev_status4 == 0))
        {
            delayMs(50);
            if (GPIOD->IDR & (1U << 1))
            {
                counter4++;
                USART3_write('4');
            }
        }

        prev_status1 = status1;
        prev_status2 = status2;
        prev_status3 = status3;
        prev_status4 = status4;
    }
}
