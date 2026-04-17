#include <stdint.h>

typedef struct
{
    volatile uint32_t MODER; //4 byte 0x00
    volatile uint32_t OTYPER;// 0x04
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
} GPIO_RegDef_t;

typedef struct
{
    volatile uint32_t CR;
    volatile uint32_t PLLCFGR;
    volatile uint32_t CFGR;
    volatile uint32_t CIR;
    volatile uint32_t AHB1RSTR;
    volatile uint32_t AHB2RSTR;
    volatile uint32_t AHB3RSTR;
    uint32_t RESERVED0;
    volatile uint32_t APB1RSTR;
    volatile uint32_t APB2RSTR;
    uint32_t RESERVED1[2];
    volatile uint32_t AHB1ENR;
} RCC_RegDef_t;

#define RCC_BASE   0x40023800
#define GPIOD_BASE 0x40020C00

#define RCC   ((RCC_RegDef_t*) RCC_BASE) // type casting
#define GPIOD ((GPIO_RegDef_t*) GPIOD_BASE)
class LED
{
    GPIO_RegDef_t* GPIOx;
    uint32_t pin;
    uint32_t* pPortAModeReg=  (uint32_t*)0x40020000;
    uint32_t* pPortAINReg = (uint32_t*)0x40020010;
public:
    LED(GPIO_RegDef_t* GPIO, uint32_t p) : GPIOx(GPIO), pin(p)
    {
        RCC->AHB1ENR |= (1 << 3)|(1<<0);
        GPIOx->MODER &= ~(3 << (2 * pin));
        GPIOx->MODER |=  (1 << (2 * pin));
        *pPortAModeReg &=~(3<<0);
    	*pPortAModeReg|=(0<<0);
    }

    void on()
    {
    	if(*pPortAINReg&1)
    	{
        	GPIOx->ODR |= (1 << pin);
        	for(uint32_t i=0;i<3000;i++);
        }
        else
        {
        	GPIOx->ODR &= ~(1 << pin);
        	for(uint32_t i=0;i<3000;i++);
        }
    }
};
int main()
{
LED l(GPIOD,12);
	while(1)
	{
		l.on();
	}
}





