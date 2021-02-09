#include "measurement.hpp"

#ifndef IOP_MEASUREMENT_DISABLED
#include "Arduino.h"
#include "core/string/static.hpp"
#include "core/string/unsafe_raw.hpp"
#include "core/string/view.hpp"
#include "core/tracer.hpp"

namespace measurement {
auto soilTemperatureCelsius(DallasTemperature &sensor) noexcept -> float {
  IOP_TRACE();
  // Blocks until reading is done
  sensor.requestTemperatures();
  // Accessing by index is bad. It's slow, we should store the sensor's address
  // and use it
  return sensor.getTempCByIndex(0);
}

auto airTemperatureCelsius(DHT &dht) noexcept -> float {
  IOP_TRACE();
  return dht.readTemperature();
}

auto airHumidityPercentage(DHT &dht) noexcept -> float {
  IOP_TRACE();
  return dht.readHumidity();
}

auto airHeatIndexCelsius(DHT &dht) noexcept -> float {
  IOP_TRACE();
  return dht.computeHeatIndex();
}

auto soilResistivityRaw(const uint8_t powerPin) noexcept -> uint16_t {
  IOP_TRACE();
  digitalWrite(powerPin, HIGH);
  delay(2000); // NOLINT *-avoid-magic-numbers
  uint16_t value1 = analogRead(A0);
  delay(500); // NOLINT *-avoid-magic-numbers
  uint16_t value2 = analogRead(A0);
  delay(500); // NOLINT *-avoid-magic-numbers
  uint16_t value = (value1 + value2 + analogRead(A0)) / 3;
  digitalWrite(powerPin, LOW);
  return value;
}
#else
namespace measurement {
auto soilTemperatureCelsius(DallasTemperature &sensor) noexcept -> float {
  IOP_TRACE();
  (void)sensor;
  return 0.;
}
auto airTemperatureCelsius(DHT &dht) noexcept -> float {
  IOP_TRACE();
  (void)dht;
  return 0.;
}
auto airHumidityPercentage(DHT &dht) noexcept -> float {
  IOP_TRACE();
  (void)dht;
  return 0.;
}
auto airHeatIndexCelsius(DHT &dht) noexcept -> float {
  IOP_TRACE();
  (void)dht;
  return 0.;
}
auto soilResistivityRaw(const uint8_t powerPin) noexcept -> uint16_t {
  IOP_TRACE();
  (void)powerPin;
  return 0.;
}
#endif
} // namespace measurement