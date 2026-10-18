#include "ESP32_NOW.h"
#include "WiFi.h"
#include <esp_mac.h>
#include <vector>
#include <esp_wifi.h>
#define ESPNOW_WIFI_CHANNEL 6

typedef struct {
  float pressure_inhg;
  float altitude_m;
  float temperature_c;
  uint32_t msg_count;
} sigmasensorskibidisixseven;

class ESP_NOW_Peer_Class : public ESP_NOW_Peer { 
public:
  ESP_NOW_Peer_Class(const uint8_t *mac_addr, uint8_t channel, wifi_interface_t iface, const uint8_t *lmk)
    : ESP_NOW_Peer(mac_addr, channel, iface, lmk) {}

  ~ESP_NOW_Peer_Class() {}

  bool add_peer() {
    if (!add()) {
      log_e("Failed to register peer");
      return false;
    }
    return true;
  }

  void onReceive(const uint8_t *data, size_t len, bool broadcast) {
    if (len != sizeof(sigmasensorskibidisixseven)) {
      Serial.printf("Unexpected packet size: %d bytes (expected %d)\n",
        len, sizeof(sigmasensorskibidisixseven));
      return;
    }

    sigmasensorskibidisixseven barometermessagething;
    memcpy(&barometermessagething, data, sizeof(barometermessagething));

    Serial.println("----------------------------------------");
    Serial.printf("Message #%lu from " MACSTR "\n",
      barometermessagething.msg_count, MAC2STR(addr()));
    Serial.printf("  Pressure    : %.4f Hectopascal hPa\n", barometermessagething.pressure_inhg);
    Serial.printf("  Altitude    : %.2f meters\n",      barometermessagething.altitude_m);
    Serial.printf("  Temperature : %.2f *C\n",          barometermessagething.temperature_c);
    Serial.println("----------------------------------------");
  }
};

std::vector<ESP_NOW_Peer_Class *> senders;

void register_new_sender(const esp_now_recv_info_t *info, const uint8_t *data, int len, void *arg) {
  if (memcmp(info->des_addr, ESP_NOW.BROADCAST_ADDR, 6) == 0) {
    Serial.printf("New sender detected: " MACSTR "\n", MAC2STR(info->src_addr));
    ESP_NOW_Peer_Class *new_sender = new ESP_NOW_Peer_Class(
      info->src_addr, ESPNOW_WIFI_CHANNEL, WIFI_IF_STA, nullptr);
    if (!new_sender->add_peer()) {
      Serial.println("Failed to register sender.");
      delete new_sender;
      return;
    }
    senders.push_back(new_sender);
    Serial.printf("Sender registered. Total senders: %zu\n", senders.size());
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  esp_wifi_set_protocol(WIFI_IF_STA, WIFI_PROTOCOL_LR);
  WiFi.setChannel(ESPNOW_WIFI_CHANNEL);
  while (!WiFi.STA.started()) delay(100);

  Serial.println("Receiver ready. MAC: " + WiFi.macAddress());

  if (!ESP_NOW.begin()) {
    Serial.println("ESP-NOW init failed. Rebooting...");
    delay(5000);
    ESP.restart();
  }

  ESP_NOW.onNewPeer(register_new_sender, nullptr);
  Serial.println("Waiting for sensor broadcasts...");
}

void loop() {
  delay(250);
}