/*
  Flash a LED given simple serial commands.
  
  Author: Paul Hamilton
  Date: 5-May-2023
  
  This work is licensed under the Creative Commons Attribution 4.0 International License. 
  To view a copy of this license, visit http://creativecommons.org/licenses/by/4.0/ or 
  send a letter to Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.

  https://github.com/visualopsholdings/i2c
*/

#include "ringbuffer.hpp"

#define LED_PIN     13

void ledOn() {
  digitalWrite(LED_PIN, HIGH);
}

void ledOff() {
  digitalWrite(LED_PIN, LOW);
}

RingBuffer buffer;
char cmdbuf[64];

void flashErr(int mode, int n) {
  // it's ok to tie up the device with delays here.
  for (int i=0; i<mode; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(100);
    digitalWrite(LED_PIN, LOW);
    delay(100);
  }
  delay(500);
  for (int i=0; i<n; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(100);
    digitalWrite(LED_PIN, LOW);
    delay(100);
  }
}

// At the start
void setup() {

  // Setup the LED
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // Setup the serial port
  Serial.begin(9600);

}

// Go around and around
void loop() {
  
  // consume the serial data into the buffer as it comes in.
  if (Serial.available()) {
    buffer.write(Serial.read());
  }

  if (buffer.length() > 0) {
    int cmd = buffer.read();
     if (cmd == 43) { // +
      ledOn();
    }
    else if (cmd == 45) { // -
      ledOff();
    }
    else if (cmd == 10) {
      // ignore newlines
    }
    else {
      flashErr(1, 2);
    }
   
  }

}

