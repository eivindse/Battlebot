#include "nRF24L01.h" //NRF24L01 library created by TMRh20 https://github.com/TMRh20/RF24
#include "RF24.h"
#include "SPI.h"

// V A R I A B L E S
int DirX = 0;
int DirY = 0;
int sendByte = 0;

// P I N S
int pinX = 2;
int pinY = 0;
int pinTrigger = 2;

RF24 radio(9, 10); // NRF24L01 used SPI pins + Pin 9 and 10 on the NANO
const uint64_t pipe = 0xE6E6E6E6E6E6; // Needs to be the same for communicating between 2 NRF24L01 

void setup(void){
    Serial.begin(9600);
    Serial.println("Startup");

    pinMode(pinTrigger, INPUT_PULLUP);

   if (!radio.begin()) {
      Serial.println("Begin false!!");
    } else {
      Serial.println("Stash");
    }
    radio.setChannel(95);
    radio.openWritingPipe(pipe); // Get NRF24L01 ready to transmit
}

void loop(void) {
    /* Read inputs and shift bits in place. */
    DirX = analogRead(2);
    DirY = analogRead(0);
    DirX = map(DirX,0,1023,0,9);
    DirY = map(DirY,0,1023,9,0);

    sendByte = 10*DirX + DirY;
    
    radio.write(&sendByte, sizeof(sendByte));
    //radio.write(&DirX, sizeof(DirX));

    //Serial.println(DirX);
    Serial.println(sendByte);
    //Serial.println(DirY);
    
    delay(100);
}
