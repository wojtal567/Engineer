#include <SQLiteDb.hpp>
int rows;
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

    String sql = "CREATE table if not exists " + _tableName + " (timestamp datetime NOT NULL PRIMARY KEY, temperature float, humidity FLOAT, pm10standard FLOAT, pm25standard FLOAT, pm100standard float, pm10env FLOAT, pm25env FLOAT, pm100env FLOAT, particles03 INTEGER, particles05 INTEGER, particles10 INTEGER, particles25 INTEGER, particles50 INTEGER, particles100 INTEGER)";
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


    String sql = "INSERT INTO " + _tableName + " ('timestamp', 'temperature', 'humidity', 'pm10standard', 'pm25standard', 'pm100standard', 'pm10env', 'pm25env', 'pm100env', 'particles03', 'particles05', 'particles10', 'particles25', 'particles50', 'particles100') VALUES ('"+
          timestamp + "', "+ (String)temperature+", "+(String)humidity+", "+(String)data["pm10_standard"]+", "+(String)data["pm25_standard"]+", "+(String)data["pm100_standard"]+", "+(String)data["pm10_env"]
          +", "+(String)data["pm25_env"]+", "+(String)data["pm100_env"]+", "+(String)data["particles_03um"]+", "+(String)data["particles_05um"]+", "+(String)data["particles_10um"]+", "+(String)data["particles_25um"]
          +", "+(String)data["particles_50um"]+", "+(String)data["particles_100um"]+")";

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

static int callback(void *data, int argc, char **argv, char **azColName){
   int i;

   
   for (i = 0; i<argc; i++){
       Serial.printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");//argv przechowuje dane
   }
   Serial.printf("\n");
   return 0;
}

int SQLiteDb::select(Stream *debugger, String datetime)
{
    if(object == NULL)
    {
        debugger->println("Database does not exist. NULL");
        return 0;
    }
    String sql = "Select timestamp, temperature, humidity, pm10standard, pm25standard, pm100standard, pm10env, pm25env, pm100env, particles03, particles05, particles10, particles25, particles50, particles100 from " + _tableName +" where timestamp>Datetime('"+datetime+"')";
    debugger->println("Executing: "+sql);
    int rc = sqlite3_exec(object, sql.c_str(), callback, (void*)"Output: ", &zErrorMessage);
    if (rc != SQLITE_OK) {
        debugger->println(F("SQL error: "));
        debugger->println(sqlite3_extended_errcode(object));
        debugger->print(" ");
        debugger->println(zErrorMessage);
        sqlite3_free(zErrorMessage);
    }else
        debugger->println(zErrorMessage);
    return rc; 
}

int SQLiteDb::getMissingSamples(int lastID, Stream *debugger)
{
    if(object == NULL)
    {
      debugger->println("Database does not exist. NULL");
      return 0;
    }    
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
