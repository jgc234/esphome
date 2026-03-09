#include "lorawan.h"
#include "esphome/core/log.h"

namespace esphome {
namespace lorawan {

static const char *TAG = "lorawan";

void LoRaWANComponent::set_pins(int cs, int reset, int busy, int dio1) {
  cs_pin_ = cs;
  reset_pin_ = reset;
  busy_pin_ = busy;
  dio1_pin_ = dio1;
}

void LoRaWANComponent::set_otaa(std::string join_eui, std::string dev_eui, std::string app_key, std::string nwk_key) {
  join_eui_ = join_eui;
  dev_eui_ = dev_eui;
  app_key_ = app_key;
  nwk_key_ = nwk_key;
}

void LoRaWANComponent::setup() {
  ESP_LOGI(TAG, "Initializing SX1262...");

  Serial.begin(115200);
  while (!Serial)
    ;
  delay(5000);  // Give time to switch to the serial monitor
  Serial.println(F("\nSetup ... "));

  Serial.println(F("Initialise the radio"));
  int16_t state = radio.begin();
  debug(state != RADIOLIB_ERR_NONE, F("Initialise radio failed"), state, true);

  // Setup the OTAA session information
  state = node.beginOTAA(joinEUI, devEUI, nwkKey, appKey);
  debug(state != RADIOLIB_ERR_NONE, F("Initialise node failed"), state, true);

  Serial.println(F("Join ('login') the LoRaWAN Network"));
  state = node.activateOTAA();
  debug(state != RADIOLIB_LORAWAN_NEW_SESSION, F("Join failed"), state, true);

  Serial.println(F("Ready!\n"));
}

void LoRaWANComponent::join_network_() {
  ESP_LOGI(TAG, "Joining LoRaWAN (OTAA)...");

  // int state = node_->beginOTAA(dev_eui_.c_str(), join_eui_.c_str(), app_key_.c_str(), nwk_key_.c_str());

  // if (state == RADIOLIB_ERR_NONE) {
  //   ESP_LOGI(TAG, "Joined successfully!");
  //   joined_ = true;
  // } else {
  //   ESP_LOGE(TAG, "Join failed: %d", state);
  // }
}

void LoRaWANComponent::loop() {
  if (!joined_)
    return;

  // if (millis() - last_send_ > 60000) {
  //   send_uplink_();
  //   last_send_ = millis();
  // }
}

void LoRaWANComponent::send_uplink_() {
  uint8_t payload[2] = {0x01, 0x02};

  ESP_LOGI(TAG, "Sending uplink...");

  int state = node_->sendReceive(payload, sizeof(payload));

  if (state == RADIOLIB_ERR_NONE) {
    ESP_LOGI(TAG, "Uplink sent");
  } else {
    ESP_LOGE(TAG, "Send failed: %d", state);
  }
}

}  // namespace lorawan
}  // namespace esphome
