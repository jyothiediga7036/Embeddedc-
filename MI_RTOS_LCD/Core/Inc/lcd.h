/*
 * lcd.h
 *
 *  Created on: 21-Apr-2026
 *      Author: mirafra
 */

#ifndef INC_LCD_H_
#define INC_LCD_H_

class LCD
{
	void delay_ms(volatile int t);
	void LCD_RS(int state);
	void LCD_EN();
	void lcd_send(uint8_t data);

public:
	void lcd_contrl();
	void LCD_SendByte(int data,int isdata);
	void lcd_init();
	void set_cursor(int row,int col);
	void lcd_string(char* str);
};



#endif /* INC_LCD_H_ */
