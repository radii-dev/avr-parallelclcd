/*
 * CLCD test.c
 *
 * Created: 2020-03-21 오후 2:22:57
 * Author : gigas
 */ 

#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "CLCD.h"

int main(void)
{
	DDRC = 0xFF;
	LCD_init();
	/*
	LCD_goto_XY(0,0);
	LCD_write_string("E JAE");
	LCD_goto_XY(1,5);
	LCD_write_string("DUE YO!");
	LCD_goto_XY(0,8);
	LCD_write_string("hmmm");
	*/
	LCD_clear();
    while (1) 
    {
		PORTC = ~PORTC;
		_delay_ms(300);
    }
}