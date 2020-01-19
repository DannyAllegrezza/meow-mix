# meow-mix
An API driven dashboard for my ESP8266 based automatic pet feeder project.

## Getting the ESP8266 configured 
1. The controller needs a library for communication with the MQTT broker. Check out [this library](https://github.com/knolleary/pubsubclient/releases/tag/v2.7) for more info. This can be accomplished by downloading the library and placing it into your OS's global Arduino library directory. On Win10, this was `Documents\Arduino\libraries\PubSubClient`. Can also be automatically installed when using VSCode by using the `Library Manager` command.

## ESP8266 VSCode Config
The free Arduino is great, however, if you prefer to use VSCode you can install the Arduino extension. When the extension finds your `.ino` file it will allow you to configure your board type as well as the programmer and serial port. 

> NOTE: On Windows 10 I needed to add the ESP8266 "Board Package" in order for this to work.

The generated `.vscode/arduino.json` file will look something like this:

```json
{
    "port": "COM6",
    "board": "esp8266:esp8266:nodemcuv2",
    "configuration": "xtal=80,vt=flash,exception=legacy,ssl=all,eesz=4M2M,led=2,ip=lm2f,dbg=Disabled,lvl=None____,wipe=none,baud=115200",
    "programmer": "AVRISP mkII",
    "sketch": "CatFeeder.ino",
    "output": "../build"
}
```