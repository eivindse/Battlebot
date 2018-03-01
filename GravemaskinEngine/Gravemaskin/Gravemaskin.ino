
//#include "nRF24L01.h" // NRF24L01 library created by TMRh20 https://github.com/TMRh20/RF24
//#include "RF24.h"
#include <PS2X_lib.h>  //for v1.6
//#include "SPI.h"

// CONSTANTS

//PS2
#define PS2_DAT        13  //14    
#define PS2_CMD        11  //15
#define PS2_SEL        10  //16
#define PS2_CLK        12  //17

#define pressures   false
//#define rumble      true
#define rumble      false

//PS2 end

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

PS2X ps2x; // create PS2 Controller Class

int error = 0;
byte type = 0;
byte vibrate = 0;

// Motor variables
int left_speed = 0;
int right_speed = 0;

unsigned long timestamp = 0;

void setup(void) {
  // Set up pins
  pinMode(RIGHT_ENABLE, OUTPUT);
  pinMode(RIGHT_FORWARD, OUTPUT);
  pinMode(RIGHT_BACKWARD, OUTPUT);
  pinMode(LEFT_ENABLE, OUTPUT);
  pinMode(LEFT_FORWARD, OUTPUT);
  pinMode(LEFT_BACKWARD, OUTPUT);
  pinMode(LAZER, OUTPUT);

  //Disable lazer to begin with
  digitalWrite(LAZER, LOW);

  Serial.begin(9600);
    
  delay(300);  //added delay to give wireless ps2 module some time to startup, before configuring it

  while (ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble) > 0) {
    Serial.println("No controller found, check wiring, see readme.txt");
    delay(1000);
  }

  Serial.println("Controller connected");
  type = ps2x.readType(); 
}

void loop(void) {
  // Read from controller
  ps2x.read_gamepad(); //read controller and set large motor to spin at 'vibrate' speed

  int left_speed = constrain(ps2x.Analog(PSS_LY) - 128, -127, 127);
  int right_speed = constrain(ps2x.Analog(PSS_RY) - 128, -127, 127);

  updateMotors();
}


void updateMotors() {

  if (left_speed > 0) {
    digitalWrite(LEFT_FORWARD, HIGH);
    digitalWrite(LEFT_BACKWARD, LOW); 
  } else {
    digitalWrite(LEFT_FORWARD, LOW);
    digitalWrite(LEFT_BACKWARD, HIGH);
  }

  if (right_speed > 0) {
    digitalWrite(RIGHT_FORWARD, HIGH);
    digitalWrite(RIGHT_BACKWARD, LOW); 
  } else {
    digitalWrite(RIGHT_FORWARD, LOW);
    digitalWrite(RIGHT_BACKWARD, HIGH);
  }

  if (left_speed < 15 && left_speed > -15) {
    analogWrite(LEFT_ENABLE, 0);
  } else {
    analogWrite(LEFT_ENABLE, 2 * abs(left_speed));
  }
    
  if (right_speed < 15 && right_speed > -15) {
    analogWrite(RIGHT_ENABLE, 0);
  } else {
    analogWrite(RIGHT_ENABLE, 2 * abs(right_speed ));
  }
}

