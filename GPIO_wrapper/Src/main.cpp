#include <cstdint>

// =======================================================
// 1. Generic Register Wrapper
// =======================================================
template<std::uintptr_t address>
struct Register
{
    static volatile std::uint32_t& value()
    {
        return *reinterpret_cast<volatile std::uint32_t*>(address);
    }
};

// =======================================================
// 2. Peripheral Base Addresses (STM32F407)
// =======================================================
constexpr std::uintptr_t RCC_BASE   = 0x40023800;
constexpr std::uintptr_t GPIOE_BASE = 0x40021000;

// =======================================================
// 3. RCC Wrapper
// =======================================================
struct RCC
{
    static auto& AHB1ENR()
    {
        return Register<RCC_BASE + 0x30>::value();
    }
};

// =======================================================
// 4. GPIO Port Wrapper
// =======================================================
template<std::uintptr_t base>
struct GPIO_Port
{
    static auto& MODER()   { return Register<base + 0x00>::value(); }
    static auto& OTYPER()  { return Register<base + 0x04>::value(); }
    static auto& OSPEEDR() { return Register<base + 0x08>::value(); }
    static auto& PUPDR()   { return Register<base + 0x0C>::value(); }
    static auto& IDR()     { return Register<base + 0x10>::value(); }
    static auto& ODR()     { return Register<base + 0x14>::value(); }
    static auto& BSRR()    { return Register<base + 0x18>::value(); }
};

// =======================================================
// 5. Define GPIOE
// =======================================================
using GPIOE = GPIO_Port<GPIOE_BASE>;

// =======================================================
// 6. Clock Mapping
// =======================================================
template<typename Port>
struct GPIOClockBit;

template<>
struct GPIOClockBit<GPIOE>
{
    static constexpr std::uint32_t value = 4U;   // GPIOE clock enable bit
};

// =======================================================
// 7. GPIO Driver
// =======================================================
template<typename Port, std::uint8_t pin>
class GPIO
{
public:
    static_assert(pin < 16, "Invalid GPIO pin");

    static void enable_clock()
    {
        RCC::AHB1ENR() |= (1U << GPIOClockBit<Port>::value);
        (void)RCC::AHB1ENR();
    }

    static void set_output()
    {
        // Clear 2 bits
        Port::MODER() &= ~(3U << (pin * 2U));

        // Set as output mode: 01
        Port::MODER() |=  (1U << (pin * 2U));

        // Push-pull
        Port::OTYPER() &= ~(1U << pin);

        // No pull-up / pull-down
        Port::PUPDR() &= ~(3U << (pin * 2U));
    }

    static void set()
    {
        Port::BSRR() = (1U << pin);
    }

    static void reset()
    {
        Port::BSRR() = (1U << (pin + 16U));
    }

    static bool read_output()
    {
        return (Port::ODR() & (1U << pin)) != 0U;
    }

    static void toggle()
    {
        if (read_output())
            reset();
        else
            set();
    }
};

// =======================================================
// 8. LED Abstraction
// =======================================================
template<typename GPIO_PIN>
class LED
{
public:
    LED()
    {
        GPIO_PIN::enable_clock();
        GPIO_PIN::set_output();
    }

    void on()
    {
        GPIO_PIN::set();
    }

    void off()
    {
        GPIO_PIN::reset();
    }

    void toggle()
    {
        GPIO_PIN::toggle();
    }
};

// =======================================================
// 9. Delay
// =======================================================
void delay()
{
    for (volatile std::uint32_t i = 0; i < 200000U; ++i)
    {
        __asm volatile("nop");
    }
}

// =======================================================
// 10. Main
// =======================================================
int main()
{
    using LED_PIN = GPIO<GPIOE, 12>;   // External LED on PE12

    LED<LED_PIN> led;

    while (true)
    {
        led.toggle();
        delay();
    }
}
