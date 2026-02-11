#include "LCD.h"
#include "include/FreeRTOS.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/adc.h"
#include "inc/hw_memmap.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/interrupt.h"
#include "include/task.h"
#include "stdio.h"
#include "stdarg.h"
void delay_ms(int time)
{
    SysCtlDelay(SysCtlClockGet() / 3000 * time);
}
void LCD_Command(unsigned char);
void LCDInit()
{	
	//Enable GPIOC, B and A 
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOC));	
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB));
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA));
	GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_5 | GPIO_PIN_4); //DB7-4
	GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_3 | GPIO_PIN_2 | GPIO_PIN_1 | GPIO_PIN_0); //DB3-0
	GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_6 | GPIO_PIN_7| GPIO_PIN_5); //6 is RS and 7 is R_W	and 5 is E
	// LCD initialization sequence
  delay_ms(20);                   // Wait for LCD to power up
  LCD_Command(0x38);              // Function set: 8-bit, 2-line, 5x7
  LCD_Command(0x0C);              // Display ON, cursor OFF
  LCD_Command(0x01);              // Clear display
  delay_ms(2);
  LCD_Command(0x06);              // Entry mode: cursor moves right
}
void LCD_EnablePulse()
{
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5, GPIO_PIN_5);  // E = 1
    delay_ms(1);
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5, 0);            // E = 0
    delay_ms(1);
}
void LCD_Command(unsigned char c)
{
    // RS = 0, RW = 0
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, 0);  // RS = 0
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_7, 0);  // RW = 0
    // Send command to data pins
    GPIOPinWrite(GPIO_PORTC_BASE, 0xF0, c & 0xF0);               // Send upper nibble to PC4-7
    GPIOPinWrite(GPIO_PORTB_BASE, 0x0F, c & 0x0F);               // Send lower nibble to PB0-3
    LCD_EnablePulse();
}
void LCD_Data(unsigned char c)
{
    // RS = 1, RW = 0
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, GPIO_PIN_6);  // RS = 1
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_7, 0);           // RW = 0
    // Send data to data pins
    GPIOPinWrite(GPIO_PORTC_BASE, 0xF0, c & 0xF0);          // Send upper nibble to PC4-7
    GPIOPinWrite(GPIO_PORTB_BASE, 0x0F, c & 0x0F);          // Send lower nibble to PB0-3
    LCD_EnablePulse();
}
void LCDprint(char* str)
{
	taskENTER_CRITICAL(); //Prevent Pre-emption
	while(*str)
	{
		LCD_Data(*str);
		str++;
	}
	taskEXIT_CRITICAL();
}
void LCDprintf(const char* restrict,...)
{
	int length;
	char buffer[1<<6];
	int i;
	va_list args;
	taskENTER_CRITICAL();
	va_start(args,restrict);
	length = vsnprintf(buffer,sizeof(buffer), restrict, args);
	va_end(args);
	for (i = 0; i < length && i < sizeof(buffer); i++) {
        LCD_Data(buffer[i]);
  }
	taskEXIT_CRITICAL();
}