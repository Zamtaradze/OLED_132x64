
#ifndef MENU_H_
#define MENU_H_


typedef struct
{
	uint8_t menu_id;
	//TODO: добавить тип экрана
	char* items[];
}MENU_t ;


typedef struct
{
	uint8_t menu_cursor;
	MENU_t* ptr_Current_Menu;
	uint8_t menu_id;
	float channel;
}MENU_PARAMS_t;



MENU_PARAMS_t MENU_PARAMS;

MENU_t Main_Menu_t;
MENU_t Connection_Menu_t;

void MenuCurrentStateDisplayer(MENU_PARAMS_t* _MENU_PARAMS);
MENU_PARAMS_t ScreenDisplayer(MENU_PARAMS_t* __MENU_PARAMS);
void SelectChannelScreen(/*un_channel*/float num/*, char buf[7]*/);
void About(void);


#endif /* MENU_H_ */
