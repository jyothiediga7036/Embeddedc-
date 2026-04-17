#include<stdint.h>
#include"stm32f407.h"
static int button_count;
void delay(int n){
	for(;n>=0;n--){
		for(int i=0;i<200000;i++);
	}
}

class observer{
public:
	virtual void update()=0;
};
class GPIO_STATUS{
	observer* observers[4];
	int observer_count;
	public:
	GPIO_STATUS(){
		GPIO_init();
		observer_count=0;
	}
	void GPIO_init(){
		RCC->AHB1ENR|=(1<<3);
		RCC->AHB1ENR|=(1<<0);

		GPIOD->MODER&=~((3<<24)|(3<<26)|(3<<28)|(3<<30));
		GPIOD->MODER|=((1<<24)|(1<<26)|(1<<28)|(1<<30));

		GPIOA->MODER&=~(3<<0);
	}
	void attach(observer* obj){
		observers[observer_count++]=obj;
	}

	void press_switch()
	{
		int prev=0;
		int status=GPIOA->IDR&(1);
		if(status==1&& prev==0){
			if(GPIOA->IDR&1){
				button_count++;
				notify();

			}
		}
		 prev = status;
	}
	void notify(){
		for(int i=0;i<observer_count;i++){
			observers[i]->update();
		}
	}

};
class find:public observer{
	public:
	void update()
	{
		delay(2);
		if(button_count%2!=0)
		{
			GPIOD->ODR|=(1<<12);
			GPIOD->ODR|=(1<<14);
			GPIOD->ODR&=~(1<<13);
			GPIOD->ODR&=~(1<<15);
			delay(2);
		}
		if(button_count%2==0)
		{
			GPIOD->ODR&=~(1<<12);
			GPIOD->ODR&=~(1<<14);
			GPIOD->ODR|=(1<<13);
			GPIOD->ODR|=(1<<15);
			delay(3);
		}
		if(button_count==10)
		{
			GPIOD->ODR|=(1<<13);
			GPIOD->ODR|=(1<<15);
			GPIOD->ODR|=(1<<12);
			GPIOD->ODR|=(1<<14);
			delay(3);
			button_count=0;
			GPIOD->ODR &= ~((1<<12)|(1<<13)|(1<<14)|(1<<15));
		}
	}
};
int main(){
	GPIO_STATUS s;
	find f;
	s.attach(&f);
	while(1)
	s.press_switch();
}
