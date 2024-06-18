//when your hearts upper part get potential
//after the delay lower part get potential

//green led to begin with
//we will turn red led on button press (rising edge) and
//after after 300ms green again

// declaration
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

int last_state = 0x01;

void port_f_init(void);
void delay_100milli(unsigned long time);

void main(void)
{
    port_f_init();
    while(1){
        GPIO_PORTF_DATA_R = (GPIO_PORTF_DATA_R & ~0x0E) | 0x08;
        if ((GPIO_PORTF_DATA_R & 0x01) == 0x01 && last_state == 0x00){ //start is button goes to low to high
            GPIO_PORTF_DATA_R = (GPIO_PORTF_DATA_R & ~0x0E) | 0x02;
            delay_100milli(3);
        }
        last_state = GPIO_PORTF_DATA_R & 0x01;

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

void delay_100milli(unsigned long time){
    unsigned long i;
    while (time > 0){
        i = 133333; //it take 100 ms time to run
        while(i>0){
            i = i - 1;
        }
        time = time - 1;
    }
}
