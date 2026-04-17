#include <stdint.h>

int main(void)
{
     uint32_t* pClkCtrlReg   = (uint32_t*)0x40023830; // RCC_AHB1ENR
     uint32_t* pPortDModeReg = (uint32_t*)0x40020C00; // GPIOD_MODER
     uint32_t* pPortDOutReg  = (uint32_t*)0x40020C14; // GPIOD_ODR
     uint32_t* pPortAModeReg=  (uint32_t*)0x40020000;
     uint32_t* pPortAINReg = (uint32_t*)0x40020010;

    *pClkCtrlReg |= (1 << 3)|(1<<0);   // Enable clock for GPIOD and GPIOA

    *pPortDModeReg &= ~((3 << 24)|(3<<26)|(3<<28)|(3<<30)); // Clear PD12 mode bits
    *pPortDModeReg |= ((1 << 24)|(1<<26)|(1<<28)|(1<<30));// Set PD12 as output
    *pPortAModeReg &=(3<<0);
    *pPortAModeReg|=(0<<0);

    while(1)
    {
    	if(*pPortAINReg&1)
    	{
    		*pPortDOutReg &= ~((1<<14)|(1<<15));
        *pPortDOutReg |= (1 << 12)|(1<<13);   // LED ON

        for(uint32_t i = 0; i < 300000; i++);
    	}
    	else
    	{
    		*pPortDOutReg &= ~((1 << 12)|(1<<13));
        *pPortDOutReg |= (1<<14)|(1<<15);  // LED OFF

        for(uint32_t i = 0; i < 300000; i++);
    	}
    }
}

