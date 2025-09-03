#include "UART.h"
#include "include/FreeRTOS.h"
#include "include/task.h"
#include "include/timers.h"
#include "include/queue.h"
#include "include/semphr.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "inc/hw_memmap.h"
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_ints.h"
#include "driverlib/interrupt.h"
#include "LCD.h"
#include "LED.h"
unsigned char button1_pressed(){return ((~GPIO_PORTF_DATA_R) &0x10);}
unsigned char button2_pressed(){return ((~GPIO_PORTF_DATA_R) &0x01);}
static unsigned long long ullIdleCount=0;
void vApplicationIdleHook(){
	ullIdleCount++;
}
void vTask1( void *pvParameters )
{
	for( ;; )
	{
	UARTprintf("Task 1 is running\n"); 
	vTaskDelay(1000/portTICK_RATE_MS);
	}
}
int main()
{
	UARTInit();
	LEDInit();
	xTaskCreate( vTask1, "T1", 240, NULL, 2, NULL );
	vTaskStartScheduler();
	for( ;; );
}