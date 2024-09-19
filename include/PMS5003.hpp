#pragma once
#include <Arduino.h>
#include <HardwareSerial.h>
#include <stdint.h>

#include <iterator>
#include <map>
#include <string>
#include <vector>

#define BUFFERSIZE 32
#define VALUES 15

class PMS5003 {
   private:
    HardwareSerial *_reader, *_debugger;
    std::map<std::string, float> data;
    const char *labels[15] = {"framelen",       "pm10_standard",  "pm25_standard",   "pm100_standard", "pm10_env",
                              "pm25_env",       "pm100_env",      "particles_03um",  "particles_05um", "particles_10um",
                              "particles_25um", "particles_50um", "particles_100um", "unused",         "checksum"};

   public:
    PMS5003(HardwareSerial *reader, HardwareSerial *debugger);
    bool readData();
    std::map<std::string, float> returnData();
    void dumpSamples();
};