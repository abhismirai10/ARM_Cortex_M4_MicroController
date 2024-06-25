# ARM Cortex M4 MicroController

## Intro

This repository contains code and documentation for the ARM Cortex-M4 microcontroller, specifically the TM4C123GH6PM model found on the TIVA LaunchPAD developer kit (any Cortex-M4 will work!). The code provided will cover various applications ranging from simple LED blinking to advanced communication protocols and many more. I will not use any libraries like `arduino.h` or others, so everything is implemented from scratch. This repo is not just useful for learning about microcontrollers from beginner to advanced levels, but also you can learn C/C++ or assembly using this repo. As a very intelligent person once said, "Any computer language can be learned by using it in real life!"


## Table of Contents
1. [Intro](#intro)
2. [Hardware](#hardware)
3. [Getting Started](#getting-started)
    - [Prerequisites](#prerequisites)
    - [Installation](#installation)
4. [Code Details](#code-details)
    - [1. Simple LED on/off](#1-simple-led-onoff)
    - [2. Different LEDs at the same time](#2-different-leds-at-the-same-time)
    - [3. Blinking LED](#3-blinking-led)
5. [Contributing](#contributing)
6. [License](#license)
7. [Acknowledgments](#acknowledgments)
8. [Contact](#contact)

## Hardware

The TIVA LaunchPAD features an ARM Cortex-M4 microcontroller named TM4C123GH6PM. The TM4C123GH6PM microcontroller is a powerful and versatile processor, well-suited for various embedded systems applications. This repository will guide you through setting up the hardware and software, as well as provide detailed instructions and examples for different projects. I strongly recommend downloading datasheets for the processor and kit, as they are very useful. Below is an image and a link to the developer kit.

![TIVA LaunchPAD](https://github.com/abhismirai10/ARM_Cortex_M4_MicroController/assets/121724635/13acc5be-48a2-4a26-89b3-509de1291eda)  
[TM4C123GH6PM TIVA LaunchPAD Developer Kit](https://www.ti.com/tool/EK-TM4C123GXL


## Getting Started

### Prerequisites

- **Hardware:**
  - TM4C123GH6PM TIVA LaunchPAD
  - LEDs, resistors, servo motors, and other components as required by each example.
- **Software:**
  - Texas Instruments Code Composer Studio (CCS)
  - Drivers and libraries for the TM4C123GH6PM (if you are using Windows! Do not use Windows)

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

I am using C/C++ for coding this microcontroller, but you can use assembly as well if you want to write the most efficient code possible! Most of the sketches are well documented, but I will add details below as per the difficulty of implementation.

### 1. Simple LED on/off
- **Code File:** `a1_LED_with_button`
- **Description:** This code will turn on/off the LED (port F 1/2/3) using the button (port F 0/4).
- [Video](https://youtu.be/uZM3S-P_QH0)

### 2. Different LEDs at the same time
- **Code File:** `a2_Color_LED_with_button`
- **Description:** Same as before but this time using 3 different color LEDs with two buttons.
- [Video](https://youtu.be/-cigAuxJvFA)

### 3. Blinking LED
- **Code File:** `a3_LED_blink_on_button`
- **Description:** This code will implement a delay function, using which we will blink the LED periodically.
- [Video](https://youtu.be/_e20fkPInfk)

### 4. Practical Application
- **Code File:** `a4_Heart _piece_maker`
- **Description:** We can generate a periodic heartbeat using the delay function.
- [Video](https://youtu.be/jsTTCv8WGcw)

### 5. Blinking LED using External Circuits
- **Code File:** `a5_LED_blink_portE`
- **Description:** Using External LED and Push Button.
- [Video](https://youtu.be/RJPthKYKAvI)

### 6. Delay function using a built-in clock
- **Code File:** `a6_delay_using_systick_timer`
- **Description:** Up until now we have been using a kind of hack for delay now we will use a system clock for the delay.
- [Video](https://youtu.be/pgsQGlkQow4)

### 7. Changing the frequency of the clock using the onboard second clock
- **Code File:** `a7_changing_hz_of_timer`
- **Description:** There are two clocks in this launch pad, to use the second one we need to configure it, and that way
                   we can use the clock in the frequency range of 3MHz to 80Mhz for precise timing.

## Contributing

Contributions are welcome! Please fork this repository and submit pull requests for any enhancements or bug fixes.

## License

This project is licensed under the MIT License. See the `LICENSE` file for details.

## Acknowledgments

- Texas Instruments for providing comprehensive resources and support for the TM4C123GH6PM.
- Community contributors who help improve and expand this repository.

## Contact

For any questions or support, please open an issue on this repository or contact me at [abhismirai10@gmail.com] or use WhatsApp at +1 4345699343.
