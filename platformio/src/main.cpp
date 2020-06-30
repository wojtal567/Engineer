#include <Arduino.h>
#include <Wire.h>
#include <lvgl.h>
#include <TFT_eSPI.h>
#include <RtcDS1307.h>
#include <WiFi.h>
#include <WEMOS_SHT3X.h> 
#define LVGL_TICK_PERIOD 60
RtcDS1307<TwoWire> Rtc(Wire);
SHT3X sht30(0x45);
//Ticker tick; /* timer for interrupt handler */
TFT_eSPI tft = TFT_eSPI(); /* TFT instance */
static lv_disp_buf_t disp_buf;
static lv_color_t buf[LV_HOR_RES_MAX * 10];

int measure_period = 10000;

//main gui
  char *pm[] = {  "PM 1,0", "PM 2,5",  "PM 10,0"};
  char *pmParticle[] = {  ">1,0 um", ">2,5 um",  ">10,0 um"};
  lv_obj_t * main_scr;
  lv_obj_t * statusAtBar;
  lv_obj_t * dateAndTimeAtBar;
  lv_obj_t * contBar;
  lv_obj_t * contTemp;
  lv_obj_t * contHumi;
  lv_obj_t * labelSetButton;
  lv_obj_t * setButton;
  lv_obj_t * labelPMTitle[3];
  lv_obj_t * labelPMData[3];
  lv_obj_t * labelParticles;
  lv_obj_t * labelPMTitleParticle[3];
  lv_obj_t * labelPMParticle[3];
  lv_obj_t * barPMParticle[3];
  lv_obj_t * labelTemp;
  lv_obj_t * labelHumi;
  lv_obj_t * labelTempValue;
  lv_obj_t * labelHumiValue;
  lv_obj_t * led[3];



//wifi gui
lv_obj_t * contBarWiFi;
lv_obj_t * wifiLabelAtBar;
lv_obj_t * wifi_scr;
lv_obj_t * keyboard;
lv_obj_t * ssid_ta;
lv_obj_t * pwd_ta;
lv_obj_t * ssid_label;
lv_obj_t * pwd_label;
lv_obj_t * apply_btn;
lv_obj_t * apply_label;
lv_obj_t * cancel_btn;
lv_obj_t * cancel_label;


lv_task_t * turnFanOn;
lv_task_t * getSample;

//synchronizacja rtc
lv_task_t * syn_rtc;
bool date_synchronized = false;

int screenWidth = 320;
int screenHeight = 240;
const char* ssid = "";
const char* password = "";
const char* ntpServer = "0.pool.ntp.org";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;



int temp, humi;
struct pms5003data {
  uint16_t framelen;
  uint16_t pm10_standard, pm25_standard, pm100_standard;
  uint16_t pm10_env, pm25_env, pm100_env;
  uint16_t particles_03um, particles_05um, particles_10um, particles_25um, particles_50um, particles_100um;
  uint16_t unused;
  uint16_t checksum;
};

struct pms5003data data;

boolean readPMSdata(Stream *s) {
  
  if (! Serial2.available())
  {
    Serial.println("dsaldasdasdsd");
    return false;
  } 
    
  
  
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





void config_time(lv_task_t * task)
{
  if(date_synchronized)
  {
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
}

void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
  uint16_t c;

  tft.startWrite(); /* Start new TFT transaction */
  tft.setAddrWindow(area->x1, area->y1, (area->x2 - area->x1 + 1), (area->y2 - area->y1 + 1)); /* set the working window */
  for (int y = area->y1; y <= area->y2; y++) {
    for (int x = area->x1; x <= area->x2; x++) {
      c = color_p->full;
      tft.writeColor(c, 1);
      color_p++;
    }
  }
  tft.endWrite(); /* terminate TFT transaction */
  lv_disp_flush_ready(disp); /* tell lvgl that flushing is done */
}

bool my_touchpad_read(lv_indev_drv_t * indev_driver, lv_indev_data_t * data)
{
  uint16_t touchX, touchY;

  bool touched = tft.getTouch(&touchX, &touchY, 600);

  if (!touched)
  {
    return false;
  }

  if (touchX > screenWidth || touchY > screenHeight)
  {
    Serial.println("Y or y outside of expected parameters..");
    Serial.print("y:");
    Serial.print(touchX);
    Serial.print(" x:");
    Serial.print(touchY);
  }
  else
  {

    data->state = touched ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL;

    /*Save the state and save the pressed coordinate*/
    //if(data->state == LV_INDEV_STATE_PR) touchpad_get_xy(&last_x, &last_y);

    /*Set the coordinates (if released use the last pressed coordinates)*/
    data->point.x = touchX;
    data->point.y = touchY;

    Serial.print("Data x");
    Serial.println(touchX);

    Serial.print("Data y");
    Serial.println(touchY);

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
          lv_obj_set_event_cb(keyboard, lv_keyboard_def_event_cb);
          lv_keyboard_set_textarea(keyboard, ta);
        }
         if(keyboard != NULL)
            lv_keyboard_set_textarea(keyboard, ta);
    }
}

static void btn_connect(lv_obj_t * obj, lv_event_t event){
  if(event==LV_EVENT_RELEASED and (lv_textarea_get_text(ssid_ta)!="" or lv_textarea_get_text(pwd_ta)!=""))
  {
    uint8_t wifiAttempts=10;
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
    if(WiFi.status()==WL_CONNECTED)
    {
      configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
      date_synchronized = true;
      lv_task_ready(syn_rtc);
      lv_label_set_text(statusAtBar, "  " LV_SYMBOL_WIFI);
    }
    lv_disp_load_scr(main_scr);
    lv_textarea_set_text(ssid_ta, "");
    lv_textarea_set_text(pwd_ta, "");
  }
}

static void setButton_task(lv_obj_t * obj, lv_event_t event)
{
  lv_disp_load_scr(wifi_scr);
}

static void btn_cancel(lv_obj_t * obj, lv_event_t event)
{
  lv_disp_load_scr(main_scr);
  lv_textarea_set_text(ssid_ta, "");
  lv_textarea_set_text(pwd_ta, "");
}

void getSampleFunc(lv_task_t * task)
{
  temp = sht30.cTemp;
  humi = sht30.humidity;
  
  char buffer[7];
  
  //TODO wrzucenie na lcd 
  if(readPMSdata(&Serial2))
  {
    itoa(data.pm10_standard, buffer, 10);
    lv_label_set_text(labelPMData[0], buffer);
    itoa(data.pm25_standard, buffer, 10);
    lv_label_set_text(labelPMData[1], buffer);
    itoa(data.pm100_standard, buffer, 10);
    lv_label_set_text(labelPMData[2], buffer);
    itoa(data.particles_10um, buffer, 10);
    lv_label_set_text(labelPMParticle[0], buffer);
    lv_bar_set_value(barPMParticle[0], data.particles_10um, LV_ANIM_ON);
    itoa(data.particles_25um, buffer, 10);
    lv_label_set_text(labelPMParticle[1], buffer);
    lv_bar_set_value(barPMParticle[1], data.particles_25um, LV_ANIM_ON);
    itoa(data.particles_100um, buffer, 10);
    lv_label_set_text(labelPMParticle[2], buffer);
    lv_bar_set_value(barPMParticle[2], data.particles_100um, LV_ANIM_ON);
  }
  itoa(temp, buffer, 10);
  lv_label_set_text(labelTempValue, buffer); //TODO DODAC KURDE BELA "°C"
  itoa(humi, buffer, 10);
  lv_label_set_text(labelHumiValue, buffer); //TODO DODAC KURDE BELA "%" 
  lv_task_reset(turnFanOn);
  digitalWrite(33, LOW);
}

void turnFanOnFunc(lv_task_t * task)
{
  digitalWrite(33, HIGH);
}

void date_time(lv_task_t * task)
{
  RtcDateTime dt = Rtc.GetDateTime();
  char datestring[20];
  snprintf_P(datestring, 
            20,
            PSTR("%02u.%02u.%04u %02u:%02u:%02u"),
            dt.Day(),
            dt.Month(),
            dt.Year(),
            dt.Hour(),
            dt.Minute(),
            dt.Second() );
  if(WiFi.status()==WL_CONNECTED){
  //TODO Rozbic datestring na date i czas oddzielnie, zeby nie zmienialy rozmiaru non stop lub cos z align ustawic
    lv_label_set_text(dateAndTimeAtBar, datestring);
    lv_label_set_text(statusAtBar, LV_SYMBOL_WIFI);
  }
}

void main_screen()
{
  contBar = lv_cont_create(main_scr, NULL);
  lv_obj_set_auto_realign(contBar, true);                    /*Auto realign when the size changes*/
  lv_obj_align(contBar, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);  /*This parametrs will be sued when realigned*/
  lv_cont_set_fit4(contBar,   LV_FIT_PARENT, LV_FIT_PARENT, LV_FIT_NONE, LV_FIT_NONE);
  lv_cont_set_layout(contBar, LV_LAYOUT_PRETTY_MID);
 
  setButton = lv_btn_create(main_scr, NULL);
  labelSetButton = lv_label_create(setButton, NULL);
  lv_obj_align(setButton, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, 40, 0);
  lv_label_set_text(labelSetButton, LV_SYMBOL_SETTINGS);
  lv_btn_set_fit(setButton,  LV_FIT_TIGHT);
  lv_obj_set_event_cb(setButton, setButton_task);
  lv_obj_set_style_local_bg_opa(setButton, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);
  lv_obj_set_style_local_border_opa(setButton, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);
  lv_obj_set_style_local_border_opa(setButton, LV_BTN_PART_MAIN, LV_BTN_STATE_PRESSED, LV_OPA_TRANSP);

  statusAtBar = lv_label_create(contBar, NULL);
  dateAndTimeAtBar = lv_label_create(contBar, NULL);
  lv_label_set_text(statusAtBar, "");
  lv_label_set_text(dateAndTimeAtBar, "Hello!");
 
  for (int i = 0; i < 3; i++) {
    led[i]  = lv_led_create(main_scr, NULL);
    labelPMData[i] = lv_label_create(main_scr, NULL);
    labelPMTitle[i] = lv_label_create(main_scr, NULL);
    lv_led_on(led[i]);
    lv_label_set_align(labelPMData[i], LV_LABEL_ALIGN_CENTER);
    lv_obj_set_style_local_text_color(labelPMTitle[i], LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);
    lv_label_set_text(labelPMTitle[i], pm[i]);
    lv_label_set_text(labelPMData[i], "-");
    lv_obj_set_size(led[i], 10, 10);
    lv_obj_set_pos(led[i], 20, 55+i*25);
    lv_obj_set_pos(labelPMTitle[i], 40, 50+i*25);
    lv_obj_set_pos(labelPMData[i], 130, 50+i*25);
  }

  labelParticles = lv_label_create(main_scr, NULL);
  lv_obj_set_pos(labelParticles, 30, 125);
  lv_label_set_text(labelParticles, "Particles");
    
  for (int i = 0; i < 3; i++) {
    labelPMTitleParticle[i]  = lv_label_create(main_scr, NULL);
    labelPMParticle[i]  = lv_label_create(main_scr, NULL);
    barPMParticle[i] = lv_bar_create(main_scr, NULL);
    lv_label_set_align(labelPMParticle[i], LV_LABEL_ALIGN_CENTER);
    lv_obj_align(barPMParticle[i], NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_local_bg_color(barPMParticle[i], LV_BAR_PART_INDIC, LV_STATE_DEFAULT, LV_COLOR_TEAL);
    lv_label_set_text(labelPMTitleParticle[i], pmParticle[i]);
    lv_label_set_text(labelPMParticle[i], "-");
    lv_obj_set_size(barPMParticle[i], 100, 20);
    lv_bar_set_anim_time(barPMParticle[i], 2000);
    lv_bar_set_range(barPMParticle[i], 0, 100);
    lv_obj_set_pos(labelPMTitleParticle[i], 40, 150+i*25);
    lv_obj_set_pos(labelPMParticle[i], 130, 150+i*25);
    lv_obj_set_pos(barPMParticle[i], 170, 150+i*25);
  }
  contTemp = lv_cont_create(main_scr, NULL);
  contHumi = lv_cont_create(main_scr, NULL);
  labelTemp = lv_label_create(contTemp, NULL);
  labelHumi = lv_label_create(contHumi, NULL);
  labelTempValue = lv_label_create(contTemp, NULL);
  labelHumiValue = lv_label_create(contHumi, NULL);
  lv_obj_align(labelTemp, NULL, LV_ALIGN_IN_TOP_MID, -17, 15);
  lv_obj_align(labelTempValue, NULL, LV_ALIGN_IN_BOTTOM_MID, -13, 20);
  lv_obj_align(labelHumi, NULL, LV_ALIGN_IN_TOP_MID, -10, 15);
  lv_obj_align(labelHumiValue, NULL, LV_ALIGN_IN_BOTTOM_MID, -13, 20);
  lv_obj_set_size(contTemp, 60, 75);
  lv_obj_set_size(contHumi, 60, 75);
  lv_obj_set_pos(contTemp, 170, 50);
  lv_obj_set_pos(contHumi, 245, 50);
  lv_label_set_align(labelTempValue, LV_LABEL_ALIGN_CENTER);
  lv_label_set_align(labelHumiValue, LV_LABEL_ALIGN_CENTER);
  lv_label_set_text(labelTemp, "Temp");
  lv_label_set_text(labelHumi, "RH");
  lv_label_set_text(labelTempValue, "-");
  lv_label_set_text(labelHumiValue, "-");
  

    
}

void wifi_screen()
{
  contBarWiFi = lv_cont_create(wifi_scr, NULL);
  lv_obj_set_auto_realign(contBarWiFi, true);                    /*Auto realign when the size changes*/
  lv_obj_align(contBarWiFi, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);  /*This parametrs will be sued when realigned*/
  lv_cont_set_fit4(contBarWiFi,   LV_FIT_PARENT, LV_FIT_PARENT, LV_FIT_NONE, LV_FIT_NONE);
  lv_cont_set_layout(contBarWiFi, LV_LAYOUT_PRETTY_TOP);

  cancel_btn = lv_btn_create(contBarWiFi, NULL);
  cancel_label = lv_label_create(cancel_btn, NULL);
  lv_label_set_text(cancel_label, LV_SYMBOL_LEFT);
  lv_obj_set_style_local_bg_opa(cancel_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);
  lv_obj_set_style_local_border_opa(cancel_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);
  lv_obj_set_size(cancel_btn, 30, 15);
  lv_obj_set_event_cb(cancel_btn, btn_cancel);

  wifiLabelAtBar = lv_label_create(contBarWiFi, NULL);
  lv_label_set_text(wifiLabelAtBar, "WiFi settings");
  
  ssid_label = lv_label_create(wifi_scr, NULL);
  lv_label_set_text(ssid_label, "SSID: ");
  lv_obj_set_pos(ssid_label, 5, 53);

  ssid_ta = lv_textarea_create(wifi_scr, NULL);
  lv_textarea_set_text(ssid_ta, "");
  lv_textarea_set_pwd_mode(ssid_ta, false);
  lv_textarea_set_one_line(ssid_ta, true);
  lv_obj_set_event_cb(ssid_ta, ta_event_cb);
  lv_textarea_set_cursor_hidden(ssid_ta, true);
  lv_obj_set_width(ssid_ta, LV_HOR_RES/2 -20);
  lv_obj_set_pos(ssid_ta, 100, 45);

  pwd_label = lv_label_create(wifi_scr, NULL);
  lv_label_set_text(pwd_label, "Password: ");
  lv_obj_set_pos(pwd_label, 5, 92);
  pwd_ta = lv_textarea_create(wifi_scr, NULL);
  lv_textarea_set_text(pwd_ta, "");
  lv_textarea_set_pwd_mode(pwd_ta, true);
  lv_textarea_set_one_line(pwd_ta, true);
  lv_obj_set_event_cb(pwd_ta, ta_event_cb);
  lv_textarea_set_cursor_hidden(pwd_ta, true);
  lv_obj_set_width(pwd_ta, LV_HOR_RES / 2 - 20);
  lv_obj_set_pos(pwd_ta, 100, 85);

  apply_btn = lv_btn_create(wifi_scr, NULL);
  apply_label = lv_label_create(apply_btn, NULL);
  lv_label_set_text(apply_label, "Connect");
  lv_obj_set_style_local_border_opa(apply_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);
  lv_obj_set_event_cb(apply_btn, btn_connect);
  lv_obj_set_width(apply_btn, 75);
  lv_obj_set_pos(apply_btn, 243, 43);
  

}

void setup() {
  pinMode(33, OUTPUT);
  digitalWrite(33, LOW);
  Serial.begin(115200); /* prepare for possible serial debug */
  Serial2.begin(9600, SERIAL_8N1, 16, 17);
  Serial.println(Serial2.available());
  lv_init();
  
  tft.begin(); /* TFT init */
  tft.setRotation(3);

  uint16_t calData[5] = { 275, 3620, 264, 3532, 1 };
  tft.setTouch(calData);

  lv_disp_buf_init(&disp_buf, buf, NULL, LV_HOR_RES_MAX * 10);
  /*Initialize the display*/
  lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.buffer = &disp_buf;
  lv_disp_drv_register(&disp_drv);

  /*Initialize the input device driver*/
  lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);             /*Descriptor of a input device driver*/
  indev_drv.type = LV_INDEV_TYPE_POINTER;    /*Touch pad is a pointer-like device*/
  indev_drv.read_cb = my_touchpad_read;      /*Set your driver function*/
  lv_indev_drv_register(&indev_drv);         /*Finally register the driver*/

  //Set the theme..
  lv_theme_t * th = lv_theme_material_init(LV_THEME_DEFAULT_COLOR_PRIMARY, LV_THEME_DEFAULT_COLOR_SECONDARY, LV_THEME_DEFAULT_FLAG, LV_THEME_DEFAULT_FONT_SMALL , LV_THEME_DEFAULT_FONT_NORMAL, LV_THEME_DEFAULT_FONT_SUBTITLE, LV_THEME_DEFAULT_FONT_TITLE);
  lv_theme_set_act(th);

  main_scr = lv_cont_create(NULL, NULL);
  wifi_scr = lv_cont_create(NULL, NULL);
  main_screen();
  wifi_screen();
  lv_disp_load_scr(main_scr);


  lv_task_t * date = lv_task_create(date_time, 1000, LV_TASK_PRIO_MID, NULL);
  syn_rtc = lv_task_create_basic();
  lv_task_set_cb(syn_rtc, config_time);
  lv_task_set_period(syn_rtc, 3600000);
  getSample = lv_task_create(getSampleFunc, measure_period, LV_TASK_PRIO_HIGH, NULL);
  turnFanOn = lv_task_create(turnFanOnFunc, 240000, LV_TASK_PRIO_HIGH, NULL);
}

void loop() {
  lv_task_handler(); /* let the GUI do its work */
  delay(5);
}