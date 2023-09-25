
#include <stdio.h>
#include "reg_adr.h"
#include "Kmx32.h"
#include "reg_def.h"
#include "main.h"
#include "uart_bitbang.h"
uint8_t tmp;

//sclk 3 фиол.
//sda 2 синий


/* ack ВЫСОКИЙ уровень, выставленный передатчиком на доп. клок такте
 * slave ПРИЁМНИК должен генерировать ACK после принятия каждого байта
 * также и master генерирует ack в случае приёма
 * устройство, которое ПОДТВЕРЖДАЕТ, должно ОПУСТИТЬ SDA во время тактового импульса ACK
 * так линия SDA стабильно НИЗКАЯ во время ВЫСОКОГО импульса клок ACK
 * мастер приёмник должен просигнализировать о конце даты  передатчику НЕ ГЕНЕРИРУЯ ACK на последнем байте клока от слейва
 * В этом случае приёмник должен оставить SDA в 1, чтобы разрешить мастеру сгенерировать сигнал стоп
*/



void I2C_Config(void)
{
	//WRITE_REG(AHB_P1_DIR, 0x000000FF); // разрешается выдача активного сигнала.
	//WRITE_REG(AHB_P1_OUT, 0x00000000); // на внешнем выводе порта низкий уровень напряжения.

	//WRITE_REG(AHB_I2C1_PRER, 0x00); // делитель частоты PRER=(SCLK/5*I2C_CLK)-1


	// Переключить выводы GPIO на I2C
	//WRITE_REGR(AHB_P3_DIR, 0x000000FF);
	WRITE_REG_OR(AHB_P3_SEL, GPIO_3 | GPIO_4);


	WRITE_REG(AHB_I2C1_CTR, 0x80); // включение контроллера AHB_I2C.
	WRITE_REG(AHB_I2C1_PRER, 0x0F); // делитель частоты PRER=(SCLK/5*I2C_CLK)-1
	//WRITE_REG(AHB_I2C1_TXR, 0x00);
}

void I2C_Off(void)
{
	// Отключить выводы GPIO на I2C
	WRITE_REG_AND(AHB_P3_SEL, ~(GPIO_3 | GPIO_4));
	WRITE_REG(AHB_I2C1_CTR, 0x00); // выключение контроллера AHB_I2C.
}

/*void I2C_Send(uint8_t data)
{
	WRITE_REG(AHB_I2C1_TXR, data);
	//WRITE_REG(AHB_I2C1_CR, 0x000000F0); // установка состояний STA, STO, RD и WR.
	WRITE_REG(AHB_I2C1_CR, 0x000000D1);//STA WR ACK
	//WRITE_REG(AHB_I2C1_CR, 0x00000080);//STA
	//uDelay(10000);//wait for ack
	//WRITE_REG(AHB_I2C1_CR, 0x00000040);//STO
	//return (((state << 8) & 0xFF00) | change);
	//return change;
	//WRITE_REG(AHB_I2C1_CR, 0x000000D0); //STA, STO, WR
}*/
void I2C_Stop(void)
{
	WRITE_REG(AHB_I2C1_CR, 0x40); //STO
}

void I2C_Ack_Waiter(uint8_t flag)
{
	uint8_t i;
	uint32_t tim = 0;
	do
	{
		READ_REG(AHB_I2C1_SR, &i);
		//UART_BitBang_Tx_Byte(i);
		tim++;
		if (tim > 1000)
			{
				/*while(!READ_REGR(AHB_P2_IN))	//отладочный джампер == закрыть и выход из сост. таймаута
					{*/
					WRITE_REG(AHB_P1_OUT, 0x40);
					uDelay(100000);				//подтверждения не произошло
					WRITE_REG(AHB_P1_OUT, 0x80);
					//WRITE_REG_OR(AHB_P1_OUT, (0x80 & !0x40));
					uDelay(100000);
					WRITE_REG(AHB_P1_OUT, 0x00);
					/*}*/
			}
	}
	//while (((i & flag) > 0) && (READ_REGR(AHB_P2_IN)) );
	while ((i & flag) != flag);
	//uDelay(1000);
}

void I2C_Stat(uint32_t val, uint32_t timeout)
{
	uint32_t tmp;

	// Завершение передачи
	while(1)
	{
		READ_REG(AHB_I2C1_SR, &tmp);
		if ((tmp & 0x02) == 0)
		{
			break;
		}
	}

	// Признаки
	while(timeout--)
	{
		READ_REG(AHB_I2C1_SR, &tmp);
		if (tmp == val)
		{
			//UART_BitBang_Tx_Byte(tmp);
			break;
		}
	}

	}

void I2C_Byte(uint8_t addr, uint8_t cntr, uint8_t byte)
{

	//WRITE_REG(AHB_I2C1_TXR, 0x00);
	WRITE_REG(AHB_I2C1_TXR, addr);
	WRITE_REG(AHB_I2C1_CR, 0x98);//STA WR ACK
	I2C_Stat(0x41,1000);
	//I2C_Ack_Waiter(0x41);
	//uDelay(1000);


	//WRITE_REG(AHB_I2C1_TXR, 0x00);
	WRITE_REG(AHB_I2C1_TXR, cntr);
	WRITE_REG(AHB_I2C1_CR, 0x18);//STA WR ACK
	I2C_Stat(0x41,1000);
	//I2C_Ack_Waiter(0x41);
	//uDelay(1000);


	//WRITE_REG(AHB_I2C1_TXR, 0x00);
	WRITE_REG(AHB_I2C1_TXR, byte);
	WRITE_REG(AHB_I2C1_CR, 0x18);//STA WR ACK
	I2C_Stat(0x41,1000);
	//I2C_Ack_Waiter(0x41);
	//uDelay(1000);

	WRITE_REG(AHB_I2C1_CR, 0x40); //STO
	I2C_Stat(0x81,1000);
	//uDelay(1000);
	//I2C_Ack_Waiter(0x81);
	/*I2C_Ack_Waiter(0x41); //IF + BUSY == окончание текущ команды и !ДО СОСТОЯНИЯ СТОП!
	UART_BitBang_Tx_Byte(READ_REGR(AHB_I2C1_SR));
	I2C_Stop();
	I2C_Ack_Waiter(0x81); //IF + RXASK == должно прийти подтверждение ACK
	//WRITE_REG(AHB_I2C1_CR, 0x40);
	//READ_REG(AHB_I2C1_SR, &tmp);
	//UART_BitBang_Tx_Byte(READ_REGR(AHB_I2C1_SR));
	//uDelay(100);
	//WRITE_REG(AHB_I2C1_CR, 0x40);
	//

	//uDelay(100);
	//WRITE_REG(AHB_I2C1_CR, 0x40);*/
}

/*void I2C_Start(void)
{
	WRITE_REG(AHB_I2C1_CR, 0x90);//STA WR
}*/
void I2C_Strobe(uint8_t addr, uint8_t cntr, uint8_t byte[], uint8_t count)
{
	uint32_t tmp;
	uint32_t i;

	//WRITE_REG(AHB_I2C1_TXR, 0x00);
	WRITE_REG(AHB_I2C1_TXR, addr);
	WRITE_REG(AHB_I2C1_CR, 0x98);//STA WR ACK
	I2C_Stat(0x41,100);
	//I2C_Ack_Waiter(0x41);
	//uDelay(1000);


	//WRITE_REG(AHB_I2C1_TXR, 0x00);
	WRITE_REG(AHB_I2C1_TXR, cntr);
	WRITE_REG(AHB_I2C1_CR, 0x18);// WR ACK
	I2C_Stat(0x41,100);
	//I2C_Ack_Waiter(0x41);
	//uDelay(1000);

	for(i = 0; i < count; i++)
	{
		// Start Wr Data
		tmp = byte[i];
		//WRITE_REG(AHB_I2C1_TXR, 0x00);
		WRITE_REG(AHB_I2C1_TXR, tmp);
		WRITE_REG(AHB_I2C1_CR, 0x18);//STA WR ACK
		I2C_Stat(0x41,100);
	}
	// Stop
		WRITE_REG(AHB_I2C1_CR, 0x40); //STO
		I2C_Stat(0x81,100);

}




uint8_t I2C_Get(void)
{
	uint8_t gotten;

	WRITE_REG(AHB_I2C1_CR, 0x000000E0); //STA, STO, RD
	READ_REG(AHB_I2C1_RXR, &gotten);
	//READ_REG(AHB_I2C1_SR, &tmp); //чтение регистра состояния.
	return gotten;
}
