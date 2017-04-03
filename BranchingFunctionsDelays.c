// BranchingFunctionsDelays.c Lab 6
// Runs on LM4F120/TM4C123
// Use simple programming structures in C to 
// toggle an LED while a button is pressed and 
// turn the LED on when the button is released.  
// This lab will use the hardware already built into the LaunchPad.
// Daniel Valvano, Jonathan Valvano
// January 15, 2016

// built-in connection: PF0 connected to negative logic momentary switch, SW2
// built-in connection: PF1 connected to red LED
// built-in connection: PF2 connected to blue LED
// built-in connection: PF3 connected to green LED
// built-in connection: PF4 connected to negative logic momentary switch, SW1

#include "TExaS.h"

#define GPIO_PORTF_DATA_R       (*((volatile unsigned long *)0x400253FC))
#define GPIO_PORTF_DIR_R        (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_AFSEL_R      (*((volatile unsigned long *)0x40025420))
#define GPIO_PORTF_PUR_R        (*((volatile unsigned long *)0x40025510))
#define GPIO_PORTF_DEN_R        (*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_AMSEL_R      (*((volatile unsigned long *)0x40025528))
#define GPIO_PORTF_PCTL_R       (*((volatile unsigned long *)0x4002552C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
#define SYSCTL_RCGC2_GPIOF      0x00000020  // port F Clock Gating Control
#define PF4											(*((volatile unsigned long *)0x40025040))
#define PF2											(*((volatile unsigned long *)0x40025010))

// basic functions defined at end of startup.s
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void Delay100ms(unsigned long time); //setup for delay mathod

int main(void){ unsigned long volatile delay;
  TExaS_Init(SW_PIN_PF4, LED_PIN_PF2);  // activate grader and set system clock to 80 MHz
  // initialization goes here
	SYSCTL_RCGC2_R |= 0x00000020; //turn on the clock for portF
	delay = SYSCTL_RCGC2_R;  			//delay the start until clock spins up
	GPIO_PORTF_AMSEL_R = GPIO_PORTF_AMSEL_R&~0x14;	//clear pf4 and pf2 bits
	GPIO_PORTF_PCTL_R = GPIO_PORTF_PCTL_R&~0x14;	//clear pf4 and pf2 bits
	GPIO_PORTF_DIR_R = GPIO_PORTF_DIR_R&~0x10;											//set pf4 to input
	GPIO_PORTF_DIR_R |= 0x04;			//set pf 2 to output
	GPIO_PORTF_AFSEL_R = GPIO_PORTF_AFSEL_R&~0x14; //clear pf4 and pf2
	GPIO_PORTF_DEN_R |= 0x14;												//set pf4 and pf2
	GPIO_PORTF_PUR_R |= 0x10;												//set pf4
	GPIO_PORTF_DATA_R |= 0x4;												//set pf2
	
  EnableInterrupts();           // enable interrupts for the grader
  while(1){
    // body goes here
		Delay100ms(1);
		if(PF4 == 0){
			PF2 = PF2^0x4;
		}
		if(PF4 > 0){
			PF2 = 0x04;
		}
  }
}

void Delay100ms(unsigned long time){
  unsigned long i;
  while(time > 0){
    i = 1333333;  // this number means 100ms
    while(i > 0){
      i = i - 1;
    }
    time = time - 1; // decrements every 100 ms
  }
}

