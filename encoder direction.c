/*
PTA1 and PTA2 are used as input, 
when the encoder rotate in a positive direction,
the counter++, otherwise, the counter--.

*/

#include <MKL25Z4.H>          // Compiler class library path inside the header file
#include "UART_display.h"     // The directory file in the relative path of the header file
int main(void){
	unsigned short last_state;
	unsigned short present_state;
	unsigned short present_B;
	signed short counter = 0;
	//signed short divideNumber = 2;
	
	SIM->SCGC5 |= 0x200;            //enable clock to Port A
	PORTA->PCR[1] = 0x102;          //make PTA1 pin as GPIO and enable pull-down
	PTA->PDDR &= ~ 0x02;            //make PTA1 as input pin
	PORTA->PCR[2] = 0x102;          //make PTA2 pin as GPIO and enable pull-down
	PTA->PDDR &= ~ 0x04;            //make PTA2 as input pin
	UART0_init();
	
	if((PTA->PDIR & 0x04))
		last_state = 1;
	else
		last_state = 0;
	
	// how to display the negative number?
	while(1){
		if((PTA->PDIR & 0x04))
			present_state = 1;
		else
			present_state = 0;		
		if(present_state != last_state){
			if((PTA->PDIR & 0x02))
				present_B = 1;
			else
				present_B = 0;
			if(present_B != present_state)
				++counter;
			else	
				--counter;
			if(counter <= 0)
				counter = 0;
			else if(counter >= 18)
				counter = 18;
			else
				;
			UART0_Function(counter/2);
		}
		last_state = present_state;
	}
}





