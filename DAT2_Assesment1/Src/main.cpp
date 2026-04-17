#include<stdint.h>
#include "stm32f407.h"
#include "Transformer.h"
#include "Performance.h"
int main()
{
	Transformer t(12,5);     // 60w ->30%
	Performance p(12,13,14,15);
	Transformer t1(10, 4);   // p= 40W->20%
	Transformer t2(10, 10);  // 100W -> 50%
	Transformer t3(12, 7);   // 84W  -> 42%
	Transformer t4(20, 8);   // 160W -> 80%

	while(1)
	{
		p.Transformer_status(t);
		p.Transformer_status(t1);
		p.Transformer_status(t2);
		p.Transformer_status(t3);
		p.Transformer_status(t4);

	}
}
