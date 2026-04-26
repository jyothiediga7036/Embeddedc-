/*
 * lcd.cpp
 *
 *  Created on: 21-Apr-2026
 *      Author: mirafra
 */
#include"stm32f407.h"
#include"lcd.h"
void LCD::delay_ms(volatile int t)
{
		for(int i=0;i<t*8000;i++);
}
void LCD::LCD_RS(int state)
{
		if(state){    // rigister select 0-> cmd, 1-> data mode
			GPIOB->ODR |= (1<<0);
		}
		else{
				GPIOB->ODR &= ~(1<<0);
			}
}
void LCD::LCD_EN()
{
			GPIOB->ODR |= (1<<1);   // output mode
			for(volatile int i=0;i<2000;i++);
			GPIOB->ODR &= ~(1<<1);
			for(volatile int i=0;i<2000;i++);
}
void LCD::lcd_send(uint8_t data)
{
		GPIOB->BSRR = (0x0F << 20);                // clear D4-D7 16+4
		for(volatile int i=0;i<50;i++);

		GPIOB->BSRR = ((data & 0x0F) << 4);     // set D4-D7

		LCD_EN();
}
void LCD::lcd_contrl()
{

		RCC->AHB1ENR |= (1<<1);  // GPIOB
		GPIOB->MODER &= ~(3<<(0*2)); // Rs
		GPIOB->MODER |=  (1<<(0*2)); // output

		GPIOB->MODER &= ~(3<<(1*2)); // EN
		GPIOB->MODER |=  (1<<(1*2)); // output

		GPIOB->MODER &= ~(3<<(4*2)); // D4
		GPIOB->MODER |=  (1<<(4*2)); // output

		GPIOB->MODER &= ~(3<<(5*2)); // D5
		GPIOB->MODER |=  (1<<(5*2)); // output

		GPIOB->MODER &= ~(3<<(6*2)); // D6
		GPIOB->MODER |=  (1<<(6*2)); // output

		GPIOB->MODER &= ~(3<<(7*2)); // D7
		GPIOB->MODER |=  (1<<(7*2)); // output
}

void LCD::LCD_SendByte(int data,int isdata)
{
		LCD_RS(isdata);

		lcd_send(data >> 4);   // upper nibble
		delay_ms(1);

		lcd_send(data & 0x0F); // lower nibble
		delay_ms(1);
}
void LCD::lcd_init()
{
		delay_ms(20);

		LCD_SendByte(0x03,0);  // enable 4- bit  mode /  8 bit mode
		delay_ms(5);

		LCD_SendByte(0x02,0);  //  enable 4 - bit mode
		delay_ms(2);

		LCD_SendByte(0x28,0); //enable 4 -bit mode 2 lines
		delay_ms(2);

		LCD_SendByte(0x0C,0); //disable cursor, display on

		LCD_SendByte(0x01,0); // clear the screen
		delay_ms(5);

		LCD_SendByte(0x06,0);   // Entry mode (cursor moves right)
}
void LCD::set_cursor(int row,int col)
{
		int addr;

		if(row==0)
			addr = 0x80 + col;   // select  0 row  0x80 is starting address of 0 row
		else
			addr = 0xC0 + col;  // select 1 row 0xc0 is starting address of 1 row

		LCD_SendByte(addr,0);
}
void LCD::lcd_string(char* str)
{
		while(*str){
			LCD_SendByte(*str,1);
			str++;
		}
}



