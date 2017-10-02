/*
 * Simple example to show the use of SD card files:
 *
 * - list the names of all top level files on the card
 * - then repeatedly read a filename from the Serial (USB CDC-ACM)
 *   port, and use that file to configure the FPGA
 *
 * NB:
 * - Before building, ensure that the setting for DOSFS in the
 *   Tools menu is set to "SDCARD".
 * - Before running, ensure that the onboard switches are in
 *   the OFF position.  The switches are connected to
 *   the same STM32 pins as the SDMMC interface, so setting
 *   the switches to ON would directly connect the SDMMC pins
 *   to ground.
 */

#include <FS.h>
#include <MyStorm.h>

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, 0);
  Serial.begin(9600);
  while (!Serial) ; // wait for USB serial connection to be established
  
  DOSFS.begin();

  Serial.println("SDCard files: ");
  Dir dir = DOSFS.openDir("/");
  do {
    Serial.println(dir.fileName());
  } while (dir.next());
  
  DOSFS.end();
}

void loop() {
  char fileName[64];

  // prompt and read the name of a bitstream file
  Serial.print("Bitstream file for ICE40: ");
  if (readLine(Serial, fileName, sizeof fileName) == 0)
    return;
  Serial.println();

  // open the file on the SD card file system
  DOSFS.begin();
  File file = DOSFS.open(fileName, "r");
  if (file) {
    // use the file as an Arduino Stream to configure the FPGA
    digitalWrite(LED_BUILTIN, 1);
    myStorm.FPGAConfigure(file);
    digitalWrite(LED_BUILTIN, 0);
    file.close();
  } else
    Serial.println("file not found");
  DOSFS.end();
}

/*
 * Read and echo a line from given input stream until terminated
 * by '\n', '\r' or '\0' or until the buffer is full.
 * Discard the terminating character and append a null character.
 * 
 * Returns the number of input characters in the buffer (excluding the null).
 */
int readLine(Stream &str, char *buf, int bufLen)
{
  int c, nread;

  // discard any extra CR or NL left from previous readLine
  do {
    c = str.read();
  } while (c == -1 || c == '\n' || c == '\r');
  // read until buffer until termination character seen or buffer full
  nread = 0;
  while (c != '\0' && c != '\n' && c != '\r') {
    str.write(c);
    buf[nread] = c;
    ++nread;
    if (nread == bufLen - 1)
      break;
    do {
      c = str.read();
    } while (c == -1);
  }
  // mark end of line and return
  buf[nread] = '\0';
  return nread;
}

