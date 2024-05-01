#include <ESP8266WiFi.h>
#include <espnow.h>

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define RDY 2

// REPLACE WITH RECEIVER MAC ADDRESS
uint8_t broadcastAddress[] = {0x50, 0x02, 0x91, 0xF9, 0xAD, 0x5B};

unsigned long lastTime = 0;  
unsigned long timerDelay = 50;

// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.flush();
}
 
void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(0, INPUT);                        // Set GPIO_0 to an input
  pinMode(2, OUTPUT);                       // Set GPIO_2 to an output - RDY signal to the TM4C
  digitalWrite(RDY, LOW);                   // Set the RDY pin LOW
 
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != 0) {
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

  digitalWrite(RDY, HIGH);                 
}
 
void loop() {
  if ((millis() - lastTime) > timerDelay) {
    tm4c_to_esp();
    lastTime = millis();
  }
}

void tm4c_to_esp(void) {
  if(Serial.available()){
    uint8_t data = Serial.read();
    esp_now_send(broadcastAddress, &data, sizeof(uint8_t));
  }
}
