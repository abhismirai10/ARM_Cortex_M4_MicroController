// lets make LED blink using port E
// no button red led(PE1) continuous
// left button(PE0) blue led(PE2) blink
// right button(PE4) green led(PE3) blink
// both button all led blink

// to define registers for port E
#define GPIO_PORTE_DATA_R       (*((volatile unsigned long *)0x400243FC))
#define GPIO_PORTE_DIR_R        (*((volatile unsigned long *)0x40024400))
#define GPIO_PORTE_AFSEL_R      (*((volatile unsigned long *)0x40024420))
#define GPIO_PORTE_DEN_R        (*((volatile unsigned long *)0x4002451C))
#define GPIO_PORTE_AMSEL_R      (*((volatile unsigned long *)0x40024528))
#define GPIO_PORTE_PCTL_R       (*((volatile unsigned long *)0x4002452C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))

void delay_100milli(unsigned long time);
void portE_init();

void main(){
    portE_init();

    while(1){

        if ((GPIO_PORTE_DATA_R & 0x01)==0x00 && (GPIO_PORTE_DATA_R & 0x10)==0x00){
            GPIO_PORTE_DATA_R = (GPIO_PORTE_DATA_R & ~0x0E) | 0x02;
        }

        else if ((GPIO_PORTE_DATA_R & 0x01)==0x01 && (GPIO_PORTE_DATA_R & 0x10)==0x00){
            GPIO_PORTE_DATA_R = (GPIO_PORTE_DATA_R & ~0x0E) | 0x04;
            delay_100milli(1);
            GPIO_PORTE_DATA_R = (GPIO_PORTE_DATA_R & ~0x0E);
            delay_100milli(1);
        }
        else if ((GPIO_PORTE_DATA_R & 0x01)==0x00 && (GPIO_PORTE_DATA_R & 0x10)==0x10){
            GPIO_PORTE_DATA_R = (GPIO_PORTE_DATA_R & ~0x0E) | 0x08;
            delay_100milli(1);
            GPIO_PORTE_DATA_R = (GPIO_PORTE_DATA_R & ~0x0E);
            delay_100milli(1);
        }
        else if ((GPIO_PORTE_DATA_R & 0x01)==0x01 && (GPIO_PORTE_DATA_R & 0x10)==0x10){
            GPIO_PORTE_DATA_R = (GPIO_PORTE_DATA_R & ~0x0E) | 0x0E;
            delay_100milli(1);
            GPIO_PORTE_DATA_R = (GPIO_PORTE_DATA_R & ~0x0E);
            delay_100milli(1);
        }

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
    GPIO_PORTE_DIR_R = 0x0E;          // PE4,PE0 in, PE3-1 out

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
