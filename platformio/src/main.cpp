//TODO przetestować kupę
//TODO STWORZYĆ KLASY ZWIĄZANE Z ZAPISYWANIEM NA SD I INNYMI PIERDOŁAMI <- done
//TODO PRZECZYTA CELE I ZAKRESY PROJEKTOW(XD)
//TODO diagnostyka
//TODO nie ip a antenka
//TODO konfigurator do sieci
//TODO zmiena formatu daty <- done
//TODO on to chce za tydzien wzionc
//TODO blokowanie dotyku przy uzyciu guzika(albo po czasie)(albo to i to)(a jednak xD)
//TODO informacja o zablokowanym dotyku
//TODO Kamil- SSID/haslo Michal - guji Czarek - blokowanie
//TODO networkmanager (przez web ustawienia sieci)
//TODO za tydzien ladne kolorky(nie lubi czarnych(kolorow ofc)), temp i wilgotnosc pokazac tam gdzie air quality
//TODO air quality w prawo blizej skali
//TODO stopnie i procenty
//TODO pm10 to nie prawda
//TODO pomyslec nad tym gui
//TODO GUI ZROBIC
//TODO wygladzone czcionki
//TODO jednostki chyba nie potrzebne
//TODO mozna slupki graficzne(zgaduje że kolorowe maja byc)(czerwone)
//TODO szata graficzna ma byc kolorowa(kurwa kolorky)
//TODO JEDNAK BEZ PRZYCISKU (to sie kurwa wyklucza)
//TODO chce zaznaczac obszar i z tego wykres
//TODO wykresy i dokonczenie parsowania danych
//TODO zmiana bazy i upchniecie do wyswietlacza
//TODO zakresy na osiach lub automatyczne skalowanie bo pewnie biblioteka ma 
//TODO przedzial czasowy
//TODO konfigurowalne (kurwa kolorky)
//TODO jednak nie laczyc urzadzenia z aplikacja bo nie chce(a moze jednak?)
//TODO zapisywanie problemow na kartce(bo kartka)
//TODO SPRAWDŹCIE SOBIE DODATEK GITLENS//////////////////////////////
#include <WiFi.h>
#include <Wire.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <SD.h>
#include <sqlite3.h>
#include <RtcDS1307.h>
#include <time.h>
RtcDS1307<TwoWire> Rtc(Wire);
//#include <NTPClient.h>
#include <WEMOS_SHT3X.h> 
//#include "TFT_eSPI/Fonts/Font16.h"
#include <HTTPClient.h>

#define dataValueRectWidth 50
#define dataValueRectHeight 30

#define menuRectTop 14
#define menuRectBottom 225
#define menuRectLeft 14
#define menuRectRight 305


sqlite3 *db = NULL;
char db_file_name[100] = "\0";
char *zErrMsg = 0; //jakos to trza wyrzucic
const char* data1 = "Output:"; //tez

struct pms5003data {
  uint16_t framelen;
  uint16_t pm10_standard, pm25_standard, pm100_standard;
  uint16_t pm10_env, pm25_env, pm100_env;
  uint16_t particles_03um, particles_05um, particles_10um, particles_25um, particles_50um, particles_100um;
  uint16_t unused;
  uint16_t checksum;
};

TFT_eSPI tft = TFT_eSPI();
SHT3X sht30(0x45);

int temp, humi;
const char* ssid = "millofthedam";
const char* password = "penis123";
const char* ntpServer = "0.pool.ntp.org";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;

bool first_measure = true;

char *titles[] = { "PM2.5     ug/m3", "TEMP",  "RH",  "PM1.0   ug/m3",  "PM10.0    ug/m3",  "PM10  PARTICLES", "Air quality:"};

uint8_t titlesPositionXY[7][2] =
              {{20, 20},
              {212, 22},
              {212, 90},
              {20, 150},
              {110, 150},
              {210, 150},
              {21, 209}};

uint8_t dataPositionXY[7][2] =
              {{90, 70},
              {240, 50},
              {240, 112},
              {50, 167},
              {140, 167},
              {244, 167},
              {21, 208}};

struct pms5003data data;


uint16_t  RGB16b(uint8_t red, uint8_t green, uint8_t blue){  
  if(red>31 or green>63 or blue>31)
    return 65535;
  return (red * 2048) + (green * 32) + blue;
}

int db_open() {
  if (db != NULL)
    sqlite3_close(db);

  int rc = sqlite3_open(db_file_name, &db);
  return rc;
}


void db_createtable()
{
  if (db == NULL) 
    Serial.println("No database open");
  
  String sql = "CREATE table if not exists samples (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, temperature float, humidity FLOAT, pm10 FLOAT, pm25 FLOAT, pm100 float, timestamp datetime NOT NULL default CURRENT_TIMESTAMP)";
  int rc = sqlite3_exec(db, sql.c_str(), 0, (void*)data1, &zErrMsg);

  if (rc != SQLITE_OK)
    sqlite3_free(zErrMsg);
  
}

boolean start_SD()
{
  bool result = SD.begin(27);
  if(result)
  {
    sqlite3_initialize();
    strncpy(db_file_name,"/sd/database.db", 100);
    db_open();
    db_createtable();
    sqlite3_close(db);
    SD.end();
  }
  return result; 
}

String timestamp(){
  RtcDateTime timestamp = Rtc.GetDateTime();
  String time = (String)timestamp.Year()+"-";

  if(timestamp.Month()<10)
    time+="0"+(String)timestamp.Month()+"-";
  else
    time+=(String)timestamp.Month()+"-";

  if(timestamp.Day()<10)
    time+="0"+(String)timestamp.Day()+" ";
  else
    time+=(String)timestamp.Day()+" ";
  
  if(timestamp.Hour()<10)
    time+="0"+(String)timestamp.Hour()+":";
  else
    time+=(String)timestamp.Hour()+":";
  
  if(timestamp.Minute()<10)
    time+="0"+(String)timestamp.Minute()+":";
  else
    time+=(String)timestamp.Minute()+":";
  
  if(timestamp.Second()<10)
    time+="0"+(String)timestamp.Second();
  else
    time+=(String)timestamp.Second();

  return time;
}

int db_save(int temp, int humi) {
  if (db == NULL) return 0;
  
  String sql = "insert into samples (temperature, humidity, pm10, pm25, pm100, timestamp) values ("+(String)temp+", "+(String)humi+", "+(String)data.pm10_standard+", "+(String)data.pm25_standard+", "+(String)data.pm100_standard+", "+"'"+timestamp()+"')";
  int rc = sqlite3_exec(db, sql.c_str(), 0, (void*)data1, &zErrMsg);
  if (rc != SQLITE_OK) {
    Serial.print(F("SQL error: "));
    Serial.print(sqlite3_extended_errcode(db));
    Serial.print(" ");
    Serial.println(zErrMsg);
    sqlite3_free(zErrMsg);
  }
  return rc;
}

void save_card(){
  if(!SD.begin(27))
  {
    tft.setTextSize(1);
    tft.setCursor(0, 0);
    tft.fillRect(0, 0, 100, 13, TFT_BLACK);
    tft.print("No SD Card");
  }    
  else
  {
    tft.setTextSize(1);
    tft.setCursor(0, 0);
    tft.fillRect(0, 0, 100, 13, TFT_BLACK);
    tft.print("SD Card detected");
    if(SD.exists("/database.db"))
    {
      db_open();
      db_createtable();
      db_save(temp, humi);
      sqlite3_close(db);
    }
    else
    {
      db_open();
      db_createtable();
      db_save(temp, humi);
      sqlite3_close(db);
    }    
    SD.end();
  }
}

void tftMenuInit() {
  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE); 
  tft.drawLine(103, 146, 103, 200, TFT_WHITE);
  tft.drawLine(203, 14, 203, 200, TFT_WHITE);
  tft.drawLine(14, 145, 305, 145, TFT_WHITE);
  tft.drawLine(14, 200, 305, 200, TFT_WHITE);
  tft.drawRect(14, 14, 292, 212, TFT_WHITE);
  tft.setTextSize(1);
  tft.setCursor(0, 0);
  if(!start_SD())
    tft.print("No SD Card");
  else
    tft.print("SD Card detected");
  tft.setTextSize(2);
  for(uint8_t i=0;i<7;i++){
    tft.setCursor(titlesPositionXY[i][0], titlesPositionXY[i][1]);
    if(i>2)
       tft.setTextSize(1);
    tft.print(titles[i]);
  }
  
  for(uint8_t i=0;i<96;i++)
    if(i<64)
      tft.drawLine(202+i, 208, 202+i, 217, RGB16b(31, 0+i, 0));
    else
      tft.drawLine(202+i, 208, 202+i, 217, RGB16b(31-(i-32)%32, 63, 0));
  
  tft.drawRect(202, 208, 96, 10, TFT_WHITE);
}

void tftClearData(){
  tft.fillRect(dataPositionXY[0][0], dataPositionXY[0][1], 80, 75, TFT_BLACK);
  for(uint8_t i = 1;i<6;i++)
    tft.fillRect(dataPositionXY[i][0], dataPositionXY[i][1], dataValueRectWidth, dataValueRectHeight, TFT_BLACK);
  
}

void tftDisplayData(){
  tft.setTextSize(7);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(dataPositionXY[0][0],dataPositionXY[0][1]);
  tft.print(data.pm10_standard);
  tft.setTextSize(3);
  tft.setCursor(dataPositionXY[1][0],dataPositionXY[1][1]);
  tft.print(temp);
  tft.setCursor(dataPositionXY[2][0],dataPositionXY[2][1]);
  tft.print(humi);
  tft.setCursor(dataPositionXY[3][0],dataPositionXY[3][1]);
  tft.print(data.pm25_standard);
  tft.setCursor(dataPositionXY[4][0],dataPositionXY[4][1]);
  tft.print(data.pm100_standard);
  tft.setCursor(dataPositionXY[5][0],dataPositionXY[5][1]);
  tft.print(data.particles_100um);
}

boolean readPMSdata(Stream *s) {
  
  if (! Serial2.available()) 
    return false;
  
  
  // Read a byte at a time until we get to the special '0x42' start-byte
  if (s->peek() != 0x42) {
    s->read();
    return false;
  }

  // Now read all 32 bytes
  if (s->available() < 32) 
    return false;
  
    
  uint8_t buffer[32];    
  uint16_t sum = 0;
  s->readBytes(buffer, 32);

  // get checksum ready
  for (uint8_t i=0; i<30; i++)
    sum += buffer[i];
  

  
  for (auto j=2; j<32; j++){
    Serial.print("0x"); Serial.print(buffer[j], HEX); Serial.print(", ");
  }

  
  Serial.println();
  
  
  // The data comes in endian'd, this solves it so it works on all platforms
  uint16_t buffer_u16[15];
  for (uint8_t i=0; i<15; i++) {
    buffer_u16[i] = buffer[2 + i*2 + 1];
    buffer_u16[i] += (buffer[2 + i*2] << 8);
  }

  // put it into a nice struct :)
  memcpy((void *)&data, (void *)buffer_u16, 30);

  if (sum != data.checksum) {
    Serial.println("Checksum failure");
    return false;
  }
  // success!
  return true;
}

void printDateTime(const RtcDateTime& dt)
{
    char datestring[20];
 
    snprintf_P(datestring, 
            20,
            PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
            dt.Day(),
            dt.Month(),
            dt.Year(),
            dt.Hour(),
            dt.Minute(),
            dt.Second() );
    tft.setTextSize(1);
    tft.setCursor(200, 0);
    tft.fillRect(200, 0, 300, 13, TFT_BLACK);
    tft.print((String) datestring);
}

void config_time()
{
  delay(500);
  Rtc.Begin();
  time_t rawtime;
  struct tm* timeinfo;
  time(&rawtime);
  timeinfo = localtime(&rawtime);
  char year[5];
  char month[5];
  strftime (year, sizeof(year), "%Y",timeinfo);
  strftime (month, sizeof(month), "%m", timeinfo);
  RtcDateTime date = RtcDateTime(atoi(year), atoi(month), timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
  Rtc.SetDateTime(date);
}

void setup(){
//  tft.loadFont();
  uint8_t wifiAttempts = 10;
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, 16, 17);
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tftMenuInit();  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED and wifiAttempts>0){
    Serial.print(".");
    delay(500);
    wifiAttempts--;
  }

  if(WiFi.status()==WL_CONNECTED)
  {
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    delay(500);
    config_time();
    tft.setTextSize(1);
    tft.setCursor(0, 230); 
    Serial.println(WiFi.localIP());
    tft.print("IP address: "+ WiFi.localIP().toString());
  } 
  else
  {
    tft.setTextSize(1);
    tft.setCursor(0, 230);
    tft.print("No WiFi connection");
  }  

}

void loop(){
  sht30.get(); 
  temp = sht30.cTemp;
  humi = sht30.humidity;
  RtcDateTime now = Rtc.GetDateTime();
  printDateTime(now);
  if(((now.Minute()%5==0)&&(now.Second()==0))||first_measure)
  {
    first_measure=false;
    if(readPMSdata(&Serial2)){
      tftClearData();
      tftDisplayData();
      save_card();
      HTTPClient client;
      client.begin("http://192.168.1.130:3434/submit");
      int httpResponseCode = client.POST("CHUJ CI W DUPE");
      if(httpResponseCode > 0) 
      {
        String response = client.getString();
        Serial.println(httpResponseCode);
        Serial.println(response);
      } 
      else 
      {
        Serial.print("ERROR ON POST");
        Serial.print(httpResponseCode);
      }
      client.end();
    }
  }
    
}
 
  
