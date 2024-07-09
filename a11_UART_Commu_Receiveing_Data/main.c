// Receiving msg via UART communication from Sender
//change the color of led as per msg

#include "stdint.h"
#include "tm4c123gh6pm.h"

// Function prototypes
void PLL_Init(void);
void SysTick_Init(void);
void SysTick_Wait(unsigned long delay);
void SysTick_Wait1ms(unsigned long delay);

void UART0_Init(void);
unsigned char UART_InChar(void);
unsigned char UART_InCharNonBlocking(void);
void UART_OutChar(unsigned char data);
void UART_OutString(char *str);

void port_f_init(void);
void digitalWriteF(uint8_t pin, uint8_t value);
uint8_t digitalReadF(uint8_t pin);

int main(void)
{
    PLL_Init();
    SysTick_Init();
    UART0_Init();
    port_f_init();

    while(1) {
        UART_OutString("enter the color you want to shine in");
        char data = UART_InChar();

        switch(data){
        case 'r':
            digitalWriteF(1, 1);
            digitalWriteF(2, 0);
            digitalWriteF(3, 0);
            break;

        case 'b':
            digitalWriteF(1, 0);
            digitalWriteF(2, 1);
            digitalWriteF(3, 0);
            break;

        case 'g':
            digitalWriteF(1, 0);
            digitalWriteF(2, 0);
            digitalWriteF(3, 1);
            break;

        default:
            UART_OutString("please enter a valid string you dufus!");
        }
    }
}

// Activate the TM4C with a 16 MHz crystal to run at desired MHz here using 80MHz
void PLL_Init(void) {
    SYSCTL_RCC2_R |= 0x80000000;  // Use RCC2
    SYSCTL_RCC2_R |= 0x00000800;  // Bypass PLL while initializing
    SYSCTL_RCC_R = (SYSCTL_RCC_R & ~0x000007C0) + 0x00000540;  // Configure for 16 MHz crystal
    SYSCTL_RCC2_R &= ~0x00000070;  // Configure for main oscillator source
    SYSCTL_RCC2_R &= ~0x00002000;  // Activate PLL by clearing PWRDN
    SYSCTL_RCC2_R |= 0x40000000;  // Use 400 MHz PLL
    SYSCTL_RCC2_R = (SYSCTL_RCC2_R & ~0x1FC00000) + (4 << 22);  // Configure for 80 MHz clock

    // Wait for the PLL to lock by polling PLLLRIS
    while((SYSCTL_RIS_R & 0x00000040) == 0) {};
    SYSCTL_RCC2_R &= ~0x00000800;  // Enable use of PLL by clearing BYPASS
}

void SysTick_Init(void) {
    NVIC_ST_CTRL_R = 0;                // Disable SysTick during setup
    NVIC_ST_RELOAD_R = 0x00FFFFFF;     // Maximum reload value
    NVIC_ST_CURRENT_R = 0;             // Any write to current clears it
    NVIC_ST_CTRL_R = 0x00000005;       // Enable SysTick with core clock
}

void SysTick_Wait(unsigned long delay) {
    NVIC_ST_RELOAD_R = delay - 1;  // Number of counts to wait
    NVIC_ST_CURRENT_R = 0;         // Any value written to CURRENT clears
    while((NVIC_ST_CTRL_R & 0x00010000) == 0) {
        // Wait for count flag
    }
}

void SysTick_Wait1ms(unsigned long delay) {
    unsigned long i;
    for(i = 0; i < delay; i++) {
        SysTick_Wait(80000);  // Wait 1ms (80 MHz clock)
    }
}

// Assumes an 80 MHz bus clock, creates 115200 baud rate
// UART0 Initialization function
void UART0_Init(void) {
    SYSCTL_RCGCUART_R |= 0x01;        // Activate UART0
    SYSCTL_RCGCGPIO_R |= 0x01;        // Activate port A
    UART0_CTL_R &= ~0x01;             // Disable UART
    UART0_IBRD_R = 43;                // IBRD = int(80,000,000 / (16 * 115,200))
    UART0_FBRD_R = 26;                // FBRD = round(0.40278 * 64)
    UART0_LCRH_R = 0x70;              // 8 bit, no parity, 1 stop bit, FIFOs
    UART0_CTL_R |= 0x301;             // Enable UART with transmit, receive enable

    GPIO_PORTA_AFSEL_R |= 0x03;       // Enable alt funct on PA1-0
    GPIO_PORTA_DEN_R |= 0x03;         // Enable digital I/O on PA1-0
    GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R & 0xFFFFFF00) + 0x00000011; // Configure PA1-0 as UART
    GPIO_PORTA_AMSEL_R &= ~0x03;      // Disable analog on PA1-0
}

// Wait for new input, then return ASCII code to receive data
unsigned char UART_InChar(void) {
    while((UART0_FR_R & 0x0010) != 0);  // Wait until RXFE is 0
    return ((unsigned char)(UART0_DR_R & 0xFF));
}

// Immediately return input or 0 if no input
unsigned char UART_InCharNonBlocking(void) {
    if((UART0_FR_R & UART_FR_RXFE) == 0) {
        return ((unsigned char)(UART0_DR_R & 0xFF));
    } else {
        return 0;
    }
}

// Wait for buffer to be not full, then output to send data
void UART_OutChar(unsigned char data) {
    while((UART0_FR_R & 0x0020) != 0);  // Wait until TXFF is 0
    UART0_DR_R = data;
}

// Send string
void UART_OutString(char *str) {
    while(*str) {
        UART_OutChar(*str);
        str++;
    }
    UART_OutChar('\n');
}

void port_f_init(void) {
    volatile unsigned long delay;
    SYSCTL_RCGC2_R |= 0x00000020;  // Activate clock for Port F
    delay = SYSCTL_RCGC2_R;        // Allow time for clock to start

    GPIO_PORTF_LOCK_R = 0x4C4F434B;  // Unlock GPIO Port F
    GPIO_PORTF_CR_R = 0x1F;          // Allow changes to PF4-0

    GPIO_PORTF_AMSEL_R = 0x00;       // Disable analog on PF
    GPIO_PORTF_PCTL_R = 0x00000000;  // PCTL GPIO on PF4-0

    GPIO_PORTF_DIR_R = 0x0E;         // PF4,0 in, PF3-1 out
    GPIO_PORTF_AFSEL_R = 0x00;       // Disable alt function on PF7-0
    GPIO_PORTF_PUR_R = 0x11;         // Enable pull-up on PF0 and PF4
    GPIO_PORTF_DEN_R = 0x1F;         // Enable digital I/O on PF4-0
}

// Function to set the state of a pin on Port F
void digitalWriteF(uint8_t pin, uint8_t value) {
    if (value) {
        GPIO_PORTF_DATA_R |= (1 << pin);  // Set the specified pin
    } else {
        GPIO_PORTF_DATA_R &= ~(1 << pin);  // Clear the specified pin
    }
}

// Function to read the state of a pin on Port F
uint8_t digitalReadF(uint8_t pin) {
    return (GPIO_PORTF_DATA_R & (1 << pin)) ? 1 : 0;  // Return the state of the specified pin
}
