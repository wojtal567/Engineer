#include "globals/Global.h"

namespace Global {
Config config = {"", "", 60000, 3600000, 30000, 5, 0, 30000};
// TODO maybe add this to the time settings?
int ntpTimeOffset = 3600;  // poland, winter - 3600, summer (DST) - 7200
const char *labels[15] = {"framelen",       "pm10_standard",  "pm25_standard",   "pm100_standard", "pm10_env",
                          "pm25_env",       "pm100_env",      "particles_03um",  "particles_05um", "particles_10um",
                          "particles_25um", "particles_50um", "particles_100um", "unused",         "checksum"};
std::string configFilePath = "/settings.json";
std::map<std::string, float> data = {};
}