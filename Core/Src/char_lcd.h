//
// Created by yuno on 22. 3. 18..
//

#ifndef L152RE_CLCD_TEST_CHAR_LCD_H
#define L152RE_CLCD_TEST_CHAR_LCD_H
#include "stdint.h"
uint8_t LCD_BF();
void LCD_4bit_Write(uint8_t rs, uint8_t data);
void LCD_init();
void LCD_Write_str(uint8_t *str, int length);
void LCD_set_cursor(uint8_t line, uint8_t pos);
void LCD_clear();


#endif //L152RE_CLCD_TEST_CHAR_LCD_H
