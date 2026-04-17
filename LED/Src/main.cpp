#include <stdint.h>

int main(void)
{
     uint32_t* pClkCtrlReg   = (uint32_t*)0x40023830; // RCC_AHB1ENR
     uint32_t* pPortDModeReg = (uint32_t*)0x40020C00; // GPIOD_MODER
     uint32_t* pPortDOutReg  = (uint32_t*)0x40020C14; // GPIOD_ODR

    *pClkCtrlReg |= (1 << 3);   // Enable clock for GPIOD

    *pPortDModeReg &= ~((3 << 24)|(3<<26)|(3<<28)|(3<<30)); // Clear PD12 mode bits
    *pPortDModeReg |= ((1 << 24)|(1<<26)|(1<<28)|(1<<30)); // Set PD12 as output

    while(1)
    {
        *pPortDOutReg |= (1 << 12)|(1<<13)|(1<<14)|(1<<15);   // LED ON

        for(uint32_t i = 0; i < 300000; i++);

        *pPortDOutReg &= ~((1 << 12)|(1<<13)|(1<<14)|(1<<15));  // LED OFF

        for(uint32_t i = 0; i < 300000; i++);
    }
}

