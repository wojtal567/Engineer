//syf jak ja nie moge
#include <WiFi.h>
#include <Wire.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <SD.h>
#include <sqlite3.h>
#include <RtcDS1307.h>
#include <time.h>
#include <lvgl.h>
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
char *zErrMsg = 0;//jakos to trza wyrzucic
const char* data1 = "Output:";//tez

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
uint8_t wifiAttempts = 10;
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

static lv_disp_buf_t disp_buf;
static lv_color_t buf[LV_HOR_RES_MAX * 10];

void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors(&color_p->full, w * h, true);
    tft.endWrite();

    lv_disp_flush_ready(disp);
}

/* Reading input device (simulated encoder here) */
bool read_encoder(lv_indev_drv_t * indev, lv_indev_data_t * data)
{
    static int32_t last_diff = 0;
    int32_t diff = 0; /* Dummy - no movement */
    int btn_state = LV_INDEV_STATE_REL; /* Dummy - no press */

    data->enc_diff = diff - last_diff;;
    data->state = btn_state;

    last_diff = diff;

    return false;
}


static void keyboard_event_cb(lv_obj_t * keyboard, lv_event_t event){
  lv_keyboard_def_event_cb(keyboard, event);
  if(event == LV_EVENT_APPLY){
    //printf("LV_EVENT_APPLY\n");
  }else if(event == LV_EVENT_CANCEL){
	  //keyboard = NULL;
  }
}

lv_obj_t * keyboard;
lv_obj_t * ssid_ta;
lv_obj_t * pwd_ta;
int screenWidth = 320;
int screenHeight = 240;
bool my_touchpad_read(lv_indev_drv_t * indev_driver, lv_indev_data_t * data)
{
    uint16_t touchX, touchY;

    bool touched = tft.getTouch(&touchX, &touchY, 600);

    if(!touched)
    {
      return false;
    }

    if(touchX>screenWidth || touchY > screenHeight)
    {
     // Serial.println("Y or y outside of expected parameters..");
      //Serial.print("y:");
     // Serial.print(touchX);
     // Serial.print(" x:");
     // Serial.print(touchY);
    }
    else
    {

      data->state = touched ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL; 
  
      /*Save the state and save the pressed coordinate*/
      //if(data->state == LV_INDEV_STATE_PR) touchpad_get_xy(&last_x, &last_y);
     
      /*Set the coordinates (if released use the last pressed coordinates)*/
      data->point.x = touchX;
      data->point.y = touchY;
  
     // Serial.print("Data x");
     // Serial.println(touchX);
      
    //  Serial.print("Data y");
    //  Serial.println(touchY);

    }

    return false; /*Return `false` because we are not buffering and no more data to read*/
}

static void ta_event_cb(lv_obj_t * ta, lv_event_t event)
{
    if(event == LV_EVENT_CLICKED) {
        
       
        if(keyboard==NULL)
        {
          keyboard = lv_keyboard_create(lv_scr_act(), NULL);
          lv_obj_set_size(keyboard, LV_HOR_RES, LV_VER_RES/2);
          lv_obj_set_event_cb(keyboard, keyboard_event_cb);
          lv_keyboard_set_textarea(keyboard, ta);
        }
         if(keyboard != NULL)
            lv_keyboard_set_textarea(keyboard, ta);
    }

    else if(event == LV_EVENT_INSERT) {
        //const char * str = lv_event_get_data();
        //if(str[0] == '\n') {
      //      printf("Ready\n");
        //}
    }
}


void btn_cancel(){
  //TODO powrot do glownego gui
}

static void btn_connect(lv_obj_t * obj, lv_event_t event){
  if(event==LV_EVENT_RELEASED)
  {
    wifiAttempts=10;
    ssid=lv_textarea_get_text(ssid_ta);
    Serial.println(ssid);
    password=lv_textarea_get_text(pwd_ta);
    Serial.println(password);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED and wifiAttempts>0){
      Serial.print(".");
      delay(500);
      wifiAttempts--;
    }
  }
}
void setup(){
//  tft.loadFont();
  
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, 16, 17);
  lv_init();
  tft.init();
  tft.setRotation(3);
  uint16_t calData[5] = {299, 3588, 348, 3474, 1};
  tft.setTouch(calData);
  lv_disp_buf_init(&disp_buf, buf, NULL, LV_HOR_RES_MAX * 10);
  /*Initialize the display*/
  lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = 320;
  disp_drv.ver_res = 240;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.buffer = &disp_buf;
  lv_disp_drv_register(&disp_drv);
    //dotyk
  lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);             /*Descriptor of a input device driver*/
  indev_drv.type = LV_INDEV_TYPE_POINTER;    /*Touch pad is a pointer-like device*/
  indev_drv.read_cb = my_touchpad_read;      /*Set your driver function*/
  lv_indev_drv_register(&indev_drv);         /*Finally register the driver*/



  lv_obj_t * ssid_label = lv_label_create(lv_scr_act(), NULL);
  lv_label_set_text(ssid_label, "SSID: ");
  lv_obj_set_pos(ssid_label, 5, 28);

  ssid_ta = lv_textarea_create(lv_scr_act(), NULL);
  lv_textarea_set_text(ssid_ta, "");
  lv_textarea_set_pwd_mode(ssid_ta, false);
  lv_textarea_set_one_line(ssid_ta, true);
  lv_obj_set_event_cb(ssid_ta, ta_event_cb);
  lv_textarea_set_cursor_hidden(ssid_ta, true);
  lv_obj_set_width(ssid_ta, LV_HOR_RES/2 -20);
  lv_obj_set_pos(ssid_ta, 100, 20);

  lv_obj_t * pwd_label = lv_label_create(lv_scr_act(), NULL);
  lv_label_set_text(pwd_label, "Password: ");
  lv_obj_set_pos(pwd_label, 5, 68);
  pwd_ta = lv_textarea_create(lv_scr_act(), NULL);
  lv_textarea_set_text(pwd_ta, "");
  lv_textarea_set_pwd_mode(pwd_ta, true);
  lv_textarea_set_one_line(pwd_ta, true);
  lv_obj_set_event_cb(pwd_ta, ta_event_cb);
  lv_textarea_set_cursor_hidden(pwd_ta, true);
  lv_obj_set_width(pwd_ta, LV_HOR_RES / 2 - 20);
  lv_obj_set_pos(pwd_ta, 100, 60);

  lv_obj_t * apply_btn = lv_btn_create(lv_scr_act(), NULL);
  lv_obj_t * apply_label = lv_label_create(apply_btn, NULL);
  lv_label_set_text(apply_label, "Connect");
  lv_obj_set_event_cb(apply_btn, btn_connect);
  lv_obj_set_width(apply_btn, 75);
  lv_obj_set_pos(apply_btn, 243, 13);

  lv_obj_t * cancel_btn = lv_btn_create(lv_scr_act(), NULL);
  lv_obj_t * cancel_label = lv_label_create(cancel_btn, NULL);
  lv_label_set_text(cancel_label, "Cancel");
  lv_obj_set_width(cancel_btn, 75);
  lv_obj_set_pos(cancel_btn, 243, 60);
  //tft.fillScreen(TFT_BLACK);
  //tftMenuInit();  
 // WiFi.begin(ssid, password);
  //while (WiFi.status() != WL_CONNECTED and wifiAttempts>0){
   // Serial.print(".");
    //delay(500);
  //  wifiAttempts--;
  //}

  /*if(WiFi.status()==WL_CONNECTED)
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
*/
}


void loop(){
  /*sht30.get(); 
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
  }*/
  lv_task_handler(); /* let the GUI do its work */
    
}
 
  
