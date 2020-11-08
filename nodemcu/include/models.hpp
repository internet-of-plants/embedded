#ifndef IOP_MODELS_H_
#define IOP_MODELS_H_

#include <array>
#include <cstdint>

// TODO: Add type safety, so we can't conflict with arrays of the same size
typedef std::array<uint8_t, 64> AuthToken;
typedef std::array<uint8_t, 19> PlantId;

typedef struct event {
  float airTemperatureCelsius;
  float airHumidityPercentage;
  float airHeatIndexCelsius;
  uint16_t soilResistivityRaw;
  float soilTemperatureCelsius;
  PlantId plantId;
} Event;

#endif
