#ifndef MAIN_H_
#define MAIN_H_
#define SEC(n)	(n*10*51200U)

#define Bit(n)	(1U << (n))
#ifndef BIT
#define BIT(n)	(1U << (n))
#endif
#define bit(n)	(1U << (n))

enum BOOLEAN {false,true};

	#define A0 bit(0)
	#define RW bit(1)
	#define E bit(2)
	#define RES bit(3)
	#define CS bit(4)

void uDelay(const uint32_t usec);
//void Buttons(void);
//void Menu_Display();
#endif /* MAIN_H_ */
