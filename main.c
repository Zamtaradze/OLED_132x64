
#include <stdint.h>
//#include <stdio.h>
#include "main.h"
#include "reg_adr.h"
#include "Kmx32.h"
#include "i2c.h"
//#include "uart_bitbang.h"
#include "font16.h"
#include "oled.h"
#include "menu.h"
#include "reg_adr.h"
#include "reg_def.h"
//

//#include "FreeSans9pt7b.h"


#define SEC(n)	(n*10*51200U)

#define Bit(n)	(1U << (n))s
#ifndef BIT
#define BIT(n)	(1U << (n))
#endif
#define bit(n)	(1U << (n))



void uDelay(const uint32_t usec);
MENU_PARAMS_t* ButtonCyclingPolling(MENU_PARAMS_t* _PARAMS);

int main()
{

	MENU_PARAMS_t MENU_PARAMS =
	{
			0,
			&Main_Menu_t,
			0,
			433.015
	};
	MENU_PARAMS_t* ptr_MENU_PARAMS = &MENU_PARAMS;


	WRITE_REGR(AHB_P1_DIR, 0xFFFF);
	WRITE_REGR(AHB_P1_DIR, 0x0000);

	I2C_Off();
	I2C_Config();

	OLED_Init();

	uint32_t tmp;
	WRITE_REGR(AHB_P2_DIR, 0x0000);
	WRITE_REGR(AHB_P2_DIR, 0xFFFFFFFF);
	WRITE_REGR(AHB_P2_OUT, 0x0000);
	WRITE_REGR(AHB_P2_OUT, 0xFFFFFFFF);

	READ_REG(AHB_P2_DIR, &tmp);
	READ_REG(AHB_P2_OUT, &tmp);

	OLED_Fill(0x00);
	Picture_Update(LO);
	uDelay(1000);
	OLED_Fill(0x00);
	//OLED_Clear();
	uDelay(100);
	Picture_Update(LO2);
	uDelay(100);
	Strobe_Update(Frame_Buf);


	OLED_Fill(0xFF);
	Strobe_Update(Frame_Buf);


	OLED_Fill(0x00);
	Strobe_Update(Frame_Buf);


	OLED_Fill(0x00);
	//Print_Str(_codingCP866("проверка русского текста"),&SmallFontRus_t, 0, 8, NORMAL);

	Strobe_Update(Frame_Buf);
	MenuCurrentStateDisplayer(&MENU_PARAMS);
	while (1)
	{
		ptr_MENU_PARAMS = ButtonCyclingPolling(ptr_MENU_PARAMS);
	}

	return 0;
}

MENU_PARAMS_t* ButtonCyclingPolling(MENU_PARAMS_t* _PARAMS)
{

		_PARAMS->menu_cursor%=8;
		uint16_t tmp;
		uDelay(20000);
		WRITE_REG(AHB_P1_DIR, 0xFFFF);
		WRITE_REG(AHB_P1_OUT, 0x0000);
		WRITE_REG(AHB_P1_DIR, 0x0000);

		READ_REG(AHB_P1_IN, &tmp);
		switch(_PARAMS->menu_id)
		{
			default:
			{
					if((tmp & 0x4000) && (tmp & 0x800))
					{
						_PARAMS->menu_cursor++;
						MenuCurrentStateDisplayer(_PARAMS);
						}
					else if((tmp & 0x1000) && (tmp & 0x800))
					{
						_PARAMS->menu_cursor--;
						MenuCurrentStateDisplayer(_PARAMS);
					}

					else if((tmp & 0x2000) && (tmp & 0x800))
					{
						_PARAMS->menu_id=0;
						_PARAMS->ptr_Current_Menu = &Main_Menu_t;
						MenuCurrentStateDisplayer(_PARAMS);
					}
					else if(!(tmp & 0x800))
					{
						*_PARAMS = ScreenDisplayer(_PARAMS);
						Strobe_Update(Frame_Buf);
					}
				break;
			}
			case 8:
			{
				if((tmp & 0x4000) && (tmp & 0x800))
				{
					(_PARAMS->channel > 434.075) ?  (_PARAMS->channel = 434.075) : (_PARAMS->channel+=0.025);
					SelectChannelScreen(_PARAMS->channel);
				}
				else if((tmp & 0x1000) && (tmp & 0x800))
				{
					(_PARAMS->channel < 433.025) ?(_PARAMS->channel = 433.025) : (_PARAMS->channel-=0.025);
					SelectChannelScreen(_PARAMS->channel);
				}
				else if(!(tmp & 0x800))
				{
					_PARAMS->menu_id=0;
					_PARAMS->ptr_Current_Menu = &Main_Menu_t;
					MenuCurrentStateDisplayer(_PARAMS);
				}
				break;
			}
			//default:break;
		}



			WRITE_REG(AHB_P1_DIR, 0xFFFF);
			WRITE_REG(AHB_P1_DIR, 0x0000);

			return _PARAMS;

}





void uDelay(const uint32_t usec)
{
	uint32_t count = 0;
	const uint32_t utime = (1 * usec);
	do
	{
		if ( ++count > utime )
		{
			return ;
		}
	}
	while (1);
}
