# Arduino board support for MyStorm BlackIce (STM32L4 + ICE40 FPGA)

### Pin assignments

| Arduino Pins    | BlackIce Usage |
| --------------- | -------------- |
| 0-7             | DIGITAL1 1-8   |
| 8-13            | DIGITAL2 1-6   |
| 14-19 (AN0-AN5) | ANALOG 1-7     |
| 20-23           | DIGITAL3 1-4   |
| *also*          | Switches (S1,S2) |
| 24-25 (SDA,SCL) | DIGITAL2 9-10  |
| 40-41           | SPI Mux (S,OE) |
| 42-44           | ICE40 (SS,CRST,DONE) |
| 45-46           | Buttons (B1,B2) |
| 47              | LED (SL1)      |

AnalogWrite is supported as PWM on pins 3, 4, 6, 11, 12, 18 19,
and as 12-bit DAC on pins 5, 9.

### Serial ports

| Name      | Connections RX,TX  |
| --------- | -----------        |
| Serial    | USB (middle connector) |
| Serial1   | CH340 USB (corner connector) |
| *also*    | RPI 8,10           |
| *also*    | ICE40 88,85 (PMOD1 3,4) |
| Serial2   | ICE40 82,81        |
| Serial3   | DIGITAL1 1,0       |

### SPI

| Name      | Connections MOSI,MISO,SCK |
| --------- | ------------------------- |
| SPI       | ICE40 config 68,67,70     |
| SPI1      | ANALOG 7,6,5              |

### I2C

| Name      | Connections SCL,SDA |
| --------- | ------------------- |
| Wire      | DIGITAL2 10,9       |
| *also*    | RPI 5,3             |
| Wire1     | DIGITAL2 6,5        |
| Wire2     | ICE40 79, 67        |

## Credits

This package is an extension to Thomas Roell's [grumpyoldpizza](https://github.com/GrumpyOldPizza/arduino-STM32L4) package, which provides the STM32L4 core.
