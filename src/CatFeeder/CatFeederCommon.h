#pragma once

typedef struct MessageBrokerConfig
{
    char *ssid;
    char *password;
    char *mqtt_server;
} MessageBrokerConfig;

void feed(int stepsPerRevolution);
void setupWifi(MessageBrokerConfig config);