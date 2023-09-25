
#include <stdio.h>
#include "reg_adr.h"
#include "Kmx32.h"
#include "reg_def.h"
#include "main.h"
//50 ���!
uint32_t period = 80; //=115 us => 8000 ���
//uint32_t period = 104; // 9600 ���
void UART_BitBang_Init(void)
{
	WRITE_REGR(AHB_P1_DIR, 0xFF);

}

void UART_BitBang_Tx_Bit(uint8_t b)
{
	if (b == 0x00)
	{
		// TX � "0"
		WRITE_REG(AHB_P1_OUT,!bit(0));
	}
	else
	{
		// TX � "1"
		WRITE_REG(AHB_P1_OUT, bit(0));
	}
	// 9600 ���: T = 10 / V = 104 ���,
	// 19200 ���: T = 10 / V = 52 ���,
	uDelay(period);
}

void UART_BitBang_Tx_Byte(uint8_t byte)
{
	int i;


	UART_BitBang_Tx_Bit(0x00);	//start bit
	for (i = 0; i < 8; i++)
	{
		if ((byte >> i) & 0x01)
		{
			UART_BitBang_Tx_Bit(0x01);	//data bit
		} else
		{
			UART_BitBang_Tx_Bit(0x00);	//data bit
		}
	}
	UART_BitBang_Tx_Bit(0x01);	//stop bit

}
