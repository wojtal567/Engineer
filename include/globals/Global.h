#pragma once

#include <Wire.h>

#include "../configs/pin_conf.h"
#include "MySD.hpp"

namespace Global {
extern Config config;
extern int ntpTimeOffset;  // TODO maybe add this to the config in time settings scr

constexpr int SCREEN_WIDTH = 320;
constexpr int SCREEN_HEIGHT = 240;

extern const char* labels[15];  // TODO think what to do about it

extern std::map<std::string, float> data;  // TODO check what is that? why it's needed?
extern std::string configFilePath;         // TODO should be passed in c-tor for mySDCard

// NTPClient
static const char ntpServerName[] = "europe.pool.ntp.org";  // TODO maybe some config file?
}  // namespace Global
