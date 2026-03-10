#pragma once

#include "esphome/core/component.h"
#include "esphome/core/preferences.h"

#include "lorawan_spi_hal.h"

// do equivalent to ropg/LoRaWAN_ESP32

namespace esphome {
namespace lorawan {

class LoRaWANComponent : public Component, public LoraWanSpiRadioLibHal {
 public:
  void set_cs_pin(GPIOPin *pin) { cs_pin_ = pin; }
  void set_reset_pin(GPIOPin *pin) { reset_pin_ = pin; }
  void set_busy_pin(GPIOPin *pin) { busy_pin_ = pin; }
  void set_dio1_pin(GPIOPin *pin) { dio1_pin_ = pin; }
  void set_band(int band) { band_ = band; }
  void set_sub_band(uint8_t sub_band) { sub_band_ = sub_band; }
  void set_join_eui(uint64_t join_eui) { join_eui_ = join_eui; }
  void set_dev_eui(uint64_t dev_uei) { dev_eui_ = dev_eui; }
  void set_app_key(uint8_t *app_key) { app_key_ = app_key; }
  void set_nwk_key(uint8_t *nwk_key) { nwk_key_ = nwk_key; }

  void setup() override;
  void loop() override;

 protected:
  GPIOPin *cs_pin_;
  GPIOPin *reset_pin_;
  GPIOPin *busy_pin_;
  GPIOPin *dio1_pin_;
  int band_;
  uint8_t sub_band_;
  uint64_t join_eui_;
  uint64_t dev_eui_;
  uint8_t *app_key_;
  uint8_t *nwk_key_;

  SX1262 *radio_ = nullptr;
  LoRaWANNode *node_ = nullptr;

  bool joined_ = false;
  unsigned long last_send_ = 0;

  void join_network_();
  void send_uplink_();
};

}  // namespace lorawan
}  // namespace esphome
