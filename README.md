# ESP32-cam-webserver-improved
An improved version of the CameraWebServer for the ESP32-cam module

If you want to use your camera module as a security camera, I highly recommend you to use this version.

It improves the performance and your camera module also doesn't get as hot anymore.
I could barely touch it whilst in use because it was so hot. With this improved code, it is alot cooler.
I have also added a webhook for the flashlight.

## Features
- Improved performance
- OTA Updates: Flash changes without disassembling your camera from its location 
- Doesn't get burning hot
- Control the LED with a webhook
- WiFi automatically reconnects if connection is lost
- Set a static IP
- The red LED is flashing whilst starting or while WiFi is reconnecting

## Removed
- Face recognition
