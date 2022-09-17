import urequests
import network
from time import sleep
from machine import Pin

led = Pin("LED", Pin.OUT)

ssid = "PicoWAccessPoint"
password = "1234567890"

def connect():
    wlan = network.WLAN(network.STA_IF)
    wlan.active(True)
    wlan.connect(ssid, password)
    
    tries = 0
    while not wlan.isconnected() and wlan.status() >= 0:
        print("Waiting to connect:")
        sleep(1)
        tries += 1
        
        if (tries == 10):
            raise RuntimeError("Unable to connect")
        
    if (wlan.isconnected()):
        print(f"connected, ip adress: {wlan.ifconfig()}")
        
def sendServerMessage(mes):
    r = urequests.get(f"http://192.168.4.1/light/{mes}") # default gateway of Pico W access point is 192.168.4.1
    print(r.content)
    r.close()

def main():
    # attempt connection
    while True:
        try:
            connect()
        except RuntimeError:
            print("Connection failed, trying again in 10 seconds")
            sleep(10)
            continue
        finally:
            led.on()
            
    # play with leds
    while True:
        sendServerMessage("on")
        led.off()
        sleep(1)
        sendServerMessage("off")
        led.on()
        sleep(1)

main()       
