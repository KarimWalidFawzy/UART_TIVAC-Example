#ifndef LED_H
#define LED_H
#define RGBSet(x){GPIO_PORTF_DATA_R|=x;}
#define RGBClear(x){GPIO_PORTF_DATA_R&=(~x);}
#define RED 0x2ULL
#define BLUE 0x4ULL
#define GREEN 0x8ULL
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "inc/hw_memmap.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/interrupt.h"
#define LEDInit(){\
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);\
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));\
  GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3); \
}
#endif