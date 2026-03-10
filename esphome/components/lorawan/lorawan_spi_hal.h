#pragma once

#include "esphome/components/spi/spi.h"
#include "esphome/core/log.h"

namespace esphome {
namespace lorawan {

#include <RadioLib.h>

// This file provides a hardware abstraction layer for the RadioLib library,
// using the ESPHome SPI and GPIO APIs. It implements the pure virtual methods
// required by RadioLib to function, as well as some of the virtual methods for
// convenience. The LoRaWANComponent class will inherit from this to provide the
// necessary functionality to use RadioLib with ESPHome.

const uint32_t RL_INPUT = 0x01;
const uint32_t RL_OUTPUT = 0x03;
const uint32_t RL_LOW = 0x00;
const uint32_t RL_HIGH = 0x01;
const uint32_t RL_RISING = 0x01;
const uint32_t RL_FALLING = 0x02;

class EspHomeSpi : public esphome::spi::SPIDevice<esphome::spi::BIT_ORDER_MSB_FIRST, esphome::spi::CLOCK_POLARITY_LOW,
                                                  esphome::spi::CLOCK_PHASE_LEADING, esphome::spi::DATA_RATE_8MHZ> {};

class LoraWanSpiRadioLibHal : public RadioLibHal, public EspHomeSpi {
 public:
  LoraWanSpiRadioLibHal() : EspHomeSpi(), RadioLibHal(RL_INPUT, RL_OUTPUT, RL_LOW, RL_HIGH, RL_RISING, RL_FALLING){};

  // implementations of pure virtual RadioLibHal methods
  void pinMode(uint32_t pin, uint32_t mode) override {}
  void digitalWrite(uint32_t pin, uint32_t value) override {}
  uint32_t digitalRead(uint32_t pin) override { return 0; }
  void attachInterrupt(uint32_t interruptNum, void (*interruptCb)(void), uint32_t mode) override {}
  void detachInterrupt(uint32_t interruptNum) override {}
  void delay(RadioLibTime_t ms) override { esphome::delay(ms); }
  void delayMicroseconds(RadioLibTime_t us) override { esphome::delayMicroseconds(us); }
  RadioLibTime_t millis() override { return ((unsigned long) esphome::millis()); }
  RadioLibTime_t micros() override { return ((unsigned long) esphome::micros()); }
  long pulseIn(uint32_t pin, uint32_t state, RadioLibTime_t timeout) override { return 0; }
  void spiBegin() override {}
  void spiBeginTransaction() override {}
  void spiTransfer(uint8_t *out, size_t len, uint8_t *in) override {
    for (size_t i = 0; i < len; i++) {
      in[i] = EspHomeSpi::transfer_byte(out[i]);
    }
  }
  void spiEndTransaction() override{};
  void spiEnd() override{};

  // implementations of virtual RadioLibHal methods
  void init() override{};
  void term() override{};
  void yield() override { ::esphome::yield(); }

 protected:
};

}  // namespace lorawan
}  // namespace esphome
