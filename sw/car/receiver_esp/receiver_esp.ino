#include <ESP8266WiFi.h>
#include <espnow.h>
#include <stdint.h>

#define RDY 2

// Callback function that will be executed when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  Serial.write(*incomingData);
}
 
void setup() {
  Serial.begin(115200);
  
  WiFi.mode(WIFI_STA);

  pinMode(0, INPUT);
  pinMode(2, OUTPUT);
  digitalWrite(RDY, LOW);
  
  // Init ESP-NOW
  if (esp_now_init() != 0) {
    return;
  }
  
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
  digitalWrite(RDY, HIGH);
}

void loop() {
  
}
