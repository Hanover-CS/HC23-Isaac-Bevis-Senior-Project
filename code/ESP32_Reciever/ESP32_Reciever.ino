/*
  ESP32_Receiver.ino
  Microcontroller code for the receiver unit
  Receives unlock and lock signals via ESP-NOW
  Acts on these signals by locking or unlocking a car 
    door via servo motors
*/

#include <esp_now.h>
#include <WiFi.h>
#include "RollingCode.h"
#include <ESP32Servo.h>

#define LOCK_LED 18       // LED That indicates that car is locked
#define UNLOCK_LED 21     // LED that indicates that car is unlocked
#define WINDOW_LED 22     // LED that indicates the window can be rolled down
#define SERVO_LK 27       // Control pin for lock actuation servo
#define SERVO_WND 25      // Control pin for window servo
#define UNLOCK_SIGNAL 0   // Number representing unlock command
#define LOCK_SIGNAL 1     // Number representing lock command
#define WINDOW_SIGNAL 2   // Number representing the toggle to window mode command
#define WINDOW_UP_SIGNAL 3    // Number represneting the roll up window command 
#define WINDOW_DOWN_SIGNAL 4  // Number representing the roll down window command

using namespace bevis_FinalProject;

RollingCode rollingCode = RollingCode(237461); // seed of 237461 with default m, a, and c values

struct timeval tv_now;

// Structure to send data
typedef struct struct_message {
    struct timeval tv_now;
    unsigned long rollingCode;
    byte action;
};
struct_message message;

enum LK_State{Lock, Unlock, Window};
LK_State state;
LK_State state_old;

bool windowMode = false;

Servo servo_lk;
Servo servo_wnd;

void setup() {
  Serial.begin(115200);

  state = Unlock;
  updateLockState();
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Set output LED pins
  pinMode(LOCK_LED, OUTPUT);
  pinMode(UNLOCK_LED, OUTPUT);
  pinMode(WINDOW_LED, OUTPUT);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Register callback for receiving data
  esp_now_register_recv_cb(OnDataRecv);

  // setup servo motors 
  //    NOTE: calibrate potentiometer on servo so that 20 is stationary
  servo_lk.setPeriodHertz(50); 
  servo_lk.attach(SERVO_LK, 500, 2400);
  servo_lk.write(20);

  servo_wnd.setPeriodHertz(50); 
  servo_wnd.attach(SERVO_WND, 500, 2400);
  servo_wnd.write(20);

}

void loop() {
  // put your main code here, to run repeatedly:

}

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&message, incomingData, sizeof(message));
  gettimeofday(&tv_now, NULL);

  int64_t thisTime = (int64_t)tv_now.tv_sec * 1000000L + (int64_t)tv_now.tv_usec;
  int64_t otherTime = (int64_t)message.tv_now.tv_sec * 1000000L + (int64_t)message.tv_now.tv_usec;

  if (rollingCode.matches(message.rollingCode) && isValidTime(thisTime, otherTime)) {
    Serial.println("recieved valid signal...");
    if(windowMode) {
      Serial.println("window mode doing stuff...");
      switch(message.action) {
        case WINDOW_UP_SIGNAL:
          servo_wnd.write(40);
          digitalWrite(LOCK_LED, HIGH);
          digitalWrite(UNLOCK_LED, LOW);
          Serial.println("window up...");
          break;
        case LOCK_SIGNAL:
          servo_wnd.write(20);
          digitalWrite(LOCK_LED, LOW);
          digitalWrite(UNLOCK_LED, LOW);
          Serial.println("window stop...");
          break;
        case WINDOW_DOWN_SIGNAL:
          servo_wnd.write(0);
          digitalWrite(LOCK_LED, LOW);
          digitalWrite(UNLOCK_LED, HIGH);
          Serial.println("window down...");
          break;
        case UNLOCK_SIGNAL:
          servo_wnd.write(20);
          digitalWrite(LOCK_LED, LOW);
          digitalWrite(UNLOCK_LED, LOW);
          Serial.println("window stop...");
          break;
        case WINDOW_SIGNAL:
          windowMode = false;
          state = state_old;
          state_old = Window;
          digitalWrite(WINDOW_LED, LOW);
          Serial.println("window mode turning off...");
          servo_wnd.write(20);
          break;
        default:
          break;
      }
    }
    else {
      Serial.println("normal mode doing stuff...");
      switch(message.action) {
        case LOCK_SIGNAL:
          state_old = state;
          state = Lock;
          Serial.println("lock...");
          break;
        case UNLOCK_SIGNAL:
          Serial.println("unlock...");
          state_old = state;
          state = Unlock;
          break;
        case WINDOW_SIGNAL:
          windowMode = true;
          state_old = state;
          state = Window;
          Serial.println("window mode turning on...");
          digitalWrite(WINDOW_LED, HIGH);
          break;
        default:
          break;
      }
      updateLockState();
    }
  }
}

void updateLockState() {
  if (state == state_old) {return;}
  switch(state) {
    case Lock:
      servo_lk.write(0);
      digitalWrite(LOCK_LED, HIGH);
      digitalWrite(UNLOCK_LED, LOW);
      delay(300);
      servo_lk.write(20);
      break;
    case Unlock:
      servo_lk.write(40);
      digitalWrite(LOCK_LED, LOW);
      digitalWrite(UNLOCK_LED, HIGH);
      delay(300);
      servo_lk.write(20);
      break;
    default:
      break;
  }
}

// isValidTime
// returns true if otherTime is within 1 minute of thisTime
// thisTime and otherTime are represented in microseconds
bool isValidTime(int64_t thisTime, int64_t otherTime) {
  int64_t minute = 60 * 1000000L;
  return (otherTime - thisTime) < minute;
}

