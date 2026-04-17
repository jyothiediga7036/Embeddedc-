#ifndef PERFORMANCE_H
#define PERFORMANCE_H
#include <stdint.h>
#include "stm32f407.h"
#include "Transformer.h"
class Performance
{
	uint32_t green;
	uint32_t orange;
	uint32_t red;
	uint32_t blue;
public:
	Performance(uint32_t g,uint32_t o,uint32_t r,uint32_t b);
	void Transformer_status(Transformer& obj);
	void on(uint32_t pin);
	void off(uint32_t pin);
	void all_off();
};
#endif
