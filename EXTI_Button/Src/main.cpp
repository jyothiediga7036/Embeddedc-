#include <stdint.h>
#include "stm32f407.h"

int g_button_pressed=0;

// LED INIT
void LED_Init(void)
{
    RCC->AHB1ENR |= (1 << 3);   // Enable GPIOD clock

    GPIOD->MODER &= ~(3 << (12 * 2));
    GPIOD->MODER |=  (1 << (12 * 2));  // Output mode

    GPIOD->ODR &= ~(1 << 12); // LED OFF
}
void delay()
{
	for(volatile uint32_t i=0;i<50000;i++);
}
//BUTTON EXTI INIT
void Button_EXTI_Init(void)
{
    /* Enable clocks */
    RCC->AHB1ENR |= (1 << 0);   // GPIOA
    RCC->APB2ENR |= (1 << 14);  // SYSCFG

    /* PA0 as input */
    GPIOA->MODER &= ~(3 << (0 * 2));

    /* Pull-down */
    GPIOA->PUPDR &= ~(3 << (0 * 2));
    GPIOA->PUPDR |=  (2 << (0 * 2));  // Pull-down

    /* Map EXTI0 to PA0 */
    SYSCFG->EXTICR[0] &= ~(0xF << 0);  // 0000 = PA0

    /* Rising edge trigger */
    EXTI->RTSR |= (1 << 0);

    /* Unmask interrupt */
    EXTI->IMR |= (1 << 0);  //

    /* NVIC Enable (IRQ6 for EXTI0) */
    //*((volatile uint32_t*)0xE000E100) |= (1 << 6);  //EXTI0_IRQn= 6,  /!< EXTI Line0 Interrupt
    volatile uint32_t*piser=((volatile uint32_t*)(0xE000E100)); // NVIC_ISER
    *piser|=(1<<6);
}

//ISR
extern "C" void EXTI0_IRQHandler(void)
{
    if (EXTI->PR & (1 << 0))  // / test triggered occur or not 1-> occur 0-> not occure
    {
        GPIOD->ODR |= (1 << 12); // Toggle LED
        g_button_pressed=1;

        EXTI->PR |= (1 << 0); // Clear pending
    }
}

int main(void)
{
    LED_Init();
    Button_EXTI_Init();

    while (1)
    {
    	if(g_button_pressed)
    	{
    		delay();
    		GPIOD->ODR&=~(1<<12);
    		g_button_pressed=0;
    	}
    }
}
