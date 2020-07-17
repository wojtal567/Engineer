#include <SQLiteDb.hpp>

SQLiteDb::SQLiteDb(String localPath, String relativePath, String tableName)
{
    _localPath = localPath;
    _relativePath = relativePath;
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
        serial->println("No database open");

    String sql = "CREATE table if not exists " + _tableName + " (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, temperature float, humidity FLOAT, pm10 FLOAT, pm25 FLOAT, pm100 float, particles1 INTEGER, particles25 INTEGER, particles10 INTEGER, timestamp datetime NOT NULL default CURRENT_TIMESTAMP)";
    int rc = sqlite3_exec(
        object,
        sql.c_str(),
        0,
        (void*)"Output:",
        &zErrorMessage
    );

    if(rc != SQLITE_OK)
    {
      serial->print(F("SQL error: "));
      serial->print(sqlite3_extended_errcode(object));
      serial->print(" ");
      serial->println(zErrorMessage);
      sqlite3_free(zErrorMessage);
    }

}

int SQLiteDb::save(std::map<std::string, uint16_t> data, int temperature, int humidity, String timestamp, Stream *debugger)
{
    if(object == NULL)
    {
      debugger->println("Database does not exist. NULL");
      return 0;
    }


    String sql = "INSERT INTO" + _tableName + " ('temperature', 'humidity', 'pm10', 'pm25', 'pm100', 'particles1', 'particles25', 'particles10', 'timestamp') VALUES ("+
          (String)temperature+", "+(String)humidity+", "+(String)data["pm10_standard"]+", "+(String)data["pm25_standard"]+", "+(String)data["pm100_standard"]+", "+(String)data["particles_10um"]+", "
          +(String)data["particles_25um"]+", "+(String)data["particles_100um"]+", '"+timestamp+"')";

    debugger->println("Executing: " + sql);

    int rc = sqlite3_exec(object, sql.c_str(), 0, (void*)"Output:", &zErrorMessage);

    if (rc != SQLITE_OK) {
        debugger->println(F("SQL error: "));
        debugger->println(sqlite3_extended_errcode(object));
        debugger->print(" ");
        debugger->println(zErrorMessage);
        sqlite3_free(zErrorMessage);
    }

    return rc;
}

int SQLiteDb::compareWithApp(String appTimestamp, Stream *debugger)
{
    if(object == NULL)
    {
      debugger->println("Database does not exist. NULL");
      return 0;
    }
    String sql = "SELECT * from" + _tableName + "WHERE DATETIME(" + appTimestamp + ") < " + _tableName +".timestamp";
    debugger->println("Executing: " + sql);
    
}

void compareCallback(void *data, int argc, char **argv, char** azColName)
{

}

String SQLiteDb::getLocalPath()
{
    return _localPath;
}

String SQLiteDb::getRelativePath()
{
  return _relativePath;
}
