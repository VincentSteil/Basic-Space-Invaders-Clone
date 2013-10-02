// ADCSWTrigger.c
// Runs on LM3S1968
// Provide functions that initialize ADC SS3 to be triggered by
// software and trigger a conversion, wait for it to finish,
// and return the result.
// Daniel Valvano
// May 21, 2012

/* This example accompanies the book
   "Embedded Systems: Interfacing to the Arm Cortex M3",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2012

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

#include "lm3s1968.h"
#include "PLL.h"

// There are many choices to make when using the ADC, and many
// different combinations of settings will all do basically the
// same thing.  For simplicity, this function makes some choices
// for you.  When calling this function, be sure that it does
// not conflict with any other software that may be running on
// the microcontroller.  Particularly, ADC sample sequencer 3
// is used here because it only takes one sample, and only one
// sample is absolutely needed.  Sample sequencer 3 generates a
// raw interrupt when the conversion is complete, but it is not
// promoted to a controller interrupt.  Software triggers the
// ADC conversion and waits for the conversion to finish.  If
// somewhat precise periodic measurements are required, the
// software trigger can occur in a periodic interrupt.  This
// approach has the advantage of being simple.  However, it does
// not guarantee real-time.
//
// A better approach would be to use a hardware timer to trigger
// the ADC conversion independently from software and generate
// an interrupt when the conversion is finished.  Then, the
// software can transfer the conversion result to memory and
// process it after all measurements are complete.

// This initialization function sets up the ADC according to the
// following parameters.  Any parameters not explicitly listed
// below are not modified:
// Max sample rate: <=125,000 samples/second
// Sequencer 0 priority: 1st (highest)
// Sequencer 1 priority: 2nd
// Sequencer 2 priority: 3rd
// Sequencer 3 priority: 4th (lowest)
// SS3 triggering event: software trigger
// SS3 1st sample source: programmable using variable 'channelNum' [0:7]
// SS3 interrupts: enabled but not promoted to controller
void ADC_InitSWTriggerSeq3(unsigned char channelNum){
  if(channelNum > 7){
    return;   // 0 to 7 are valid channels on the LM3S1968
  }
  SYSCTL_RCGC0_R |= 0x00010000;   // 1) activate ADC
  SYSCTL_RCGC0_R &= ~0x00000300;  // 2) configure for 125K
  ADC_SSPRI_R = 0x3210;           // 3) Sequencer 3 is lowest priority
  ADC_ACTSS_R &= ~0x0008;         // 4) disable sample sequencer 3
  ADC_EMUX_R &= ~0xF000;          // 5) seq3 is software trigger
  ADC_SSMUX3_R &= ~0x0007;        // 6) clear SS3 field
  ADC_SSMUX3_R += channelNum;     //    set channel
  ADC_SSCTL3_R = 0x0006;          // 7) no TS0 D0, yes IE0 END0
  ADC_IM_R &= ~0x0008;            // 8) disable SS3 interrupts
  ADC_ACTSS_R |= 0x0008;          // 9) enable sample sequencer 3
}

//------------ADC_InSeq3------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 10-bit result of ADC conversion
unsigned long ADC_InSeq3(void){  unsigned long result;
  ADC_PSSI_R = 0x0008;             // 1) initiate SS3
  while((ADC_RIS_R&0x08)==0){};    // 2) wait for conversion done
  result = ADC_SSFIFO3_R&0x3FF;    // 3) read result
  ADC_ISC_R = 0x0008;              // 4) acknowledge completion
  return result;
}

void PLL_Init(void);

// void PLL_Init(void){    // program 4.6 volume 1
//   // 1) bypass PLL and system clock divider while initializing
//   SYSCTL_RCC_R |=  0x00000800;   // PLL bypass
//   SYSCTL_RCC_R &= ~0x00400000;   // do not use system divider
//   // 2) select the crystal value and oscillator source
//   SYSCTL_RCC_R &= ~0x000003C0;   // clear XTAL field, bits 9-6
//   SYSCTL_RCC_R +=  0x00000380;   // 0x0E, configure for 8 MHz crystal
//   SYSCTL_RCC_R &= ~0x00000030;   // clear oscillator source field
//   SYSCTL_RCC_R +=  0x00000000;   // configure for main oscillator source
//   // 3) activate PLL by clearing PWRDN and OEN
//   SYSCTL_RCC_R &= ~(0x00002000|0x00001000);
//   // 4) set the desired system divider and the USESYSDIV bit
//   SYSCTL_RCC_R &= ~0x07800000;   // system clock divider field
//   SYSCTL_RCC_R +=  0x03800000;   // configure for 25 MHz clock
//   SYSCTL_RCC_R |=  0x00400000;   // Enable System Clock Divider
//   // 5) wait for the PLL to lock by polling PLLLRIS
//   while((SYSCTL_RIS_R&0x00000040)==0){};  // wait for PLLRIS bit
//   // 6) enable use of PLL by clearing BYPASS
//   SYSCTL_RCC_R &= ~0x00000800;
// }

// //debug code
// //
// // This program periodically samples ADC channel 0 and stores the
// // result to a global variable that can be accessed with the JTAG
// // debugger and viewed with the variable watch feature.
// #define PG2   (*((volatile unsigned long *)0x40026010))
// void DisableInterrupts(void); // Disable interrupts
// void EnableInterrupts(void);  // Enable interrupts
// long StartCritical (void);    // previous I bit, disable interrupts
// void EndCritical(long sr);    // restore I bit to previous value
// void WaitForInterrupt(void);  // low power mode

// volatile unsigned long ADCvalue;
// // This debug function initializes Timer0A to request interrupts
// // at a 10 Hz frequency.  It is similar to FreqMeasure.c.
// void Timer0A_Init10HzInt(void){
//   volatile unsigned long delay;
//   DisableInterrupts();
//   // **** general initialization ****
//   SYSCTL_RCGC1_R |= SYSCTL_RCGC1_TIMER0;// activate timer0
//   delay = SYSCTL_RCGC1_R;          // allow time to finish activating
//   TIMER0_CTL_R &= ~TIMER_CTL_TAEN; // disable timer0A during setup
//   TIMER0_CFG_R = TIMER_CFG_16_BIT; // configure for 16-bit timer mode
//   // **** timer0A initialization ****
//                                    // configure for periodic mode
//   TIMER0_TAMR_R = TIMER_TAMR_TAMR_PERIOD;
//   TIMER0_TAPR_R = 249;             // prescale value for 10us
//   TIMER0_TAILR_R = 9999;           // start value for 10 Hz interrupts
//   TIMER0_IMR_R |= TIMER_IMR_TATOIM;// enable timeout (rollover) interrupt
//   TIMER0_ICR_R = TIMER_ICR_TATOCINT;// clear timer0A timeout flag
//   TIMER0_CTL_R |= TIMER_CTL_TAEN;  // enable timer0A 16-b, periodic, interrupts
//   // **** interrupt initialization ****
//                                    // Timer0A=priority 2
//   NVIC_PRI4_R = (NVIC_PRI4_R&0x00FFFFFF)|0x40000000; // top 3 bits
//   NVIC_EN0_R |= NVIC_EN0_INT19;    // enable interrupt 19 in NVIC
// }
// void Timer0A_Handler(void){
//   TIMER0_ICR_R = TIMER_ICR_TATOCINT;    // acknowledge timer0A timeout
//   PG2 = 0x04;                           // profile
//   ADCvalue = ADC_InSeq3();
//   PG2 = 0x00;
// }
// int main(void){
//   PLL_Init();      // 25 MHz
//   SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOG; // activate port G
//   ADC_InitSWTriggerSeq3(0);             // allow time to finish activating
//   Timer0A_Init10HzInt();                // set up Timer0A for 10 Hz interrupts
//   GPIO_PORTG_DIR_R |= 0x04;             // make PG2 out (built-in LED)
//   GPIO_PORTG_AFSEL_R &= ~0x04;          // disable alt func on PG2
//   GPIO_PORTG_DEN_R |= 0x04;             // enable digital I/O on PG2
//   PG2 = 0;                              // turn off LED
//   EnableInterrupts();
//   while(1){
//     WaitForInterrupt();
//   }
// }
