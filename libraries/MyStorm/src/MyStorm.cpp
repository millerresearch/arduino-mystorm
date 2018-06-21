#include <Arduino.h>
#include <SPI.h>
#include "MyStorm.h"

// Synchronisation marker at the beginning of an ICE40 bitstream
const byte syncBytes[] = { 0x7E, 0xAA, 0x99, 0x7E };
// Length of a bitstream file (excluding empty initial comment and marker)
const int bitstreamBodyLength = 135100 - 8;

// Arduino Stream subclass to read data from a byte[] array
//   or from a given address in (flash) memory 
class MemStream : public Stream {
  private:
    const byte *addr;    // address of next byte to read
    int len;             // number of bytes left to read
  public:
    MemStream(const byte *addr, int len) : addr(addr), len(len) { }
    int available(void) { return len; }
    int read(void) { if (len <= 0) return 0; len--; return *addr++; }
    int peek(void) { if (len <= 0) return 0; return *addr; }
    size_t write(byte) { return 0; }
    size_t write(byte*, size_t) { return 0; }
    void flush(void) { }
};

// Put ICE40 into SPI slave configuration mode
// returns true iff successful
static bool beginConfig(void)
{
  pinMode(PIN_ICE40_CRST, OUTPUT);
  pinMode(PIN_ICE40_CDONE, INPUT);
  digitalWrite(PIN_ICE40_CRST, 0);
  digitalWrite(PIN_SPI_SS, 0);
  delay(1);
  digitalWrite(PIN_ICE40_CRST, 1);
  for (int i = 100; digitalRead(PIN_ICE40_CDONE) == 1; i--) {
    if (i == 0)
      return false;
  }
  delay(2);
  return true;
}

// Check ICE40 CDONE signal for successful configuration, and send
//   required extra SPI clocks to allow pins to switch to user mode
// returns true iff successful
static bool endConfig(void)
{
  for (int i = 100; digitalRead(PIN_ICE40_CDONE) != 1; i--) {
    if (i == 0)
      return false;
    SPI.write(0);
  }
  for (int i = 0; i < 7; i++)
    SPI.write(0);
  return true;
}

// Disable the SPI muliplexor output so extra signals won't interfere with SPI
void MyStorm::muxDisable(void)
{
  digitalWrite(PIN_MUX_OE, 1);
}

// Enable the SPI muliplexor output
void MyStorm::muxEnable(void)
{
  digitalWrite(PIN_MUX_OE, 0);
}

// Enable the SPI multiplexor and connect shared SPI signals to LED1-4
void MyStorm::muxSelectLeds(void)
{
  digitalWrite(PIN_MUX_S, 0);
  digitalWrite(PIN_MUX_OE, 0);
}

// Enable the SPI multiplexor and connect shared SPI signals to RPI header
void MyStorm::muxSelectPi(void)
{
  digitalWrite(PIN_MUX_S, 1);
  digitalWrite(PIN_MUX_OE, 0);
}

// This class is used only for its destructor, to clean up
//   on return from FPGAConfigure whether successful or not
class OnExit {
  public: ~OnExit() {
    SPI.endTransaction();
    digitalWrite(PIN_SPI_SS, 1);
    pinMode(PIN_SPI_SS, INPUT);
  }
};

// Configure the ICE40 with a bitstream read from an Arduino stream
// returns true iff successful
bool MyStorm::FPGAConfigure(Stream &str)
{
  static byte buf[64];
  int nbytes, lastread;

  // read and skip anything before the sync marker
  nbytes = 0;
  do {
    lastread = millis();
    while (!str.available()) {
      if (millis() - lastread > 1000)
        return false;
    }
    buf[nbytes] = str.read();
    if (buf[nbytes] == syncBytes[nbytes])
      ++nbytes;
    else
      nbytes = 0;
  } while (nbytes != sizeof syncBytes);

  if (!SPI.isEnabled())
    SPI.begin();
  muxDisable();
  pinMode(PIN_SPI_SS, OUTPUT);

  if (!beginConfig())
    return false;
  
  SPI.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE3));
  OnExit spiDetacher = OnExit();
  SPI.write(syncBytes, sizeof syncBytes);
  for (int nleft = bitstreamBodyLength; nleft > 0; nleft -= nbytes) {
    int lastread = millis();
    do {
      nbytes = str.available();
      if (millis() - lastread > 1000)
        return false;
    } while (nbytes == 0);
    if (nbytes > sizeof buf)
      nbytes = sizeof buf;
    if (nbytes > nleft)
      nbytes = nleft;
    str.read(buf, nbytes);
    SPI.write(buf, nbytes);
  }
  if (!endConfig())
    return false;
  muxSelectLeds();
  return true;
}

// Configure the ICE40 with a bitstream from memory or flash
// returns true iff successful
bool MyStorm::FPGAConfigure(const byte *bitstream, int bitstreamLength)
{
  // skip optional comment header FF 00 ... comments ... 00 FF
  if (bitstreamLength >= 4 && bitstream[0] == 0xFF && bitstream[1] == 0x00) {
    bitstream += 2;
    bitstreamLength -= 2;
    while (!(bitstream[0] == 0x00 && bitstream[1] == 0xFF)) {
      ++bitstream;
      --bitstreamLength;
      if (bitstreamLength < 2)
        return false;
    }
    bitstream += 2;
    bitstreamLength -= 2;
  }
  // check for sync marker
  if (bitstreamLength < sizeof syncBytes)
    return false;
  for (int i = 0; i < sizeof syncBytes; i++) {
    if (bitstream[i] != syncBytes[i])
      return false;
  }
  MemStream m = MemStream(bitstream, bitstreamLength);
  return FPGAConfigure(m);
}

// Constructor
MyStorm::MyStorm(void)
{
}

MyStorm myStorm;
