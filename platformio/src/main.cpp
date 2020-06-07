#include <WiFi.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <SD.h>
//#include <NTPClient.h>
#include <WEMOS_SHT3X.h> 
//#include "TFT_eSPI/Fonts/Font16.h"

#define dataValueRectWidth 50
#define dataValueRectHeight 30

#define menuRectTop 14
#define menuRectBottom 225
#define menuRectLeft 14
#define menuRectRight 305

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
const char* ssid = "";
const char* password = "";

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

void tftMenuInit(){
  double col_hex =  0xF800;
  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE); 
  tft.drawLine(103, 146, 103, 200, TFT_WHITE);
  tft.drawLine(203, 14, 203, 200, TFT_WHITE);
  tft.drawLine(14, 145, 305, 145, TFT_WHITE);
  tft.drawLine(14, 200, 305, 200, TFT_WHITE);
  tft.drawRect(14, 14, 292, 212, TFT_WHITE);
  tft.setTextSize(2);
  for(uint8_t i=0;i<7;i++){
    tft.setCursor(titlesPositionXY[i][0], titlesPositionXY[i][1]);
    if(i>2)
       tft.setTextSize(1);
    tft.print(titles[i]);
  }
  
  for(uint8_t i=0;i<96;i++){
    if(i<64)
      tft.drawLine(202+i, 208, 202+i, 217, RGB16b(31, 0+i, 0));
    else
      tft.drawLine(202+i, 208, 202+i, 217, RGB16b(31-(i-32)%32, 63, 0));
  }
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
  
  if (! Serial2.available()) {
    return false;
  }
  
  // Read a byte at a time until we get to the special '0x42' start-byte
  if (s->peek() != 0x42) {
    s->read();
    return false;
  }

  // Now read all 32 bytes
  if (s->available() < 32) {
    return false;
  }
    
  uint8_t buffer[32];    
  uint16_t sum = 0;
  s->readBytes(buffer, 32);

  // get checksum ready
  for (uint8_t i=0; i<30; i++) {
    sum += buffer[i];
  }

  
  for (uint8_t i=2; i<32; i++) {
    Serial.print("0x"); Serial.print(buffer[i], HEX); Serial.print(", ");
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
}

void loop(){
  sht30.get(); 
  temp = sht30.cTemp;
  humi = sht30.humidity;
  if(readPMSdata(&Serial2)){
    tftClearData();
    tftDisplayData();
  }
  delay(3000);
}
