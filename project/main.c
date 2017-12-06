
/* 	Partners Jonathan Herrera jherr029@ucr.edu
*			Gustavo Correa gcorr003@ucr.edu
*
*	Lab section:022
*	Assignment: Lab # 3, Exercise # 3 Slave
*
*	I acknowledge all content contained herein, excluding template or example
*	code, is my own original work
*/



#include <avr/io.h>
//#include "timer.h"
#include "scheduler.h"

char receiveData = 0x00;
char button;


enum SPI_STATES { Receive_data } SPI_State;

// Servant code
void SPI_ServantInit(void) {
	// set DDRB to have MISO line as output and MOSI, SCK, and SS as input
	DDRB = 0x40; PORTB = 0xBF;
	// set SPCR register to enable SPI and enable SPI interrupt (pg. 168)
	SPCR |= (1<<SPE) | (1<<SPIE);
	// make sure global interrupts are enabled on SREG register (pg. 9)
	SREG = 0x80;
}


ISR(SPI_STC_vect) { // this is enabled in with the SPCR register’s “SPI
	// Interrupt Enable”
	// SPDR contains the received data, e.g. unsigned char receivedData =
	//PORTA = 0x0f;
	receiveData = SPDR;
	// SPDR;
}



//SPI COMMUNICATION STATE MACHINE
void SPI_SlaveTick(){
	
	button = ~PIND & 0x0F;
	
	switch(SPI_State){
		case Receive_data:
		//PORTA = 0xF0;
		SPI_State = Receive_data;
		break;
		default:
		break;
	}
	switch(SPI_State){
		case Receive_data:
		PORTA = receiveData;
		
		if ( button == 0x01 ){
			SPDR = 0x0F;
			PORTA |= 0x08;
		}
		break;
		default:
		break;
	}
}


//MAIN FUNCTION
int main( ){

	DDRA = 0xFF; PORTA = 0x00; // LCD control lines
	DDRD = 0x00; PORTD = 0xFF;
	
	SPI_ServantInit();
	
	tasksNum = 1; // declare number of tasks
	task tsks[1]; // initialize the task array
	tasks = tsks; // set the task array
	
	
	// define tasks
	unsigned char i=0; // task counter
	tasks[i].state = -1;
	tasks[i].period = 100;
	tasks[i].elapsedTime = tasks[i].period;
	tasks[i].TickFct = &SPI_SlaveTick;
	
	TimerSet(100); // value set should be GCD of all tasks
	TimerOn();

	while(1){
	}
	
	return 0;
}
