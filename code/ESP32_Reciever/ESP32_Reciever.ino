#include <esp_now.h>
#include <WiFi.h>
#include "RollingCode.h"
#define ONBOARD_LED  2

using namespace bevis_FinalProject;

const byte UNLOCK_SIGNAL = 0;
const byte LOCK_SIGNAL = 1;

RollingCode rollingCode = RollingCode(237461); // seed of 237461 with default m, a, and c values

struct timeval tv_now;

// Structure to send data
typedef struct struct_message {
    struct timeval tv_now;
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
  gettimeofday(&tv_now, NULL);

  // ***** debug messages ******
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Time S: ");
  Serial.println(message.tv_now.tv_sec);
  Serial.print("Time Ms: ");
  Serial.println(message.tv_now.tv_usec);
  Serial.print("Rolling code: ");
  Serial.println(message.rollingCode);
  Serial.print("Action: ");
  Serial.println(message.action);
  Serial.println();
  Serial.print("This time S: ");
  Serial.println(tv_now.tv_sec);
  Serial.print("This time Ms: ");
  Serial.println(tv_now.tv_usec);
  

  int64_t thisTime = (int64_t)tv_now.tv_sec * 1000000L + (int64_t)tv_now.tv_usec;
  int64_t otherTime = (int64_t)message.tv_now.tv_sec * 1000000L + (int64_t)message.tv_now.tv_usec;
  
  Serial.print("isValidTime: ");
  Serial.println(isValidTime(thisTime, otherTime));
  Serial.println();

  if (rollingCode.matches(message.rollingCode) && isValidTime(thisTime, otherTime)) {
    switch(message.action) {
      case LOCK_SIGNAL:
        digitalWrite(ONBOARD_LED, HIGH);
        break;
      case UNLOCK_SIGNAL:
        digitalWrite(ONBOARD_LED, LOW);
        break;
      default:
        break;
    }
  }
}

// isValidTime
// returns true if otherTime is within 1 minute of thisTime
// thisTime and otherTime are represented in microseconds
bool isValidTime(int64_t thisTime, int64_t otherTime) {
  int64_t minute = 60 * 1000000L;
  return (otherTime - thisTime) < minute;
}
