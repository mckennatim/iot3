// config_store.cpp
#include "config_store.h"

Preferences prefs;  // Actual global instance

void initPrefs() {
  prefs.begin("my-app", false);  // false = read/write mode
}