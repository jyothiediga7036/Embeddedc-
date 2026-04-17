#include"stm32f407.h"
enum class RCC_AHB1ENR_bits : uint32_t {
GPIOA_EN =0,
GPIOB_EN =1,
GPIOC_EN =2,
GPIOD_EN =3
};

enum class GPIO_moder:uint32_t
{
	INPUT =0b00,
	OUTPUT =0b01,
	ALT =0b10,
	ANALOG =0b11
};
enum pin:uint8_t
{
	green =12,
	orange=13,
	red=14,
	blue=15,

};
class clock
{
public:
	clock()
	{
		RCC->AHB1ENR|= (1U<<static_cast<uint32_t>(RCC_AHB1ENR_bits::GPIOD_EN));   // enable GPIOD
		RCC->AHB1ENR|= (1U<<static_cast<uint32_t>(RCC_AHB1ENR_bits::GPIOA_EN));    // Enable GPIOA
	}
	~clock()
	{
		RCC->AHB1ENR&= ~(1U<<static_cast<uint32_t>(RCC_AHB1ENR_bits::GPIOD_EN));   // enable GPIOD
		RCC->AHB1ENR&= ~(1U<<static_cast<uint32_t>(RCC_AHB1ENR_bits::GPIOA_EN));    // Enable GPIOA
	}

};

class GPIO
{
public:
    static void set_output()
    {
       GPIOD->MODER&= ~(3U << (pin::orange * 2));
       GPIOD->MODER |=  (static_cast<uint32_t>(GPIO_moder::OUTPUT)<< (pin::orange * 2)); // Output mode
    }

    static void set_input()
    {
        GPIOA->MODER &= ~(static_cast<uint32_t>(GPIO_moder::INPUT) <<0); // Input mode (00)
    }

    static void toggle()
    {
       GPIOD->ODR^= (1U << pin::orange);
    }

    static bool read()
    {
        return (GPIOA->IDR & 1);
    }
};

void delay()
{
    for (volatile uint32_t i = 0; i < 100000; ++i)
    {
        // Prevent optimization
    }
}

int main()
{
   // constexpr uint8_t LED_PIN = 12; // PD12
    //constexpr uint8_t BTN_PIN = 0;  // PA0

    {
        clock c;

        GPIO::set_output();
        GPIO::set_input();

        while (true)
        {
            if (GPIO::read())
            {
                break;  //  Exit loop → triggers destructor
            }

            GPIO::toggle();
            delay();
        }
    } // Destructor runs here (clock disabled)
}


