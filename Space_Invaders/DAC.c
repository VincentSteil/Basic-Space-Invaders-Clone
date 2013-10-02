//	DAC.c
//	Vincent Steil
//	vjs432
//	s1008380

//	Driver for a 4bit DAC interface for the LM1968 microcontroller

//	Input	Output Voltage (1,2,4,8 resistor assembly)
//	0000	0V
//	0001	0.22V
//	0010	0.44V
//	0011	0.66V
//	0100	0.88V
//	0101	1.1V
//	0110	1.32V
//	0111	1.54V
//	1000	1.76V 
//	1001	1.98V
//	1010	2.2V
//	1011	2.42V 
//	1100	2.64V
//	1101	2.86V 
//	1110	3.08V 
//	1111	3.3V

#include "DAC.h"
#include "hw_types.h"
#include "sysctl.h"
#include "lm3s1968.h"
/*
#define GPIO_PORTF_DATA_R		(*((volatile unsigned long *)0x400253FC))
#define GPIO_PORTF_DIR_R		(*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_AFSEL_R	(*((volatile unsigned long *)0x40025420))
#define GPIO_PORTF_DEN_R		(*((volatile unsigned long *)0x4002551C))
#define SYSCTL_RCGC2_R    	(*((volatile unsigned long *)0x400FE108))
*/

//	DAC_Init
//	Initialises a 4-bit DAC interface for use with headphones and the LM1968 microcontroller
//	Input: none
//	Output: none

void DAC_Init(void){
  unsigned long wait;
  SYSCTL_RCGC2_R |= 0x00000020; 	// clock to Port F
  wait = 0;
  wait++;
  GPIO_PORTF_DEN_R |= 0x0F;     					// data enable register
  GPIO_PORTF_AFSEL_R &= 0x00;		  				// Alternative function register    
  GPIO_PORTF_DIR_R |= 0x0F;								// PF0-PF3 outputs
}

//	DAC_Out
//	Outputs a signal to the four ports 
//	Input: R0 (least significant four bits)
//	Output: none

void DAC_Out(unsigned char data){
		GPIO_PORTF_DATA_R &= 0xF0;	// clear bits 0-3
		GPIO_PORTF_DATA_R |= data;	// write to bits 0-3
}
