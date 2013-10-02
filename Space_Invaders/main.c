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
#include "DAC.h"



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

	
volatile unsigned long period = 0;

volatile unsigned long toggle_enemies = 0;						// counter that dertermines timing of toggle
volatile unsigned long toggle_enemies_bool = 0;					// bool that toggles static enemy movement
volatile unsigned long missile_fly_speed = 125;					// period between moving (redrawing all missiles)
volatile unsigned long missile_fly_timer = 0;					// timer that determines if the period between redraws was reached
volatile unsigned long M=1;
volatile unsigned long missile_creation_rate = 300;				// period between each new missile being created
volatile unsigned long missile_create_timer = 0;				// timer for reaching missile creation period
volatile unsigned long laser_fly_timer = 0;						// timer that dertermines if the period between laser redraws was reached
volatile unsigned long laser_fly_speed = 200;					// period between all lasers being moved and redrawn
volatile unsigned long TimerCount = 0;							// for debugging only
volatile unsigned long shoot_sound = 0;							// bool for playing shoot sound
volatile unsigned long shoot_sound_count = 0;					// shoot array index variable
volatile unsigned long explosion_sound = 0;						// bool for playing explosion sound
volatile unsigned long explosion_sound_count = 0;				// explosion array index variable

struct State {
	unsigned long x;						// x coordinate
	unsigned long y;						// y coordinate
	const unsigned char *image;				// pointer to image
	long life;								// 0=dead, 1=alive (or display)
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
			Enemy[i][j].image = SmallEnemy10PointA;
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
		}
	}
}
void Enemy_Row_Init(int l){			// create new rows of enemies up to row l 
	int i;
	int j;
	for(j=0; j<l; j++){
		for(i=0; i<6; i++){
			Enemy[i][j].life = 1;
		}
	}
}
void Enemy_Toggle(void){					// change sprites to producte moving enemies in a static location
	int i;
	int j;
	for(j=0; j<8; j++){
		for(i=0; i<6; i++){
			if(toggle_enemies_bool){
				Enemy[i][j].image = SmallEnemy10PointA;
			}
			else{
				Enemy[i][j].image = SmallEnemy10PointB;
			}
		}
	}
	toggle_enemies_bool ^= 0x01;
}

STyp Player;									// initialize player ship													
void Player_Init(void){
	Player.x = (ADC_InSeq3()*109/1023);
	Player.y = 95;
	Player.image = PlayerShip0;
	Player.life = 1;
}
void Player_Display(void){
	RIT128x96x4_BMP(Player.x, Player.y, SmallEnemy30PointAClear);
	Player.x = (ADC_InSeq3()*109/1023);
	RIT128x96x4_BMP(Player.x, Player.y, Player.image);
}

STyp Missile[6];							// enemy missiles
void Missile_Init(void){
	int i;
	for(i=0; i<6; i++){				
		Missile[i].x = 20*i + 8;
		Missile[i].y = 0;
		Missile[i].image = Missile0;
		Missile[i].life = 0;
	}
	
}
void Missile_Fly(void){					// move all missiles and check for hit-detection
	int i;
	for(i=0; i<6; i++){
		if(Missile[i].life){
			Missile[i].y++;
			RIT128x96x4_BMP(Missile[i].x, Missile[i].y, Missile[i].image);
			if(Missile[i].y > 97){
				Missile[i].life = 0;
				RIT128x96x4_BMP(Missile[i].x, Missile[i].y, SmallEnemy30PointAClear);
			}
			if(((Missile[i].x < (Player.x + 16))  && (Missile[i].x > (Player.x+2)))&&(Missile[i].y>87)){					//collision detection
				Player.life = 0;
				explosion_sound = 1;
				RIT128x96x4_BMP(Player.x, Player.y, BigExplosion0);
				Delay(1000000);
				RIT128x96x4_BMP(Player.x, Player.y, BigExplosion1);
				RIT128x96x4_BMP(0, 87, marquee);	
				while(1);																																		// you die
			}
											// display missile
		}
	}
}
void Missile_Create(long lane){					// have an enemy in lane (lane) fire a missile
	int i;
	int row = 0;
	Missile[lane].life = 1;
	for(i=0; i<8; i++){
		if(Enemy[lane][i].life){
			row = i;
		}
	}
		Missile[lane].life = 1;
		Missile[lane].y = 10*row + 19;	
}


void Seed(unsigned long n){
	M = n;
}
unsigned long Random(void){
	M = 1664525*M+1013904223;
	return(M);
}

	STyp Laser[18];
	void Laser_Init(void){							// initialize lasers, 3 in each lane
	int i;
	for(i=0; i<18; i++){				
		Laser[i].x = 20*(i%6) + 8;
		Laser[i].y = 87;
		Laser[i].image = Laser0;
		Laser[i].life = 0;
	}
}
void Laser_Fly(void){									// engine that moves the lasers and handles hit-detection
	int i;
	int j;
	for(i=0; i<18; i++){
		if(Laser[i].life){
			Laser[i].y--;
			RIT128x96x4_BMP(Laser[i].x, Laser[i].y, Laser[i].image);
			if(Laser[i].y ==0){
				Laser[i].life = 0;
				Laser[i].y = 87;
				RIT128x96x4_BMP(Laser[i].x, Laser[i].y, SmallEnemy30PointAClear);
			}
			for(j=7; j>=0; j--){
				if(Enemy[i%6][j].life && Laser[i].life){
					unsigned long test = Enemy[i%6][j].life && Laser[i].life;
					if(((Laser[i].x < (Enemy[i%6][j].x + 14))  && (Laser[i].x  > (Enemy[i%6][j].x+2)))&&((Laser[i].y-9) < Enemy[i%6][j].y)){					//collision detection							Enemy[i%6][j].life = 0;
						explosion_sound = 1;
						explosion_sound_count = 0;
						RIT128x96x4_BMP(Laser[i].x, Laser[i].y, Laser1);		
						RIT128x96x4_BMP(Enemy[i%6][j].x, Enemy[i%6][j].y, SmallExplosion0);
						Delay(1000000);
						RIT128x96x4_BMP(Enemy[i%6][j].x, Enemy[i%6][j].y, SmallExplosion1);	
						Laser[i].life = 0;
						Laser[i].y = 87;
						Enemy[i%6][j].life = 0;
					}
				}
			} 
		}
	}
}
void Laser_Shoot(void){					// firing mah lazzzors (create a laser beam)
	long n;
	n = Player.x + 8;
	n = n/20;
	if(!(Laser[n].life)){
		Laser[n].life = 1;
		Laser[n].x = Player.x + 7;
		shoot_sound = 1;
		shoot_sound_count = 0;
	}
	else if((!Laser[n+6].life) && (Laser[n].y<75)){
		Laser[n+6].life = 1;
		Laser[n+6].x = Player.x + 7;
		shoot_sound = 1;
		shoot_sound_count = 0;
	}
	else if((!Laser[n+12].life) && (Laser[n+6].y<75)){
		Laser[n+12].life = 1;
		Laser[n+12].x = Player.x + 7;
		shoot_sound = 1;
		shoot_sound_count = 0;
	}
}

void Timer0B_Init(unsigned short period){			// init Timer0B
	SYSCTL_RCGC1_R |= SYSCTL_RCGC1_TIMER0;
	TimerCount = 0;
	TIMER0_CTL_R &= ~0x00000100;
	TIMER0_CFG_R = 0x00000004;
	TIMER0_TBMR_R = 0x00000002;
	TIMER0_TBILR_R = period-1;
	TIMER0_TBPR_R = 0;
	TIMER0_ICR_R = 0x00000100;
	TIMER0_IMR_R |= 0x00000100;
	NVIC_PRI5_R = (NVIC_PRI5_R&0xFFFFFF00)|0x00000040;
	NVIC_EN0_R |= NVIC_EN0_INT20;
}

int main(void){
		
	SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_8MHZ);
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOG; // activate port G
  Delay(1000000);			
  GPIO_PORTG_DIR_R |= 0x04;					// make PG2 out (built-in LED)
	GPIO_PORTG_DIR_R &= 0x07;				// make PG3-7 in PG3=Up	PG4=Down	PG5=Left	PG6=Right, PG7=Select
  GPIO_PORTG_AFSEL_R &= 0x00;				// disable alt func on PG2
  GPIO_PORTG_DEN_R |= 0xFF;					// enable digital I/O on PG2
	RIT128x96x4Init(1000000);
	ADC_InitSWTriggerSeq3(0);				// activate adc on channel 0
	DAC_Init();
	SysTick_Init(50000);   					// initialize SysTick timer at 1000Hz -> period = 1ms
	Timer0B_Init(4535);
  EnableInterrupts();
	TIMER0_CTL_R |= 0x00000100;
	
	RIT128x96x4_BMP(0, 87, marquee);		// display start screen
	while(GPIO_PORTG_DATA_R & 0xF8){		// wait for button press
	}
	RIT128x96x4Clear();
	
	Enemy_Init();
	Missile_Init();
	Player_Init();
	Laser_Init();
	toggle_enemies = 0;
	Enemy_Row_Init(3);
	Seed(NVIC_ST_CURRENT_R);
	
	
	
  
  while(1){
			Player_Display();
			Enemy_Display();
			if(missile_fly_timer > missile_fly_speed){
				Missile_Fly();
			}
			if(laser_fly_timer > laser_fly_speed){
				Laser_Fly();
			}
			if(!(GPIO_PORTG_DATA_R & 0xF8)){
				Laser_Shoot();
			}
			if(toggle_enemies>500){
				Enemy_Toggle();
				toggle_enemies = 0;
			}
			if(missile_create_timer > missile_creation_rate){
				int n;
				n = Random()%6;
				if(Missile[n].life == 0){
					Missile_Create(n);
					missile_create_timer = 0;
				}
			}			
	}
}

void SysTick_Handler(void){																	// controls time flow and happening of events in main via global vars
	toggle_enemies++;
	missile_create_timer++;
	missile_fly_timer++;
	laser_fly_timer++;
	//GPIO_PORTG_DATA_R ^= 0x04;
}

void Timer0B_Handler(void){																	// handles sound output via global vars
	TIMER0_ICR_R = 0x00000100;
	if(shoot_sound){
		DAC_Out(shoot[shoot_sound_count]);
		shoot_sound_count++;
		if(shoot_sound_count>4080){
			shoot_sound = 0;
			shoot_sound_count = 0;
		}
	}
	if(explosion_sound){
		DAC_Out(SmallExplosion[explosion_sound_count]);
		explosion_sound_count++;
		if(explosion_sound_count>1500){
			explosion_sound = 0;
			explosion_sound_count = 0;
		}
	}
}


