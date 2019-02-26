#include "gpio.h"
#include "led.h"
#include "cpu_io.h"

void led_init(){
	struct s5pv210_gpio *gpio_base = (struct s5pv210_gpio *)S5PV210_GPIO_BASE;
	unsigned int var;
	var = _REG(&gpio_base->gpio_j2.con);
	var &= ~(0xffff<<0);
	var |= (0x1111<<0);
	writel(var, &gpio_base->gpio_j2.con);
}

void led_blink(int status){
	struct s5pv210_gpio *gpio_base = (struct s5pv210_gpio *)S5PV210_GPIO_BASE;
	// turn on the leds
	unsigned int var;
	if(status){
		var = _REG(&gpio_base->gpio_j2.dat);
		var &= ~0xf;
		writel(var, &gpio_base->gpio_j2.dat);
	}
	// turn off the leds
	else{
		var = _REG(&gpio_base->gpio_j2.dat);
		var |= 0xf;
		writel(var, &gpio_base->gpio_j2.dat);	
	}
}
	
