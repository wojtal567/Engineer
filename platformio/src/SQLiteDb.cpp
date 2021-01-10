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
    String sql = "CREATE table if not exists " + _tableName + " (timestamp datetime NOT NULL PRIMARY KEY, Temperature float, Humidity FLOAT, Pm10 FLOAT, Pm25 FLOAT, Pm100 float, Particles03 FLOAT, Particles05 FLOAT, Particles10 FLOAT, Particles25 FLOAT, Particles50 FLOAT, Particles100 FLOAT)";
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
    else if (rc == SQLITE_OK)
    {
        Serial.println("SQLITE_OK");
    }
    close();
}

int SQLiteDb::save(std::map<std::string, float> data, float temperature, float humidity, String timestamp, Stream *debugger)
{
    if (object == NULL)
    {
        debugger->println("Database does not exist. NULL");
        return 0;
    }

    String sql = "INSERT INTO " + _tableName + " ('Timestamp', 'Temperature', 'Humidity', 'Pm10', 'Pm25', 'Pm100', 'Particles03', 'Particles05', 'Particles10', 'Particles25', 'Particles50', 'Particles100') VALUES ('" +
                 timestamp + "', " + (String)temperature + ", " + (String)humidity + ", " + (String)data["pm10_standard"] + ", " + (String)data["pm25_standard"] + ", " + (String)data["pm100_standard"] + ", " + (String)data["particles_03um"] + ", " + (String)data["particles_05um"] + ", " + (String)data["particles_10um"] + ", " + (String)data["particles_25um"] + ", " + (String)data["particles_50um"] + ", " + (String)data["particles_100um"] + ")";

    debugger->println("Executing: " + sql);

    open();

    int rc = sqlite3_exec(object, sql.c_str(), 0, (void *)"Output:", &zErrorMessage);

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
    else if (rc == SQLITE_OK)
    {
        Serial.println("SQLITE_OK: Successfully inserted record into table " + _tableName);
    }
    close();
    return rc;
}

static int selectCallback(void *data, int argc, char **argv, char **azColName)
{

    JsonArray *records = static_cast<JsonArray *>(data);
    StaticJsonDocument<600> doc;
    for (int i = 0; i < argc; i++)
        doc[azColName[i]] = argv[i];

    records->add(doc);
    return 0;
}

int SQLiteDb::select(Stream *debugger, String datetime, JsonArray *array)
{
    if (object == NULL)
    {
        debugger->println("Database does not exist. NULL");
        return 0;
    }
    String sql = "";
    if (datetime != "null")
        sql = "select * from " + _tableName + " where timestamp > '" + datetime.c_str() + "' limit 50;";
    else
        sql = "select * from " + _tableName + " order by timestamp limit 50;";
    debugger->println("Executing: " + sql);
    open();
    int rc = sqlite3_exec(object, sql.c_str(), selectCallback, array, &zErrorMessage);

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

int SQLiteDb::getLastRecord(Stream *debugger, JsonArray *array)
{
    if (object == NULL)
    {
        debugger->println("Database does not exist. NULL");
        return 0;
    }

    String sql = "SELECT * FROM " + _tableName + " order by timestamp desc limit 1";
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
