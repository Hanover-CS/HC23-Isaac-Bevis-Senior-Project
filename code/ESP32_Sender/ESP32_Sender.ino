#include <esp_now.h>
#include <WiFi.h>
#include "RollingCode.h"

using namespace bevis_FinalProject;

// NOTE: MAC addresses:
//    1: 94:B5:55:2D:35:BC
//    2: 94:B5:55:26:44:B8

esp_now_peer_info_t peerInfo;

byte UNLOCK_SIGNAL = 0;
byte LOCK_SIGNAL = 1;

RollingCode rollingCode = RollingCode(237461); // seed of 237461 with default m, a, and c values

// Structure to send data
typedef struct struct_message {
    int64_t time; 
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
}

void loop() {
  // Stand in for button
  String cmd = Serial.readString();
  if (cmd == "L") {
    Serial.print("Sending...");
    rollingCode.next();
    message.time = 0;
    message.rollingCode = rollingCode.getSeed();
    message.action = LOCK_SIGNAL;

    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &message, sizeof(message));
  }

  else if (cmd == "U") {
    Serial.print("Sending...");
    rollingCode.next();
    message.time = 0;
    message.rollingCode = rollingCode.getSeed();
    message.action = UNLOCK_SIGNAL;

    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &message, sizeof(message));
  }

  else if (cmd == "T") {
    struct timeval tv_now;
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

// callback for when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
