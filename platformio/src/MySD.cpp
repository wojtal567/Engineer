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

    if(result)
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

void MySD::save(std::map<std::string, uint16_t> data, int temperature, int humidity, String timestamp, SQLiteDb *object, Stream *debugger)
{
  debugger->println("MySD::save");
    if(begin())
    {
        debugger->println("SD Card detected");
        if(SD.exists(object->getRelativePath()))
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

void MySD::select(SQLiteDb *object, Stream *debugger, String datetime, JsonArray* array)
{
    debugger->println("MySD::select");
    if(begin())
    {
        debugger->println("SD Card detected");
        if(SD.exists(object->getRelativePath()))
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

void MySD::getLastRecord(SQLiteDb *object, Stream *debugger, JsonArray* array)
{
    debugger->println("MySD::getLastRecord");
    if(begin())
    {
        debugger->println("SD Card detected");
        if(SD.exists(object->getRelativePath()))
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

void MySD::saveConfig(SQLiteDb *object, int measurePeriod, int lockTime, int samplesNumber, int avgTime)
{

    if(begin())
    {
        File settings = SD.open("/settings.csv", FILE_WRITE);
        std::string temp = "";
        char buffer[10];
        itoa(measurePeriod, buffer, 10);
        temp += std::string(buffer + '%');
        itoa(lockTime, buffer, 10);
        temp += std::string(buffer + '%');
        itoa(samplesNumber, buffer, 10);
        temp += std::string(buffer + '%');
        itoa(avgTime, buffer, 10);
        temp += std::string(buffer);
        settings.print(temp.c_str());
        settings.close();
    }
    end();
}

int MySD::loadConfig(int& measure_period, int& lcd_lock_time, uint& samplesNumber, int& averageTime)
{
	File settings;
	String stn="";
	if(begin())
	{
		if(SD.exists("/settings.csv"))
		{
			settings = SD.open("/settings.csv", FILE_READ);
			Serial.println("som");
			while(settings.available())
			{
				stn += (char)settings.read();
			}
			int pos = stn.indexOf("%");
			measure_period=stn.substring(0,stn.indexOf("%")).toInt();
			stn[pos]='0';
			lcd_lock_time=stn.substring(pos+1, stn.indexOf("%")).toInt();
			pos = stn.indexOf("%");
			stn[pos]='0';
			samplesNumber = stn.substring(pos+1, stn.indexOf("%")).toInt();
			averageTime = stn.substring(stn.indexOf("%")+1).toInt();
			settings.close();
			switch(lcd_lock_time)
			{
				case -1:
					return 7;
					break;
				case 30000: 
					return 0;
					break;
				case 60000: 
					return 1;
					break;
				case 120000:
					return 2;
					break;
				case 300000:
					return 3;
					break;
				case 600000:
					return 4;
					break;
				case 1800000:
					return 5;
					break;
				case 3600000:
					return 6;
					break;
			}
		}else
		{
			settings = SD.open("/settings.csv", FILE_WRITE);
			settings.print("3600000%60000");
			settings.close();
			measure_period=3600000;
			lcd_lock_time=60000;
			return 1;
		}
		
	}
	else
	{
		measure_period=3600000;
		lcd_lock_time=60000;
		samplesNumber=5;
		averageTime=5000;
		return 1;
	}
}