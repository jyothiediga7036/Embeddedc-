#include <stdint.h>
#include "stm32f407.h"

class GPIO
{
public:
    void enable_clock()
    {
        RCC->AHB1ENR |= (1U << 3);
    }

    void set_output(uint32_t pin)
    {
        GPIOD->MODER &= ~(3U << (pin * 2));
        GPIOD->MODER |=  (1U << (pin * 2));
    }

    void set(uint32_t pin)
    {
        GPIOD->BSRR = (1U << pin);
    }

    void reset(uint32_t pin)
    {
        GPIOD->BSRR = (1U << (pin + 16)); //12+16 set  bit
    }

    void toggle(uint32_t pin)
    {
        if (GPIOD->ODR & (1U << pin))
            reset(pin);
        else
            set(pin);
    }
};

class LED:public GPIO
{
    uint32_t pin;

public:
    explicit LED(uint32_t p) : pin(p)
    {
        GPIO::set_output(pin);
    }

    void toggle()
    {
        GPIO::toggle(pin);
    }
};

void delay()
{
    for (volatile uint32_t i = 0; i < 100000; ++i);
}

int main()
{
    constexpr uint32_t LED_PIN = 12;

    GPIO g;
	g.enable_clock();

    LED led(LED_PIN);

    while (true)
    {
        led.toggle();
        delay();
    }
}
