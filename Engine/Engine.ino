
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
int motor_speed = 0;
int motor_turn = 0;
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

  int new_turn = constrain(ps2x.Analog(PSS_LX) - 128, -127, 127);
  int new_speed = constrain(ps2x.Analog(PSS_RY) - 128, -127, 127);
  if (new_turn <= 1 || new_turn >= -1) 
    new_turn = 0;
  if (new_speed <= 1 || new_speed >= -1)
    new_speed = 0;
  
  setMotor(new_speed, new_turn);

  // Debug
  if (new_turn > 5 || new_turn < 5) { 
    Serial.print("Turn: ");
    Serial.println(new_turn);
  }
  if (new_speed > 5 || new_speed < 5) {
    Serial.print("Speed: ");
    Serial.println(new_speed);
  }
}

void setMotorSpeed(int new_speed) {
  motor_speed = new_speed;
  updateMotors();
}

void setMotorTurn(int new_turn) {
  motor_turn = new_turn;
  updateMotors();
}

void setMotor(int new_speed, int new_turn) {
  motor_speed = new_speed;
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

  int left_damping = (motor_turn > 50) ? 127 : 0;
  int right_damping = (motor_turn < 50) ? 127 : 0;
  
  analogWrite(RIGHT_ENABLE, (2 * abs(motor_speed )) - left_damping);
  analogWrite(LEFT_ENABLE, (2 * abs(motor_speed)) - right_damping);
}

