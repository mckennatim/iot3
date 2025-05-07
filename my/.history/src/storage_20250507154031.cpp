// This project uses the Preferences library for ESP32/ESP8266 to store configuration data in non-volatile memory.
// store.cpp
#include "storage.h"

Preferences store;  // Actual global instance

void initStore() {
  store.begin("my-app", false);  // false = read/write mode
}

int getStore(const char* key, int defaultValue) {
  return store.getInt(key, defaultValue);
}

void setStored(const char* key, int value) {
  store.putInt(key, value);
}