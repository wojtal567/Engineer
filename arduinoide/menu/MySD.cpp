#include "MySD.hpp"

MySD::MySD(int port)
{
    _port = port;
}

bool MySD::begin()
{
    return SD.begin(_port);
}

void MySD::end()
{
    SD.end();
}

bool MySD::start(SQLiteDb *object, Stream *debugger)
{
    bool result = begin();

    if (result)
    {
        Serial.println("MySD::start -> Initializing.");
        object->init();
        object->open();
        object->createTable(debugger);
        object->close();
        object->kill();
    }
    else
        Serial.println("MySD::start -> Can't initilize - no access to SD card.");
    end();
    return result;
}

void MySD::save(std::map<std::string, float> data, float temperature, float humidity, String timestamp, SQLiteDb *object, Stream *debugger)
{
    debugger->println("MySD::save");
    if (begin())
    {
        debugger->println("SD Card detected");
        if (SD.exists(object->getRelativePath()))
        {
            Serial.println("MySD::save -> File exists. Trying to save data.");
            debugger->println("Database " + object->getLocalPath() + " exists. Saving data...");
            object->init();
            object->open();
            object->save(data, temperature, humidity, timestamp, debugger);
            object->close();
            object->kill();
        }
        else
        {
            Serial.println("MySD::save -> No database file. Trying to create one and saving data.");
            debugger->println("Database " + object->getLocalPath() + " don't exist. Saving data...");
            object->init();
            object->open();
            object->createTable(debugger);
            object->save(data, temperature, humidity, timestamp, debugger);
            object->close();
            object->kill();
        }
    }
    else
        Serial.println("MySD::save -> Cannot access SD card.");
    end();
}

void MySD::select(SQLiteDb *object, Stream *debugger, String datetime, JsonArray *array)
{
    debugger->println("MySD::select");
    if (begin())
    {
        debugger->println("SD Card detected");
        if (SD.exists(object->getRelativePath()))
        {
            Serial.println("MySD::select -> File exists. Trying to select data.");
            debugger->println("Database " + object->getLocalPath() + " exists.");
            object->init();
            object->open();
            object->select(debugger, datetime, array);
            object->close();
            object->kill();
        }
        else
            Serial.println("MySD::select -> File don't exist.");
    }
    else
        Serial.println("MySD::select -> Cannot access SD card.");
    end();
}

void MySD::getLastRecord(SQLiteDb *object, Stream *debugger, JsonArray *array)
{
    debugger->println("MySD::getLastRecord");
    if (begin())
    {
        debugger->println("SD Card detected");
        if (SD.exists(object->getRelativePath()))
        {
            Serial.println("MySD::getLastRecord -> File exists. Trying to sget last saved sample record.");
            debugger->println("Database " + object->getLocalPath() + " exists.");
            object->init();
            object->open();
            object->getLastRecord(debugger, array);
            object->close();
            object->kill();
        }
        else
            Serial.println("MySD::getLastRecord -> File don't exist.");
    }
    else
        Serial.println("MySD::getLastRecord -> Cannot access SD card.");
    end();
}

void MySD::saveConfig(Config config, std::string filePath)
{
    if (begin())
    {
        SD.remove(filePath.c_str());
        File configurationFile = SD.open(filePath.c_str(), FILE_WRITE);
        if (!configurationFile)
        {
            Serial.println("Failed to create configuration file.");
            return;
        }
        else
            Serial.println("Configuration file exists. Trying to save...");
        StaticJsonDocument<512> doc;
        doc["ssid"] = config.ssid.c_str();
        doc["password"] = config.password.c_str();
        doc["lcdLockTime"] = config.lcdLockTime;
        doc["timeBetweenSavingSample"] = config.timeBetweenSavingSample;
        doc["measurePeriod"] = config.measurePeriod;
        doc["countOfSamples"] = config.countOfSamples;
        doc["currentSampleNumber"] = config.currentSampleNumber;
        doc["turnFanTime"] = config.turnFanTime;
        if (serializeJson(doc, configurationFile) == 0)
        {
            Serial.println("Failed to write to file.");
        }
        configurationFile.close();
    }
    end();
}

void MySD::loadConfig(Config &config, std::string filePath)
{
    if (begin())
    {
        File configurationFile = SD.open(filePath.c_str(), FILE_READ);
        if (!configurationFile)
        {
            Serial.print("Failed to read configuration file. Creating file...");
            configurationFile.close();
            end();
            saveConfig(config, filePath);
            return;
        }
        else
            Serial.println("Configuration file exists. Reading...");

        StaticJsonDocument<512> doc;
        DeserializationError error = deserializeJson(doc, configurationFile);

        if (error)
        {
            Serial.println("Failed to read file, using default Configuration");
            return;
        }

        char tmp[64];
        strlcpy(tmp, doc["ssid"], sizeof(tmp));
        config.ssid = tmp;
        strlcpy(tmp, doc["password"], sizeof(tmp));
        config.password = tmp;
        config.lcdLockTime = doc["lcdLockTime"];
        config.timeBetweenSavingSample = doc["timeBetweenSavingSample"];
        config.measurePeriod = doc["measurePeriod"];
        config.countOfSamples = doc["countOfSamples"];
        config.currentSampleNumber = doc["currentSampleNumber"];
        config.turnFanTime = doc["turnFanTime"];
        configurationFile.close();
        Serial.println("Reading config and making changes.");
    }
    end();
}

void MySD::printConfig(std::string filePath)
{
    if (begin())
    {
        File configurationFile = SD.open(filePath.c_str(), FILE_READ);
        if (!configurationFile)
        {
            Serial.print("Failed to read configuration file.");
            return;
        }
        else
            Serial.println("Configuration file exists. Reading...");
        Serial.println("Current config.json file:");
        while (configurationFile.available())
            Serial.print((char)configurationFile.read());
        Serial.println();
        configurationFile.close();
    }
    end();
}

void MySD::loadWiFi(Config &config, std::string filePath)
{
    if (begin())
    {
        File configurationFile = SD.open(filePath.c_str(), FILE_READ);
        if (!configurationFile)
        {
            Serial.print("Failed to read configuration file. Creating file...");
            configurationFile.close();
            end();
            saveConfig(config, filePath);
            return;
        }
        else
            Serial.println("Configuration file exists. Reading...");

        StaticJsonDocument<512> doc;
        DeserializationError error = deserializeJson(doc, configurationFile);

        if (error)
        {
            Serial.println("Failed to read file, using default Configuration");
            return;
        }

        char tmp[64];
        strlcpy(tmp, doc["ssid"], sizeof(tmp));
        config.ssid = tmp;
        strlcpy(tmp, doc["password"], sizeof(tmp));
        config.password = tmp;
        configurationFile.close();
        Serial.println("Succesfully loaded Wi-Fi credentials.");
    }
    end();
}
