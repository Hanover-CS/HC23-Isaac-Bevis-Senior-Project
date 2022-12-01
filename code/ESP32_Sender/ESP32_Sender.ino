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

#define S_DEBUG true
#define LKBTTN 21
#define UNLKBTTN 35
#define UNLOCK_SIGNAL 0
#define LOCK_SIGNAL 1

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

  // initialize pins 21 and 35 for buttons
  pinMode(LKBTTN, INPUT);
  pinMode(UNLKBTTN, INPUT);
}

void loop() {
  int lkBttnSt = digitalRead(LKBTTN);
  int unlkBttnSt = digitalRead(UNLKBTTN);
  // int lkBttnSt = 0;
  // int unlkBttnSt = 0;
  Serial.print("unlock button state: ");
  Serial.println(unlkBttnSt);
  Serial.print("lock button state: ");
  Serial.println(lkBttnSt);

  if (lkBttnSt == HIGH && lkBttnSt != LKBTTNSTATE) {
    LKBTTNSTATE = lkBttnSt;
    sendLockSignal();
  }
  else if (lkBttnSt == LOW && lkBttnSt != LKBTTNSTATE) {
    LKBTTNSTATE = lkBttnSt;
    sleep(1);
  }

  if (unlkBttnSt == HIGH && unlkBttnSt != UNLKBTTNSTATE) {
    UNLKBTTNSTATE = unlkBttnSt;
    sendUnlockSignal();
  }
  else if (unlkBttnSt == LOW && unlkBttnSt != UNLKBTTNSTATE) {
    UNLKBTTNSTATE = unlkBttnSt;
    sleep(1);
  }

  // ********* For Serial Debugging *******
  if (S_DEBUG){
    String cmd = Serial.readString();
    if (cmd == "L") {
      Serial.println("Sending via debug");
      esp_err_t result = sendLockSignal();
      Serial.print("Result = ");
      Serial.println(result);
    }
  
    else if (cmd == "U") {
      Serial.print("Sending via debug");
      esp_err_t result = sendUnlockSignal();
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

}

esp_err_t sendLockSignal() {
  rollingCode.next();
  gettimeofday(&tv_now, NULL);

  message.tv_now = tv_now;
  message.rollingCode = rollingCode.getSeed();
  message.action = LOCK_SIGNAL;

  return esp_now_send(broadcastAddress, (uint8_t *) &message, sizeof(message));
}

esp_err_t sendUnlockSignal() {
  rollingCode.next();
  gettimeofday(&tv_now, NULL);

  message.tv_now = tv_now;
  message.rollingCode = rollingCode.getSeed();
  message.action = UNLOCK_SIGNAL;

  return esp_now_send(broadcastAddress, (uint8_t *) &message, sizeof(message));
}

// callback for when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
