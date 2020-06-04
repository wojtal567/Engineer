#include <WiFi.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <WEMOS_SHT3X.h> 

#define dataRectWidth 80
#define dataRectHeight 18

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

char *pmType[] = { "PM0.3", "PM0.5",  "PM1.0",  "PM2.5",  "PM5.0",  "PM10.0"};

int dataPositionXY[6][2] =
              {{20, 54},
              {20, 116},
              {20, 175},
              {120, 54},
              {120, 116},
              {120, 175}};

struct pms5003data data;

void setup(){
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
  tft.setCursor(213, 54);
  tft.print(temp);
  tft.setCursor(213, 116);
  tft.print(humi);
  
  if(readPMSdata(&Serial2)){
    tftClearData();
    tftDisplayData();
  }
  Serial.print(sht30.cTemp);
  Serial.print(sht30.humidity);
  delay(3000);
}

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
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE); 
  tft.setCursor(213, 22);
  tft.print("Temp");
  tft.setCursor(213, 84);
  tft.print("RH");
  tft.drawLine(105, 14, 105, 203, TFT_WHITE);
  tft.drawLine(205, 14, 205, 203, TFT_WHITE);
  for(uint8_t i=0;i<6;i++){
    tft.setCursor(dataPositionXY[i][0], dataPositionXY[i][1]-32);
    tft.print(pmType[i]);
  }
  tft.drawRect(14, 14, 292, 190, TFT_WHITE);
  for(uint8_t i=0;i<96;i++){
    if(i<64)
      tft.drawLine(210+i, 215, 210+i, 224, RGB16b(31, 0+i, 0));
    else
      tft.drawLine(210+i, 215, 210+i, 224, RGB16b(31-(i-32)%32, 63, 0));
  }
  
  tft.drawRect(210, 215, 96, 10, TFT_WHITE);
}

void tftClearData(){
  for(uint8_t i = 0;i<6;i++)
    tft.fillRect(dataPositionXY[i][0], dataPositionXY[i][1], dataRectWidth, dataRectHeight, TFT_BLACK);
}

void tftDisplayData(){
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(dataPositionXY[0][0],dataPositionXY[0][1]);
  tft.print(data.particles_03um);
  tft.setCursor(dataPositionXY[1][0],dataPositionXY[1][1]);
  tft.print(data.particles_05um);
  tft.setCursor(dataPositionXY[2][0],dataPositionXY[2][1]);
  tft.print(data.particles_10um);
  tft.setCursor(dataPositionXY[3][0],dataPositionXY[3][1]);
  tft.print(data.particles_25um);
  tft.setCursor(dataPositionXY[4][0],dataPositionXY[4][1]);
  tft.print(data.particles_50um);
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
