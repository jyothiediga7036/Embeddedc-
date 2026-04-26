#include "stm32f407.h"
#include <stdint.h>

int data;
double volt, temp;

void delay(void)
{
for(int i=0;i<50000;i++);
}

int main(void)
{
/* Enable ADC1 clock */
RCC->APB2ENR |= (1<<8);
/* Enable temperature sensor */
ADC->CCR |= (1<<23);     // TSVREFE
ADC->CCR &= ~(1<<22);    // Disable VBAT

delay(); // stabilization time

/* Sampling time */
ADC1->SMPR1 |= (7<<18);   // 480 cycles for channel 16

/* Select channel 16 */
ADC1->SQR3 = 16;

/* Enable ADC */
ADC1->CR2 |= 1;

while(1)
{
    /* Start conversion */
    ADC1->CR2 |= (1<<30);   // SWSTART

    /* Wait for conversion complete */
    while(!(ADC1->SR & (1<<1)));

    data = ADC1->DR;

    volt = (double)data * 3.3 / 4095;// 12 bit ADC
    temp = ((volt - 0.76) / 0.0025) + 25;

    delay();
}
}
