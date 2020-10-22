#include <SD.h>
#include <SQLiteDb.hpp>
#include <string>

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
        void select(SQLiteDb *object, Stream *debugger, String datetime, JsonArray* array);
        void getLastRecord(SQLiteDb *object, Stream *debugger, JsonArray* array);
        void saveConfig(SQLiteDb *object, int measurePeriod, int lockTime, int samplesNumber, int avgTime);
        int loadConfig(int& measure_period, int& lcd_lock_time, uint& samplesNumber, int& averageTime);
};
