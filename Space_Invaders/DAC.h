// DAC.h


//	DAC_Init
//	Initialises a 4-bit DAC interface for use with headphones and the LM1968 microcontroller
//	Input: none
//	Output: none
void DAC_Init(void);

//	DAC_Out
//	Outputs a signal to the four ports 
//	Input: R0 (least significant four bits)
//	Output: none
void DAC_Out(unsigned char data);
