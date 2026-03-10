#include "lorawan.h"
#include "esphome/core/log.h"

namespace esphome {
namespace lorawan {

static const char *TAG = "lorawan";

void LoRaWANComponent::setup() { ESP_LOGI(TAG, "Initializing SX1262..."); }

void LoRaWANComponent::join_network_() { ESP_LOGI(TAG, "Joining LoRaWAN (OTAA)..."); }

void LoRaWANComponent::loop() {}

void LoRaWANComponent::send_uplink_() {}

}  // namespace lorawan
}  // namespace esphome
