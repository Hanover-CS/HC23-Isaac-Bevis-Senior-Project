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
| CPU Clock Speed      | 32bit Dual core @ 133 MHz     | 32bit Dual core @ 240 MHz         |
| RAM and ROM storage  | 264 KiB RAM / 2 MiB ROM | 520KiB RAM / 488 KiB ROM    |
| Supported Libraries  | WiFi Only               | WiFi, BLE, ESP-NOW          |

On the hardware side there are a few options, but I will only be discussing the two cheapest options.  The two options are the Raspberry Pi Pico W (shortened to Pico from here on) and the ESP32.  The first aspect I will look at to decide on hardware is the power draw of each device.  The Pico draws around 0.23 watts (4.6V * 0.05A = 0.23W) and around 0.69 watts (4.6V * 0.15A = 0.69W) at full load[^1]. The ESP32 on the other hand is a little bit more power hungry, coming in at 0.3 watts at idle and 0.83 watts under full load (downloading and uploading files to network)[^2].  The Pico seems to be slightly more efficient than the ESP32, but the ESP32 runs at double the clock speed as the Pico so they are probably pretty similar in the performance per watt side.  Speaking of clock speed, the Pico is powered by the RP2040 which is a dual core Arm cpu clocking at 133 MHz and has 264 KiB RAM and 2 MB of program storage[^3].  The ESP32 is powered by a Xtensa dual core cpu clocking at 240 MHz and has 520 KiB of SRAM and 488 KiB ROM[^4].  The ESP32 has a lot more 'horsepower' on paper than the Pico but the only thing that might work better for my application would be the larger program storage on the Pico, but they would still both work.  The next thing to look at for this comparison is software compatibility. The ESP32 has libraries for WiFi, bluetooth and the custom protocol [ESP-NOW][3], whereas the Pico currently only has libraries for WiFi, so to figure out which microcontrollers to use, we need to look at these protocols.
## Software (protocols)
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

## Software (Programming Languages)
In terms of programming languages, I have two to choose from, [MicroPython][7] and [C++][8].  Both microcontrollers support these languages and all three protocols can be used with these languages.
- ### MicroPython
    MicroPython is a "boiled down" version of [Python 3][9] that only includes a few of the standard libraries, allowing it to run on microcontrollers with just 16kB of RAM.  MicroPython is also different to regular python 3 in the sense that it is a full compiler and runtime and runs on the bare metal.  MicroPython has special machine libraries that allow the user to program at a low level.  In terms of average usabillity, there seems be less documentation and tutorials out there for the various protocols on the ESP32.  MycroPython has the most support on their special PiBoard or the Raspberry Pico.
- ### C++
    C++ (C plus plus), is a programming language in the C family that was made in 1985.  C++ was made with embedded systems and resource restricted systems, as well as larger systems in mind.  C++ is a fully compiled language that runs on the bare metal, which makes it faster than other languages like Python.  C++ is the language of choice for tutorials and it has the most documentation on the ESP32.  So, in terms of usabillity, C++ is the obvious choice for this project.

## Security (Rolling Codes)
### Overview
Modern cars use a 40-bit rolling code, sometimes called a hopping code, to securely verify that a particular key fob belongs to a car.  Both the receiver and the transmitter use the same pseudo-random number generator and remain "in sync" so that the next random number will be the same on both the transmitter and the receiver.  The receiver is able to accept the next 256 "random" codes so that if you accidentally press the button when the car is out of range it doesn't de-sync them, rendering the key fob useless.[^5]  Based on my use case, there isn't really anything else to use, so it becomes a matter of how to implement a rolling code algorithm.  Since the microcontrollers I will be using have a Xtensa dual-core 32-bit microprocessor, I am limited to 32-bits for a rolling code for best performance.  With the ESP-NOW communication protocol, I have 250 bits to work with for sending data.  This means I can set up a message system using the following amount of bits for each part:
- 32 bits for the rolling code, our maximum as stated previously
- 64 bits for system time, this serves two functions:
    - 1: prevents the code jamming and storing vulnerability that was discovered by Samy Kamkar in 2015 called a "Rolljam" attack.[^6]  We can prevent this by only allowing codes from X minutes prior, even if they are "valid" rolling codes.
    - 2: allows to semi accurately tell the distance from key fob to vehicle, allowing for remote keyless entry
    - I don't necessarily need 64 bits but it would allow for the most precision and compensate for the far future Y2K38 issue.
- 4 bits for action code,
    - this allows for up to 16 actions, we really only need to lock, unlock, and check range this just adds future proofing and rounds the total message up to a nice even number.  

With this method, I have a nice even 100 bits for the total message length, easily fitting inside our maximum message size.

### Implementing the rolling code
In a rolling code system, the key fob stores its current code in memory (C).  This code is incremented every time a button is pressed, by using a pseudo random number generator with seed (S), and bundles this code with the current time (T).  The receiver in the car stores the most recent verified code it has received (N).  When the receiver gets a new code, it checks this code against the next few (K) codes using seed (S).  If it matches it first will check the 64 bit time value, if received time T is not within M minutes of the current time, it will ignore this code, else it replaces N with the new code received, and does specified action.  If the codes do not match it also will of course ignore the code. See [appendix 3.a](https://hanover-cs.github.io/HC23-Isaac-Bevis-Senior-Project/appendix.html#3a-rolling-code-diagram) for detailed diagram.

### Implementing the range finding
In a modern car, the remote keyless entry (RKE) works by a complex array of antennas in and around the car that can pinpoint the key's exact location, these allow the car to know how close the key is and weather it is inside the car or not.  They also have capacitive touch sensing handles that unlock the car when someone touches it and the key is detected within a certain distance[^7].  For obvious reasons, I cannot equip my system with a complex array of antennas or capacitive touch sensors, so I will be exploring two other possible options; estimating based on the signal strength, or using the time traveled of the wireless signal.  

- ### Using signal strength (Bluetooth RSSI) for distance
    In this method, the receiver in the car would also be scanning for bluetooth devices as well as connecting to the key by ESP-NOW.  This means that the long range communication would be handled by ESP-NOW but the short range proximity detection would be handled by bluetooth.  This works when the receiver in the car detects any bluetooth device within a specified range (lets say 5 feet) it checks the name and metadata of the device.  If this matches the predefined device id of my phone or some other bluetooth device I authorize in the code, it will send out a signal to the key fob basically saying "are you there."  If the key hears this, it will send the unlock signal with a rolling code.  In terms of security, this is not the best because someone could just spoof a bluetooth device id if they knew the id of my phone.  But since the old car doesn't have any other security measures and this procedure is more complicated than just breaking the windows I don't think this is that big of an issue.  As long as the doors don't just unlock when someone parked next to me accidentally brings their phone within range it should be alright.

- ### Using time traveled for distance calculation
    This means that both the key fob and the receiver must have very accurately synchronized hardware clocks.  The ESP32 has two available [internal hardware clocks][6]:
    - the RTC time with resolution of 6.667 μs; this clock is persistent through sleep modes and soft resets,
    - and the high-resolution timer; this clock is not persistent through either sleep modes or soft resets but has resolution of 1.0 μs.
    
    In this method, the receiver would also serve as a sender and would send out "pings" to the key fob every 10-20 seconds.  When the key fob receives this signal, it will use the time the other ESP32 in the car sent in its message and its own time to determine the distance.  Then if the key decides it is close enough, it will automatically send the unlock signal with the rolling code to the car.



[^1]: No source found because the Pico W is so new, but here is a forum thread where I got some info from [forums.raspberrypi.com](https://forums.raspberrypi.com/viewtopic.php?t=337145)  
[^2]: (no author listed), ESP32 Board Power Consumption, December 20, 2021. Accessed: 10/1/22, \[Online\] Available: https://therandomwalk.org/wp/esp32-power-consumption/  
[^3]: Raspberry Pi Org, Raspberry Pi Pico W Datasheet Rev 1.0, June 30, 2022. Accessed: 10/1/22, \[Online\] Available: https://datasheets.raspberrypi.com/picow/pico-w-datasheet.pdf  
[^4]: Espressif, ESP32 Series: Datasheet Rev 3.9, March, 2022. Accessed: 10/1/22, \[Online\] Available: https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf  
[^5]: Marshall Brain, How Remote Entry Works, How Stuff Works, Accessed: 10/1/22, \[Online\] Available: https://auto.howstuffworks.com/remote-entry2.htm  
[^6]: Cameron Coward, Hacking a Car’s Key Fob with a Rolljam Attack, hackster.io, Accessed: 10/1/22 \[Online\] Available: https://www.hackster.io/news/hacking-a-car-s-key-fob-with-a-rolljam-attack-7f863c10c8da  
[^7]: Vivek Shah, Proximity keys and other unlocking innovations, carexpert September 25, 2021, Accessed: 10/1/22 \[Online\] Available: https://www.carexpert.com.au/car-news/proximity-keys-and-other-unlocking-innovations

[1]: https://github.com/fryefryefrye/Open-Source-RKS "Open Source Remote Keyless System"
[2]: https://www.electronicsforu.com/electronics-projects/hardware-diy/esp32cam-based-smart-bluetooth-lock "Smart Bluetooth Lock using ESP32"
[3]: https://github.com/espressif/esp-now "ESP-NOW protocol github page"
[4]: https://datasheets.raspberrypi.com/picow/connecting-to-the-internet-with-pico-w.pdf "Raspberry Pi Pico W WiFi documentation"
[5]: https://docs.micropython.org/en/latest/library/bluetooth.html "BLE ESP32 MicroPython Docs"
[6]: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/system_time.html "ESP32 System Time Docs"
[7]: https://micropython.org/ "MicroPython Organization page"
[8]: https://en.wikipedia.org/wiki/C%2B%2B "Wikipedia on c++"
[9]: https://www.python.org/ "Python Organization page"

