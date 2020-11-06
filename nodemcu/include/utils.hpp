#ifndef IOP_UTILS_H_
#define IOP_UTILS_H_

#include <Arduino.h>
#include <DHT.h>
#include <DallasTemperature.h>
#include <models.hpp>

// (Un)Comment this line to toggle wifi dependency
#define IOP_ONLINE

// (Un)Comment this line to toggle monitor server dependency
#define IOP_MONITOR

// (Un)Comment this line to toggle serial dependency
#define IOP_SERIAL

// (Un)Comment this line to toggle sensors dependency
//#define IOP_SENSORS

enum InterruptEvent {
  NONE,
  WPS
};

void handlePlantId(const AuthToken token, const String macAddress);
void handleInterrupt();
AuthToken stringToAuthToken(const String &val);
PlantId stringToPlantId(const String &val);

class MockSerial {
  public:
    MockSerial() {}
    void begin(unsigned long baud) {}
    void flush() {}
    size_t println(const char * msg) { return 0; }
    size_t println(const String &s) { return 0; }
};

static MockSerial mockSerial;

#ifndef IOP_SERIAL
  #define Serial MockSerial
#endif

#undef panic
#define panic(msg) panic__(String(msg), String(__FILE__), (uint32_t) __LINE__, String(__PRETTY_FUNCTION__))
void panic__(const String msg, const String file, const uint32_t line, const String func) __attribute__((noreturn));

template<typename T>
class Option {
 private:
  bool filled;
  T value;
 public:
  Option(): filled(false), value{0} {}
  Option(const T v): filled(true), value(v) {}

  bool isSome() const { return filled; }
  bool isNone() const { return !filled; }
  T unwrap() const { return expect("Tried to unwrap an empty Option"); }
  T expect(const String msg) const {
    if (!filled) { panic(msg); }
    return value;
  }
  T unwrap_or(const T or_) const {
    if (isSome()) { return value; }
    return or_;
  }
    
  template <typename U>
  Option<U> andThen(std::function<Option<U> (const T&)> f) const {
    if (isSome()) {
      return f(value);
    }
    return Option<U>();
  }

  template <typename U>
  Option<U> map(std::function<U (const T&)> f) const {
    if (isSome()) {
      return Option<U>(f(value));
    }
    return Option<U>();
  }
};

static volatile enum InterruptEvent interruptEvent = NONE;

#endif