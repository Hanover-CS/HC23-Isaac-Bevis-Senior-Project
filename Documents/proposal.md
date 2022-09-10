# Remote Unlocking System for Older Vehicles
## Main features
This project is designed to add remote unlocking, locking and keyless entry to older vehicles that do not have these features.  This project will involve a keyfob and a main control unit inside the vehicle.  Incorporating ideas from Computer Networks and Cybersecurity, these devices will comunicate securely through wireless channels.  The main control unit should be able to detect the keys presence automaticly unlock the car when you are close to the driver's door.  This device should also be able to automatically lock the vehicle when the key is no longer detected (in case the user forgot to lock the doors).  Lastly, this device should be able to function as a normal keyfob (i.e. unlock/lock car with the press of a button). 

## Compeating projects, Similarities and Differences

The most similar project to mine is the [Open-Source-RKS][1] on GitHub.  This project uses bluetooth to detect the presence of a key and unlock the vehicle, and lock when key is no longer detected.  The similarities of this project are that the main control device can detect the keyfob and automatically unlock/lock the car.  But in my project I plan to have manual buttons as well to add another layer of reliability.  Open RKS allows the user to connect a smartphone via bluetooth to serve as the keyfob, I would not be adding this feature to my project because I think it adds potential security vulnerabillities.

[1]: https://github.com/fryefryefrye/Open-Source-RKS "Open Souce Remote Keyless System"
[2]: https://github.com/espressif/esp-now "ESP-NOW protocol github page"