#include<MKL25Z4.h>

void UART0_init(void);
void delayMs(int n);
// the function
int UART0_Function(signed short count){
	while(!(UART0->S1 & 0x80)){}
	UART0->D = count + '0';
	while(!(UART0->S1 & 0x80)){}
	UART0->D = '\n';
	delayMs(2);
	
}

// initial the UART
void UART0_init(void){
	SIM->SCGC4 |= 0x0400;
	SIM->SOPT2 |= 0x04000000;
	UART0->C2 = 0;
	UART0->BDH = 0x00;
	UART0->BDL = 0x17;
	UART0->C4 = 0x0F;
	UART0->C1 = 0x00;
	UART0->C2 = 0x08;
	SIM->SCGC5 |= 0x0200;
	PORTA->PCR[2] = 0x0200;
}

// delya n millisenconds
void delayMs(int n){
	int i;
	int j;
	for(i = 0; i < n; i++)
		for(j = 0; j < 7000; j++){}
}

