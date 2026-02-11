#include <stdint.h>
#include  "inc/tm4c123gh6pm.h"
#include "include/FreeRTOS.h"
#include "include/task.h"
#include <stdarg.h>
#include <stdio.h>
#include "UART.h"
unsigned char UART0_Recieve(void) 
{
    while (UART0_FR_R & UART_FR_RXFE);
    return (unsigned char)(UART0_DR_R & 0xFF);
}
void UART0_Transmit(unsigned char data) 
{
    while (UART0_FR_R & UART_FR_TXFF);
    UART0_DR_R = data;
}
void UART0_TransmitString(const char* str) 
{
	  taskENTER_CRITICAL();
    while (*str)
		{
        UART0_Transmit(*str);
        str++;
    }
		taskEXIT_CRITICAL();
}
void UARTInit()
{
	//Enable UART and GPIOA peripherals 
	SYSCTL_RCGCUART_R |= 0x01U;
	SYSCTL_RCGCGPIO_R |= 0x01U;
	UART0_CTL_R = 0x0U;
	UART0_CC_R = 0x0U;
	UART0_IBRD_R = 0x1B; 
	UART0_FBRD_R = 0x8;
	UART0_LCRH_R = 0x3<<5;
	GPIO_PORTA_AFSEL_R |= 0x3;
	GPIO_PORTA_PCTL_R = 0x11U;
	GPIO_PORTA_DEN_R |=0x3U;
	UART0_CTL_R = 0x0301U;	
}
void UARTprintf(const char* str,...)
{
	int length;
	char buffer[1<<6];
	int i;
	va_list args;
	taskENTER_CRITICAL();
	//We have declared stdargs earlier
	va_start(args,str);
	length = vsnprintf(buffer,sizeof(buffer), str, args);
	va_end(args);
	for (i = 0; i < length && i < sizeof(buffer); i++) {
        UART0_Transmit(buffer[i]);
  	}
	taskEXIT_CRITICAL();
}