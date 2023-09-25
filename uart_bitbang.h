
#ifndef UART_BITBANG_H_
#define UART_BITBANG_H_

void UART_BitBang_Init(void);
void UART_BitBang_Tx_Bit(uint8_t b);
void UART_BitBang_Tx_Byte(uint8_t byte);
#endif /* UART_BITBANG_H_ */
