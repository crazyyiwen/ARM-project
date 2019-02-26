#include "cpu_io.h"
#include "gpio.h"
#include "Key.h"
#include "Uart.h"

extern unsigned int asm_irq;
void irq_init(){
	//unsigned int tmp;

	//tmp = *((unsigned int *)asm_irq);
	//pEXECPTION_IRQ = tmp;

	pEXECPTION_IRQ = (unsigned int)asm_irq;
}
void irq_c_handler(void){
	void (*handler)(void);

	myputs("in irq c handler...\n");
	handler = (void (*)(void))VIC0ADDRESS;
	if(handler)
		handler();
	
}

void key1_handler(void){
	myputs("key1 down!!!\n");
	EXT_INT_2_PEND |= (0x1<<0);
	VIC0ADDRESS = 0x00;
}

void key_init(){
	struct s5pv210_gph_bank *gph2_base = (struct s5pv210_gph_bank *)S5PV210_GPH2_BASE;
	unsigned int var;
	/*外部引脚功能选择 EINT*/
	var = readl(&gph2_base->con);
	var |= (0xf<<0);
	writel(var, &gph2_base->con);
	/*配置EINT功能*/
	EXT_INT_2_CON &= ~(0x7<<0);
	EXT_INT_2_CON |= (0x2<<0);

	EXT_INT_2_MASK &= ~(0x1<<0);
	/*配置主中断控制器*/
	VIC0INTENABLE |= (0x1<<16);
	VIC0VECTADDR16 = (unsigned int)key1_handler;
}
