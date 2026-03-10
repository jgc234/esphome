#pragma once

#include "esphome/core/component.h"
#include "esphome/core/preferences.h"

// do equivalent to ropg/LoRaWAN_ESP32

#include "lorawan_spi_hal.h"

namespace esphome {
namespace lorawan {

class LoRaWANComponent : public Component, public LoraWanSpiRadioLibHal {
 public:
  void set_chipset(std::string chipset) { this->chipset_ = chipset; }
  void set_reset_pin(InternalGPIOPin *pin) { this->reset_pin_ = pin; }
  void set_busy_pin(InternalGPIOPin *pin) { this->busy_pin_ = pin; }
  void set_dio1_pin(InternalGPIOPin *pin) { this->dio1_pin_ = pin; }
  void set_band(int band) { this->band_ = band; }
  void set_sub_band(uint8_t sub_band) { this->sub_band_ = sub_band; }
  void set_join_eui(uint64_t join_eui) { this->join_eui_ = join_eui; }
  void set_dev_eui(uint64_t dev_eui) { this->dev_eui_ = dev_eui; }
  //  void set_app_key(uint8_t *app_key) { this->app_key_ = app_key; }
  //  void set_nwk_key(uint8_t *nwk_key) { this->nwk_key_ = nwk_key; }
  void set_app_key(std::initializer_list<uint8_t> key) { this->app_key_vec_ = std::vector<uint8_t>(key); }
  void set_nwk_key(std::initializer_list<uint8_t> key) { this->nwk_key_vec_ = std::vector<uint8_t>(key); }

  void setup() override;
  void loop() override;
  void dump_config() override;

 protected:
  InternalGPIOPin *reset_pin_{nullptr};
  InternalGPIOPin *busy_pin_{nullptr};
  InternalGPIOPin *dio1_pin_{nullptr};
  std::string chipset_;
  int band_;
  uint8_t sub_band_;
  uint64_t join_eui_;
  uint64_t dev_eui_;
  uint8_t *app_key_{nullptr};
  uint8_t *nwk_key_{nullptr};
  std::vector<uint8_t> app_key_vec_;
  std::vector<uint8_t> nwk_key_vec_;
  SX1262 *radio_{nullptr};
  LoRaWANNode *node_{nullptr};
  bool joined_{false};
  unsigned long last_send_{0UL};
  void join_network_();
  void send_uplink_();
};

}  // namespace lorawan
}  // namespace esphome
