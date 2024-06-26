// to control the direction of spin in DC motor using L293D driver via push button(PE4)
// I have included image of L293D for wire connections
//PE1-3, enable, out1, out2 and PE0 push button
//you need to use external power supply for DC motor driver
//here PE0(enable motor) needs more then 3 volts volts TM4C GPIO is between 0 and 3
//you need to use a transistor or connect enable pin to 5 volt from power supply

// library include 2nd one is contains all location of all the registers
#include "stdint.h"
#include "tm4c123gh6pm.h"

uint8_t enable_motor1 = 1;
uint8_t out1 = 2;
uint8_t out2 = 3;
uint8_t push_button = 0;

unsigned int delay = 1000; //x second delay

void SysTick_Init(void);
void SysTick_Wait(unsigned long delay);
void Delay_ms(unsigned long delay);

void portE_init(void);
void digitalWriteE(uint8_t pin, uint8_t value);
uint8_t digitalReadE(uint8_t pin);

int main(void){
    portE_init();
    SysTick_Init();
    uint8_t read_button = digitalReadE(push_button);

    while (1){
        if (read_button == 1){
            digitalWriteE(enable_motor1, 1);
            digitalWriteE(out1, 1);
            digitalWriteE(out2, 0);
            Delay_ms(delay);
        }

        else if(read_button == 0){
            digitalWriteE(enable_motor1, 1);
            digitalWriteE(out1, 0);
            digitalWriteE(out2, 1);
            Delay_ms(delay);
        }

        read_button = digitalReadE(push_button);
    }
}

void portE_init(){
    volatile unsigned long delay;
    SYSCTL_RCGC2_R |= 0x00000010; // activate clock for port E
    delay = SYSCTL_RCGC2_R;       // allow time for clock to start

    GPIO_PORTE_AMSEL_R = 0x00;        // disable analog
    GPIO_PORTE_AFSEL_R = 0x00;        // disable alternate function
    GPIO_PORTE_PCTL_R = 0x00000000;   // disable port control

    GPIO_PORTE_DEN_R = 0x1F;          // enable digital I/O on PE4-0
    GPIO_PORTE_DIR_R = 0x0E;          // PE3-1 out, PE0 in
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

void Delay_ms(unsigned long delay){
    unsigned long i;
    for(i=0; i<delay; i++){
        SysTick_Wait(16000);  // wait 1ms (16 MHz clock)
    }
}

// Function to set the state of a pin on Port E
void digitalWriteE(uint8_t pin, uint8_t value) {
    if (value) {
        GPIO_PORTE_DATA_R |= (1 << pin);  // Set the specified pin
    } else {
        GPIO_PORTE_DATA_R &= ~(1 << pin);  // Clear the specified pin
    }
}
// Function to read the state of a pin on Port E
uint8_t digitalReadE(uint8_t pin) {
    return (GPIO_PORTE_DATA_R & (1 << pin)) ? 1 : 0;  // Return the state of the specified pin
}
