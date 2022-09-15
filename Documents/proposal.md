# Remote Unlocking System for Older Vehicles
## Main features
This project is designed to add remote unlocking, locking and keyless entry to older vehicles that do not have these features.  This project will involve a key fob and a main control unit inside the vehicle.  Incorporating ideas from Computer Networks and Cybersecurity, these devices will communicate securely through wireless channels.  The main control unit should be able to detect the keys presence automatically unlock the car when you are close to the driver's door.  This device should also be able to automatically lock the vehicle when the key is no longer detected (in case the user forgot to lock the doors).  Lastly, this device should be able to function as a normal key fob (i.e. unlock/lock car with the press of a button). 

## Competing projects, Similarities and Differences

The most similar project to mine is [Open-Source-RKS][1] on GitHub.  This project uses bluetooth to detect the presence of a key and unlock the vehicle, and lock when key is no longer detected.  The similarities of this project are that the main control device can detect the key fob and automatically unlock/lock the car.  But in my project I plan to have manual buttons as well to add another layer of reliability.  Open RKS allows the user to connect a smartphone via bluetooth to serve as the key fob, I would not be adding this feature to my project because I think it adds potential security vulnerabilities.

## Projects to pull ideas from
One project I would be pulling ideas from is this [ESP32-Based Smart Bluetooth Lock][2]. This project is a "smart home" lock that can unlock your house with the presence of your phone.  The similarities in ideas from this project are that an ESP32 board gets a signal from somewhere, then uses some sort of verification to verify the user, and then unlocks the door via a relay and electronic actuator.  In my project, I would be using a different wireless protocol, but I will be using basic ideas of receive, verify, unlock.

## Hardware/Software
On the hardware side there are a few options, but I will only be discussing the two cheapest options.  The two options are the Raspberry Pi Pico W (shortened to Pico from here on) and the ESP32.  The first aspect I will look at to decide on hardware is the power draw of each device.  The Pico draws around 0.23 watts (4.6V * 0.05A = 0.23W) and around 0.69 watts (4.6V * 0.15A = 0.69W) at full load.`[1]` The ESP32 on the other hand is a little bit more power hungry, coming in at 0.3 watts at idle and 0.83 watts under full load (downloading and uploading files to network).`[2]`  The Pico seems to be slightly more efficient than the ESP32, but the ESP32 runs at double the clock speed as the Pico so they are probably pretty similar in the performance per watt side.  Speaking of clock speed, the Pico is powered by the RP2040 which is a dual core Arm cpu clocking at 133 MHz and has 264 KiB RAM and 2 MB of program storage.`[3]`  The ESP32 is powered by a Xtensa dual core cpu clocking at 240 MHz and has 520 KiB of SRAM and 488 KiB ROM.`[4]`  The ESP32 has a lot more 'horsepower' on paper than the Pico but the only thing that might work better for my application would be the larger program storage on the Pico, but they would still both work.  The next thing to look at for this comparison is software compatibility. The ESP32 has libraries for WiFi, bluetooth and the custom protocol [ESP-NOW][3], whereas the Pico currently only has libraries for WiFi, so to figure out witch microcontrollers to use, we need to look at these protocols.


`[1]` Forum thread where they are talking about power draw of the Pico https://forums.raspberrypi.com/viewtopic.php?t=337145

`[2]` Article on ESP32 power draw https://therandomwalk.org/wp/esp32-power-consumption/

`[3]` Raspberry Pi Pico W data sheet https://datasheets.raspberrypi.com/picow/pico-w-datasheet.pdf

`[4]` ESP32 data sheet PDF https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf

[1]: https://github.com/fryefryefrye/Open-Source-RKS "Open Source Remote Keyless System"
[2]: https://www.electronicsforu.com/electronics-projects/hardware-diy/esp32cam-based-smart-bluetooth-lock "Smart Bluetooth Lock using ESP32"
[3]: https://github.com/espressif/esp-now "ESP-NOW protocol github page"