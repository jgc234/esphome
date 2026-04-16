#include "lorawan.h"
#include "esphome/core/log.h"

namespace esphome {
namespace lorawan {

static const char *const TAG = "lorawan";

void LoRaWANComponent::setup() {
  ESP_LOGI(TAG, "Initializing SX1262...");

  // TODO: at this stage we can't access the chip select pin because its owned
  // by the upstream spi_device and the field is private.

  // uint32_t cs, uint32_t irq, uint32_t rst, uint32_t gpio, SPIClass& spi, SPISettings spiSettings

  hspi = new SPIClass(HSPI);

  Module *module = new Module(this, this->cs_pin_->get_pin(), this->dio1_pin_->get_pin(), this->reset_pin_->get_pin(),
                              this->busy_pin_->get_pin(), *hspi);

  bool spi_setup_success =
      hspi->begin(config.spi_clk_pin, config.spi_miso_pin, config.spi_mosi_pin, config.spi_nss_pin);
  if (!spi_setup_success) {
    ESP_LOGE(TAG, "Failed to initialize SPI");
    return;
  }

  PhysicalLayer *radio;

  switch (this->chipset_) {
    case Chipset_SX1262: {
      ESP_LOGI(TAG, "Chipset_SX1262 - Creating SX1262 instance...");

      SX1262 *radio = new SX1262(module);
      this->radio_ = radio;
      ESP_LOGI(TAG, "Creating lorawan node instance...");
      this->node_ = new LoRaWANNode(radio, this->band_ptr_, this->sub_band_);
      int16_t state;

      ESP_LOGI(TAG, "Initializing radio...");

      state = radio->begin();
      if (state != RADIOLIB_ERR_NONE) {
        ESP_LOGE(TAG, "Radio did not initialize. Error code: %d", state);
        return;
      }

      ESP_LOGI(TAG, "Radio initialized successfully!");
      ESP_LOGI(TAG, "Initializing LoRaWAN node...");
      state = this->node_->beginOTAA(this->join_eui_, this->dev_eui_, this->nwk_key_, this->app_key_);
      if (state != RADIOLIB_ERR_NONE) {
        ESP_LOGE(TAG, "Failed to start joining process. Error code: %d", state);
        return;
      }

      ESP_LOGI(TAG, "Starting OTAA join process...");
      state = this->node_->activateOTAA();
      if (state != RADIOLIB_ERR_NONE) {
        ESP_LOGE(TAG, "Failed to start joining process. Error code: %d", state);
        return;
      }

      this->radio_ = radio;
      break;
    }

    case Chipset_SX1276: {
      SX1276 *radio = new SX1276(module);
      this->radio_ = radio;
      radio->begin();
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
                "  Chipset: %d\n"
                "  Band: %d\n"
                "  Sub Band: %d\n"
                "  Join EUI: 0x%016llX\n"
                "  Dev EUI: 0x%016llX\n"
                "  App Key: %p\n"
                "  NWK Key: %p\n",
                this->chipset_, this->band_, this->sub_band_, this->join_eui_, this->dev_eui_, this->app_key_,
                this->nwk_key_);
}

}  // namespace lorawan
}  // namespace esphome
