#include<cstdint>
#include"stm32f407.h"
class GPIO
{
	uint8_t*pin;
public:
	GPIO(std::uint8_t x)
	{
		pin=new uint8_t(x);
		*pin=x;
		RCC->AHB1ENR |= (1U << 3); // Enable GPIOD
		RCC->AHB1ENR |= (1U << 0); // Enable GPIOA
	}
    void set_output()
    {
        GPIOD->MODER &= ~(3U << (*pin * 2));
       GPIOD->MODER |=  (1U << (*pin * 2)); // Output mode
    }

    void set_input()
    {
        GPIOA->MODER &= ~(3U << 0); // Input mode (00)
    }

    void toggle()
    {
        GPIOD->ODR^= (1U << *pin);
    }

    bool read()
    {
        return (GPIOA->IDR& (1U << 0));
    }
    ~GPIO()
    {
    	delete pin;
    	 RCC->AHB1ENR &= ~(1U << 3); // Disable GPIOD
    	 RCC->AHB1ENR &= ~(1U << 0); // Disable GPIOA
    }
};

void delay()
{
    for (std::uint32_t i = 0; i < 100000; ++i)
    {
        // Prevent optimization
    }
}
int main()
{
    GPIO p(13);       // PD13
    p.set_output();
    p.set_input();

    while (1)
    {
        if (p.read())
        {
            break;
        }

        p.toggle();
        delay();
    }
}
