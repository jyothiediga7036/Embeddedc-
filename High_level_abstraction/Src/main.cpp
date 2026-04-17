#include <cstdint>
constexpr std::uint32_t RCC_AHB1ENR_ADDR = 0x40023830;
constexpr std::uint32_t GPIOD_MODER_ADDR = 0x40020C00;
constexpr std::uint32_t GPIOD_ODR_ADDR   = 0x40020C14;
constexpr std::uint32_t GPIOA_MODER_ADDR=0x40020000;
constexpr std::uint32_t GPIOA_IDR_ADDR=0x40020010;

inline volatile std::uint32_t& RCC_AHB1ENR =*reinterpret_cast<volatile std::uint32_t*>(RCC_AHB1ENR_ADDR);
inline volatile std::uint32_t& GPIOD_MODER =*reinterpret_cast<volatile std::uint32_t*>(GPIOD_MODER_ADDR);
inline volatile std::uint32_t& GPIOD_ODR =*reinterpret_cast<volatile std::uint32_t*>(GPIOD_ODR_ADDR);
inline volatile std::uint32_t& GPIOA_MODER =*reinterpret_cast<volatile std::uint32_t*>(GPIOA_MODER_ADDR);
inline volatile std::uint32_t&GPIOA_IDR =*reinterpret_cast<volatile std::uint32_t*>(GPIOA_IDR_ADDR);

class GPIO
{
public:
    static void enable_clock()noexcept
    {
        RCC_AHB1ENR |= (1U << 3);  // Enable GPIOD clock
        RCC_AHB1ENR|=(1<<0);
    }

    static void enable_moder(std::uint32_t pin,std::uint32_t p1)noexcept
    {
        GPIOD_MODER &= ~(3U << (pin * 2));
        GPIOD_MODER |=  (1U << (pin * 2));
        GPIOD_MODER &= ~(3U << (p1 * 2));
        GPIOD_MODER |=  (1U << (p1 * 2));
        GPIOA_MODER&=~(3<<0);
        //GPIOA_IDR|=(0<<0);
    }

    static void toggle(std::uint32_t pin,std::uint32_t p)noexcept
    {
    	if(GPIOA_IDR&1)
    	{
    		GPIOD_ODR &= ~(1U << pin);
    		GPIOD_ODR |= (1U << p);
    	}
    	else
    	{
    		GPIOD_ODR &= ~(1U << p);
    		GPIOD_ODR |= (1U << pin);
    	}

    }
};

void delay()
{
    for (volatile std::uint32_t i = 0; i < 100000; ++i)
    {
        // Prevent optimization
    }
}
int main()
{
    constexpr std::uint32_t LED_PIN = 12;
    constexpr std::uint32_t led1=14;

    // Explicit hardware initialization
    GPIO::enable_clock();
    GPIO::enable_moder(LED_PIN,led1);


    while (true)
    {
        GPIO::toggle(LED_PIN,led1);
        delay();
    }
}
