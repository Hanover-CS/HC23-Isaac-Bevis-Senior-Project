#include <esp_now.h>
#include<WiFi.h>

// NOTE: MAC addresses:
//    1: 94:B5:55:2D:35:BC (This, sender)
//    2: 94:B5:55:26:44:B8 (Other, receiver)

// MAC Address of the ESP32 to send message to
uint8_t broadcastAddress[] = {0x94, 0xB5, 0x55, 0x26, 0x44, 0xB8}; // ESP32 2

esp_now_peer_info_t peerInfo;

// Structure to send data
typedef struct struct_message {
    String a;
    bool b;
} struct_message;

struct_message LED_Off_Sig;
struct_message LED_On_Sig;

// callback for when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  Serial.begin(115200);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  pinMode(LED_BUILTIN, OUTPUT);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  LED_Off_Sig.a = "LED Off signal";
  LED_Off_Sig.b = false;
  
  LED_On_Sig.a = "LED On signal";
  LED_On_Sig.b = true;

  // Register the send callback
  esp_now_register_send_cb(OnDataSent);

  // Register a peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;  // NOTE: ESP-NOW messages can be encrypted

  // Add the peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  Serial.println("Sending off message...");
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &LED_Off_Sig, sizeof(LED_Off_Sig));
  digitalWrite(LED_BUILTIN, HIGH);
  reportError(result);
  delay(2000);
  Serial.println("Sending on message...");
  result = esp_now_send(broadcastAddress, (uint8_t *) &LED_On_Sig, sizeof(LED_On_Sig));
  digitalWrite(LED_BUILTIN, LOW);
  reportError(result);
  delay(2000);

}

void reportError(esp_err_t result){
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
}
