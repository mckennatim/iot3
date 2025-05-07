// store.h
#pragma once
#include <Preferences.h>

extern Preferences store;  // Declare as extern (defined in .cpp)

// Optional helper functions
void initStore();
int getStored(const char* key, int defaultValue);
void setStored(const char* key, int value);