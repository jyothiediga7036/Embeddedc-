#include "stm32f407.h"
#include <stdint.h>

uint32_t adc_ch12_value = 0; // PC2
uint32_t adc_ch11_value = 0; // PC1

void delay(void)
{
    for(volatile int i=0; i<50000; i++);
}

void adc_config(void)
{
    /* 1. Enable GPIOC clock */
    RCC->AHB1ENR |= (1<<2);

    /* 2. Set PC1 & PC2 as ANALOG */
    GPIOC->MODER |= (3<<2) | (3<<4);

    /* 3. Enable ADC1 clock */
    RCC->APB2ENR |= (1<<8);

    /* 4. ADC Common config */
    ADC->CCR &= ~(3<<16);   // PCLK2 / 2

    /* 5. ADC1 basic config */
    ADC1->CR1 = 0;          // clear all settings
    ADC1->CR2 = 0;

    /* 6. Resolution = 12-bit */
    ADC1->CR1 &= ~(3<<24);

    /* 7. Right alignment */
    ADC1->CR2 &= ~(1<<11);

    /* 8. Scan mode ENABLE */
    ADC1->CR1 |= (1<<8);

    /* 9. Single conversion mode */
    ADC1->CR2 &= ~(1<<1);

    /* 10. EOC after each conversion */
    ADC1->CR2 |= (1<<10);

    /* 11. Sampling time (better accuracy) */
    ADC1->SMPR1 |= (4<<6);   // CH12 → 84 cycles
    ADC1->SMPR1 |= (4<<3);   // CH11 → 84 cycles

    /* 12. Sequence length = 2 conversions */
    ADC1->SQR1 &= ~(0xF<<20);
    ADC1->SQR1 |= (1<<20);   // L = 1 → 2 conversions

    /* 13. Set channel sequence */
    ADC1->SQR3 = 0;
    ADC1->SQR3 |= (12<<0);   // 1st → CH12 (PC2)
    ADC1->SQR3 |= (11<<5);   // 2nd → CH11 (PC1)

    /* 14. Enable ADC */
    ADC1->CR2 |= (1<<0);

    delay(); // small stabilization delay
}

int main(void)
{
    adc_config();

    while(1)
    {
        /* Start conversion */
        ADC1->CR2 |= (1<<30);

        /* First conversion (CH12 - PC2) */
        while(!(ADC1->SR & (1<<1)));
        adc_ch12_value = ADC1->DR;

        /* Second conversion (CH11 - PC1) */
        while(!(ADC1->SR & (1<<1)));
        adc_ch11_value = ADC1->DR;

        delay();
    }
}
