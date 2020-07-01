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
        end();
    }
    return result;
}

void MySD::save(std::map<std::string, uint16_t> data, int temperature, int humidity, String timestamp, SQLiteDb *object, Stream *debugger)
{
    if(begin())
    {
        debugger->print("SD Card detected");
        if(SD.exists(object->getLocalPath()))
        {
            object->init();
            object->open();
            object->createTable(debugger);
            object->save(data, temperature, humidity, timestamp, debugger);
            object->close();
            object->kill();
        }
        end();
    }
        
}