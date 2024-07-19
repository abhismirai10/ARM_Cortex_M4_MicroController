#include "tm4c123gh6pm.h"
#include "stdint.h"

volatile unsigned long Counts=0;

void SysTick_Init(unsigned long period){ // priority 2
  NVIC_ST_CTRL_R = 0;         // disable SysTick during setup
  NVIC_ST_RELOAD_R = period-1;// reload value
  NVIC_ST_CURRENT_R = 0;      // any write to current clears it
  NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x40000000;
  NVIC_ST_CTRL_R = 0x07; // enable SysTick with core clock and interrupts
  // finish all initializations and then enable interrupts
}

void SysTick_Handler(void){
  GPIO_PORTF_DATA_R ^= 0x04;       // toggle PF2
  Counts = Counts + 1;
}


int main(void){ // running at 16 MHz
  SYSCTL_RCGC2_R |= 0x00000020; // activate port F
  Counts = 0;
  GPIO_PORTF_DIR_R |= 0x04;   // make PF2 output (PF2 built-in LED)
  GPIO_PORTF_AFSEL_R &= ~0x04;// disable alt funct on PF2
  GPIO_PORTF_DEN_R |= 0x04;   // enable digital I/O on PF2
  GPIO_PORTF_PCTL_R = (GPIO_PORTF_PCTL_R&0xFFFFF0FF)+0x00000000;
  GPIO_PORTF_AMSEL_R = 0;     // disable analog functionality on PF
  SysTick_Init(1600000);        // initialize SysTick timer, every 1ms

  // Enable global interrupts
  __asm("    cpsie   i"); //for assmebly

  while(1){                   // interrupts every 1ms, 500 Hz flash
  }
}
