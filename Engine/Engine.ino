#include "nRF24L01.h" // NRF24L01 library created by TMRh20 https://github.com/TMRh20/RF24
#include "RF24.h"
#include "SPI.h"

// CONSTANTS

// Pins
#define RIGHT_ENABLE 3
#define RIGHT_FORWARD 4
#define RIGHT_BACKWARD 6
#define LEFT_ENABLE 5
#define LEFT_FORWARD 8
#define LEFT_BACKWARD 7
#define LAZER 2

// Safety
#define TIMEOUT 1000 // Maximum waiting time in milliseconds

// VARIABLES

RF24 radio(9, 10);
const uint64_t pipe = 0xE6E6E6E6E6E6;

int movement[5];
int DirX = 0;
int DirY = 0;
int speeds = 0;
int turn = 0;

int motor_speed = 0;
int motor_turn = 0;
unsigned long timestamp = 0;

void setup(void) {
    if (!radio.begin()) {
      //Serial.println("Begin false!!");
    } else {
      //Serial.println("Stash");
    }
    radio.setChannel(95);
    radio.openReadingPipe(1, pipe); // Get NRF24L01 ready to receive
    radio.startListening(); // Listen to see if information received

    pinMode(RIGHT_ENABLE, OUTPUT);
    pinMode(RIGHT_FORWARD, OUTPUT);
    pinMode(RIGHT_BACKWARD, OUTPUT);
    pinMode(LEFT_ENABLE, OUTPUT);
    pinMode(LEFT_FORWARD, OUTPUT);
    pinMode(LEFT_BACKWARD, OUTPUT);
    pinMode(LAZER, OUTPUT);

    //Serial.begin(9600);
    //Serial.println("Startup");
    
    timestamp = millis();
}

void loop(void) {
    while (radio.available()) {
      radio.read(&movement, sizeof(movement));
      
      speeds = movement[2];
      turn = movement[1];

      motorSpeed(speeds);
      motorTurn(turn);

      timestamp = millis();
    }

    if (millis() - timestamp > TIMEOUT) {
      motorSpeed(0);
      timestamp = millis();
    }
    
    //Serial.println(speeds+turn);
    //Serial.println(speeds-turn);
    //Serial.println(speeds);
    //Serial.println(turn);
}

void motorSpeed(int new_speed) {
  motor_speed = new_speed;
  updateMotors();
}

void motorTurn(int new_turn) {
  motor_turn = new_turn;
  updateMotors();
}

void updateMotors() {
  if (motor_speed >= 0) {
    digitalWrite(RIGHT_FORWARD, HIGH);
    digitalWrite(RIGHT_BACKWARD, LOW);
    digitalWrite(LEFT_FORWARD, HIGH);
    digitalWrite(LEFT_BACKWARD, LOW);
  } else {
    digitalWrite(RIGHT_FORWARD, LOW);
    digitalWrite(RIGHT_BACKWARD, HIGH);
    digitalWrite(LEFT_FORWARD, LOW);
    digitalWrite(LEFT_BACKWARD, HIGH);
  }
  analogWrite(RIGHT_ENABLE, 25 * abs(motor_speed - motor_turn));
  analogWrite(LEFT_ENABLE, 25 * abs(motor_speed + motor_turn));
}

