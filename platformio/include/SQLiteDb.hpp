#include <sqlite3.h>
#include <string>
#include <stddef.h>
#include <cstring>
#include <Stream.h>
#include <PMS5003.hpp>

class SQLiteDb
{
    private:
        sqlite3 *object;
        char fileName[100] = "\0";
        String _localPath;
        String _tableName;
        char *zErrorMessage = 0;
        bool isOpened;
    public:
        SQLiteDb(String localPath, String tableName);
        void init();
        void kill();
        int open();
        void close();
        void createTable(Stream *serial);
        int save(std::map<std::string, uint16_t> data, int temperature, int humidity, String timestamp, Stream *debugger);
        String getLocalPath();
};