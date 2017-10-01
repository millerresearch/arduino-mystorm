/*
 * Simple example to show interaction with an FPGA circuit:
 * first configure the FPGA with a bitstream compiled into
 * this program, then use SPI to send a changing pattern
 * of bits which the FPGA circuit will use to control the
 * onboard LEDs.
 */
#include <MyStorm.h>
#include <SPI.h>

/*
 * FPGA configuration bitstream built from fpga/writeleds.v
 * by icestorm toolchain (see fpga/makefile).
 */
const byte bitstream[] = {
#include "ice40bitstream.h"
};

const int PIN_SPI2_SS = 20;
bool configured;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  // Use SPI2 to communicate with the FPGA,
  // because the primary SPI bus signals are multiplexed
  // with the signals to the LEDs.
  // NB: SPI2 bus signals are shared with signals to the
  // onboard switches, so switches must be in OFF position.
  SPI2.begin();
  pinMode(PIN_SPI2_SS, OUTPUT);
  digitalWrite(PIN_SPI2_SS, 1);
  // configure the FPGA
  configured = myStorm.FPGAConfig(bitstream, sizeof bitstream);
}

void loop() {
  static byte leds = 1;
  static int toggle;

  if (!configured) {
    // if FPGConfig failed, just flash LED5 at 1Hz
    digitalWrite(LED_BUILTIN, toggle);
    toggle = ~toggle;
    delay(500);
    return;
  }
  SPI2.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE1));
  digitalWrite(PIN_SPI2_SS, 0);
  // send current bit pattern for the LEDs, ignoring the received value
  (void)SPI2.transfer(leds);
  digitalWrite(PIN_SPI2_SS, 1);
  SPI2.endTransaction();
  // rotate the bit pattern
  leds = ((leds<<1)&0xF) | (leds>>3);
  delay(100);
}
