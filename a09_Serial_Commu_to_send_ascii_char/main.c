//writing 'A' on PC5 UART1 transmit
//can you osciloscope or serial receiver to check the signal

#include "stdint.h"
#include "tm4c123gh6pm.h"

void PLL_Init(void);
void SysTick_Init(void);
void SysTick_Wait(unsigned long delay);
void SysTick_Wait1ms(unsigned long delay);

void UART_Init(void);
unsigned char UART_InChar(void);
void UART_OutChar(unsigned char data);
unsigned char UART_InCharNonBlocking(void);

void port_f_init(void);
void digitalWriteF(uint8_t pin, uint8_t value);
uint8_t digitalReadF(uint8_t pin);

void main(void)
{
    PLL_Init();
    SysTick_Init();

    UART_Init();
    port_f_init();

    while(1){
        int push_button = digitalReadF(0);
        if (push_button == 0){
            UART_OutChar(' ');
            digitalWriteF(1, 1);
        }
        else {
            digitalWriteF(1, 0);  // Ensure to set pin low in the else case
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

void SysTick_Wait1ms(unsigned long delay){
    unsigned long i;
    for(i=0; i<delay; i++){
        SysTick_Wait(80000);  // wait 1ms (80 MHz clock)
    }
}

// Assumes a 80 MHz bus clock, creates 115200 baud rate
void UART_Init(void){            // should be called only once
  SYSCTL_RCGC1_R |= 0x00000002;  // activate UART1
  SYSCTL_RCGC2_R |= 0x00000004;  // activate port C
  UART1_CTL_R &= ~0x00000001;    // disable UART
  UART1_IBRD_R = 43;     // IBRD = int(80,000,000/(16*115,200)) = int(43.40278)
  UART1_FBRD_R = 26;     // FBRD = round(0.40278 * 64) = 26
  UART1_LCRH_R = 0x00000070;  // 8 bit, no parity bits, one stop, FIFOs
  UART1_CTL_R |= 0x00000001;     // enable UART

  GPIO_PORTC_AMSEL_R &= ~0x30;   // disable analog on PC5-4
  GPIO_PORTC_AFSEL_R |= 0x30;    // enable alt funct on PC5-4
  GPIO_PORTC_DEN_R |= 0x30;      // configure PC5-4 as UART1
  GPIO_PORTC_PCTL_R = (GPIO_PORTC_PCTL_R&0xFF00FFFF)+0x00220000; // related to alt function

}

// Wait for new input, then return ASCII code
//to receive data
unsigned char UART_InChar(void){
  while((UART1_FR_R&0x0010) != 0);      // wait until RXFE is 0
  return((unsigned char)(UART1_DR_R&0xFF));
}

// Wait for buffer to be not full, then output
//to send data
void UART_OutChar(unsigned char data){
  while((UART1_FR_R&0x0020) != 0);      // wait until TXFF is 0
  UART1_DR_R = data;
}

// Immediately return input or 0 if no input
unsigned char UART_InCharNonBlocking(void){
  if((UART1_FR_R&UART_FR_RXFE) == 0){
    return((unsigned char)(UART1_DR_R&0xFF));
  } else{
    return 0;
  }
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

  GPIO_PORTF_DIR_R = 0x02;          // 5) PF0 in, PE1 out

  GPIO_PORTF_AFSEL_R = 0x00;        // 6) disable alternate function on PF7-0
  GPIO_PORTF_PUR_R = 0x11;          // enable pull-up on PF0 and PF4

  GPIO_PORTF_DEN_R = 0x03;          // 7) enable digital I/O on PF4-0
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
