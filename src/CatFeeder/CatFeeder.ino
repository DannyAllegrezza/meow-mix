#include "CatFeederCommon.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Stepper.h>
#include "FS.h"
#include <NTPClient.h>
#include <WiFiUdp.h>

#define THINGNAME "ESP8266-PetFeeder"

static const char *IN_TOPIC = "inTopic/petfeeder/feed";
static const char *OUT_TOPIC = "outTopic/petfeeder";

const int stepsPerRevolution = 200;                    // change this to fit the number of steps per revolution
Stepper myStepper(stepsPerRevolution, D1, D2, D5, D6); // initialize the stepper motor on preferred pins of ESP8266
MessageBrokerConfig config = {ssid, password, mqtt_server, 8883};

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

WiFiClientSecure espClient;
PubSubClient client(espClient);

long lastMsg = 0;
char msg[50];
int value = 0;

void setup()
{
  pinMode(BUILTIN_LED, OUTPUT); // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);

  myStepper.setSpeed(55);

  setupWifi(config);

  client.setServer(config.mqtt_server, config.port);
  client.setCallback(callback);

  setupAndRegisterCertificates();
}

void loop()
{
  // block main loop until we're reconnected
  
  if (!client.connected())
  {
    Serial.println("Client was not connected, calling reconnect()....");
    reconnect();
    Serial.print("Left reconnect, client.connected() = ");
    Serial.println(client.connected());
  }
  else 
  {
    //Serial.println("client is connected");
    //Serial.println(client.state());  
  }

  client.loop();
  
  /** 
   *  Uncomment to publish a message every 2 seconds
   *  

  long now = millis();
  
  if (now - lastMsg > 2000)
  {
    // 2 seconds have passed, publish a new Message
    lastMsg = now;
    ++value;
    snprintf(msg, 50, "!!hello world #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish(OUT_TOPIC, msg);
  }
  */
}

void feed(int stepsPerRevolution)
{
  Serial.println("feeding!");

  // step one revolution - in one direction:
  myStepper.step(stepsPerRevolution);

  digitalWrite(D1, LOW);
  digitalWrite(D2, LOW);
  digitalWrite(D5, LOW);
  digitalWrite(D6, LOW);

  client.publish(OUT_TOPIC, "Just fed Kneesox :D");

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

  timeClient.begin();
  while (!timeClient.update())
  {
    timeClient.forceUpdate();
  }

  espClient.setX509Time(timeClient.getEpochTime());
}

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1')
  {
    digitalWrite(BUILTIN_LED, LOW); // Turn the LED on
    feed(stepsPerRevolution);
  }
  else if ((char)payload[0] == '2')
  {
    digitalWrite(BUILTIN_LED, LOW); // Turn the LED on
    feed(500);
  }
  else
  {
    digitalWrite(BUILTIN_LED, HIGH); // Turn the LED off
  }
}

void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
     String clientId = "ESP8266Client-";
     clientId += String(random(0xffff), HEX);

    // Attempt to connect
    if (client.connect(clientId.c_str()))
    {
      Serial.println("connected!");
      Serial.println("publishing first message to the OUT_TOPIC");
      // Once connected, publish an announcement...
      client.publish(OUT_TOPIC, "hello world - esp8266 petfeeder");
      // ... and resubscribe
      client.subscribe(IN_TOPIC);
      Serial.println(client.connected());
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      char buf[256];
      espClient.getLastSSLError(buf, 256);
      Serial.print("WiFiClientSecure SSL error: ");
      Serial.println(buf);
      delay(5000);
    }
  }
}

void setupAndRegisterCertificates()
{
  Serial.println("****** setupAndRegisterCertificates() ******");

  if (!SPIFFS.begin())
  {
    Serial.println("Failed to mount file system");
    return;
  }

  Serial.print("Getting ready to load certs. Curent Heap: ");
  Serial.println(ESP.getFreeHeap());

  // Load certificate file
  File cert = SPIFFS.open("/cert.der", "r"); //replace cert.crt eith your uploaded file name
  if (!cert)
  {
    Serial.println("Failed to open cert file");
  }
  else
    Serial.println("Success to open cert file");

  delay(1000);

  if (espClient.loadCertificate(cert))
    Serial.println("cert loaded");
  else
    Serial.println("cert not loaded");

  // Load private key file
  File private_key = SPIFFS.open("/private.der", "r"); //replace private eith your uploaded file name
  if (!private_key)
  {
    Serial.println("Failed to open private cert file");
  }
  else
    Serial.println("Success to open private cert file");

  delay(1000);

  if (espClient.loadPrivateKey(private_key))
    Serial.println("private key loaded");
  else
    Serial.println("private key not loaded");

  // Load CA file
  File ca = SPIFFS.open("/ca.der", "r"); //replace ca eith your uploaded file name
  if (!ca)
  {
    Serial.println("Failed to open ca ");
  }
  else
    Serial.println("Success to open ca");

  delay(1000);

  if (espClient.loadCACert(ca))
    Serial.println("ca loaded");
  else
    Serial.println("ca failed");

  Serial.print("Heap: ");
  Serial.println(ESP.getFreeHeap());
}
