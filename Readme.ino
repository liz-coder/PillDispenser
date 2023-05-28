# MedMinder

This is a code repository for an Arduino-based automated pill dispenser named "MedMinder". This project is aimed to assist individuals who require regular medication, reducing the guesswork and stress of managing complex medication schedules.

## Features

- Real-Time Clock (RTC) based scheduling for accurate pill dispensing
- I2C LCD for displaying current time and pill dispensing time
- Water pump controlled dispensing for easy pill intake
- Audio-visual alerts via a buzzer and LED 

## Components Used

- Arduino (Uno/Nano/Mega)
- DS3231 RTC module
- 16x2 LCD with I2C module
- Servo Motor (for pill dispensing mechanism)
- IRF3710 MOSFET (for controlling water pump)
- 1N4007 Diode
- Buzzer and LED for alerts
- Various buttons for user interface

## Usage

Clone or download this repository and open `MedMinder.ino` with the Arduino IDE. You can modify the pill dispense time and quantity as per your needs. 

Please refer to the comments in the code for more detailed information about each function and variable.

## Circuit

Here are the connections you need to make:

1. RTC module SDA to Arduino A4, SCL to Arduino A5. Connect VCC to 5V and GND to GND.
2. I2C LCD VCC to 5V, GND to GND, SDA to A4, and SCL to A5.
3. Servo motor's signal wire (usually orange or yellow) to Arduino A0. Connect the power wire (usually red) to 5V and GND wire (usually brown or black) to GND.
4. Water Pump: Source of IRF3710 to Ground, Drain to one terminal of the pump. The other terminal of the pump to positive of the power supply. The Gate of the IRF3710 to digital pin 6 on the Arduino via a 10K resistor.
5. Connect a 1N4007 diode across the water pump, with the cathode (end with the stripe) connected to the positive supply and the anode to the Drain of the IRF3710.
6. Connect the Buzzer and LED to digital pins 5 and 13 respectively. Make sure to connect the other terminal to ground (use a current limiting resistor for LED).
7. Connect button pins to digital pins 2, 3, 4, 7, 8, and 9, and the other terminal of buttons to ground. Enable the internal pull-up resistors in the code.

Please note that it's recommended to use an external power supply for the water pump and the servo motor, as they can draw more current than the Arduino's 5V pin can provide. Make sure to connect the grounds of the Arduino and the external power supply together.

## License

This project is open-sourced under the MIT License. Feel free to use the code for your projects, but remember to provide attribution if you do so. 
