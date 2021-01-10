#include <SD.h>
#include "SQLiteDb.hpp"
#include <string>
struct Config
{
    std::string ssid;            // * WiFi ssid
    std::string password;        // * WiFi password
    int lcdLockTime;             // * time after
    int timeBetweenSavingSample; // * time between sampling @measurePeriod
    int measurePeriod;           // * time of taking samples to calculate average
    uint countOfSamples;         // * count of samples used to calculate average sample
    uint currentSampleNumber;    // * number of currently taken sample
    int turnFanTime;             // * time of turning fan on
};

class MySD
{
private:
    int _port;

public:
    MySD(int port);
    bool begin();
    void end();
    bool start(SQLiteDb *object, Stream *debugger);
    void save(std::map<std::string, float> data, float temperature, float humidity, String timestamp, SQLiteDb *object, Stream *debugger);
    void select(SQLiteDb *object, Stream *debugger, String datetime, JsonArray *array);
    void getLastRecord(SQLiteDb *object, Stream *debugger, JsonArray *array);
    void saveConfig(Config config, std::string filePath);
    void loadConfig(Config &config, std::string filePath);
    void printConfig(std::string filePath);
    void loadWiFi(Config &config, std::string filePath);
};
