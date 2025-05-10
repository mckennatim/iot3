// This project uses the Preferences library for ESP32/ESP8266 to store configuration data in non-volatile memory.
// store.cpp
#include "storage.h"

Preferences store;  // Actual global instance

void initStore() {
  prefs.begin("my-app", false);  // false = read/write mode
}

int getStore(const char* key, int defaultValue) {
  return prefs.getInt(key, defaultValue);
}

void setStored(const char* key, int value) {
  prefs.putInt(key, value);
}