#include<MKL25Z4.H>
#include<stdio.h>

#define SLAVE_ADDR 0x60       // slave address (MSb) (A6, A5, A4, A3) are factory-set to <1001>
															// Address Pins (A2, A1, A0)
#define Exp_Addr 0x20
#define SLAVE_ADDR 0x60
#define Command_address 0x12
#define Command_data 0x00
#define Command_start 0x00
#define SLAVE_REGISTER 0x00
#define Reg_Addr 0x00
#define GPIO_Dir 0x00
#define GPIO_Addr 0x00
#define GP0 0x01
#define GP1 0x02
#define ERR_NONE 0x00
#define ERR_NO_ACK 0x01
#define ERR_ARB_LOST 0x02
#define ERR_BUS_BUSY 0x03


void I2C1_init(void); // configure GPIO pins for I2C and data transmission
void UART0_init(void); // initiate the UART 
int  I2C1_burstRead(unsigned char slave_Addr, unsigned char mem_Addr, int byteCount, unsigned char*data);
int I2C1_writeStart(unsigned char slave_Addr, unsigned char command_address, unsigned char comd_Data);

void delayUs(int n);
void delayMs(int m);

int main(void){
	unsigned char timeDateReadback[2]; 
	int non_value;
	int rv = 0;
	
	UART0_init();
	I2C1_init();
	non_value = I2C1_writeStart(Exp_Addr, Reg_Addr, GPIO_Dir);
	while(1){	
	
	// enable GP0 and disable GP1	
	non_value = I2C1_writeStart(Exp_Addr, GPIO_Addr, GP0);
	non_value = I2C1_writeStart(SLAVE_ADDR, Command_address, Command_start);
	rv = non_value = I2C1_burstRead(SLAVE_ADDR, SLAVE_REGISTER, 2, timeDateReadback);
	// enable GP1 and disable GP0
	non_value = I2C1_writeStart(Exp_Addr, GPIO_Addr, GP1);
	non_value = I2C1_writeStart(SLAVE_ADDR, Command_address, Command_start);
	rv = I2C1_burstRead(SLAVE_ADDR, SLAVE_REGISTER, 2, timeDateReadback);
	
	if(rv != 0){
		UART0_init();
		I2C1_init();
	}
	}
}
// initial UART0 9600 baud rate
void UART0_init(void){
	SIM->SCGC4 |= 0x0400;      // enable clock for UART0
	SIM->SOPT2 |= 0x04000000;  // use FLL for UART Baud rate generator
	UART0->C2 = 0x00;	// turn off UART0
	// Baud rate    SBR(in decimal)     SBR(in hex)
	// 4800         546                 0x0222
	// 9600         273                 0x0111
	// 19200        137                 0x0089
	// 38400        68                  0x0044
	// 115200       23                  0x0017
	UART0->BDH = 0x00;  
	UART0->BDL = 0x17;         // set SBR
	UART0->C4 = 0x0F;          // set OSR
	UART0->C1 = 0x00;          // 8-bit data
	UART0->C2 = 0x08;          // enable UART transmit
	
	SIM->SCGC5 |= 0x0200;      // enable clock for PORTA
	PORTA->PCR[2] = 0x0200;    // make PTA2 UART0_TX pin
}

// initialize I2C1 and the port pins	
void I2C1_init(void){
	SIM->SCGC4 |= 0x80;      // enable clock to I2C
	SIM->SCGC5 |= 0x2000;    // enable GPIO clock
	PORTE->PCR[1] = 0x0600;  // I2C1SCL--PTE1
	PORTE->PCR[0] = 0x0600;  // I2C1SDA--PTE0
	
	I2C1->C1 = 0x00;    // disable I2C 
	I2C1->S = 0x02;     // no interrupt pending
	I2C1->F = 0x1C;     // I2C clock speed divider
	I2C1->C1 = 0x80;    // enable I2C1 model
}

// I2C write to start the conversion
int I2C1_writeStart(unsigned char slave_Addr, unsigned char command_address, unsigned char comd_Data){
		int retry = 100;
		while(I2C1->S&0x20){
			if(--retry<=0)
				return ERR_BUS_BUSY;
			delayUs(50);
		}
		
		// send start
		I2C1->C1 |= 0x10;   // transmit mode
		// When the MST bit is changed from a 0 to a 1, 
		// a START signal is generated on the bus and master mode
    // is selected.
		I2C1->C1 &= ~0x20;
		delayUs(2);
		I2C1->C1 |= 0x20;   // enable master node
	
		//send slave address + write flag
		I2C1->D = (slaveAddr << 1);   // move the 7-bits to left
		while(!(I2C1->S & 0x02));   // see if no interrupt pending
		I2C1->S |= 0x02;
		if(I2C1->S & 0x10){
			I2C1->S |= 0x010;
			return ERR_ARB_LOST;
		}
		if(I2C1->S & 0x01)
			return ERR_NO_ACK;
		
		// send memory address
		I2C1->D = command_address;
		while(!(I2C1->S & 0x02));     
		I2C1->S |= 0x02;
		if(I2C1->S & 0x01)
			return ERR_NO_ACK;
		
		// send data
		I2C1->D = comdData;
		while(!(I2C1->S & 0x02));   
		I2C1->S |= 0x02;
		if(I2C1->S & 0x01)
			return ERR_NO_ACK;
		
		// stop
		I2C1->C1 &= ~0x30;
		return ERR_NONE;	
}

// Burst read: 
// S-( slave_addr + w)-ACK-(memory_addr)-ACK-R-( slave_addr + r)-ACK-data-ACK-data-ACK-...-data-NACK-P
int I2C1_burstRead(unsigned char slave_Addr, unsigned char mem_Addr, int byte_Count,
		unsigned char*data){
		//unsigned char ten_place;
		//unsigned char one_place;
		int retry = 100;
		volatile unsigned char dummy;
	
		// wait when bus is available
		while(I2C1->S&0x20){
			if(--retry<=0)
				return ERR_BUS_BUSY;
			delayUs(50);
		}
// start
		I2C1->C1 |= 0x10;   // transmit mode
		// When the MST bit is changed from a 0 to a 1, 
		// a START signal is generated on the bus and master mode
    // is selected.
		I2C1->C1 &= ~0x20;
		delayUs(2);
		I2C1->C1 |= 0x20;   // enable master node
//--------------------------------------
// ( slave_addr + w)
		I2C1->D = (slaveAddr << 1);   // move the 7-bits to left
		while(!(I2C1->S & 0x02));   // see if no interrupt pending
		I2C1->S |= 0x02;
		if(I2C1->S & 0x10){
			I2C1->S |= 0x010;
			return ERR_ARB_LOST;
		}
		if(I2C1->S & 0x01)
			return ERR_NO_ACK;
//---------------------------------------
// (memory_addr)
		I2C1->D = memAddr;
		while(!(I2C1->S & 0x02));
		I2C1->S |= 0x02;
		if(I2C1->S & 0x01)
			return ERR_NO_ACK;
// restart
		I2C1->C1 |= 0x04;

// ( slave_addr + r)
// change bus direction to read
		I2C1->D = (slaveAddr << 1) | 0x01;
		while(!(I2C1->S & 0x02));
		I2C1->S |= 0x02;
		if(I2C1->S & 0x01)
			return ERR_NO_ACK;
		I2C1->C1 &= ~0x18;
		if(byteCount == 1)
			I2C1->C1 |= 0x08;
		dummy = I2C1->D;
//  read data	
		while(byteCount > 0){
			//0 An acknowledge signal is sent to the bus on the following receiving byte (if FACK is cleared) or the
			//current receiving byte (if FACK is set).
      //1 No acknowledge signal is sent to the bus on the following receiving data byte (if FACK is cleared) or
      //the current receiving data byte (if FACK is set).
			if(byteCount == 1)
				// NACK
				I2C1->C1 |= 0x08;
			while(!(I2C1->S & 0x02));
			I2C1->S |= 0x02;
			// When this bit changes from a 1 to a 0, a STOP signal is generated
			if(byteCount == 1){
				I2C1->C1 |= 0x20;
				delayUs(1);
				I2C1->C1 &= ~0x20;
			}
			data[byteCount - 1] = I2C1->D;
			byteCount--;	
		}			
//-----------------------------------------
//  transmit the data to the UART						
		while(!(UART0->S1 & 0x80)){}
		UART0->D = (data[1]/100) + '0';
		while(!(UART0->S1 & 0x80)){}
		UART0->D = ((data[1]%100)/10) + '0';
		while(!(UART0->S1 & 0x80)){}
		UART0->D = ((data[1]%100)%10) + '0';
		while(!(UART0->S1 & 0x80)){}
		UART0->D = '\n';
		return ERR_NONE;
}

// us delay
void delayUs(int n){
		int i, j;
		for(i = 0; i < n; i++){
			for(j = 0; j < 7; j++);
	}
}

// ms delay
void delayMs(int m){
		int i, j;
		for(i = 0; i < m; i++){
			for(j = 0; j < 7000; j++);
	}
}

