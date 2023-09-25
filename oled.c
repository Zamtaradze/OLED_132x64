
//#include <stdio.h>
#include <stdlib.h>
#include "reg_adr.h"
#include "Kmx32.h"
#include "reg_def.h"
#include "main.h"
#include "i2c.h"
//#include "uart_bitbang.h"
//#include "math.h"
#include "oled.h"
#include "font16.h"
#include <string.h>
/*
 * Перед тем, как какие-либо данные будут переданы, устройство должно быть АДРЕСОВАНО
 * два семибитных адреса зарезервированы под SH1106 (0111100 and 0111101)
 * Последний значащий бит адреса выставляется соеднинением входа SA0 к лог. 0(VSS) или 1(VDD1)
 * 			SA0 - бит D/C# в режиме I2C
 * протокол IIC:
 * Начинается с сигнала START от мастера, сопровождающимся АДРЕСОМ slave
 * После подтверждения идут одно или несколько командных слов
 * Командное слово состоит из Control byte, который определяется Co и D/~C (note 1) плюс дата байт
 * Последний Control byte отмечен ОТЧИЩЕННЫМ СТАРШИМ БИТОМ, продолжением бита Co.
 * После control byte с отчищенным Co-bit идут только дата байты
 *
 * Состояние D/~C определяет, как интерпретировать дата-байт(как команда, или как РАМ-дату)
 * Контрол и дата байты тоже подтверждаются всеми адресованными слейвами на шине
 * После последнего контрол байта, определяющего настройку D/~C бита,
 * могут следовать дата или команд байты
 * Если D/~C ==1, то дата байты записываются в РАМ по адресу в указателе, он автоматически обновляется
 * Если D/~C ==0, то комманд байты будут декодированы и настрйока уствройства будет изменена соотв. командам
 *
 * */





/*FONT_t Font11x18_t =
{
	18,
	11,
	16,
	Font11x18,
	ENG
};*/

FONT_t font_default_t =
{
	8,
	5,
	8,
	font_default,
	ENG
};

FONT_t SmallFontRus_t =
{
	8,
	6,
	8,
	SmallFontRus,
	RUS
};

/*struct Font_cast=
{
	FONT_T& caster;
	Font_cast
}
/*FontDef_t Font_7x10 = {
	7,
	10,
	Font7x10
};*/
void Reset(void)
{
	WRITE_REG(AHB_P3_OUT, 0xFF);
	uDelay(SEC(1));
	WRITE_REG(AHB_P3_OUT, 0x00);
	uDelay(SEC(1));
	WRITE_REG(AHB_P3_OUT, 0xFF);
	WRITE_REG(AHB_P3_OUT, 0x02);
	/*uDelay(SEC(1));
	WRITE_REG(AHB_P3_OUT, 0x00);*/

}

void OLED_Config(void)
{
	//I2C_Start();
	//Command(0x3D);
	//пока что
	//Command(0xAE);//--turn off oled panel
	uDelay(1000);
	I2C_Byte(0x78,0x00,0xAE); //display off

	I2C_Byte(0x78,0x00,0x20); //Set Memory Addressing Mode
	I2C_Byte(0x78,0x00,0x00); //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	I2C_Byte(0x78,0x00,0xB0); //Set Page Start Address for Page Addressing Mode,0-7
	I2C_Byte(0x78,0x00,0xC8); //Set COM Output Scan Direction
	//I2C_Byte(0x78,0x00,0xC0);
	I2C_Byte(0x78,0x00,0x00); //---set low column address
	I2C_Byte(0x78,0x00,0x10); //---set high column address
	I2C_Byte(0x78,0x00,0x40); //--set start line address
	//I2C_Byte(0x78,0x00,0x7F);
	I2C_Byte(0x78,0x00,0x81); //--set contrast control register
	I2C_Byte(0x78,0x00,0xFF);
	//I2C_Byte(0x78,0x00,0xA0);
	I2C_Byte(0x78,0x00,0xA1); //--set segment re-map 0 to 127
	I2C_Byte(0x78,0x00,0xA6); //--set normal display
	I2C_Byte(0x78,0x00,0xA8); //--set multiplex ratio(1 to 64)
	I2C_Byte(0x78,0x00,0x3F); //
	I2C_Byte(0x78,0x00,0xA4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	I2C_Byte(0x78,0x00,0xD3); //-set display offset
	I2C_Byte(0x78,0x00,0x00); //-not offset
	I2C_Byte(0x78,0x00,0xD5); //--set display clock divide ratio/oscillator frequency
	I2C_Byte(0x78,0x00,0xF0); //--set divide ratio
	I2C_Byte(0x78,0x00,0xD9); //--set pre-charge period
	I2C_Byte(0x78,0x00,0x22); //
	I2C_Byte(0x78,0x00,0xDA); //--set com pins hardware configuration
	I2C_Byte(0x78,0x00,0x12);
	I2C_Byte(0x78,0x00,0xDB); //--set vcomh
	I2C_Byte(0x78,0x00,0x20); //0x20,0.77xVcc
	I2C_Byte(0x78,0x00,0x8D); //--set DC-DC enable
	I2C_Byte(0x78,0x00,0x14); //

	I2C_Byte(0x78,0x00,0xAF);

}

void OLED_Init(void)
{
	//Reset();
	OLED_Config();
	OLED_Fill(0x00);
	//OLED_Clear();
	uDelay(1000);
	I2C_Byte(0x78,0x00,0xAF);
	//Command(0xAF);
}

void OLED_Clear(void)
{
	//Default
    // UWORD Width, Height;
    uint8_t i, j;
    // Width = (OLED_1IN3_WIDTH % 8 == 0)? (OLED_1IN3_WIDTH / 8 ): (OLED_1IN3_WIDTH / 8 + 1);
    // Height = OLED_1IN3_HEIGHT;
    for (i=0; i<8; i++) {
    	//Command_Preamb();
        /* адрес страницы */
        I2C_Byte(0x78,0x00,0xB0 + i);
        /* адрес нижней строчки*/
        I2C_Byte(0x78,0x00,0x02);
        /* адрес верхней строчки */
        I2C_Byte(0x78,0x00,0x10);
        for(j=0; j<OLED_WIDTH; j++)
        {
            /* записать дату */
            //Data_Preamb();
            I2C_Byte(0x78,0x40,0x00);
        }
    }
}

void Fplotter(void)
{
	//Default
    uint8_t i, j;
    for (i=0; i<8; i++)
    {
    	I2C_Byte(0x78,0x00,0xB0 + i);
    	I2C_Byte(0x78,0x00,0x02);
    	I2C_Byte(0x78,0x00,0x10);
        for(j=0; j<OLED_WIDTH; j++)
        {
            I2C_Byte(0x78,0x40,0xFF);
        }
    }
}

void Update(uint8_t *Image)
{
	uint8_t m,col, tmp;

	for (m = 0; m < 8; m++)
	{
		I2C_Byte(0x78,0x00,0xB0 + m);
		I2C_Byte(0x78,0x00,0x00);
		I2C_Byte(0x78,0x00,0x10);

		// записать автоинкрементом
		//I2C_Strobe(0x78, 0x40, &OLED_Buf[128 * m], 128);
		for(col=0; col<OLED_WIDTH; col++)
		{
		    tmp = Image[(7-m) + col*8];
			//tmp = Image[m + col*8];
		    I2C_Byte(0x78,0x40,tmp);
		}
		//I2C_Strobe(0x78, 0x40, &OLED_Buf[128 * m], 1030);

	}
}

void Strobe_Update(uint8_t *Image)
{
	uint8_t p/*,col, tmp*/;

	for (p = 0; p < 8; p++)
	{
		I2C_Byte(0x78,0x00,0xB0 + p);
		I2C_Byte(0x78,0x00,0x00);
		I2C_Byte(0x78,0x00,0x10);

		I2C_Strobe(0x78,0x40,&Image[OLED_WIDTH*p],OLED_WIDTH);


	}
}

void Picture_Update(uint8_t *Image)
{
	uint8_t m/*,col, tmp*/;

	for (m = 0; m < 8; m++)
	{
		I2C_Byte(0x78,0x00,0xB0 + m);
		I2C_Byte(0x78,0x00,0x00);
		I2C_Byte(0x78,0x00,0x10);

		I2C_Strobe(0x78,0x40,&Image[128*(7-m)],128);

	}
}
//---------------------------------------------------------------------
#define swap(a, b)                                                     \
  (((a) ^= (b)), ((b) ^= (a)), ((a) ^= (b))) ///< No-temp-var swap operation



//#define drawpix(x,y) Frame_Buf[x + (y / 8) * 128] |= 1 << (y % 8);


/*uint8_t* font_ptr;

font;*/

//		Преобразование строки из кодировки UTF-8 в кодировку CP866:														//	Возвращаемое значение: строка в кодировке CP866.
/*char*	Text_Cast(char* string_in)
{																	//	Параметр: строка в кодировке UTF-8.
			/*char*	StrOut=StrIn;																						//	Определяем строку для вывода результата.
			uint8_t	numIn    =	0,																						//	Определяем переменную хранящую номер символа в строке StrIn.
					numOut   =	0,																						//	Определяем переменную хранящую номер символа в строке StrOut.
					charThis =	StrIn[0];																				//	Определяем переменную хранящую код текущего символа в строке StrIn.
					//charNext =	StrIn[1];																				//	Определяем переменную хранящую код следующего символа в строке StrIn.
																//
			//	Преобразуем текст из кодировки WINDOWS-1251:															//
				//TXT_WIN1251																					//
					while (charThis> 0 && numIn < 0xFF)
					{																//	Если код текущего символа строки StrIn больше 0 и № текушего символа строки StrIn меньше 255, то ...
						if(charThis>=0xC0 && charThis <= 0xEF)
							//{StrOut[numOut]=charThis-0x40;}
							 * else							//	Если код текущего символа равен 192...239, значит это буква «А»...«п» требующая преобразования к коду 128...175
							{StrOut[numOut]=charThis - 0x40;}
							else if(charThis>=0xF0 && charThis<=0xFF)
							{StrOut[numOut]=charThis-0x10;}
							else if(charThis==0xA8)
						{StrOut[numOut]=0xF0;}
						else if(charThis==0xB8){StrOut[numOut]=0xF1;}
						else
						 {StrOut[numOut]=charThis;}									//	Иначе не меняем символ.
						numIn++; numOut++; charThis=StrIn[numIn];														//	Переходим к следующему символу.
					}	StrOut[numOut]='\0';																			//	Добавляем символ конца строки и возвращаем строку StrOut.
																													//
				return StrOut;	*/																						//	Возвращаем строку StrOut.
		/*	char* string_out;
			uint8_t casted_StrIn;
			uint8_t count=0;
			while (*string_in)
			{
				casted_StrIn = string_in[count];
				if((casted_StrIn >=0xC0) & (casted_StrIn <= 0xEF)) //192-239
					//StrOut = StrIn - 0x40;
					casted_StrIn -= 0x40;
					else if ((casted_StrIn >=0xF0) & (casted_StrIn <=0xFF))
					//StrOut = StrIn - 0x10;
					casted_StrIn-=0x10;
					else if (casted_StrIn == 0xA8)
					casted_StrIn = 0xF0;
					//StrOut = 0xF0;
				else if (casted_StrIn == 0xB8)
					casted_StrIn = 0xF1;
					//StrOut = 0xF1;
				/*else
					StrOut = StrIn;*/
				//count++;
				//StrOut[count] = casted_StrIn;
			/*	*(string_out + (sizeof(char)*count)) = (char) casted_StrIn;
				count++;
				string_in++;
				//---
				//StrIn++;
				//StrOut++;
			}
			return string_out;

}*/

char*	_codingCP866(char* StrIn/*,uint8_t codingName*/)
{																	//	Параметр: строка в кодировке UTF-8.
			char*	StrOut=StrIn;																						//	Определяем строку для вывода результата.
			uint8_t	numIn    =	0,																						//	Определяем переменную хранящую номер символа в строке StrIn.
					numOut   =	0,																						//	Определяем переменную хранящую номер символа в строке StrOut.
					charThis =	StrIn[0];																				//	Определяем переменную хранящую код текущего символа в строке StrIn.
																					//
					while (charThis> 0   &&numIn    <0xFF)
					{																//	Если код текущего символа строки StrIn больше 0 и № текушего символа строки StrIn меньше 255, то ...
						if(charThis>=0xC0&&charThis<=0xFF)
						{
							StrOut[numOut]=charThis-0x60;
						}
						/*else if(charThis>=0xEF&&charThis<=0xFF)
						{
							StrOut[numOut]=charThis-0x40;
						}*/
						else if(charThis==0x20)
						{
							StrOut[numOut]=0x5F;
						}
						else if(charThis>=0x21&&charThis<=0xC0)
						{
							StrOut[numOut]=charThis-0x20;
						}
						else if(charThis==0xA8)
						{
							StrOut[numOut]=0xD0;
						}
						else if(charThis==0xB8)
						{
							StrOut[numOut]=0xD1;
						}
						else							//	Если код текущего символа равен 184      , значит это буква «ё»       требующая преобразования к коду 241
						{
							StrOut[numOut]=charThis;
						}									//	Иначе не меняем символ.
						numIn++;
						numOut++;
						charThis=StrIn[numIn];														//	Переходим к следующему символу.
					}	StrOut[numOut]='\0';																			//	Добавляем символ конца строки и возвращаем строку StrOut.
				//break;																									//
				return StrOut;																							//	Возвращаем строку StrOut.
}

void Set_Cursor(uint16_t x, uint16_t y)
{
	X_cursor=x;
	Y_cursor=y;
}


void Draw_Cursor (FONT_t* Font, uint16_t x, uint16_t y )
{
	Draw_Line(x, y, x+Font->Width, y);
}


void Print_Str(char* str, FONT_t* Font, uint16_t x, uint16_t y, uint8_t invert_state)
{
		//strcpy(str, _codingCP866(str));
		if(Font->lang == RUS)
		{
			char buf_str[strlen(str)];// = malloc(strlen(str));
			strcpy(buf_str,str);
			char* ru_str = _codingCP866(buf_str);
			while (*ru_str!='\0')
			{
				Print_Char(x, y, *ru_str, Font, invert_state);
				x+=Font->Width;
				ru_str++;
				//Strobe_Update(Frame_Buf);
			}
		}
		else
		{
			while (*str!='\0')
			{
				Print_Char(x, y, *str, Font, invert_state);
				x+=Font->Width;
				str++;
				//Strobe_Update(Frame_Buf);
			}
		}
	X_cursor = x;
}

void Print_Char(uint16_t x, uint16_t y, unsigned char c,FONT_t* Font,uint8_t invert_state)
{

	if((Font->bitsize) == 8)
	{
		//uint8_t* font_ptr_casted = (uint8_t*) Font->font_ptr_16 ;
		Print_Char_8(x,y,c,Font->font_ptr_8, Font->Height, Font->Width, invert_state);
	}
	else if ((Font->bitsize) == 16)
	{
		uint16_t* font_ptr_casted = (uint16_t*) Font->font_ptr_8 ;
		Print_Char_16(x, y, c, font_ptr_casted, Font->Height, Font->Width, invert_state);
	}

}

void Print_Char_8(uint16_t x, uint16_t y, unsigned char c, const uint8_t* font_ptr, uint8_t height, uint8_t width,uint8_t invert_state)
{
	uint8_t i,j;
	uint8_t line;
//ветвим инверсию до захода в цикл по причине требований ко времени ответа
	if(!invert_state)
	{
		for(i=0;i<width;i++)
		{
			line = *(const unsigned char *)(&font_ptr[c*width + i]);
			for (j=0; j<height; j++, line>>=1)
			{
				if (line & 0x01)
				//if (!(line & 0x01))
				{
					//Draw_Pixel((y+i), (x+j));
					Draw_Pixel((x+i),(y+j));

				}

			}
		}
	}
	else
	{
		for(i=0;i<width;i++)
				{
					line = *(const unsigned char *)(&font_ptr[c*width + i]);
					for (j=0; j<height; j++, line>>=1)
					{
						//if (line & 0x01)
						if (!(line & 0x01))
						{
							//Draw_Pixel((y+i), (x+j));
							Draw_Pixel((x+i),(y+j));

						}

					}
				}

	}
}




void Print_Char_16(uint16_t x, uint16_t y,unsigned char charac, uint16_t *font_ptr, uint16_t height, uint16_t width, uint8_t invert_state)
{
	uint32_t i,j;
	uint32_t character_offset;

	if(!invert_state)
	{
		for(i=0;i<height;i++)
		{
			character_offset  = (font_ptr[(charac-32) * height + i]);
			for (j=0; j<width; j++)
			{
				//if (*ptr & (0x80 >> (j % 8)))
				if((character_offset << j) & 0x8000 )
				{
					Draw_Pixel((x+j), (y+i));

				}

			}

		}
	}
	else
	{
		for(i=0;i<height;i++)
				{
					character_offset  = (font_ptr[(charac-32) * height + i]);
					for (j=0; j<width; j++)
					{
						if(!((character_offset << j) & 0x8000 ))
						{
							Draw_Pixel((x+j), (y+i));
						}
					}

				}

	}

}


void OLED_Fill(uint8_t choose_col)
{
  memset(Frame_Buf, choose_col, sizeof(Frame_Buf));
}



void Draw_Line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
	uint16_t i;
	uint8_t /*dx, dy,*/ tilt;
	tilt = abs(y1 - y0) > abs(x1 - x0);
	if (x0 >= OLED_WIDTH)
		{
			//uint16_t _x0=x0;
			x0 = x0 - OLED_WIDTH;
		}
	if (x1 >= OLED_WIDTH)
		{
			//uint16_t _x1=x1;
			x1 = x1 - OLED_WIDTH;
		}
	if (y0 >= OLED_HEIGHT)
		{
			y0 = y0 - OLED_HEIGHT;
		}
	if (y1 >= OLED_HEIGHT)
		{
			y1 =y1 - OLED_HEIGHT;
		}
//___________________________


	  if (tilt)
	  {
		  swap(x0,y0);
		  swap(x1, y1);
	  }
	  if (x0 > x1)
	  {
	    swap(x0, x1);
	    swap(y0, y1);
	  }

	 // dx = x1-x0;
	 // dy = abs(y1-y0);

	  for(i = x0; i <= x1; i++)
	  {
		  if(tilt)
		  {
			  //drawpix(i,y0);
			  Draw_Pixel(i,y0);
			  //Strobe_Update(Frame_Buf);
		  }
		  else
		  {
			  //drawpix(i,x0);
			  //Draw_Pixel(i,x0);
			  Draw_Pixel(x0,i);
			  //Strobe_Update(Frame_Buf);
		  }
	  }
}



uint8_t LO[1024] = {
	// Offset 0x00000000 to 0x000008A7
		0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
		0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
		0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
		0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
		0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
		0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
		0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
		0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
		0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X0E,0X0E,0X0E,0X0E,0X00,0X0E,0X0E,0X00,0X0E,
		0X0E,0X00,0X00,0X1E,0X0C,0X0C,0X1E,0X0C,0X0C,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
		0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
		0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
		0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
		0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
		0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
		0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
		0X00,0X0C,0X0C,0X00,0X1F,0X3F,0X70,0XE0,0XE0,0XE0,0XE0,0XE0,0XE0,0XE0,0XE0,0XE0,
		0XE0,0XE0,0XE0,0XE0,0XE0,0XE0,0XE0,0XE0,0X60,0X3F,0X1F,0X1F,0X1F,0X00,0X0C,0X0C,
		0X00,0X00,0X00,0X08,0X0F,0X00,0X0F,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X0F,
		0X0F,0X00,0X0F,0X00,0X00,0X00,0X0F,0X00,0X0F,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
		0X00,0X00,0X0F,0X00,0X0F,0X00,0X00,0X0F,0X00,0X0F,0X0F,0X00,0X00,0X00,0X00,0X00,
		0X00,0X00,0X00,0X0F,0X00,0X0F,0X0F,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
		0X0F,0X0F,0X0F,0X00,0X0F,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0X0F,
		0X0F,0X0F,0X0F,0X0F,0X0F,0X0F,0X0F,0X0F,0X0F,0X0F,0X0F,0X0F,0X0F,0X0F,0X00,0X00,
		0X00,0X55,0X55,0X00,0XFF,0XFF,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
		0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XFF,0XFF,0XFF,0XFF,0X00,0X55,0X55,
		0X00,0X00,0X00,0X00,0XFF,0X00,0XFE,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0XFF,
		0XFC,0X00,0XFF,0X00,0X00,0X00,0XFF,0X00,0XFF,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
		0X00,0X00,0XFF,0X00,0XFF,0X00,0X00,0XFF,0X00,0XFF,0XFF,0X03,0X03,0X03,0X03,0X03,
		0X03,0X03,0X03,0XFE,0X00,0XFF,0XFE,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
		0XFF,0XFF,0XFF,0X00,0XFF,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X3F,0X60,0XC0,0X9F,
		0XB0,0XB0,0XB0,0XB0,0XB0,0XB0,0XB0,0X9F,0X80,0X80,0X9F,0X9F,0XB0,0XB0,0X00,0X00,
		0X00,0X65,0X65,0X00,0XFF,0XFF,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
		0X00,0X00,0X00,0X03,0X07,0X03,0X00,0X00,0X00,0XFF,0XFF,0XFF,0XFF,0X00,0X65,0X65,
		0X00,0X00,0X00,0X00,0XFF,0X00,0X17,0X38,0X38,0X38,0X38,0X78,0X38,0X78,0X78,0X1F,
		0X06,0X00,0XFF,0X00,0X00,0X00,0XFF,0X00,0XFF,0X01,0X00,0X00,0X00,0X00,0X00,0X00,
		0X00,0X01,0XFF,0X00,0XFF,0X00,0X00,0XFF,0X00,0X1F,0X1F,0X18,0X18,0X18,0X18,0X18,
		0X18,0X18,0X18,0X07,0XE0,0XFF,0X1F,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,
		0XFF,0XFF,0XFF,0X00,0XFF,0X01,0X00,0X00,0X00,0X00,0X00,0X00,0XFF,0X00,0X00,0XFF,
		0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XFF,0X00,0X00,0XFF,0XFF,0X00,0X00,0X00,0X00,
		0X00,0X5C,0X5C,0X00,0XFC,0XFF,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,
		0X03,0X03,0X03,0X83,0X83,0X83,0X03,0X03,0X03,0XFF,0XFC,0XFC,0XFC,0X00,0X5C,0X5C,
		0X00,0X00,0X00,0X10,0XFC,0X00,0XFC,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XFC,
		0X38,0X00,0XFC,0X00,0X00,0X00,0XFC,0X1C,0X9C,0X9C,0X9C,0X9C,0X9C,0X9C,0X9C,0X9C,
		0X9C,0X9C,0X38,0X60,0XC0,0X00,0X00,0XFC,0X00,0XF8,0XF8,0X00,0X00,0X00,0X00,0X00,
		0X00,0X00,0X00,0XFC,0X00,0XFC,0XF8,0X00,0X00,0X00,0X9C,0X9C,0X9C,0X9C,0X9C,0X9C,
		0X1C,0X1C,0X1C,0X1C,0X1C,0X9C,0X9C,0X9C,0X9C,0X9C,0X9C,0X00,0XFC,0X00,0X00,0XFC,
		0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XFC,0X00,0X00,0XFC,0XFC,0X00,0X00,0X00,0X00,
		0X00,0X00,0X00,0X00,0X00,0XC0,0XE0,0XE7,0XE7,0XE7,0XE7,0XE7,0XE7,0XE7,0XE0,0XE7,
		0XE7,0XE0,0XE0,0XE7,0XE7,0XE7,0XE7,0XE7,0XE7,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,
		0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
		0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
		0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
		0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
		0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
		0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
		0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
		0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
		0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
		0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
		0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
		0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
		0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
		0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
		};
uint8_t LO2[1024] = {
		0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
		0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
		0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
		0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
		0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
		0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
		0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
		0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
		0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFB,0XE5,0XF1,0XF5,0XE1,0XFF,0XF1,0XE1,0XFF,0XF1,
		0XE5,0XFB,0XF7,0XF1,0XF5,0XEB,0XF1,0XEB,0XF5,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
		0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
		0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
		0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
		0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
		0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
		0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
		0XFF,0XF3,0XF2,0XFF,0XE0,0XC0,0X1F,0X9F,0X1E,0X9F,0X1F,0X9A,0X1F,0X9E,0X1F,0X9D,
		0X1F,0X9E,0X1F,0X8E,0X1F,0X9D,0X1F,0X9D,0X0F,0XC0,0XE0,0XF0,0XE0,0XFF,0XF3,0XF2,
		0XFF,0XFF,0XFF,0XF2,0XF4,0XFF,0XF0,0XFB,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,
		0XFA,0XFF,0XE0,0XFF,0XF7,0XFD,0XF0,0XFF,0XF0,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
		0XFF,0XF5,0XF0,0XFF,0XF0,0XFD,0XFF,0XF0,0XFF,0XF8,0XF2,0XFF,0XFF,0XFF,0XFF,0XFF,
		0XFF,0XFF,0XFD,0XF0,0XFF,0XF0,0XF5,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF5,
		0XF0,0XF0,0XF0,0XFF,0XF0,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF4,0XF0,
		0XF2,0XF1,0XF2,0XF1,0XF2,0XF0,0XF3,0XF0,0XF2,0XF1,0XF2,0XF0,0XF1,0XF2,0XFF,0XFF,
		0XFF,0XAA,0XAA,0XFF,0X00,0X00,0XFF,0XFF,0XD5,0X7F,0XDD,0XF7,0XBE,0XEB,0XBF,0XF5,
		0X5F,0XFA,0XEF,0XBD,0XF7,0XBD,0X6F,0XFB,0XFF,0X00,0X00,0X00,0X00,0XFF,0XAA,0XAA,
		0XFF,0XFF,0XFF,0X4A,0X91,0XFF,0X01,0XFC,0XFE,0XFC,0XFE,0XFC,0XFC,0XFE,0XFC,0X08,
		0X53,0XFF,0X00,0XFF,0X77,0XDD,0X00,0XFF,0X00,0XBD,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
		0XFF,0X55,0X88,0XFF,0X00,0XDD,0XFF,0X00,0XFF,0X90,0X26,0XFC,0XFE,0XFC,0XFE,0XFC,
		0XFE,0XFC,0XDC,0X01,0XFF,0X00,0XB5,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X55,
		0X00,0X22,0X00,0XFF,0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XC0,0X8F,0X35,0X60,
		0XCF,0X4F,0X4F,0XCF,0X4F,0XCF,0X4F,0X60,0XF7,0X7D,0X60,0XC0,0X4F,0XCF,0XFF,0XFF,
		0XFF,0X9A,0X8A,0XFF,0X00,0X00,0XFF,0XFF,0XAE,0XFB,0X56,0XFF,0XBB,0XEF,0X7A,0XDF,
		0XB6,0XFF,0XD5,0X7C,0XD8,0XFC,0X57,0XFF,0XFF,0X00,0X00,0X00,0X00,0XFF,0X9A,0X8A,
		0XFF,0XFF,0XFF,0XA9,0X12,0XFF,0XE8,0XA3,0XC7,0XA7,0XA7,0XA3,0XE7,0X87,0XE7,0XA0,
		0XFE,0XFF,0X00,0XFF,0X5D,0XF7,0X00,0XFF,0X00,0XEF,0XFD,0XFF,0XFF,0XFF,0XFF,0XFF,
		0XFE,0X55,0X20,0XFF,0X00,0XDE,0XFF,0X00,0XFF,0XA9,0XA2,0XE7,0X83,0XF7,0X83,0XE7,
		0XA3,0XD7,0XA3,0XD8,0X1F,0X40,0XF5,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFF,0XFE,0X55,
		0X00,0X10,0X00,0XFF,0X00,0XFE,0XFF,0XFF,0XFE,0XFF,0XFF,0XFF,0X00,0XFF,0X55,0X00,
		0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X00,0XDD,0X77,0X00,0X00,0XFF,0XFF,0XFF,0XFF,
		0XFF,0XAB,0X23,0XFF,0X03,0X00,0XFC,0XFD,0XBC,0XEF,0XBC,0XED,0X7E,0XAD,0XFC,0XEF,
		0XBC,0XED,0XFE,0X5D,0X2C,0X7F,0XEC,0X7D,0XFC,0X00,0X03,0X00,0X03,0XFF,0XAB,0X23,
		0XFF,0XFF,0XFF,0X57,0X23,0XFF,0X03,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X23,
		0XCF,0XFF,0X03,0XFF,0X6F,0XF7,0X03,0XF3,0X67,0X23,0X6B,0X63,0X27,0X63,0X33,0X67,
		0X63,0X53,0XC7,0X9F,0X7F,0XFF,0XFF,0X03,0XFF,0X27,0X4B,0XFF,0XFF,0XFF,0XFF,0XFF,
		0XFF,0XFF,0X6F,0X03,0XFF,0X03,0XB7,0XFF,0XFF,0XFF,0X63,0X63,0X23,0X67,0X63,0X63,
		0XE3,0XE7,0XE3,0XE3,0XE3,0X67,0X63,0X63,0X63,0X63,0X67,0XFF,0X03,0XFF,0X57,0X03,
		0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X03,0XDF,0X77,0X03,0X03,0XFF,0XFF,0XFF,0XFF,
		0XFF,0XFF,0XFF,0XFF,0XFF,0XBF,0X1D,0X1A,0X18,0X14,0X18,0X1B,0X1C,0X10,0X1F,0X18,
		0X18,0X1F,0X1A,0X18,0X1D,0X1A,0X18,0X0D,0X1A,0XBF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
		0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
		0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
		0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
		0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
		0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
		0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
		0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
		0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
		0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
		0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
		0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
		0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
		0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
		0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
		};

