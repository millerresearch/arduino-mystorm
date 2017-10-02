/*
 * Configure ICE40 with bitstream from offset 0x0801F000 in STM32 flash
 * (if present), and then repeatedly from Serial (USB CDC-ACM) port
 */

#include <MyStorm.h>

void setup()
{
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, 1);
  myStorm.FPGAConfigure((const byte*)0x0801F000, 135100);
  digitalWrite(LED_BUILTIN, 0);
}

void loop()
{
  if (!Serial.available())
    return;
  digitalWrite(LED_BUILTIN, 1);
  if (myStorm.FPGAConfigure(Serial)) {
    while (Serial.available())
      Serial.read();
  }
  digitalWrite(LED_BUILTIN, 0);
}

