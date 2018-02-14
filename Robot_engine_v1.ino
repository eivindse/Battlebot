#include "nRF24L01.h" // NRF24L01 library created by TMRh20 https://github.com/TMRh20/RF24
#include "RF24.h"
#include "SPI.h"

/* Arduino / Radio code*/

RF24 radio(9, 10);
const uint64_t pipe = 0xE6E6E6E6E6E6;

// V A R I A B L E S
int movement[5];
int DirX = 0;
int DirY = 0;
int speeds = 0;
int turn = 0;

// P I N S
int RIGHT_ENABLE = 3;
int RIGHT_FORWARD = 4;
int RIGHT_BACKWARD = 6;
int LEFT_ENABLE = 5;
int LEFT_FORWARD = 8;
int LEFT_BACKWARD = 7;
int LAZER = 2;

void setup(void) {
    Serial.begin(9600);
    Serial.println("Startup");
    
   if (!radio.begin()) {
      Serial.println("Begin false!!");
    } else {
      Serial.println("Stash");
    }
    radio.setChannel(95);
    radio.openReadingPipe(1, pipe); // Get NRF24L01 ready to receive
    radio.startListening(); // Listen to see if information received

    pinMode(2, OUTPUT);
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);
    pinMode(7, OUTPUT);
    pinMode(8, OUTPUT);
    
}

void loop(void) {
  
    while (radio.available()) {
    radio.read(&movement, sizeof(movement));
    
        for(int i = 0;i<4;i++){
           //Serial.print(movement[i]);
        }
    }
    
    speeds = movement[2];
    turn = movement[1];
    
    if( speeds > 0 ){

      digitalWrite(RIGHT_FORWARD, HIGH);
      digitalWrite(RIGHT_BACKWARD, LOW);
      analogWrite(RIGHT_ENABLE, 25*abs(speeds-turn));
      digitalWrite(LEFT_FORWARD, HIGH);
      digitalWrite(LEFT_BACKWARD, LOW);
      analogWrite(LEFT_ENABLE, 25*abs(speeds+turn));
    }
    else if( speeds < 0 ){ 
      speeds=abs(speeds);
      digitalWrite(RIGHT_FORWARD, LOW);
      digitalWrite(RIGHT_BACKWARD, HIGH);
      analogWrite(RIGHT_ENABLE, 25*abs(speeds-turn));
      digitalWrite(LEFT_FORWARD, LOW);
      digitalWrite(LEFT_BACKWARD, HIGH);
      analogWrite(LEFT_ENABLE, 25*abs(speeds+turn));
    }
    else {
      digitalWrite(RIGHT_FORWARD, HIGH);
      digitalWrite(RIGHT_BACKWARD, HIGH);
      digitalWrite(LEFT_FORWARD, LOW);
      digitalWrite(LEFT_BACKWARD, LOW);
    }

    //Serial.println(speeds+turn);
    //Serial.println(speeds-turn);
    //Serial.println(speeds);
    //Serial.println(turn);
}
