#include <SQLiteDb.hpp>

SQLiteDb::SQLiteDb(String localPath, String tableName)
{
    _localPath = localPath;
    _tableName = tableName;
}

void SQLiteDb::init()
{
    sqlite3_initialize();
}

void SQLiteDb::kill()
{
    sqlite3_shutdown();
}

int SQLiteDb::open()
{
    strncpy(fileName, _localPath.c_str(), 100);
    if (object != NULL)
        sqlite3_close(object);
    return sqlite3_open(fileName, &object);
}

void SQLiteDb::close()
{
    sqlite3_close(object);
}

void SQLiteDb::createTable(Stream *serial)
{
    if(object == NULL)
        serial->print("No database open");

    String sql = "CREATE table if not exists " + _tableName + " (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, temperature float, humidity FLOAT, pm10 FLOAT, pm25 FLOAT, pm100 float, timestamp datetime NOT NULL default CURRENT_TIMESTAMP)";
    int rc = sqlite3_exec(
        object,
        sql.c_str(),
        0,
        (void*)"Output:",
        &zErrorMessage
    );

    if(rc != SQLITE_OK)
        sqlite3_free(zErrorMessage);
}

int SQLiteDb::save(std::map<std::string, uint16_t> data, int temperature, int humidity, String timestamp, Stream *debugger)
{
    if(object == NULL)
        return 0;
    
    String sql = "insert into " + _tableName + " (temperature, humidity, pm10, pm25, pm100, timestamp) values (" +
                  ", " + (String)temperature + ", " + (String)humidity + ", " + (String)data["pm10"] + ", " +
                  ", " + (String)data["pm25"] + ", " + (String)data["pm100"] + ", " +"'"+timestamp+"')";

    int rc = sqlite3_exec(object, sql.c_str(), 0, (void*)"Output:", &zErrorMessage);         

    if (rc != SQLITE_OK) {
        debugger->print(F("SQL error: "));
        debugger->print(sqlite3_extended_errcode(object));
        debugger->print(" ");
        debugger->println(zErrorMessage);
        sqlite3_free(zErrorMessage);
    }
    
    return rc;
}

int SQLiteDb::save(std::map<std::string, uint16_t> data, int temperature, int humidity, Stream *debugger)
{
    if(object == NULL)
        return 0;
    
    String sql = "insert into " + _tableName + " (temperature, humidity, pm10, pm25, pm100, timestamp) values (" +
                  ", " + (String)temperature + ", " + (String)humidity + ", " + (String)data["pm10"] + ", " +
                  ", " + (String)data["pm25"] + ", " + (String)data["pm100"] + ", " +")";

    int rc = sqlite3_exec(object, sql.c_str(), 0, (void*)"Output:", &zErrorMessage);         

    if (rc != SQLITE_OK) {
        debugger->print(F("SQL error: "));
        debugger->print(sqlite3_extended_errcode(object));
        debugger->print(" ");
        debugger->println(zErrorMessage);
        sqlite3_free(zErrorMessage);
    }
    
    return rc;
}

String SQLiteDb::getLocalPath()
{
    return _localPath;
}