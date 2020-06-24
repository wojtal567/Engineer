#include <SQLiteDb.hpp>

SQLiteDb::SQLiteDb()
{
    sqlite3_initialize();
}

int SQLiteDb::open(const char name[100])
{
    strncpy(fileName, name, 100);
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

    int rc = sqlite3_exec(
        object,
        "CREATE table if not exists samples (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, temperature float, humidity FLOAT, pm10 FLOAT, pm25 FLOAT, pm100 float, timestamp datetime NOT NULL default CURRENT_TIMESTAMP)",
        0,
        (void*)"Output:",
        &zErrorMessage
    );

    if(rc != SQLITE_OK)
        sqlite3_free(zErrorMessage);
}