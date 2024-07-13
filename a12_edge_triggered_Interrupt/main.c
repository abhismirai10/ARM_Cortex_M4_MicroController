
#include "tm4c123gh6pm.h"
#include "stdint.h"

volatile unsigned long FallingEdges = 0;

void PortF_Init(void);
void GPIOPortF_Handler(void);

void SysTick_Init(void);
void SysTick_Wait(unsigned long delay);
void SysTick_Wait100ms(unsigned long delay);

void digitalWriteF(uint8_t pin, uint8_t value);
uint8_t digitalReadF(uint8_t pin);


int main(void){
    SysTick_Init();
    PortF_Init(); // initialize GPIO Port F interrupt

    while (1){
        SysTick_Wait100ms(10);

    }
}

void PortF_Init(void){
  SYSCTL_RCGC2_R |= 0x00000020; // (a) activate clock for port F
  FallingEdges = 0;             // (b) initialize count and wait for clock
  GPIO_PORTF_DIR_R |= 0x02;     // for led
  GPIO_PORTF_DIR_R &= ~0x10;    // (c) make PF4 in other out (built-in button)
  GPIO_PORTF_AFSEL_R &= ~0x12;  //     disable alt funct on PF4
  GPIO_PORTF_DEN_R |= 0x12;     //     enable digital I/O on PF4
  GPIO_PORTF_PCTL_R &= ~0x000F0000; //  configure PF4 as GPIO
  GPIO_PORTF_AMSEL_R &= ~0x12;  //    disable analog functionality on PF4
  GPIO_PORTF_PUR_R |= 0x10;     //     enable weak pull-up on PF4

  GPIO_PORTF_IS_R &= ~0x10;     // (d) PF4 is edge-sensitive
  GPIO_PORTF_IBE_R &= ~0x10;    //     PF4 is not both edges
  GPIO_PORTF_IEV_R &= ~0x10;    //     PF4 falling edge event
  GPIO_PORTF_ICR_R = 0x10;      // (e) clear flag4
  GPIO_PORTF_IM_R |= 0x10;      // (f) arm interrupt on PF4
  NVIC_PRI7_R = (NVIC_PRI7_R&0xFF00FFFF)|0x00A00000; // (g) priority 5
  NVIC_EN0_R = 0x40000000;      // (h) enable interrupt 30 in NVIC
  // Enable global interrupts
  __asm("    cpsie   i"); //for assmebly
  //EnableInterrupts();    // for C
}

void GPIOPortF_Handler(void){
  GPIO_PORTF_ICR_R = 0x10;      // acknowledge flag4
  FallingEdges = FallingEdges + 1;
  GPIO_PORTF_DATA_R ^= 0x02;

}

void SysTick_Init(void){
    NVIC_ST_CTRL_R = 0;                // disable SysTick during setup
    NVIC_ST_RELOAD_R = 0x00FFFFFF;     // maximum reload value
    NVIC_ST_CURRENT_R = 0;             // any write to current clears it
    NVIC_ST_CTRL_R = 0x00000005;       // enable SysTick with core clock
}

void SysTick_Wait(unsigned long delay){
    NVIC_ST_RELOAD_R = delay - 1;  // number of counts to wait
    NVIC_ST_CURRENT_R = 0;         // any value written to CURRENT clears
    while((NVIC_ST_CTRL_R & 0x00010000) == 0){
        // wait for count flag
    }
}

void SysTick_Wait100ms(unsigned long delay){
    unsigned long i;
    for(i=0; i<delay; i++){
        SysTick_Wait(1600000);  // wait 100ms (16 MHz clock)
    }
}

// Function to set the state of a pin on Port F
void digitalWriteF(uint8_t pin, uint8_t value) {
    if (value) {
        GPIO_PORTF_DATA_R |= (1 << pin);  // Set the specified pin
    } else {
        GPIO_PORTF_DATA_R &= ~(1 << pin);  // Clear the specified pin
    }
}
// Function to read the state of a pin on Port E
uint8_t digitalReadF(uint8_t pin) {
    return (GPIO_PORTF_DATA_R & (1 << pin)) ? 1 : 0;  // Return the state of the specified pin
}
