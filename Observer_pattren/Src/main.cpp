#include <cstdint>
#include<cstdio>
#include "stm32f407.h"
class observer
{
public:
    virtual void onbuttonpressed() = 0;
};
class button
{
    uint16_t pin;
    GPIO_TypeDef* port;
    observer* observers[10];
    int observer_count;

public:
    button(GPIO_TypeDef* p, uint16_t x)
    {
        port = p;
        pin = x;
        observer_count = 0;
    }

    void attach(observer* obj)
    {
        if (observer_count < 10)
        {
            observers[observer_count++] = obj;
        }
    }

    int read()
    {
        return (port->IDR & pin) ? 1 : 0;
    }

    void status()
    {
        static int last_state = 0;
        int current = read();

        if (current == 1 && last_state == 0)
        {
            notify();
        }

        last_state = current;
    }

private:
    void notify()
    {
        for (uint16_t i = 0; i < observer_count; i++)
        {
            observers[i]->onbuttonpressed();
        }
    }
};
class led_green : public observer
{
    GPIO_TypeDef* port;
    uint16_t pin;

public:
    led_green(GPIO_TypeDef* p, uint16_t x)
    {
        port = p;
        pin = x;
    }

    void onbuttonpressed() override
    {
        if (port->ODR & pin)
        {
            port->BSRR = (pin << 16);
            printf(" green led reset \n");
        }
        else
        {
            port->BSRR = pin;
            printf(" orange led set \n");
        }
    }
};
class led_orange : public observer
{
    GPIO_TypeDef* port;
    uint16_t pin;

public:
    led_orange(GPIO_TypeDef* p, uint16_t x)
    {
        port = p;
        pin = x;
    }

    void onbuttonpressed() override
    {
        if (port->ODR & pin)
        {
        	printf(" Orange led reset \n");
            port->BSRR = (pin << 16);
        }
        else
        {
            port->BSRR = pin;
            printf(" orange led set \n");
        }
    }
};
class led_red: public observer
{
    GPIO_TypeDef* port;
    uint16_t pin;

public:
    led_red(GPIO_TypeDef* p, uint16_t x)
    {
        port = p;
        pin = x;
    }

    void onbuttonpressed() override
    {
        if (port->ODR & pin)
        {
            port->BSRR = (pin << 16);
            printf(" red led reset \n");
        }
        else
        {
            port->BSRR = pin;
            printf(" red led set \n");
        }
    }
};
class led_blue : public observer
{
    GPIO_TypeDef* port;
    uint16_t pin;

public:
    led_blue(GPIO_TypeDef* p, uint16_t x)
    {
        port = p;
        pin = x;
    }

    void onbuttonpressed() override
    {
        if (port->ODR & pin)
        {
            port->BSRR = (pin << 16);
            printf(" blue led reset  \n");
        }
        else
        {
            port->BSRR = pin;
            printf(" blue led set \n");
        }
    }
};

void GPIO_init()
{
    RCC->AHB1ENR |= (1 << 0); // GPIOA
    RCC->AHB1ENR |= (1 << 3); // GPIOD

    // PA0 input
    GPIOA->MODER &= ~(3 << (0 * 2));

    // PD12–PD15 output
    for (uint16_t i = 12; i <= 15; i++)
    {
        GPIOD->MODER &= ~(3 << (i * 2));
        GPIOD->MODER |=  (1 << (i * 2));
    }
}
int main()
{
    GPIO_init();

    button b(GPIOA, (1 << 0));

    led_green g(GPIOD, (1 << 12));
    led_orange o(GPIOD, (1 << 13));
    led_red r(GPIOD, (1 << 14));
    led_blue b1(GPIOD, (1 << 15));

    b.attach(&g);
    b.attach(&o);
    b.attach(&r);
    b.attach(&b1);

    printf(" Observer pattren \n");
    while (1)
    {
        b.status();

        for (volatile uint32_t i = 0; i < 500000; i++);
    }
}
