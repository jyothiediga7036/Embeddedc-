#include <cstdint>
#include"stm32f407.h"

// ==============================
// 🔹 Step 1: Low-level driver (explicit)
// ==============================
class GPIO
{
public:
    static void enable_clock()
    {
        RCC->AHB1ENR |= (1U << 3);  // Enable GPIOD clock
    }

    static void set_output(std::uint32_t pin)
    {
        GPIOD->MODER &= ~(3U << (pin * 2));
        GPIOD->MODER |=  (1U << (pin * 2));
    }

    static void toggle(std::uint32_t pin)
    {
        GPIOD->ODR ^= (1U << pin);
    }
};

// ==============================
// 🔹 Step 2: High-level abstraction (constructor)
// ==============================
class LED
{
    std::uint32_t pin;

public:
    explicit LED(std::uint32_t p) : pin(p)
    {
        // Assumes clock is already enabled
        GPIO::set_output(pin);
    }

    void toggle()
    {
        GPIO::toggle(pin);
    }
};

// ==============================
// 🔹 Delay function
// ==============================
void delay()
{
    for (volatile std::uint32_t i = 0; i < 100000; ++i)
    {
        // Prevent optimization
    }
}

// ==============================
// 🔹 Step 3: Controlled initialization in main()
// ==============================
int main()
{
    constexpr std::uint32_t LED_PIN = 12;

    // Explicit hardware initialization
    GPIO::enable_clock();

    // Safe object creation (after hardware is ready)
    LED led(LED_PIN);

    while (true)
    {
        led.toggle();
        delay();
    }
}
