#include "stm32f407.h"
#include <stdint.h>


uint8_t read_Ch;
uint8_t pb=1;

void USART1_init(void);
void LED_init(void);
void USART1_write(uint8_t ch);
uint8_t USART1_read(void);
void delayMs(int);

int main(void)
{
    USART1_init();   // initialize UART
    LED_init();      // initialize LED

   while (1)
    {
	  /* GPIOB->BSRR = (1 << 3);
	   GPIOE->BSRR = (1 << 2);
	   GPIOB->BSRR = (1 << 4);
	   GPIOE->BSRR = (1 << 5);
	   */

	   read_Ch=USART1_read();

         if(read_Ch=='1')
         {
        	  GPIOB->BSRR = (1 << 4);
        	  delayMs(1000);
        	  GPIOB->BSRR = (1 << (4+16));


         }

         else if(read_Ch=='2')

         {

        	 GPIOB->BSRR = (1 << 3);
        	 delayMs(1000);
        	  GPIOB->BSRR = (1 << (3+16));

         }
         else if(read_Ch=='3')
         {
        	 GPIOE->BSRR = (1 << 2);
        	 delayMs(1000);
        	    GPIOE->BSRR = (1 << (2+16));
         }
         else if(read_Ch=='4')
         {
        	 GPIOE->BSRR = (1 << 5);
        	 delayMs(1000);
        	  GPIOE->BSRR = (1 << (5+16));
         }
         read_Ch=0;


   }
}

void USART1_init(void)
{
    RCC->AHB1ENR |= (1 << 1);   // enable GPIOB clock
    RCC->APB2ENR |= (1 << 4);  // enable USART1 clock

    // Pb6 → TX
    GPIOB->MODER &= ~(3 << 12); // pin10 =pin*2, claered the bits
    GPIOB->MODER |=  (1 << 13);   // Alternate function
    // created Array :AFR[2]  , AFR[0]=AFRL ,AFR[1]= AFRH
    GPIOB->AFR[0] &= ~(0xF << 24); // AFRH =AFR[1]- Clearing 4 bits
    GPIOB->AFR[0] |=  (7 << 24);

    // Pb7 → RX
    GPIOB->MODER &= ~(3 << 14);
    GPIOB->MODER |=  (1 << 15);// 10 AFR mode

    GPIOB->AFR[0] &= ~(0xF << 28);// AFRH Register
    GPIOB->AFR[0] |=  (7 << 28); // AF7 -0111

    // USART settings
    USART1->BRR = 0x0683;
   // USART1->CR1 |= (1 << 3);   // TE
    USART1->CR1 |= (1 << 2);   // RE
    USART1->CR1 |= (1 << 13);  // UE - USART enable

}


uint8_t USART1_read(void)
{
    while (!(USART1->SR & (1 << 5))); // RXNE
    return USART1->DR;
}

void LED_init(void)
{
    RCC->AHB1ENR |= (1 << 1);
    RCC->AHB1ENR |= (1 << 4);

    GPIOB->MODER &= ~(3 << (4 * 2));
    GPIOB->MODER |=  (1 << (4 * 2));   // PB4 output

    GPIOB->MODER &= ~(3 << (3* 2));
    GPIOB->MODER |=  (1 << (3 * 2));   // PB3 output

    GPIOE->MODER &= ~(3 << (2 * 2));
    GPIOE->MODER |=  (1 << (2 * 2));   // PE2 output

    GPIOE->MODER &= ~(3 << (5 * 2));
    GPIOE->MODER |=  (1 << (5 * 2));   // PE5 output

}



void delayMs(int n)
{
    for (; n > 0; n--)
        for (volatile int i = 0; i < 2000; i++);
}

