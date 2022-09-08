# Remote Bluetooth Powered Key fob for Old Car

## Links to similar projects listed from most similar to least
1. Remote Keyless System using bluetooth:
    [Open-Source-RKS.][1]
    RKS is a proximity system that is triggered by the detection of an physical authorized key.  Systems like these are typically used in modern vehicles as a convenient way to unlock your car without digging your key out of your bag.  This project uses bluetooth which might not be as secure as I would like for a vehicle unlocking device, so instead I would use something like [ESP-NOW][4] in my version.  

2. "Smart Home" door unlocking mechanism using esp32 bluetooth:
    [ESP32 Cam-Based Smart Bluetooth Lock][2].
    This system is used to unlock a door in your house without a key or RFID chip, it just connects to a phone via bluetooth which then authorizes the opening of the door. 
3. Two way communication between two ESP32 microcontrollers using WiFi module:
    [ESP-NOW Two-Way Communication Between ESP32 Boards][3].  This Project uses the [ESP-NOW protocol][4] to send messages of up to 250 bytes in length without using WiFi.
4. Bluetooth controlled robot car using ESP32 and Android phone:
    [Bluetooth controlled car][5].  In this project, you use an ESP32 and it's built-in Bluetooth module to control a robot car.

## Importent protocols
1. [ESP-NOW][4] is a WiFi protocol that doesn't require a network connection/router to function.  It functions similar to a peer to peer network.  It simplifies the upper 5 layers of WiFi network comunication (Network, Transport, Session, Presentation, and Application) are simplified into one layer in ESP-NOW, making it more power efficient and resposive.  I think that for a hacker standing by your car this would be harder to figure out how to crack and unlock (obviously you could just break a window but that is outside the scope of this project).
2. [Bluetooth][6] The ESP32 can use the standard Bluetooth LE (Low Energy) protocol to comunicate to other bluetooth compatible devices, such as a phone, laptop, or another ESP32-like device.  

## Microcontrolers
The two options for microcontrolers that I have for this project are the [Raspberry Pi Pico Wireless][9] or the [ESP32][10].  I picked these particular microcontrolers because they are very inexpensive ($6 each for the Pico, if you can find them in stock, and around $8 to $12 each for the ESP32 depending on where you get them, I got mine on eBay).

## Importent Languages
Both ESP32 and the Raspberry Pi Pico support using Arduino IDE with [C++][8] or they also support [MicroPython][7] using the Thonney IDE.  MicroPython is a lightweight version of Python desighed for low power devices such as microcontrolers.


[1]: https://github.com/fryefryefrye/Open-Source-RKS "Open Souce Remote Keyless System"
[2]: https://www.electronicsforu.com/electronics-projects/hardware-diy/esp32cam-based-smart-bluetooth-lock "Smart Bluetooth Lock using ESP32"
[3]: https://randomnerdtutorials.com/esp-now-two-way-communication-esp32/ "ESP-NOW example project"
[4]: https://github.com/espressif/esp-now "ESP-NOW protocol github page"
[5]: https://www.androiderode.com/bluetooth-controlled-car-using-esp32-and-smartphone/ "Remote controlled car using bluetooth and smartphone"
[6]: https://espressif.com/sites/default/files/documentation/esp32_bluetooth_architecture_en.pdf "ESP32 Bluetooth Architecture PDF guide"
[7]: https://docs.micropython.org/en/latest/esp32/quickref.html "MicroPython documentation for ESP32"

[8]: https://espressif-docs.readthedocs-hosted.com/projects/arduino-esp32/en/latest/libraries.html "C++ documentation for ESP32"

[9]: https://www.raspberrypi.com/products/raspberry-pi-pico/ "Raspberry Pi Pico product page"

[10]: https://www.espressif.com/en/products/socs/esp32 "ESP32 product page"
