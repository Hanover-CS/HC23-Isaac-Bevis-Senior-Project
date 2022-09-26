# Remote Unlocking System for Older Vehicles
## Main features
This project is designed to add remote unlocking, locking and keyless entry to older vehicles that do not have these features.  This will involve a key fob and a main control unit inside the vehicle that operates the lock mechanism.  Incorporating ideas from Computer Networks and Cybersecurity, these devices will communicate securely through wireless channels.  The device should be expected to do these three things as follows:
- The main control unit should be able to detect the keys presence and automatically unlock the car when you are close.
- Be able to automatically lock the vehicle when the key is no longer detected (in case the user forgot to lock the doors)
- Lastly, The device should be able to function as a normal key fob (i.e. unlock/lock car with the press of a button). 

## Similar projects

The most similar project to mine is [Open-Source-RKS][1] on GitHub.  This project uses bluetooth to detect the presence of a key and unlock the vehicle, and lock when the key is no longer detected.  The similarities of this project are that the main control device can detect the key fob and automatically unlock/lock the car.  But in my project I plan to have manual buttons as well to add another layer of reliability.  Open RKS allows the user to connect a smartphone via bluetooth to serve as the key fob, I would not be adding this feature to my project because I think it adds potential security vulnerabilities.

## Relevant projects
One project I would be pulling ideas from is this [ESP32-Based Smart Bluetooth Lock][2]. This project is a "smart home" lock that can unlock your house with the presence of your phone.  The similarities in ideas from this project are that an ESP32 board gets a signal from somewhere, then uses some sort of verification to verify the user, and then unlocks the door via a relay and electronic actuator.  In my project, I would be using a different wireless protocol, but I will be using the basic ideas of receive, verify, unlock.

## Hardware  

|                      |          Pico W         |             ESP32           |
| -------------------- | ----------------------- | --------------------------- |
| Power Draw           | 0.69 Watts Max          | 0.83 Watts Max              |
| CPU Clock Speed      | 32bit Dual core @ 133 MHz     | 32bit Dual code @ 240 MHz         |
| RAM and ROM storage  | 264 KiB RAM / 2 MiB ROM | 520KiB RAM / 488 KiB ROM    |
| Supported Libraries  | WiFi Only               | WiFi, BLE, ESP-NOW          |

On the hardware side there are a few options, but I will only be discussing the two cheapest options.  The two options are the Raspberry Pi Pico W (shortened to Pico from here on) and the ESP32.  The first aspect I will look at to decide on hardware is the power draw of each device.  The Pico draws around 0.23 watts (4.6V * 0.05A = 0.23W) and around 0.69 watts (4.6V * 0.15A = 0.69W) at full load[^1]. The ESP32 on the other hand is a little bit more power hungry, coming in at 0.3 watts at idle and 0.83 watts under full load (downloading and uploading files to network)[^2].  The Pico seems to be slightly more efficient than the ESP32, but the ESP32 runs at double the clock speed as the Pico so they are probably pretty similar in the performance per watt side.  Speaking of clock speed, the Pico is powered by the RP2040 which is a dual core Arm cpu clocking at 133 MHz and has 264 KiB RAM and 2 MB of program storage[^3].  The ESP32 is powered by a Xtensa dual core cpu clocking at 240 MHz and has 520 KiB of SRAM and 488 KiB ROM[^4].  The ESP32 has a lot more 'horsepower' on paper than the Pico but the only thing that might work better for my application would be the larger program storage on the Pico, but they would still both work.  The next thing to look at for this comparison is software compatibility. The ESP32 has libraries for WiFi, bluetooth and the custom protocol [ESP-NOW][3], whereas the Pico currently only has libraries for WiFi, so to figure out which microcontrollers to use, we need to look at these protocols.
## Software
In terms of protocols we have three choices, [ESP-NOW][3], [WiFi][4], and [Bluetooth][5].  Since not all of these protocols are supported by both microcontrollers, our choice here will decide what microcontrollers we will have to use.  So, to help figure out which is best, I made a simple test program using each.  
### WiFi (Pico W)
First I tested using the Pico W using its only current supported protocol, WiFi.  In terms of actual programming, it was relatively easy to set up, but as soon as you get into functionality it soon became a nightmare.  For this test I used two Picos, one serving as a WiFi access point and hosting a simple web server, the other as a client which would connect to the access point and then send HTTP requests to the server.  To set up the access point we just need to set a ssid and password and setup a network in access point mode. [See appendix 1.a](https://hanover-cs.github.io/HC23-Isaac-Bevis-Senior-Project/appendix.html#1a-micropython-wifi-example-1).
Then to connect to this access point we can use a while loop with a 10 second timeout, if fails it will raise an exception and back in main() it will wait 10 seconds then try the whole thing again. [See appendix 1.b](https://hanover-cs.github.io/HC23-Isaac-Bevis-Senior-Project/appendix.html#1b-micropython-wifi-example-2).
Then back on the server side, the web server is just a super simple html page.  Then while listening for connections we look for the /light/on or /off to turn off or on the built in LED. [See appendix 1.c](https://hanover-cs.github.io/HC23-Isaac-Bevis-Senior-Project/appendix.html#1c-micropython-wifi-example-3).
Then on the client side the sending of these HTTP requests is dead simple only taking up 3 lines of code.  The 192.168.4.1 is the default gateway for the Pico access point, I was able to connect to this address on my phone when connected to the AP. [See appendix 1.d](https://hanover-cs.github.io/HC23-Isaac-Bevis-Senior-Project/appendix.html#1d-micropython-wifi-example-4).
These components worked great separately, I was able to connect to the access point on my phone and send HTTP requests to turn on and off a LED, and the other was able to connect to a network and send out HTTP requests to various web servers just fine.  But as soon as I put these together everything stopped working, and not to mention that the client seemed to only connect to the access point if the access point was on and in range when the client is first turned on, even though the code keeps trying to connect every 10 seconds for 10 seconds.  So for now we keep looking at the other protocols.  
### ESP-NOW
Next I tested ESP-NOW, it was a bit harder to understand and program it at first but the finished product is much more robust.  Both the receiver and sender can be turned off at will (simulating losing connection from range) and once turned back on, they immediately (within a few milliseconds) begin transmitting again.  To use ESP-NOW we first establish a peer to peer connection, directly connecting with the devices MAC address. [See appendix 2.a](https://hanover-cs.github.io/HC23-Isaac-Bevis-Senior-Project/appendix.html#2a-esp-now-example-1).
Then to make a message we just use a simple struct with two attributes, there could be more if needed (maximum of 250 bytes total). [See appendix 2.b](https://hanover-cs.github.io/HC23-Isaac-Bevis-Senior-Project/appendix.html#2b-esp-now-example-2).
And then to send the message, it must be converted to `uint8_t` data type and sent with the `esp_now_send` method.  [See appendix 2.c](https://hanover-cs.github.io/HC23-Isaac-Bevis-Senior-Project/appendix.html#2c-esp-now-example-3).
My project would greatly benefit from the added robustness of ESP-NOW, when comparing to WiFi, it is a night and day difference.  With WiFi you have to create an access point with a secure password and host a web server to communicate.  Whereas with ESP-NOW all one has to do is register a device to broadcast to by it's MAC address and then you can send any data up to 250 bytes in size.  In the case of connection drop off, the system continues to function and immediately resumes sending data when the other device becomes in range again.  So for my project I will be using ESP-NOW to communicate.

## Security (Rolling Codes)
### Overview
Modern cars use a 40-bit rolling code, sometimes called a hopping code, to securly varify that a particular key fob belongs to a car.  Both the receiver and the trasmitter use the same seudo-random number generator and remain "in sync" so that the next random number will be the same on both the transmitter and the reciever.  The reciever is able to accept the next 256 "random" codes so that if you accedentilly press the button when the car is out of range it doesnt desync them, rendering the key fob useless.[^5]  Based on my use case, there isn't really anything else to use, so it becomes a matter of how to implement a rolling code algorithm.  Since the microcontollers I will be using have a Xtensa dual-core 32-bit LX6 microprocessor, I am limited to 32-bits for a rolling code for best preformance.  With the ESP-NOW communication protocol, I have 250 bits to work with for sending data.  This means I can set up a message system using the following amount of bits for each part:
- 32 bits for the rolling code, our maximum as stated previously
- 64 bits for system time, this serves two functions:
    - 1: prevents the code jamming and storing vulnerabillity that some rolling code algorithms suffer from
    - 2: allows to semi accuratly tell the distance from key fob to vehicle, allowing for remote keyless entry
    - I don't necisarilly need 64 bits but it would allow for the most precission and compensate for the far future Y2K38 issue.
- 4 bits for action code,
    - this allows for up to 16 actions, we really only need to lock, unlock, and check range this just adds fute proofing and rounds the total message up to a nice even number.  

With this method, I have a nice even 100 bits for the total message length, easilly fitting inside our maximum message size.

### Implementing the rolling code
To be continued.


[^1]: Forum thread where they are talking about power draw of the Pico [forums.raspberrypi.com](https://forums.raspberrypi.com/viewtopic.php?t=337145)  
[^2]: Article on ESP32 power draw [therandomwalk.org](https://therandomwalk.org/wp/esp32-power-consumption/)  
[^3]: Raspberry Pi Pico W data sheet [datasheets.raspberrypi.com](https://datasheets.raspberrypi.com/picow/pico-w-datasheet.pdf)  
[^4]: ESP32 data sheet PDF [espressif.com](https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf)  
[^5]: [how stuff works](https://auto.howstuffworks.com/remote-entry2.htm)

[1]: https://github.com/fryefryefrye/Open-Source-RKS "Open Source Remote Keyless System"
[2]: https://www.electronicsforu.com/electronics-projects/hardware-diy/esp32cam-based-smart-bluetooth-lock "Smart Bluetooth Lock using ESP32"
[3]: https://github.com/espressif/esp-now "ESP-NOW protocol github page"
[4]: https://datasheets.raspberrypi.com/picow/connecting-to-the-internet-with-pico-w.pdf "Raspberry Pi Pico W WiFi documentation"
[5]: https://docs.micropython.org/en/latest/library/bluetooth.html "BLE ESP32 MicroPython Docs"
