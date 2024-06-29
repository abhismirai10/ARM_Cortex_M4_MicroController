//blink LED but this time using systick timer

// Locations
#define GPIO_PORTF_DATA_R       (*((volatile unsigned long *)0x400253FC))
#define GPIO_PORTF_DIR_R        (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_AFSEL_R      (*((volatile unsigned long *)0x40025420))
#define GPIO_PORTF_PUR_R        (*((volatile unsigned long *)0x40025510))
#define GPIO_PORTF_DEN_R        (*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_LOCK_R       (*((volatile unsigned long *)0x40025520))
#define GPIO_PORTF_CR_R         (*((volatile unsigned long *)0x40025524))
#define GPIO_PORTF_AMSEL_R      (*((volatile unsigned long *)0x40025528))
#define GPIO_PORTF_PCTL_R       (*((volatile unsigned long *)0x4002552C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))

//Initialization of SysTick.
#define NVIC_ST_CTRL_R      (*((volatile unsigned long *)0xE000E010))
#define NVIC_ST_RELOAD_R    (*((volatile unsigned long *)0xE000E014))
#define NVIC_ST_CURRENT_R   (*((volatile unsigned long *)0xE000E018))

// more locations

void port_f_init(void);
void SysTick_Init(void);
void SysTick_Wait(unsigned long delay);
void SysTick_Wait100ms(unsigned long delay);

void main(void)
{
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

//lets tell turtle we are going to do this and it should be ready.
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
        SysTick_Wait(1600000);  // wait 100ms (16 MHz clock)
    }
}
