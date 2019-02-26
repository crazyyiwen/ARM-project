#ifndef KEY_H
#define KEY_H

#define pEXECPTION_IRQ  	(*(volatile unsigned int *)(0xD0037400 + 0x18))

#define EXT_INT_2_CON  		(*(volatile unsigned int *)(0xE0200E08))
#define EXT_INT_2_MASK 		(*(volatile unsigned int *)(0xE0200F08))
#define EXT_INT_2_PEND 		(*(volatile unsigned int *)(0xE0200F48))

#define VIC0INTENABLE  		(*(volatile unsigned int *)(0xF2000010))
#define VIC0ADDRESS    		(*(volatile unsigned int *)(0xF2000F00))
#define VIC0VECTADDR16    	(*(volatile unsigned int *)(0xF2000140))


void irq_c_handler(void);

void irq_init(void);

void key_init(void);

void key1_handler(void);

#endif
