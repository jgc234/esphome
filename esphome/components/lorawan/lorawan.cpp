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

void LoRaWANComponent::setup() { ESP_LOGI(TAG, "Initializing SX1262..."); }

void LoRaWANComponent::join_network_() { ESP_LOGI(TAG, "Joining LoRaWAN (OTAA)..."); }

void LoRaWANComponent::loop() {}

void LoRaWANComponent::send_uplink_() {}

}  // namespace lorawan
}  // namespace esphome
