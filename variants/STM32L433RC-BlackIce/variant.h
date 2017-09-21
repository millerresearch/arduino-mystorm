/*
 * Copyright (c) 2016 Thomas Roell.  All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal with the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 *  1. Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimers.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimers in the
 *     documentation and/or other materials provided with the distribution.
 *  3. Neither the name of Thomas Roell, nor the names of its contributors
 *     may be used to endorse or promote products derived from this Software
 *     without specific prior written permission.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * CONTRIBUTORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * WITH THE SOFTWARE.
 */

#ifndef _VARIANT_BLACKICE_STM32L433RC_
#define _VARIANT_BLACKICE_STM32L433RC_

// The definitions here need a STM32L4 core >=1.6.6
#define ARDUINO_STM32L4_VARIANT_COMPLIANCE 10606

/*----------------------------------------------------------------------------
 *        Definitions
 *----------------------------------------------------------------------------*/

#define STM32L4_CONFIG_LSECLK             32768
#define STM32L4_CONFIG_HSECLK             0
#define STM32L4_CONFIG_SYSOPT             0
#define STM32L4_CONFIG_USB_VBUS           GPIO_PIN_NONE

#define USBCON

/** Master clock frequency */
#define VARIANT_MCK			  F_CPU

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

#ifdef __cplusplus
#include "USBAPI.h"
#include "Uart.h"
#endif // __cplusplus

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/*----------------------------------------------------------------------------
 *        Pins
 *----------------------------------------------------------------------------*/

// Number of pins defined in PinDescription array
#define PINS_COUNT           (29u)
#define NUM_DIGITAL_PINS     (24u)
#define NUM_TOTAL_PINS       (48u)
#define NUM_ANALOG_INPUTS    (6u)
#define NUM_ANALOG_OUTPUTS   (2u)
#define analogInputToDigitalPin(p)  ((p < 6u) ? (p) + 14u : -1)

// LEDs
#define LED_BUILTIN          (47u)

/*
 * Analog pins
 */
#define PIN_A0               (14u)
#define PIN_A1               (15u)
#define PIN_A2               (16u)
#define PIN_A3               (17u)
#define PIN_A4               (18u)
#define PIN_A5               (19u)
#define PIN_DAC0             (5u)
#define PIN_DAC1             (9u)

static const uint8_t A0  = PIN_A0;
static const uint8_t A1  = PIN_A1;
static const uint8_t A2  = PIN_A2;
static const uint8_t A3  = PIN_A3;
static const uint8_t A4  = PIN_A4;
static const uint8_t A5  = PIN_A5;
static const uint8_t DAC0 = PIN_DAC0;
static const uint8_t DAC1 = PIN_DAC1;
#define ADC_RESOLUTION		12
#define DAC_RESOLUTION		12

// Other pins

#define PIN_BUTTON1           (45u)
#define PIN_BUTTON2           (46u)
static const uint8_t BUTTON1 = PIN_BUTTON1;
static const uint8_t BUTTON2 = PIN_BUTTON2;
#define PIN_BUTTON	PIN_BUTTON1
static const uint8_t BUTTON = PIN_BUTTON;

/*
 * Serial interfaces
 */

#define SERIAL_INTERFACES_COUNT 4

#define PIN_SERIAL1_RX       (33ul)
#define PIN_SERIAL1_TX       (32ul)
#define PIN_SERIAL2_RX       (35ul)
#define PIN_SERIAL2_TX       (34ul)
#define PIN_SERIAL3_RX       (1ul)
#define PIN_SERIAL3_TX       (0ul)

/*
 * SPI Interfaces
 */
#define SPI_INTERFACES_COUNT 2

#define PIN_SPI_MISO         (26u)
#define PIN_SPI_MOSI         (28u)
#define PIN_SPI_SCK          (27u)
#define PIN_SPI1_MISO        (21u)
#define PIN_SPI1_MOSI        (20u)
#define PIN_SPI1_SCK         (22u)

static const uint8_t SS	  = 42;
static const uint8_t MOSI = PIN_SPI_MOSI;
static const uint8_t MISO = PIN_SPI_MISO;
static const uint8_t SCK  = PIN_SPI_SCK;

/*
 * Wire Interfaces
 */
#define WIRE_INTERFACES_COUNT 3

#define PIN_WIRE_SDA         (24u)
#define PIN_WIRE_SCL         (25u)
#define PIN_WIRE1_SDA        (12u)
#define PIN_WIRE1_SCL        (13u)
#define PIN_WIRE2_SDA        (26u)
#define PIN_WIRE2_SCL        (38u)

static const uint8_t SDA = PIN_WIRE_SDA;
static const uint8_t SCL = PIN_WIRE_SCL;

/*
 * I2S Interfaces
 */
#define I2S_INTERFACES_COUNT 1

#define PIN_I2S_SCK          (10u) pb12
#define PIN_I2S_FS           (13u) pb13
#define PIN_I2S_SD           (12u) pb14
#define PIN_I2S_MCK          (11u) pb15

/*
 * USB
 */
//#define PIN_USB_VBUS         (29u)
#define PIN_USB_DM           (30u)
#define PIN_USB_DP           (31u)

#define PWM_INSTANCE_COUNT   4

#ifdef __cplusplus
}
#endif

/*----------------------------------------------------------------------------
 *        Arduino objects - C++ only
 *----------------------------------------------------------------------------*/

#ifdef __cplusplus
extern CDC  Serial;
extern Uart Serial1;
extern Uart Serial2;
extern Uart Serial3;
#endif

// These serial port names are intended to allow libraries and architecture-neutral
// sketches to automatically default to the correct port name for a particular type
// of use.  For example, a GPS module would normally connect to SERIAL_PORT_HARDWARE_OPEN,
// the first hardware serial port whose RX/TX pins are not dedicated to another use.
//
// SERIAL_PORT_MONITOR        Port which normally prints to the Arduino Serial Monitor
//
// SERIAL_PORT_USBVIRTUAL     Port which is USB virtual serial
//
// SERIAL_PORT_LINUXBRIDGE    Port which connects to a Linux system via Bridge library
//
// SERIAL_PORT_HARDWARE       Hardware serial port, physical RX & TX pins.
//
// SERIAL_PORT_HARDWARE_OPEN  Hardware serial ports which are open for use.  Their RX & TX
//                            pins are NOT connected to anything by default.
#define SERIAL_PORT_USBVIRTUAL      Serial
#define SERIAL_PORT_MONITOR         Serial
#define SERIAL_PORT_HARDWARE1       Serial1
#define SERIAL_PORT_HARDWARE2       Serial2
//#define SERIAL_PORT_HARDWARE3       Serial3
#define SERIAL_PORT_HARDWARE_OPEN1  Serial3
//#define SERIAL_PORT_HARDWARE_OPEN2  Serial2
//#define SERIAL_PORT_HARDWARE_OPEN3  Serial3

// Alias SerialUSB to Serial
#define SerialUSB SERIAL_PORT_USBVIRTUAL

// BlackIce specific pin assignments
#define PIN_MUX_S	(40u)
#define PIN_MUX_OE	(41u)
#define PIN_ICE40_CRST	(43u)
#define PIN_ICE40_CDONE	(44u)


#endif /* _VARIANT_BLACKICE_STM32L433RC_ */

