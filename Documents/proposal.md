# Remote Unlocking System for Older Vehicles
## Main features
This project is designed to add remote unlocking, locking and keyless entry to older vehicles that do not have these features.  This project will involve a key fob and a main control unit inside the vehicle.  Incorporating ideas from Computer Networks and Cybersecurity, these devices will communicate securely through wireless channels.  The main control unit should be able to detect the keys presence automatically unlock the car when you are close to the driver's door.  This device should also be able to automatically lock the vehicle when the key is no longer detected (in case the user forgot to lock the doors).  Lastly, this device should be able to function as a normal key fob (i.e. unlock/lock car with the press of a button). 

## Competing projects, Similarities and Differences

The most similar project to mine is [Open-Source-RKS][1] on GitHub.  This project uses bluetooth to detect the presence of a key and unlock the vehicle, and lock when key is no longer detected.  The similarities of this project are that the main control device can detect the key fob and automatically unlock/lock the car.  But in my project I plan to have manual buttons as well to add another layer of reliability.  Open RKS allows the user to connect a smartphone via bluetooth to serve as the key fob, I would not be adding this feature to my project because I think it adds potential security vulnerabilities.

## Projects to pull ideas from
One project I would be pulling ideas from is this [ESP32-Based Smart Bluetooth Lock][2]. This project is a "smart home" lock that can unlock your house with the presence of your phone.  The similarities in ideas from this project are that an ESP32 board gets a signal from somewhere, then uses some sort of verification to verify the user, and then unlocks the door via a relay and electronic actuator.  In my project, I would be using a different wireless protocol, but I will be using basic ideas of receive, verify, unlock.

## What Microcontrollers will I use

## What wireless protocols will I use

[1]: https://github.com/fryefryefrye/Open-Source-RKS "Open Source Remote Keyless System"
[2]: https://www.electronicsforu.com/electronics-projects/hardware-diy/esp32cam-based-smart-bluetooth-lock "Smart Bluetooth Lock using ESP32"