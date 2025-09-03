#ifndef UART_H
#define UART_H
unsigned char UART0_Recieve(void);
void UART0_Transmit(unsigned char);
void UART0_TransmitString(const char*);
void UARTInit();
void UARTprintf(const char* restrict,...);
#define UARTprint(x) UART0_TransmitString(x)
#endif 