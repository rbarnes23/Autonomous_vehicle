//#include <HardwareSerial.h>
#include "./CoolerDefinitions.h"
#include "./Compass.h"
#include "./Location.h"


// Setup start up routines for the Compass and GPS location
void setup()
{
  Serial.begin(9600);
  Serial.println("Start AV");
  configCompass(); // turn the MAG3110 on
  configLocation();
}

void loop()
{
  getCompassReadings();
  getLocationReadings();
}



// Used to drive from the GPS location to the local destination
void drive(int distance, float turn) {
  int fullSpeed = 230;
  int stopSpeed = 0;

  // drive to location
  int s = fullSpeed;
  if ( distance < 8 ) {
    int wouldBeSpeed = s - stopSpeed;
    wouldBeSpeed *= distance / 8.0f;
    s = stopSpeed + wouldBeSpeed;
  }

  int autoThrottle = constrain(s, stopSpeed, fullSpeed);
  autoThrottle = 230;

  float t = turn;
  while (t < -180) t += 360;
  while (t >  180) t -= 360;

  Serial.print("turn: ");
  Serial.println(t);
  Serial.print("original: ");
  Serial.println(turn);

  float t_modifier = (180.0 - abs(t)) / 180.0;
  float autoSteerA = 1;
  float autoSteerB = 1;

  if (t < 0) {
    autoSteerB = t_modifier;
  } else if (t > 0) {
    autoSteerA = t_modifier;
  }

  Serial.print("steerA: "); Serial.println(autoSteerA);
  Serial.print("steerB: "); Serial.println(autoSteerB);

  int speedA = (int) (((float) autoThrottle) * autoSteerA);
  int speedB = (int) (((float) autoThrottle) * autoSteerB);

  setSpeedMotorA(speedA);
  setSpeedMotorB(speedB);
}

// Need to change this so it adjusts the "speed" per PWM based on turn rate, etc.
void setSpeedMotorA(int speed) {
  digitalWrite(MOTOR_A_IN_1_PIN, LOW);
  digitalWrite(MOTOR_A_IN_2_PIN, HIGH);

  // set speed to 200 out of possible range 0~255
  //analogWrite(MOTOR_A_EN_PIN, speed + MOTOR_A_OFFSET);
}

void setSpeedMotorB(int speed) {
  digitalWrite(MOTOR_B_IN_1_PIN, LOW);
  digitalWrite(MOTOR_B_IN_2_PIN, HIGH);

  // set speed to 200 out of possible range 0~255
  //analogWrite(MOTOR_B_EN_PIN, speed + MOTOR_B_OFFSET);
}


