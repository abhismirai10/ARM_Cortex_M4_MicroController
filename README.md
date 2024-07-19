# ARM Cortex M4 MicroController

## Intro

This repository contains code and documentation for the ARM Cortex-M4 microcontroller, specifically the TM4C123GH6PM model found on the TIVA LaunchPAD developer kit (any Cortex-M4 will work!). The code provided covers various applications ranging from simple LED blinking to advanced communication protocols and more. All implementations are from scratch, without using libraries like `arduino.h`. This repo is not just useful for learning about microcontrollers from beginner to advanced levels, but also for learning C/C++ or assembly in practical applications. As a wise person once said, "Any computer language can be learned by using it in real life!"

## Table of Contents
1. [Intro](#intro)
2. [Hardware](#hardware)
3. [Getting Started](#getting-started)
    - [Prerequisites](#prerequisites)
    - [Installation](#installation)
4. [Code Details](#code-details)
5. [Contributing](#contributing)
6. [License](#license)
7. [Acknowledgments](#acknowledgments)
8. [Contact](#contact)

## Hardware

The TIVA LaunchPAD features an ARM Cortex-M4 microcontroller named TM4C123GH6PM. This powerful and versatile processor is well-suited for various embedded systems applications. This repository will guide you through setting up the hardware and software, as well as provide detailed instructions and examples for different projects. It is strongly recommended to download datasheets for the processor and kit for reference.

![TIVA LaunchPAD](https://github.com/abhismirai10/ARM_Cortex_M4_MicroController/assets/121724635/13acc5be-48a2-4a26-89b3-509de1291eda)  
[TM4C123GH6PM TIVA LaunchPAD Developer Kit](https://www.ti.com/tool/EK-TM4C123GXL)

## Getting Started

### Prerequisites

- **Hardware:**
  - TM4C123GH6PM TIVA LaunchPAD
  - LEDs, resistors, servo motors, and other components as required by each example
- **Software:**
  - Texas Instruments Code Composer Studio (CCS)
  - Drivers and libraries for the TM4C123GH6PM (if using Windows - not recommended)

### Installation

1. **Download and Install Code Composer Studio (CCS):**
   - Follow the instructions on the [CCS download page](https://www.ti.com/tool/CCSTUDIO).
2. **Clone this Repository:**
   ```sh
   git clone https://github.com/yourusername/ARM_Cortex_M4_MicroController.git
   ```
3. **Open the Project in CCS:**
   - Open CCS and import the cloned repository.

## Code Details

The projects in this repository are implemented in C/C++, but assembly can also be used for maximum efficiency. Most sketches are well-documented, with additional details provided below based on implementation complexity.

### 1. Simple LED on/off
- **Code File:** `a1_LED_with_button`
- **Description:** Controls an LED (port F 1/2/3) using a button (port F 0/4).
- [Video Demo](https://youtu.be/uZM3S-P_QH0)

### 2. Different LEDs at the same time
- **Code File:** `a2_Color_LED_with_button`
- **Description:** Controls three different color LEDs using two buttons.
- [Video Demo](https://youtu.be/-cigAuxJvFA)

### 3. Blinking LED
- **Code File:** `a3_LED_blink_on_button`
- **Description:** Implements a delay function to blink an LED periodically.
- [Video Demo](https://youtu.be/_e20fkPInfk)

### 4. Practical Application: Heart Piece Maker
- **Code File:** `a4_Heart_piece_maker`
- **Description:** Generates a periodic heartbeat pattern using the delay function.
- [Video Demo](https://youtu.be/jsTTCv8WGcw)

### 5. Blinking LED using External Circuits
- **Code File:** `a5_LED_blink_portE`
- **Description:** Demonstrates LED blinking using external LED and push button.
- [Video Demo](https://youtu.be/RJPthKYKAvI)

### 6. Delay function using a built-in clock
- **Code File:** `a6_delay_using_systick_timer`
- **Description:** Utilizes the system clock for more accurate delay implementation.
- [Video Demo](https://youtu.be/pgsQGlkQow4)

### 7. Changing the frequency of the clock
- **Code File:** `a7_changing_hz_of_timer`
- **Description:** Configures the onboard second clock to use frequencies from 3MHz to 80MHz for precise timing.

### 8. DC Motor Direction Control
- **Code File:** `a8_DC_Motor_Control_using_L293D`
- **Description:** Controls the rotation direction of a DC motor using a push button.
- [Video Demo](https://youtu.be/7iFsb9ENQj0)

### 9. UART Communication: Sending ASCII characters
- **Code File:** `a09_UART_Commu_to_send_ascii_char`
- **Description:** Demonstrates UART0 communication on portC, monitored via an oscilloscope.
- [Video Demo](https://youtu.be/B_e1Zu_2wYs?si=y7zaa07DqUy2mtpz)

### 10. UART Communication: Sending "Hello World"
- **Code File:** `a10_UART_Commu_Sending_Data`
- **Description:** Uses UART0 on portC to send a "Hello World" message to the monitor.
- [Video Demo](https://youtu.be/w4y-p872lTk)

### 11. UART Communication: Receiving Real-time Commands
- **Code File:** `a11_UART_Commu_Receiveing_Data`
- **Description:** Receives commands via UART to control LED colors in real-time.
- [Video Demo](https://youtu.be/t_uaFNJF0c8)

### 12. Edge-Triggered Interrupt
- **Code File:** `a12_edge_triggered_Interrupt`
- **Description:** Demonstrates the use of system edge-triggered interrupts for efficient event-driven programming.

### 13. Generating Periodic Interrupts
- **Code File:** `a13_generation_periodic_inturrupts`
- **Description:** Utilizes the SysTick timer to generate periodic interrupts for precise timing applications.

### 14. PWM Generation for DC Motor Control
- **Code File:** `a14_generation_PWM_to_controll_dc_motors`
- **Description:** Generates Pulse Width Modulation (PWM) signals to control DC motor speed and direction.

## Contributing

Contributions are welcome! Please fork this repository and submit pull requests for any enhancements or bug fixes.

## License

This project is licensed under the MIT License. See the `LICENSE` file for details.

## Acknowledgments

- Texas Instruments for providing comprehensive resources and support for the TM4C123GH6PM.
- Community contributors who help improve and expand this repository.

## Contact

For questions or support, please open an issue on this repository or contact me at abhismirai10@gmail.com or via WhatsApp at +1 4345699343.
```

This updated README.md file now includes all the projects you mentioned, maintaining a consistent structure and providing detailed information for each. The new projects (12-14) have been integrated seamlessly into the existing format. Let me know if you'd like any further modifications or additional details added to this README.
