#include <Stepper.h>

const int stepsPerRevolution = 200;                    // change this to fit the number of steps per revolution
Stepper myStepper(stepsPerRevolution, D1, D2, D5, D6); // initialize the stepper motor on preferred pins of ESP8266

void setup()
{

  // set the speed at 100 rpm:
  myStepper.setSpeed(100);
  Serial.begin(115200);
  feed();
}

void loop()
{
}

void feed()
{
  Serial.println("feeding!");
  // danny: for debugging purposes, was just verifying that I could use the const values for the digitalWrite calls below..
  Serial.println(D1);
  Serial.println(D2);
  Serial.println(D5);
  Serial.println(D6);

  // step one revolution - in one direction:
  myStepper.step(stepsPerRevolution);

  digitalWrite(D1, LOW);
  digitalWrite(D2, LOW);
  digitalWrite(D5, LOW);
  digitalWrite(D6, LOW);

  delay(5000);
}
