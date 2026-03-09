#pragma once

#include "esphome/core/component.h"
#include "esphome/core/preferences.h"

#include "lorawan_spi_hal.h"

// do equivalent to ropg/LoRaWAN_ESP32

namespace esphome {
namespace lorawan {

class LoRaWANComponent : public Component, public LoraWanSpiRadioLibHal {
 public:
  void set_pins(int cs, int reset, int busy, int dio1);
  void set_otaa(std::string join_eui, std::string dev_eui, std::string app_key, std::string nwk_key);
  void setup() override;
  void loop() override;

 protected:
  int cs_pin_, reset_pin_, busy_pin_, dio1_pin_;
  std::string band, subband, join_eui_, dev_eui_, app_key_, nwk_key_;

  SX1262 *radio_ = nullptr;
  LoRaWANNode *node_ = nullptr;

  bool joined_ = false;
  unsigned long last_send_ = 0;

  void join_network_();
  void send_uplink_();
};

}  // namespace lorawan
}  // namespace esphome
