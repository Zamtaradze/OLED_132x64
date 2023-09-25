
#ifndef SNAKE_H_
#define SNAKE_H_

typedef struct Snake Snake;
typedef struct Food Food;

enum BOOLEAN bool;

typedef struct Snake Snake;
typedef struct Food Food;

enum BOOLEAN bool;


struct Snake
{
	uint8_t X_coord;
	uint8_t Y_coord;
	//uint8_t Size;
	Snake *next;
	Snake *prev;
};

struct Food
{
	uint8_t X_coord;
	uint8_t Y_coord;
};


typedef struct
{
	Snake *SnakeHead;
	Snake *SnakeTail;
	Food  *Food;
	uint8_t Score;
	uint8_t direction;
	uint8_t KeyState;

}GAME_PARAMS_t;

char scorebuf[];

void Game_Init(GAME_PARAMS_t *game);
uint8_t GPIO_DirectionController(uint8_t dir);
void AddNewPartToSnake(GAME_PARAMS_t *game);
void SetFoodCoords(GAME_PARAMS_t *game);
uint8_t CheckCollision(GAME_PARAMS_t *game);
void Destroy(GAME_PARAMS_t *game);
void Engine(GAME_PARAMS_t *game);
void SnaketoLED(GAME_PARAMS_t *game);
void SnakeMain();
void Borders();
uint8_t DirectionProtect(uint8_t new_dir,uint8_t prev_dir);
#endif /* SNAKE_H_ */
