#include <MySD.hpp>

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
        object->init();
        object->open();
        object->createTable(debugger);
        object->close();
        object->kill();
    }
    end();
    return result;
}

void MySD::save(std::map<std::string, int32_t> data, int temperature, int humidity, String timestamp, SQLiteDb *object, Stream *debugger)
{
    debugger->println("MySD::save");
    if (begin())
    {
        debugger->println("SD Card detected");
        if (SD.exists(object->getRelativePath()))
        {
            debugger->println("Database " + object->getLocalPath() + " exists. Saving data...");
            object->init();
            object->open();
            object->save(data, temperature, humidity, timestamp, debugger);
            object->close();
            object->kill();
        }
        else
        {
            debugger->println("Database " + object->getLocalPath() + " don't exist. Saving data...");
            object->init();
            object->open();
            object->createTable(debugger);
            object->save(data, temperature, humidity, timestamp, debugger);
            object->close();
            object->kill();
        }
    }
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
            debugger->println("Database " + object->getLocalPath() + " exists.");
            object->init();
            object->open();
            object->select(debugger, datetime, array);
            object->close();
            object->kill();
        }
    }
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
            debugger->println("Database " + object->getLocalPath() + " exists.");
            object->init();
            object->open();
            object->getLastRecord(debugger, array);
            object->close();
            object->kill();
        }
    }
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
    }
    end();
    /*
        else
        {
            settings = SD.open("/settings.csv", FILE_WRITE);
            settings.print("3600000%60000%5%5000");
            settings.close();
            measure_period = 3600000;
            lcd_lock_time = 60000;
            samplesNumber = 5;
            averageTime = 5000;
        }
    }
    else
    {
        measure_period = 3600000;
        lcd_lock_time = 60000;
        samplesNumber = 5;
        averageTime = 5000;
    }
    return 1;*/
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
    }
    end();
}