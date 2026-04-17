#include <stdint.h>
#include <stdio.h>
#include "stm32f407.h"

/* ---------------- C LINKAGE ---------------- */
extern "C" void UsageFault_Handler(void);
extern "C" void HardFault_Handler(void);
extern "C" void MemManage_Handler(void);
extern "C" void BusFault_Handler(void);
extern "C" void UsageFault_Handler_c(uint32_t *pMSP);

/* ---------------- GPIO INIT ---------------- */
void gpio_init()
{
    RCC->AHB1ENR |= (1 << 3);   // Enable GPIOD clock

    GPIOD->MODER &= ~(3 << 24);
    GPIOD->MODER |= (1 << 24);  // PD12 output
}

/* ---------------- MAIN ---------------- */
int main(void)
{
    uint32_t *pSHCSR = (uint32_t*)0xE000ED24;
    *pSHCSR |= (1 << 17); // Enable BusFault

    gpio_init();

    printf("Before BusFault\n");
    /* Force BusFault */
    //volatile uint32_t *ptr = (uint32_t*)0xFFFFFFFF;
    volatile uint32_t *ptr = (uint32_t*)0x20010000;
    *ptr = 10;

    while(1);
}

/* ---------------- FAULT HANDLERS ---------------- */

extern "C" void HardFault_Handler(void)
{
    __asm("BKPT #0");
    while(1);
}

extern "C" void MemManage_Handler(void)
{
    __asm("BKPT #0");
    while(1);
}

/* ---------------- USAGE FAULT HANDLER ---------------- */
extern "C" __attribute__((naked)) void UsageFault_Handler(void)
{
    __asm volatile(
        "MRS r0, MSP \n"
        "B UsageFault_Handler_c \n"
    );
}

/* ---------------- BUS FAULT HANDLER ---------------- */
extern "C" __attribute__((naked)) void BusFault_Handler(void)
{
    __asm volatile(
        "MRS r0, MSP \n"
        "B BusFault_Handler_c \n"
    );
}

/* ---------------- BUS FAULT C HANDLER ---------------- */
extern "C" void BusFault_Handler_c(uint32_t *pBaseStackFrame)
{
    uint8_t *pBFSR = (uint8_t*)0xE000ED29;
    uint8_t bfsr = *pBFSR;

    printf("\n--- BusFault Occurred ---\n");

    /* LED indication */
    GPIOD->ODR |= (1 << 12);

    printf("BFSR = %x\n", bfsr);

    printf("PC  = %lx\n", pBaseStackFrame[6]); // When a fault occurs, ARM Cortex-M automatically pushes some registers into the stack:
    // stored to program counter

    __asm("BKPT #0");

    while(1);
}

/* ---------------- USAGE FAULT C HANDLER ---------------- */
extern "C" void UsageFault_Handler_c(uint32_t *pBaseStackFrame)
{
    uint32_t *pUFSR = (uint32_t*)0xE000ED2A;

    printf("\n--- UsageFault Occurred ---\n");
    printf("UFSR = %lx\n", (*pUFSR) & 0xFFFF);

    __asm("BKPT #0");

    while(1);
}







