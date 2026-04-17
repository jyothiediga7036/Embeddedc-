#ifndef TRANSFORMER_H
#define TRANSFORMER_H
#include"stm32f407.h"
class Transformer
{
	uint32_t voltage;
	uint32_t current;
	uint32_t power;
public:
	Transformer(uint32_t v=0,uint32_t i=0,uint32_t p=0);
	uint32_t power_calculation();
	void reset_power();
};
#endif
