
#ifndef OLED_H_
#define OLED_H_
//#define drawpix(x,y) Frame_Buf[y + (x / 8) * 128] |= (1 << (y & 7));
//#define drawpix(x,y) Frame_Buf[x + (y / 8) * 128] |= 1 << (y % 8);

//#define Draw_Pixel(x,y) Frame_Buf[x + (y / 8) * 128] |= 1 << (y % 8);

uint16_t X_cursor;
uint16_t Y_cursor;

#define OLED_WIDTH 132
#define OLED_HEIGHT 64

#define RUS 1
#define ENG 0

#define NORMAL 0
#define INVERTED 1

#define Draw_Pixel(x,y) Frame_Buf[x + (y / 8) * OLED_WIDTH] |= (1 << (y % 8));

typedef struct
{
	uint8_t Height;
	uint8_t Width;
	uint8_t bitsize;
	//uint8_t *font_ptr_8;
	const uint8_t* font_ptr_8;
	uint8_t lang;
} FONT_t;
//FONT_t Font11x18_t;
FONT_t font_default_t;
FONT_t SmallFontRus_t;
char*	_codingCP866(char* StrIn/*, uint8_t codingName*/);
void Reset();

uint8_t Frame_Buf[1056];
void OLED_Config(void);
void OLED_Init(void);
void OLED_Clear(void);
void Fplotter(void);
void OLED_Fill(uint8_t choose_col);
void Update(uint8_t *Image);
void Strobe_Update(uint8_t *Image);
void Picture_Update(uint8_t *Image);
//__________________________________
void Fill_Display(uint8_t choose_col);
void Draw_Line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

void Set_Cursor(uint16_t x, uint16_t y);
void Print_Char_8(uint16_t x, uint16_t y, unsigned char c, const uint8_t* font_ptr, uint8_t height, uint8_t width,uint8_t invert_state);
void Print_Char_16(uint16_t x, uint16_t y,unsigned char charac, uint16_t* font_ptr, uint16_t height, uint16_t width,uint8_t invert_state);
void Print_Char(uint16_t x, uint16_t y, unsigned char c,FONT_t* Font,uint8_t invert_state);
void Print_Str(char* str, FONT_t* Font, uint16_t x, uint16_t y, uint8_t invert_state);

void Draw_Cursor (FONT_t* Font, uint16_t x, uint16_t y );
uint8_t gImage_1in3[1056] ;
uint8_t LO[1024];
uint8_t LO2[1024];
uint8_t tcen[1024];


#endif /* OLED_H_ */
