//blink LED but this time using 80 MHz systick timer
//you can set between 3.125 to 80 MHz

// locations
#include "stdint.h"
#include "tm4c123gh6pm.h"

// more Declarations
void PLL_Init(void);
void port_f_init(void);
void SysTick_Init(void);
void SysTick_Wait(unsigned long delay);
void SysTick_Wait100ms(unsigned long delay);

void main(void)
{
    PLL_Init();
    port_f_init();
    SysTick_Init();

    while(1){
        if ((GPIO_PORTF_DATA_R & 0x01) == 0x00){ //if button is press
            GPIO_PORTF_DATA_R = (GPIO_PORTF_DATA_R & ~0x0E) | 0x04; //turn on the LED
            SysTick_Wait100ms(1);            //wait 1 second
            GPIO_PORTF_DATA_R = (GPIO_PORTF_DATA_R & ~0x0E);    //turn off the LED
            SysTick_Wait100ms(1);
        }

        else {
            GPIO_PORTF_DATA_R = (GPIO_PORTF_DATA_R & ~0x0E) | 0x04;
        }
    }
}

//Activate the TM4C with a 16 MHz crystal to run at desire MHz here using 80MHz
void PLL_Init(void){
  // Use RCC2
  SYSCTL_RCC2_R |=  0x80000000;  // USE RCC2

  // bypass PLL while initializing
  SYSCTL_RCC2_R |=  0x00000800;  // BYPASS2, PLL bypass

  // select the crystal value and oscillator source(external in this case)
  SYSCTL_RCC_R = (SYSCTL_RCC_R &~0x000007C0)   // clear XTAL field, bits 10-6
                 + 0x00000540;   // 10101, configure for 16 MHz crystal
  SYSCTL_RCC2_R &= ~0x00000070;  // configure for main oscillator source

  // activate PLL by clearing PWRDN
  SYSCTL_RCC2_R &= ~0x00002000;

  // set the desired system divider (set the MHz you wanna use)
  SYSCTL_RCC2_R |= 0x40000000;   // use 400 MHz PLL
  SYSCTL_RCC2_R = (SYSCTL_RCC2_R&~ 0x1FC00000)  // clear system clock divider
                  + (4<<22);      // configure for 80 MHz clock here n=4 change n as per you need

  // wait for the PLL to lock by polling PLLLRIS
  while((SYSCTL_RIS_R&0x00000040)==0){};  // wait for PLLRIS bit

  // enable use of PLL by clearing BYPASS
  SYSCTL_RCC2_R &= ~0x00000800;
}

void port_f_init(void){
  volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000020;     // 1) activate clock for Port F
  delay = SYSCTL_RCGC2_R;           // allow time for clock to start

  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock GPIO Port F
  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0
  // only PF0 needs to be unlocked, other bits can't be locked

  GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog on PF

  GPIO_PORTF_PCTL_R = 0x00000000;   // 4) PCTL GPIO on PF4-0

  GPIO_PORTF_DIR_R = 0x0E;          // 5) PF4,PF0 in, PF3-1 out

  GPIO_PORTF_AFSEL_R = 0x00;        // 6) disable alternate function on PF7-0
  GPIO_PORTF_PUR_R = 0x11;          // enable pull-up on PF0 and PF4

  GPIO_PORTF_DEN_R = 0x1F;          // 7) enable digital I/O on PF4-0
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
        SysTick_Wait(8000000);  // wait 100ms (80 MHz clock)
    }
}
