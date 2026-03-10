#include "lorawan.h"
#include "esphome/core/log.h"

namespace esphome {
namespace lorawan {

static const char *const TAG = "lorawan";

void LoRaWANComponent::setup() {
  ESP_LOGI(TAG, "Initializing SX1262...");

  // at this stage we can't access the chip select pin because its owned by the
  // upstream spi_device and the field is private.

  Module *module = new Module(this, RADIOLIB_NC, this->dio1_pin_->get_pin(), this->reset_pin_->get_pin(),
                              this->busy_pin_->get_pin());
  this->radio_ = new SX1262(module);

  int16_t state = radio_->begin();
  if (state != RADIOLIB_ERR_NONE) {
    ESP_LOGE(TAG, "Radio did not initialize. We'll try again later.");
  }
}

void LoRaWANComponent::join_network_() { ESP_LOGI(TAG, "Joining LoRaWAN (OTAA)..."); }

void LoRaWANComponent::loop() {}

void LoRaWANComponent::send_uplink_() {}

}  // namespace lorawan
}  // namespace esphome
