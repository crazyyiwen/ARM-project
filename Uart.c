#include "cpu_io.h"
#include "Uart.h"
#include "gpio.h"

void uart_init(void){
	// set up the base address
	struct s5pv210_gpio *gpio_base = (struct s5pv210_gpio *)S5PV210_GPIO_BASE;
	struct s5pv2xx_uart *uart_base = (struct s5pv2xx_uart *)s5PV210_UART_BASE;
	unsigned int var;
	// choose function of the port
	var = readl(&gpio_base->gpio_a0.con);
	var &= ~(0xff<<0);
	var |= (0x22<<0);
	writel(var, &gpio_base->gpio_a0.con);
	// configure the uart controller
	_REG(&uart_base->ulcon) = 0x3;			// 8-bit  1stop  non-even
	_REG(&uart_base->ucon)  = 0x5;			// enable recv send
	_REG(&uart_base->ubrdiv) = 35;  		// baud rate setting
	_REG(&uart_base->udivslot) = 0x80;  	// baud rate setting
}	

void myputc(char c){
	struct s5pv2xx_uart *uart_base = (struct s5pv2xx_uart *)s5PV210_UART_BASE;

	//_REG(&uart_base->utxh) = c;
	// wait for send completion
	while(!(_REG(&uart_base->utrstat) & (0x1<<2)));
	// send next data
	_REG(&uart_base->utxh) = c;
	return;
}

void myputs(const char *str){
	while(*str){
		myputc(*str);
		str++;
	}
}
