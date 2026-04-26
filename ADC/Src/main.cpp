#include "stm32f407.h"
#include <stdint.h>

/*uint32_t result;

void delay(void)
{
    for(volatile int i=0; i<50000; i++);
}

int main(void)
{
    // 1. Enable GPIOC clock
    RCC->AHB1ENR |= (1<<2);

    // 2. Configure PC6 as OUTPUT (LED)
    GPIOC->MODER &= ~(3<<12);
    GPIOC->MODER |=  (1<<12);   // 01 = output

    // 3. Configure PC1 as ANALOG (ADC input)
    GPIOC->MODER &= ~(3<<2);
    GPIOC->MODER |=  (3<<2);    // 11 = analog

    // 4. Enable ADC1 clock
    RCC->APB2ENR |= (1<<8);

    // 5. Configure ADC

    ADC1->CR2 = 0;              // Ensure ADC is OFF

    //Sampling time for Channel 11 (PC1)
    ADC1->SMPR1 |= (7<<3);      // 480 cycles (CH11 → bits 5:3)

    //Conversion sequence length = 1
    ADC1->SQR1 = 0;

    // Select Channel 11
    ADC1->SQR3 = 11;

    // Enable ADC
    ADC1->CR2 |= (1<<0);

    while(1)
    {
        // Start conversion
        ADC1->CR2 |= (1<<30);

        // Wait until conversion complete
        while(!(ADC1->SR & (1<<1)));

        // Read result
        result = ADC1->DR;

        // Threshold check
        if(result > 2000)
        {
            GPIOC->ODR &= ~(1<<6);   // LED ON (active low)
        }
        else
        {
            GPIOC->ODR |= (1<<6);    // LED OFF
        }

        delay();
    }
}*/


int result;
int main()
{
	RCC->AHB1ENR|=(1<<2);  //GPIOC
	RCC->AHB1ENR|=(1<<3);   // GPIOD
	RCC->APB2ENR |= (1<<8); // ADC clock
	GPIOC->MODER&=~(3<<4);   // channel 12 - PC2
	GPIOC->MODER|=(3<<4);

	GPIOD->MODER&=~(3<<24);
	GPIOD->MODER|=(1<<24);
	GPIOD->MODER&=~(3<<26);
	GPIOD->MODER|=(1<<26);


	ADC1->CR2=0;  // ADC off
//ADC1->SMPR1&=~(0xf<<0);
	ADC1->SMPR1=(7<<6); //channel 12, 480 cycles

    //Sampling time for Channel 12 (PC2)

	ADC1->SQR1=0; //Conversion sequence length = 1
	ADC1->SQR3 &=~(0xf<<0);
	ADC1->SQR3 |=(12<<0);// select channel 11

	ADC1->CR2|=(1<<0); //ADC on

	while(1)
	{
		ADC1->CR2|=(1<<30); //start conversion on regural channel
		while(!(ADC1->SR&(1<<1))); //Wait until conversion complete
		result=ADC1->DR;  // read data

		if(result > 2000)
		{
           GPIOD->ODR &= ~(1<<12);   // LED ON (active low)
		}
		else
		{
            GPIOD->ODR |= (1<<13);    // LED OFF
		}
		for(uint32_t i=0;i<100000;i++);
	}

}




