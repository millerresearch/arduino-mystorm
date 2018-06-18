/*
 * Simple example of QSPI communication
 *
 * STM32 reads switch settings from ICE40, and sends them
 * back to be used as LED settings.
 *
 * Settings are printed to USB Serial when they change
 * (Not de-bounced).
 */

#include <MyStorm.h>
#include <QSPI.h>

/*
 * FPGA configuration bitstream built from fpga/swtoleds.v
 * by icestorm toolchain (see fpga/makefile).
 */
const byte bitstream[] = {
#include "ice40bitstream.h"
};

bool configured;  // true if FPGA has been configured successfully

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  QSPI.begin(40000000, QSPI.Mode3);
  Serial.begin(9600);
  // configure the FPGA
  configured = myStorm.FPGAConfigure(bitstream, sizeof bitstream);
  QSPI.beginTransaction();
}

void loop() {
  static int toggle = 0;
  static byte leds = 0;
  byte switches;

  if (!configured) {
    // if FPGConfig failed, just flash LED5 at 1Hz
    digitalWrite(LED_BUILTIN, toggle);
    toggle = ~toggle;
    delay(500);
    return;
  }

  if (!QSPI.write(&leds, 1))
    Serial.println("QSPI.transmit failed");
  if (!QSPI.read(&switches, 1))
    Serial.println("QSPI.receive failed");

  // print the received value if it differs from the previous one
  if (switches != leds)
    Serial.println(switches);
  leds = switches;
}
