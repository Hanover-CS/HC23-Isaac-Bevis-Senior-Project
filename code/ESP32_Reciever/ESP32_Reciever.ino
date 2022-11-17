#include <esp_now.h>
#include <WiFi.h>
#include "RollingCode.h"
#define ONBOARD_LED  2

using namespace bevis_FinalProject;

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

void setup() {
  Serial.begin(115200);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  pinMode(ONBOARD_LED, OUTPUT);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Register callback for recieving data
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  // put your main code here, to run repeatedly:

}

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&message, incomingData, sizeof(message));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Time: ");
  Serial.println(message.time);
  Serial.print("Rolling code: ");
  Serial.println(message.rollingCode);
  Serial.print("Action: ");
  Serial.println(message.action);

  if (rollingCode.matches(message.rollingCode) && message.action == LOCK_SIGNAL){
    digitalWrite(ONBOARD_LED, HIGH);
  }
  else if (rollingCode.matches(message.rollingCode) && message.action == UNLOCK_SIGNAL){
    digitalWrite(ONBOARD_LED, LOW);
  }
}

