/*
 * Simple example to show interaction with an FPGA circuit:
 * first configure the FPGA with a bitstream compiled into
 * this program, then use SPI to communicate with the
 * configured circuit:  reading the onboard switch settings
 * and printing them to the USB Serial port whenever they change.
 * Output can be read using the Arduino Serial Monitor tool.
 */

#include <MyStorm.h>
#include <SPI.h>

/*
 * FPGA configuration bitstream built from fpga/readswitches.v
 * by icestorm toolchain (see fpga/makefile).
 */
const byte bitstream[] = {
#include "ice40bitstream.h"
};

bool configured;  // true if FPGA has been configured successfully

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  // wait for serial monitor to be connected (only needed for USB)
  while (!Serial) ;
  // configure the FPGA
  configured = myStorm.FPGAConfig(bitstream, sizeof bitstream);
  if (configured) {
    // disable other connections multiplexed to the SPI bus
    myStorm.muxDisable();
    // configure the slave select pin for SPI
    pinMode(PIN_SPI_SS, OUTPUT);
  }
}

void loop() {
  static int toggle = 0;
  static int prevswitches = -1;
  byte switches;
  if (!configured) {
    // if FPGConfig failed, just flash LED5 at 1Hz
    digitalWrite(LED_BUILTIN, toggle);
    toggle = ~toggle;
    delay(500);
    return;
  }
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE1));
  digitalWrite(PIN_SPI_SS, 0);
  // send a dummy byte and simultaneously receive the input value
  switches = SPI.transfer((byte)0);
  digitalWrite(PIN_SPI_SS, 1);
  SPI.endTransaction();
  // print the received value if it differs from the previous one
  if (switches != prevswitches)
    Serial.println(switches);
  prevswitches = switches;
}
