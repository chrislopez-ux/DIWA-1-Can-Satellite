
#include "ESP32_NOW.h"
#include "WiFi.h"
#include <esp_mac.h> // For the MAC2STR and MACSTR macros
#include <esp_wifi.h>

#include <Wire.h>
#include <Adafruit_MPL3115A2.h>

/* Definitions */
#define ESPNOW_WIFI_CHANNEL 6

/*data packet stuffz*/
typedef struct {
  float pressure_inhg;
  float altitude_m;
  float temperature_c;
  uint32_t msg_count;
} sigmasensorskibidisixseven;

/* Classes */

//creating a new class that inherits from the ESP_NOW_Peer class
class ESP_NOW_Broadcast_Peer : public ESP_NOW_Peer {
public:
  // constructor of the class using the broadcast address
  ESP_NOW_Broadcast_Peer(uint8_t channel, wifi_interface_t iface, const uint8_t *lmk) : ESP_NOW_Peer(ESP_NOW.BROADCAST_ADDR, channel, iface, lmk) {}
  // destructor of the class
  ~ESP_NOW_Broadcast_Peer() { 
    remove(); 
  }

  //initialize the ESP-NOW and register the broadcast peer
  bool begin() {
    if (!ESP_NOW.begin() || !add()) {
      log_e("Failed to initialize ESP-NOW or register broadcast peer");
      return false;
    }
    return true;
  }

  // function to send a message to all devices within the network
  bool send_message(const uint8_t *data, size_t len) {
    if (!send(data, len)) {
      log_e("Failed to broadcast message");
      return false;
    }
    return true;
  }
};

/* global variables */
Adafruit_MPL3115A2 baro = Adafruit_MPL3115A2();
// create a broadcast peer object
ESP_NOW_Broadcast_Peer broadcast_peer(ESPNOW_WIFI_CHANNEL, WIFI_IF_STA, nullptr);
uint32_t msg_count = 0;

/* main */

void setup() {
  Serial.begin(115200);
  Wire.begin();

  WiFi.mode(WIFI_STA);
  esp_wifi_set_protocol(WIFI_IF_STA, WIFI_PROTOCOL_LR);
  WiFi.setChannel(ESPNOW_WIFI_CHANNEL);
  while (!WiFi.STA.started()) delay(100);

  Serial.println("Sender ready. MAC: " + WiFi.macAddress());

  if (!broadcast_peer.begin()) {
    Serial.println("it failed so esp resetting now");
    delay(5000);
    ESP.restart();
  }

  if (!baro.begin()) {
    Serial.println("cant find sensor chck wiring and stuff");
    while (1) delay(100);
  }
  baro.setSeaPressure(1012);

  Serial.println("Sensor Found");
}

void loop() {
  sigmasensorskibidisixseven barometermessagething;
  barometermessagething.pressure_inhg = baro.getPressure();
  barometermessagething.altitude_m    = baro.getAltitude();
  barometermessagething.temperature_c = baro.getTemperature();
  barometermessagething.msg_count     = msg_count++;

  Serial.printf("[Message #%lu] %.4f hPa | %.2f m | %.2f C\n",
    barometermessagething.msg_count, barometermessagething.pressure_inhg,
    barometermessagething.altitude_m, barometermessagething.temperature_c);

  if (!broadcast_peer.send_message((uint8_t *)&barometermessagething, sizeof(barometermessagething))) {
    Serial.println("Broadcast failed.");
  }

  delay(150);
}