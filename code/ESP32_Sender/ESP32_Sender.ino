/*
  ESP32_Sender.ino
  Microcontrollers code for the key fob unit
  Sends unlock and lock signals via ESP-NOW to receiver
  Allows for hardware buttons
*/

// NOTE: MAC addresses:
//    1: 94:B5:55:2D:35:BC
//    2: 94:B5:55:26:44:B8

#include <esp_now.h>
#include <WiFi.h>
#include "RollingCode.h"

#define S_DEBUG false         // Enable serial debug
#define LKBTTN 23             // Lock Button (roll up back window in window mode)
#define UNLKBTTN 5            // Unlock Button (Roll down back window in window mode)
#define WNDBTTN 19            // Toggle window mode button
#define STLED 22              // LED that lights up when signal sends successfully
#define UNLOCK_SIGNAL 0       // Number representing unlock command
#define LOCK_SIGNAL 1         // Number representing lock command
#define WINDOW_SIGNAL 2       // Number representing the toggle to window mode command
#define WINDOW_UP_SIGNAL 3    // Number represneting the roll up window command 
#define WINDOW_DOWN_SIGNAL 4  // Number representing the roll down window command

using namespace bevis_FinalProject;

int LKBTTNSTATE;
int UNLKBTTNSTATE;


esp_now_peer_info_t peerInfo;
RollingCode rollingCode = RollingCode(237461); // seed of 237461 with default m, a, and c values
struct timeval tv_now; // time val for storing time in microseconds since power-on

// Structure to send data
typedef struct struct_message {
    struct timeval tv_now;
    unsigned long rollingCode;
    byte action;
};
struct_message message;

uint8_t broadcastAddress[] = {0x94, 0xB5, 0x55, 0x26, 0x44, 0xB8};

void setup() {
  Serial.begin(115200);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register the send callback
  esp_now_register_send_cb(OnDataSent);

  // Register a peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Add the peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  // initialize pins for buttons and LED
  pinMode(LKBTTN, INPUT_PULLUP);
  pinMode(UNLKBTTN, INPUT_PULLUP);
  pinMode(WNDBTTN, INPUT_PULLUP);
  pinMode(STLED, OUTPUT);
}

void loop() {

  if(digitalRead(WNDBTTN) == HIGH) {
    sendSignal(WINDOW_SIGNAL);    // turn on window mode
    Serial.println("before loop");
    while(digitalRead(WNDBTTN) == HIGH) {
      Serial.println("inloop");
      pollWindowUButton();
      pollWindowDButton();
    }
    Serial.println("after loop");
    sendSignal(WINDOW_SIGNAL);    // turn off window mode
  }

  pollLockButton();
  pollUnlockButton();

  // ********* For Serial Debugging *******
  if (S_DEBUG) {
    runDebug();      
  }

}

void pollLockButton() {
  int lkBttnSt = digitalRead(LKBTTN);
  switch (lkBttnSt) {
    case HIGH:
      if(lkBttnSt != LKBTTNSTATE) {
        LKBTTNSTATE = lkBttnSt;
        sendSignal(LOCK_SIGNAL);
      }
      break;
    case LOW:
      if(lkBttnSt != LKBTTNSTATE) {
        LKBTTNSTATE = lkBttnSt;
        delay(1);
      }
      break;
    default:
      break;
  }
}

void pollWindowUButton() {
  int lkBttnSt = digitalRead(LKBTTN);
  switch (lkBttnSt) {
    case HIGH:
      sendSignal(WINDOW_UP_SIGNAL);
      while(digitalRead(LKBTTN) == HIGH){}//halt further execution until nolonger holding button
      sendSignal(LOCK_SIGNAL);
      break;
    case LOW:
      delay(1);
      break;
    default:
      break;
  }
}

void pollUnlockButton() {
  int unlkBttnSt = digitalRead(UNLKBTTN);
  switch(unlkBttnSt) {
    case HIGH:
      if(unlkBttnSt != UNLKBTTNSTATE) {
        UNLKBTTNSTATE = unlkBttnSt;
        sendSignal(UNLOCK_SIGNAL);
      }
      break;
    case LOW:
      if(unlkBttnSt != UNLKBTTNSTATE) {
        UNLKBTTNSTATE = unlkBttnSt;
        delay(1);
      }
      break;
    default:
      break;
  }
}

void pollWindowDButton() {
  int unlkBttnSt = digitalRead(UNLKBTTN);
  switch(unlkBttnSt) {
    case HIGH:
      sendSignal(WINDOW_DOWN_SIGNAL);
      while(digitalRead(UNLKBTTN) == HIGH){}//halt further execution until nolonger holding button
      sendSignal(UNLOCK_SIGNAL);
      break;
    case LOW:
      delay(1);
      break;
    default:
      break;
  }
}

esp_err_t sendSignal(byte sig) {
  rollingCode.next();
  gettimeofday(&tv_now, NULL);

  message.tv_now = tv_now;
  message.rollingCode = rollingCode.getSeed();
  message.action = sig;

  return esp_now_send(broadcastAddress, (uint8_t *) &message, sizeof(message));
}

// callback for when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");

  if(status == ESP_NOW_SEND_SUCCESS){
    digitalWrite(STLED, HIGH);
    delay(300);
    digitalWrite(STLED, LOW);
  }
}

void runDebug() {
  String cmd = Serial.readString();
    if (cmd == "L") {
      Serial.println("Sending via debug");
      esp_err_t result = sendSignal(LOCK_SIGNAL);
      Serial.print("Result = ");
      Serial.println(result);
    }
  
    else if (cmd == "U") {
      Serial.print("Sending via debug");
      esp_err_t result = sendSignal(UNLOCK_SIGNAL);
      Serial.print("Result = ");
      Serial.println(result);
    }
  
    else if (cmd == "T") {
      gettimeofday(&tv_now, NULL);
      int64_t time_sec = (int64_t)tv_now.tv_sec * 1000000L + (int64_t)tv_now.tv_usec;
  
  
      Serial.print("Current tv_now.tv_usec is: ");
      Serial.println((int64_t)tv_now.tv_usec);
      Serial.print("Current tv_now.tv_sec is: ");
      Serial.println((int64_t)tv_now.tv_sec);
      Serial.print("Current time is: ");
      Serial.println(time_sec);
      Serial.println();
    }
  
    else if (cmd == "T2") {
      time_t now;
      char strftime_buf[64];
      struct tm timeinfo;
      time(&now);
  
      localtime_r(&now, &timeinfo);
      strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
      Serial.print("Current time is: ");
      Serial.println(strftime_buf);
    }
}
