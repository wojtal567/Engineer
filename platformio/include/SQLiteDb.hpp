#include <sqlite3.h>
#include <string>
#include <stddef.h>
#include <cstring>
#include <Stream.h>

//TODO zrobić tak, żeby można było zmienić nazwę tabeli czyli dodać zmienną klasową
//TODO oprogramować metodę zapisywania do bazy

class SQLiteDb
{
    private:
        sqlite3 *object;
        char fileName[100] = "\0";
        char *zErrorMessage = 0;
        bool isOpened;
    public:
        SQLiteDb();
        int open(const char name[100]);
        void close();
        void createTable(Stream *serial);

};