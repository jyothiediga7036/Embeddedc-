#include "Transformer.h"
Transformer::Transformer(uint32_t v, uint32_t i, uint32_t p)
{
		voltage=v;
		current=i;
		power=p;
}
uint32_t Transformer::power_calculation()
{
		power=(voltage*current);
		uint32_t p=(power*100)/200;
		if(p>100)
			return 0;

		return p;
}
void Transformer::reset_power()
{
		power = 0;
		voltage = 0;
		current = 0;
		power = 0;
}
