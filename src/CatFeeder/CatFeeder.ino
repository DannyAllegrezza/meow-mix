#include "CatFeederCommon.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Stepper.h>

const int stepsPerRevolution = 200;                    // change this to fit the number of steps per revolution
Stepper myStepper(stepsPerRevolution, D1, D2, D5, D6); // initialize the stepper motor on preferred pins of ESP8266
MessageBrokerConfig config = {"", "", ""};

void setup()
{
  Serial.begin(115200);
  myStepper.setSpeed(100);

  setupWifi(config);

  for (int i = 0; i < 3; i++)
  {
    feed();
  }
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

void setupWifi(MessageBrokerConfig config)
{
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(config.ssid);

  WiFi.begin(config.ssid, config.password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}