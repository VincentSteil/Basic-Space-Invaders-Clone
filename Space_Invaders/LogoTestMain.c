// LogoTestMain.c
// Runs on LM3S1968 or LM3S8962
// Jonathan Valvano
// November 12, 2012

/* This example accompanies the books
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2012
   
   "Embedded Systems: Introduction to Arm Cortex M Microcontrollers",
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2012
 Copyright 2012 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */
#include "hw_types.h"
#include "sysctl.h"
#include "lm3s1968.h"
#include "rit128x96x4.h"
#include "logo.h"
#include "SpaceInvaders.h"
#include "ADCSWTrigger.h"



// which delays 3*ulCount cycles
#ifdef __TI_COMPILER_VERSION__

	//Code Composer Studio Code
	void Delay(unsigned long ulCount){
	__asm (	"    subs    r0, #1\n"
			"    bne     Delay\n"
			"    bx      lr\n");
}

#else
	//Keil uVision Code
	__asm void
	Delay(unsigned long ulCount)
	{
    subs    r0, #1
    bne     Delay
    bx      lr
	}

#endif

/*
struct State {
	unsigned long x;						// x coordinate
	unsigned long y;						// y coordinate
	const unsigned char *image;	// pointer to image
	long life;									// 0=dead, 1=alive (or display)
};
typedef struct State STyp;


STyp Enemy[6][8];							// 6 by 8 matrix of enemies ([x][y])
void Enemy_Init(void){				// initialize the 6x8 array of enemies
	int i;
	int j;
	for(j=0; j<8; j++){
		for(i=0; i<6; i++){
			Enemy[i][j].x = 20*i;
			Enemy[i][j].y = 10*j + 10;
			Enemy[i][j].image = SmallEnemy30PointA;
			Enemy[i][j].life = 0;
		}
	}
}
void Enemy_Display(void){			// display all enemies that are alive
	
	int i;
	int j;
	for(j=0; j<8; j++){
		for(i=0; i<6; i++){
			if(Enemy[i][j].life){
				RIT128x96x4_BMP(Enemy[i][j].x, Enemy[i][j].y, Enemy[i][j].image);
			}
			else{
				RIT128x96x4_BMP(Enemy[i][j].x, Enemy[i][j].y, SmallEnemy30PointAClear);
			}
		}
	}
}


STyp Player;									// initialize player ship													
void Player_Init(void){
	Player.x = (ADC_InSeq3()*109/1023);
	Player.y = 0;
	Player.image = PlayerShip0;
	Player.life = 1;
}
void Player_Display(void){
	Player.x = (ADC_InSeq3()*109/1023);
	RIT128x96x4_BMP(Player.x, Player.y, Player.image);
}


	


volatile unsigned long period = 0;
*/






	STyp Enemy[48]={
		{0,10, SmallEnemy30PointA, 0},
		{20,10, SmallEnemy30PointA, 0},
		{40,10, SmallEnemy30PointA, 0},
		{60,10, SmallEnemy30PointA, 0},
		{80,10, SmallEnemy30PointA, 0},
		{100,10, SmallEnemy30PointA, 0},
		{0,20, SmallEnemy30PointA, 0},
		{20,20, SmallEnemy30PointA, 0},
		{40,20, SmallEnemy30PointA, 0},
		{60,20, SmallEnemy30PointA, 0},
		{80,20, SmallEnemy30PointA, 0},
		{100,20, SmallEnemy30PointA, 0},
		{0,30, SmallEnemy30PointA, 0},
		{20,30, SmallEnemy30PointA, 0},
		{40,30, SmallEnemy30PointA, 0},
		{60,30, SmallEnemy30PointA, 0},
		{80,30, SmallEnemy30PointA, 0},
		{100,30, SmallEnemy30PointA, 0},
		{0,40, SmallEnemy30PointA, 0},
		{20,40, SmallEnemy30PointA, 0},
		{40,40, SmallEnemy30PointA, 0},
		{60,40, SmallEnemy30PointA, 0},
		{80,40, SmallEnemy30PointA, 0},
		{100,40, SmallEnemy30PointA, 0},
		{0,50, SmallEnemy30PointA, 0},
		{20,50, SmallEnemy30PointA, 0},
		{40,50, SmallEnemy30PointA, 0},
		{60,50, SmallEnemy30PointA, 0},
		{80,50, SmallEnemy30PointA, 0},
		{100,50, SmallEnemy30PointA, 0},
		{0,60, SmallEnemy30PointA, 0},
		{20,60, SmallEnemy30PointA, 0},
		{40,60, SmallEnemy30PointA, 0},
		{60,60, SmallEnemy30PointA, 0},
		{80,60, SmallEnemy30PointA, 0},
		{100,60, SmallEnemy30PointA, 0},
		{0,70, SmallEnemy30PointA, 0},
		{20,70, SmallEnemy30PointA, 0},
		{40,70, SmallEnemy30PointA, 0},
		{60,70, SmallEnemy30PointA, 0},
		{80,70, SmallEnemy30PointA, 0},
		{100,70, SmallEnemy30PointA, 0},
		{0,80, SmallEnemy30PointA, 0},
		{20,80, SmallEnemy30PointA, 0},
		{40,80, SmallEnemy30PointA, 0},
		{60,80, SmallEnemy30PointA, 0},
		{80,80, SmallEnemy30PointA, 0},
		{100,80, SmallEnemy30PointA, 0},
	};
	*/

int main(void){
	int i;
	int j;
	SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_8MHZ);
	SysTick_Init(50000);    							// initialize SysTick timer
  EnableInterrupts();
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOG; // activate port G
  Delay(1000000);
  GPIO_PORTG_DIR_R |= 0x04;             // make PG2 out (built-in LED)
	GPIO_PORTG_DIR_R &= 0x07;             // make PG3-7 in PG3=Up	PG4=Down	PG5=Left	PG6=Right, PG7=Select
  GPIO_PORTG_AFSEL_R &= 0x00;           // disable alt func on PG2
  GPIO_PORTG_DEN_R |= 0xFF;             // enable digital I/O on PG2
	RIT128x96x4Init(1000000);
	ADC_InitSWTriggerSeq3(0);							// activate adc on channel 0
	
	//Enemy_Init();
	//Player_Init();
	
	RIT128x96x4_BMP(0, 87, marquee);			// display start screen
	while(GPIO_PORTG_DATA_R & 0xF8){			// wait for button press
	}
	RIT128x96x4Clear();
	
	
	
	for(j=0; j<8; j++){
		for(i=0; i<6; i++){
			//Enemy[i][j].life = 0;
		}
	}
	
	
  
  while(1){
		if(1){
			//Player_Display();
			//Enemy_Display();
			
			
			//period = 0;
		}		
	}
}

void SysTick_Handler(void){
	//period = 1;
}

