#include <SD.h>
#include <SQLiteDb.hpp>

class MySD
{
    private:
        int _port;
    public:
        MySD(int port);
        bool begin();
        void end();
        bool start(SQLiteDb *object, Stream *debugger);
        void save(std::map<std::string, uint16_t> data, int temperature, int humidity, String timestamp, SQLiteDb *object, Stream *debugger);
};
