/*
 * menu.c
 *
 *  Created on: 14.09.2022
 *      Author: Artyom
 */


#include <stdio.h>
#include "reg_adr.h"
#include "Kmx32.h"
#include "reg_def.h"
#include "menu.h"
#include "main.h"
#include "oled.h"
#include "font16.h"
#include "snake.h"
/*char Pos_0[] = "�����������";
char Pos_1[] = "������������";
char Pos_2[] = "����";
char Pos_3[] = "����";
char Pos_4[] = "�����";
char Pos_5[] = "���� � �����";
char Pos_6[] = "� ���";
char Pos_7[] = "����� ���������";*/
#define item_len 16
enum BOOLEAN CHOSEN;
//enum main_menu_items {Connection, Security, Themes, Sound, Res, Date_and_Time, About, Artyom};
/*char* main_item_names[] = {
	"�����������    ",
	"������������   ",
	"����           ",
	"����           ",
	"�����          ",
	"���� � �����   ",
	"� ���          ",
	"����� ���������"
};*/
//char* uncasted_rus_items ="�����������    ������������   ����           ����           �����          ���� � �����   � ���          ����� ���������";
//char* it0 = memcpy(it0,_codingCP866(uncasted_rus_items),);
MENU_t Main_Menu_t =
{
	0,
	{
		"�����������    ",
		"����� ������   ",
		"����           ",
		"����           ",
		"�����          ",
		"���� � �����   ",
		"� ���          ",
		"������         "
	}
	//{Connection, Security, Themes, Sound, Res, Date_and_Time, About, Artyom}
	//{Connection, Security, Themes, Sound, Res, Date_and_Time, About, Artyom}

};

MENU_t Connection_Menu_t =
{
	1,
	{
		"�������� 1     ",
		"�������� 2     ",
		"�������� 3     ",
		"�������� 4     ",
		"�������� 5     ",
		"�������� 6     ",
		"�������� 7     ",
		"�������� 8     "
	}
	//{Connection, Security, Themes, Sound, Res, Date_and_Time, About, Artyom}
	//{Connection, Security, Themes, Sound, Res, Date_and_Time, About, Artyom}

};

//void MenuCurrentStateDisplayer(/*FONT_t* Font*/MENU_t* Current_Menu,uint8_t in_menu_cursor)
void MenuCurrentStateDisplayer(MENU_PARAMS_t* _MENU_PARAMS)
{
	uint8_t t=0;
	//OLED_Clear();
	OLED_Fill(0x00);
	//Strobe_Update(Frame_Buf);
	//Strobe_Update(Frame_Buf);
	//in_menu_cursor%=8;
	_MENU_PARAMS->menu_cursor%=8;
	for(t=0;t<8;t++)
	{
		Set_Cursor(2,t*8);
		//Print_Str(Current_Menu->items[t],/*&font_default_t*/&SmallFontRus_t, X_cursor, Y_cursor, (t==in_menu_cursor)); //t == in_menu_cursor ? INVERTED : NORMAL
		Print_Str(_MENU_PARAMS->ptr_Current_Menu->items[t],&SmallFontRus_t, X_cursor, Y_cursor, (t==_MENU_PARAMS->menu_cursor)); //t == in_menu_cursor ? INVERTED : NORMAL
	}



	Strobe_Update(Frame_Buf);
	//OLED_Fill(0x00);
}

MENU_PARAMS_t ScreenDisplayer(MENU_PARAMS_t* __MENU_PARAMS)
{
	//MENU_t ptr = &Main_Menu_t;
	//MENU_PARAMS_t PARAMS;
	//__MENU_PARAMS->ptr_Current_Menu= &Main_Menu_t;
	//switch(__MENU_PARAMS->ptr_Current_Menu)
	//__MENU_PARAMS->menu_id+=1;
	switch(__MENU_PARAMS->menu_id)
	{
		case 0://&Main_Menu_t:
		{
			__MENU_PARAMS->ptr_Current_Menu= &Main_Menu_t;

			switch(__MENU_PARAMS->menu_cursor)
			{
				case 0:
				{
					//ptr = &Connection_Menu_t;
					__MENU_PARAMS->ptr_Current_Menu = &Connection_Menu_t;
					__MENU_PARAMS->menu_id=1;
					__MENU_PARAMS->menu_cursor=0;
					MenuCurrentStateDisplayer(__MENU_PARAMS);
					break;
				}
				case 1:
				{
					__MENU_PARAMS->menu_id=8;
					SelectChannelScreen(__MENU_PARAMS->channel);
					Strobe_Update(Frame_Buf);
					break;
				}
				case 2:
					break;
				case 3:break;
				case 4:break;
				case 5:break;
				case 6:
				{
					About();
					break;
				}
				case 7:{SnakeMain();break;}

			}
			break;
		}
		case 8://Channel Selection Screen
		{
			__MENU_PARAMS->menu_id=8;
			SelectChannelScreen(__MENU_PARAMS->channel);
			Strobe_Update(Frame_Buf);
			break;
		}
		default: break;
	}

	return *__MENU_PARAMS;
}

/*union un_channel
{
	float number;
	char string[7];
};*/

void SelectChannelScreen(/*un_channel*/float num/*, char buf[7]*/)
{

	char buf[7];
	OLED_Fill(0x00);
	Set_Cursor(2,0);
	Print_Str("�������� �����������",&SmallFontRus_t, X_cursor, Y_cursor, NORMAL);
	Set_Cursor(2,8);
	Print_Str("�����:",&SmallFontRus_t, X_cursor, Y_cursor, INVERTED);
	sprintf(buf, "%3.3f", num);
	Print_Str(buf,&SmallFontRus_t, X_cursor, Y_cursor, INVERTED);
	Strobe_Update(Frame_Buf);
	//sprintf(buffer, "%3.3f", Number);
}

void About(void)
{
	OLED_Fill(0x00);
	Set_Cursor(2,0);
	Print_Str("�����:",&SmallFontRus_t, X_cursor, Y_cursor, INVERTED);
	Set_Cursor(2,8);
	Print_Str("124498,����������",&SmallFontRus_t, X_cursor, Y_cursor, NORMAL);
	Set_Cursor(2,16);
	Print_Str("��.������,�.1,���.7",&SmallFontRus_t, X_cursor, Y_cursor, NORMAL);
	Set_Cursor(2,24);
	Print_Str("����.7237,",&SmallFontRus_t, X_cursor, Y_cursor, NORMAL);
	//Print_Str("������-���������������� �������� ��������������� �����",&SmallFontRus_t, X_cursor, Y_cursor, NORMAL);
	Set_Cursor(2,32);
	Print_Str("��������:",&SmallFontRus_t, X_cursor, Y_cursor, INVERTED);
	Set_Cursor(2,40);
	Print_Str("+ 7 (499) 734-45-21",&SmallFontRus_t, X_cursor, Y_cursor, NORMAL);
	Set_Cursor(2,48);
	Print_Str("tc@tcen.ru",&SmallFontRus_t, X_cursor, Y_cursor, NORMAL);
}
/*struct MENU_t All_Menus[1] =
{
	{

	}
	Connection_Menu_t,
};*/




