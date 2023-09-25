
#include <stdint.h>
#include "snake.h"
#include <stdio.h>
#include "reg_adr.h"
#include "Kmx32.h"
#include "reg_def.h"
#include "main.h"
#include "oled.h"
#include "font16.h"
#include <time.h>
#include <stdlib.h>

#define RIGHT 0;
#define LEFT  1;
#define UP    2;
#define DOWN  3;
#define QUIT  4;


void Game_Init(GAME_PARAMS_t *game)
{
	game->direction = RIGHT;
	game->Score = 0;
	game->Food = (Food*) malloc(sizeof(Food));
	Snake *snak = (Snake*) malloc(sizeof(Snake));
	snak->prev=NULL;
	snak->X_coord = 4;
	snak->Y_coord = 10;
	game->SnakeHead = game->SnakeTail = snak;
	AddNewPartToSnake(game);
	AddNewPartToSnake(game);
	AddNewPartToSnake(game);
	AddNewPartToSnake(game);
	SetFoodCoords(game);
}


uint8_t GPIO_DirectionController(uint8_t dir)
{
			//uint8_t dir;
			uint16_t tmp;
			uDelay(20000);
			WRITE_REG(AHB_P1_DIR, 0xFFFF);
			WRITE_REG(AHB_P1_OUT, 0x0000);
			WRITE_REG(AHB_P1_DIR, 0x0000);

			READ_REG(AHB_P1_IN, &tmp);

			if(!(tmp & 0x800))
				{dir = RIGHT;}		//RIGHT
			else if((tmp & 0x2000) && (tmp & 0x800))
				{dir = LEFT;}		//LEFT
			else if((tmp & 0x4000) && (tmp & 0x800))
				{dir = UP;}		//UP
			else if((tmp & 0x1000) && (tmp & 0x800))
				{dir = DOWN;}		//DOWN
			else if ((tmp & 0x20) && (tmp & 0x800))
				{dir = QUIT;}

		return dir;

}

void AddNewPartToSnake(GAME_PARAMS_t *game)
{
	Snake *NewPart = (Snake *) malloc(sizeof(Snake));
	NewPart->prev = game->SnakeTail;
    NewPart->X_coord = NewPart->prev->X_coord;
    NewPart->Y_coord = NewPart->prev->Y_coord;
    NewPart->next = NULL;
    game->SnakeTail->next = NewPart;
    game->SnakeTail = NewPart;
}

void SetFoodCoords(GAME_PARAMS_t *game)
{
	game->Food->X_coord=rand() % 132;
	game->Food->Y_coord=rand() % 64;

	Snake *snake = game->SnakeHead;
	while(snake)
	{
		if(game->Food->X_coord == snake->X_coord &&
		   game->Food->Y_coord == snake->Y_coord	)
		{
			SetFoodCoords(game);
		}
		break;
		snake = snake->next;
	}
}

uint8_t CheckCollision(GAME_PARAMS_t *game)
{
	Snake *snake = game->SnakeHead->next;
	while(snake)
	{
		if(snake->X_coord == game->SnakeHead->X_coord &&
		   snake->Y_coord == game->SnakeHead->Y_coord    )
		{
			return true;
		}
		snake = snake->next;
	}
	return 0;
}

void Destroy(GAME_PARAMS_t *game)
{
	if(game)
	{
		if(game->SnakeHead)
		{
			Snake *sna, *tmp;
			sna = game->SnakeHead;
			while(sna)
			{
				tmp = sna->next;
				free(sna);
				sna=tmp;
			}
		}
		if(game->Food)
			free(game->Food);
		free(game);
	}
	//free(scorebuf);
}
void Engine(GAME_PARAMS_t *game)
{
	Snake *Current_Snake = game->SnakeTail;
	while(Current_Snake != game->SnakeHead)
	{
		Current_Snake->X_coord = Current_Snake->prev->X_coord;
		Current_Snake->Y_coord = Current_Snake->prev->Y_coord;
		Current_Snake = Current_Snake->prev;
	}
	game->KeyState=0;


	switch(game->direction)
	{
		case 0: //RIGHT
		{
			Current_Snake->X_coord++;
			break;
		}
		case 1:
		{
			Current_Snake->X_coord--;
			break;
		}
		case 2:
		{
			Current_Snake->Y_coord--;
			break;
		}
		case 3:
		{
			Current_Snake->Y_coord++;
			break;
		}
		case 4:
		{

			OLED_Fill(0x00);
			Print_Str("Выход",&SmallFontRus_t, 56, 32, INVERTED);
			Strobe_Update(Frame_Buf);
			//Destroy(game);
			break;
		}
		default:break;
	}


	if (Current_Snake->X_coord == 255)
		Current_Snake->X_coord = 131;
    if(Current_Snake->X_coord > 132 && Current_Snake->X_coord !=255 )
        Current_Snake->X_coord = 0;
    if(Current_Snake->Y_coord > 64 && Current_Snake->Y_coord !=255 )
        Current_Snake->Y_coord = 0;
    if(Current_Snake->Y_coord == 255)
        Current_Snake->Y_coord = 63;

    if(CheckCollision(game))
    {
    	Destroy(game);
    	//
    }
    if(Current_Snake->X_coord == game->Food->X_coord &&
       Current_Snake->Y_coord == game->Food->Y_coord)
    {
    	AddNewPartToSnake(game);
    	SetFoodCoords(game);
    	game->Score++;
    }
    OLED_Fill(0x00);
    //Borders();
    sprintf(scorebuf, "%d", game->Score);
    Print_Str(scorebuf,&SmallFontRus_t, 2, 2, NORMAL);
    Print_Str("SNAKE",&SmallFontRus_t, 50, 24, INVERTED);

    if(game->direction == 4)
    {

		OLED_Fill(0x00);
		Print_Str("Выход",&SmallFontRus_t, 56, 32, INVERTED);
		Strobe_Update(Frame_Buf);
		Destroy(game);
    }
}

void SnaketoLED(GAME_PARAMS_t* game)
{
	Snake *snake = game->SnakeTail;
	while(snake!= game->SnakeHead)
	{
		Draw_Pixel(snake->X_coord, snake->Y_coord);
		snake=snake->prev;
	}

	Draw_Pixel(game->Food->X_coord, game->Food->Y_coord);

	Strobe_Update(Frame_Buf);
}

void SnakeMain()
{
	uint8_t dirbuf;
	GAME_PARAMS_t *game = (GAME_PARAMS_t*)malloc(sizeof(GAME_PARAMS_t));
	Game_Init(game);
	while(game)
	{
	dirbuf = GPIO_DirectionController(game->direction);
	if(!DirectionProtect(dirbuf, game->direction ))
	{
		game->direction = dirbuf;
	}
	Engine(game);
	SnaketoLED(game);
	}
}

void Borders()
{
	uint8_t i;
	for(i=0;i<132;i++)
	{
		Draw_Pixel(i,4);
		Draw_Pixel(i,60);
		if(i<65)
		{
			Draw_Pixel(4,i);
			Draw_Pixel(132,i);
		}
	}
	Print_Str("SNAKE",&SmallFontRus_t, 50, 24, INVERTED);
}

uint8_t DirectionProtect(uint8_t new_dir,uint8_t prev_dir)
{
	return(new_dir < 4 && (abs(new_dir^prev_dir)==1));
		 //1;
}
