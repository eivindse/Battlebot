#include "nRF24L01.h" //NRF24L01 library created by TMRh20 https://github.com/TMRh20/RF24
#include "RF24.h"
#include "SPI.h"

// V A R I A B L E S
int sendArray[5];

// P I N S
int pinLeftY = 0;
int pinLeftX = 1;
int pinRightY = 5;
int pinRightX = 6;
int pinTrigger = 3;

RF24 radio(9, 10); 

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
    radio.openWritingPipe(pipe); 
}

void loop(void) {
  
    sendArray[0] = analogRead(pinLeftY);
    //Serial.println(sendArray[0]);

    sendArray[1] = analogRead(pinLeftX);
    //Serial.print(sendArray[1]);
   
    sendArray[2] = analogRead(pinRightY);
    //Serial.print(sendArray[2]);

    sendArray[3] = analogRead(pinRightX);
    //Serial.print(sendArray[3]);
   
    sendArray[4] = digitalRead(pinTrigger);
    //Serial.print(sendArray[4]);

    sendArray[0] = map(sendArray[0],0,1023,-10,10);
    sendArray[1] = map(sendArray[1],0,1023,-10,10);
    sendArray[2] = map(sendArray[2],0,1023, 10,-10);
    sendArray[3] = map(sendArray[3],0,1023,-10,10);

    for(int i = 0; i<4; i++){
        if(abs(sendArray[i])<=2){
            sendArray[i] = 0;
        } 
        //Serial.println(sendArray[i]); 
    }
    
    radio.write(&sendArray, sizeof(sendArray));
}
