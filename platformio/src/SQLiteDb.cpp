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
    if (object == NULL)
        serial->println("No database open");

    open();
    String sql = "CREATE table if not exists " + _tableName + " (timestamp datetime NOT NULL PRIMARY KEY, temperature float, humidity FLOAT, pm10standard FLOAT, pm25standard FLOAT, pm100standard float, pm10env FLOAT, pm25env FLOAT, pm100env FLOAT, particles03 INTEGER, particles05 INTEGER, particles10 INTEGER, particles25 INTEGER, particles50 INTEGER, particles100 INTEGER)";
    int rc = sqlite3_exec(
        object,
        sql.c_str(),
        0,
        (void *)"Output:",
        &zErrorMessage);

    if (rc != SQLITE_OK)
    {
        serial->print(F("SQL error: "));
        serial->print(sqlite3_extended_errcode(object));
        serial->print(" ");
        serial->println(zErrorMessage);
        sqlite3_free(zErrorMessage);
    }
    close();
}

int SQLiteDb::save(std::map<std::string, uint16_t> data, int temperature, int humidity, String timestamp, Stream *debugger)
{
    if (object == NULL)
    {
        debugger->println("Database does not exist. NULL");
        return 0;
    }

    String sql = "INSERT INTO " + _tableName + " ('timestamp', 'temperature', 'humidity', 'pm10standard', 'pm25standard', 'pm100standard', 'pm10env', 'pm25env', 'pm100env', 'particles03', 'particles05', 'particles10', 'particles25', 'particles50', 'particles100') VALUES ('" +
                 timestamp + "', " + (String)temperature + ", " + (String)humidity + ", " + (String)data["pm10_standard"] + ", " + (String)data["pm25_standard"] + ", " + (String)data["pm100_standard"] + ", " + (String)data["pm10_env"] + ", " + (String)data["pm25_env"] + ", " + (String)data["pm100_env"] + ", " + (String)data["particles_03um"] + ", " + (String)data["particles_05um"] + ", " + (String)data["particles_10um"] + ", " + (String)data["particles_25um"] + ", " + (String)data["particles_50um"] + ", " + (String)data["particles_100um"] + ")";

    debugger->println("Executing: " + sql);

    open();

    int rc = sqlite3_exec(object, sql.c_str(), 0, (void *)"Output:", &zErrorMessage);


    Serial.printf("\nHeap size: %d\n", ESP.getHeapSize());
    Serial.printf("Free Heap: %d\n", esp_get_free_heap_size());
    Serial.printf("Min Free Heap: %d\n", esp_get_minimum_free_heap_size());
    Serial.printf("Max Alloc Heap: %d\n", ESP.getMaxAllocHeap());

    if (rc != SQLITE_OK)
    {
        debugger->println(F("SQL error: "));
        debugger->println(sqlite3_extended_errcode(object));
        debugger->print(" ");
        debugger->println(zErrorMessage);
        sqlite3_free(zErrorMessage);
        close();
        return 1;
    }
    close();
    return rc;
}

static int selectCallback(void *data, int argc, char **argv, char **azColName)
{

    JsonArray *records = static_cast<JsonArray*>(data);
    StaticJsonDocument<600> doc;
    for (int i = 0; i < argc; i++)
        doc[azColName[i]] = argv[i];
    
    records->add(doc);
    return 0;
}

int SQLiteDb::select(Stream *debugger, String datetime, JsonArray* array)
{
    if (object == NULL)
    {
        debugger->println("Database does not exist. NULL");
        return 0;
    }

    String sql = "select * from " + _tableName + " where timestamp > '" + datetime.c_str() + "' limit 50;";
    debugger->println("Executing: " + sql);
    open();
    int rc = sqlite3_exec(object, sql.c_str(), selectCallback, array, &zErrorMessage);

    Serial.printf("\nHeap size: %d\n", ESP.getHeapSize());
    Serial.printf("Free Heap: %d\n", esp_get_free_heap_size());
    Serial.printf("Min Free Heap: %d\n", esp_get_minimum_free_heap_size());
    Serial.printf("Max Alloc Heap: %d\n", ESP.getMaxAllocHeap());

    if (rc != SQLITE_OK)
    {
        debugger->println(F("SQL error: "));
        debugger->println(sqlite3_extended_errcode(object));
        debugger->print(" ");
        debugger->println(zErrorMessage);
        sqlite3_free(zErrorMessage);
        close();
        return 1;
    }
    else
        debugger->println(zErrorMessage);

    close();
    return rc;
}

int SQLiteDb::getLastRecord(Stream *debugger, JsonArray* array)
{
    if (object == NULL)
    {
        debugger->println("Database does not exist. NULL");
        return 0;
    }
    
    String sql = "SELECT * FROM " + _tableName+  " order by timestamp desc limit 1";
    debugger->println("Executing: " + sql);
    int rc = sqlite3_exec(object, sql.c_str(), selectCallback, array, &zErrorMessage);
    if (rc != SQLITE_OK)
    {
        debugger->println(F("SQL error: "));
        debugger->println(sqlite3_extended_errcode(object));
        debugger->print(" ");
        debugger->println(zErrorMessage);
        sqlite3_free(zErrorMessage);
    }
    else
        debugger->println(zErrorMessage);

    return rc;
}

String SQLiteDb::getLocalPath()
{
    return _localPath;
}

String SQLiteDb::getRelativePath()
{
    return _relativePath;
}
