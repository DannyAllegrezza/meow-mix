#pragma once

typedef struct MessageBrokerConfig
{
    char *ssid;
    char *password;
    char *mqtt_server;
    int port;
} MessageBrokerConfig;

void feed(int stepsPerRevolution);
void setupWifi(MessageBrokerConfig config);

const char *AWS_endpoint = "us-east-1.amazonaws.com"; //MQTT broker ip

char *ssid = "";
char *password = "";
char *mqtt_server = "us-east-1.amazonaws.com";
int port = 1883;
