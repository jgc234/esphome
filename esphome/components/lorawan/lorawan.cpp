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

  PhysicalLayer *radio;

  switch (this->chipset_) {
    case Chipset_SX1262: {
      SX1262 *radio = new SX1262(module);
      radio->begin();
      // this->radio_ = radio;
      int16_t state = radio->begin();
      if (state != RADIOLIB_ERR_NONE) {
        ESP_LOGE(TAG, "Radio did not initialize. We'll try again later.");
        return;
      }
      break;
    }

    case Chipset_SX1276: {
      SX1276 *radio = new SX1276(module);
      radio->begin();
      this->radio_ = radio;
      break;
    }
  }
}

void LoRaWANComponent::join_network_() { ESP_LOGI(TAG, "Joining LoRaWAN (OTAA)..."); }

void LoRaWANComponent::loop() {}

void LoRaWANComponent::send_uplink_() {}

void LoRaWANComponent::dump_config() {
  ESP_LOGCONFIG(TAG,
                "LoraWAN Component:\n"
                "  Chipset: %s\n"
                "  Band: %d\n"
                "  Sub Band: %d\n"
                "  Join EUI: 0x%016llX\n"
                "  Dev EUI: 0x%016llX\n"
                "  App Key: %p\n"
                "  NWK Key: %p\n",
                this->chipset_.c_str(), this->band_, this->sub_band_, this->join_eui_, this->dev_eui_, this->app_key_,
                this->nwk_key_);
}

}  // namespace lorawan
}  // namespace esphome
