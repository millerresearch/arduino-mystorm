/*
 * Library class for using the ICE40 FPGA on myStorm boards
 */

#ifndef MyStorm_h
#define MyStorm_h

class MyStorm {
  public:
    MyStorm();
    // Configure the FPGA with a bitstream from a compiled-in array
    //   or an address in flash memory
    bool FPGAConfig(const byte *bitstream, int bitstreamLength);
    // Configure the FPGA with a bitstream from a Serial input stream
    //   (eg a UART or a file on SD card)
    bool FPGAConfig(Stream &str);
    // Disable output of SPI multiplexor to prevent interference
    //   with SPI channel to FPGA
    void muxDisable(void);
    // Re-enable output of SPI multiplexor
    void muxEnable(void);
    // Enable output of SPI multiplexor and connect to onboard LEDs
    void muxSelectLeds(void);
    // Enable output of SPI multiplexor and connect to external SPI
    //  pins on Pi connector
    void muxSelectPi(void);
};

extern MyStorm myStorm;

#endif
