#ifndef LCD_H
#define LCD_H
void delay_ms(int time);
void LCD_Command(unsigned char);
void LCDInit();
void LCD_EnablePulse();
void LCD_Data(unsigned char c);
void LCDprint(char* str);
void LCDprintf(const char* restrict,...);
#endif