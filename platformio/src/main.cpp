#include <Arduino.h>
#include <Wire.h>
#include <lvgl.h>
#include <TFT_eSPI.h>
#include <MyWiFi.hpp>
#include <MyRTC.hpp>
#include <WEMOS_SHT3X.h> 
#include <PMS5003.hpp>
#define LVGL_TICK_PERIOD 60
#define GMT_OFFSET_IN_SEC = 3600;
#define DAYLIGHT_OFFSET_IN_SEC = 3600;
#define NTP_SERVER = "0.pool.ntp.org";

PMS5003 *pmsSensor;
MyWiFi *myWiFi;
MyRTC myRTC(3600, 3600, "0.pool.ntp.org");

SHT3X sht30(0x45);
//Ticker tick; /* timer for interrupt handler */
TFT_eSPI tft = TFT_eSPI(); /* TFT instance */
static lv_disp_buf_t disp_buf;
static lv_color_t buf[LV_HOR_RES_MAX * 10];

//main gui
  char *pm[] = { "PM 1.0", "PM 2.5",  "PM 10.0"};
  lv_obj_t * main_scr;
  lv_obj_t * statusAtBar;
  lv_obj_t * dateAndTimeAtBar;
  lv_obj_t * contBar;
  lv_obj_t * labelSetButton;
  lv_obj_t * setButton;
  lv_obj_t * labelPMTitle[3];
  lv_obj_t * labelPMData[3];
  lv_obj_t * labelTemp;
  lv_obj_t * labelRH;
  lv_obj_t * led[3];



//wifi gui
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



int temp, humi;

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
  if(event==LV_EVENT_RELEASED)
  {
    uint8_t wifiAttempts=10;
    char ssid=lv_textarea_get_text(ssid_ta);
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
  Serial.println("Start measuring...");
  char buffer[7];
  
  //TODO wrzucenie na lcd 
  if(pmsSensor->readData())
  {
    pmsSensor->readData();
    std::map<std::string, uint16_t> data = pmsSensor->returnData();
    Serial.println(data["pm10standard"]);
    itoa(data["pm10_standard"], buffer, 10);
    lv_label_set_text(labelPMData[0], buffer);
    itoa(data["pm25_standard"], buffer, 10);
    lv_label_set_text(labelPMData[1], buffer);
    itoa(data["pm100_standard"], buffer, 10);
    lv_label_set_text(labelPMData[2], buffer);
    pmsSensor->dumpSamples();
  }
  Serial.println("Done.");
  lv_task_reset(turnFanOn);
  digitalWrite(33, LOW);
}

void turnFanOnFunc(lv_task_t * task)
{
  digitalWrite(33, HIGH);
}

void date_time(lv_task_t * task)
{
  if(WiFi.status()==WL_CONNECTED)
    lv_label_set_text(dateAndTimeAtBar, myRTC.getTimestamp().c_str());
}

void config_time(lv_task_t * task)
{
  myRTC.config(date_synchronized);
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
  lv_label_set_text(statusAtBar, LV_SYMBOL_SD_CARD "  " LV_SYMBOL_WIFI);
  lv_label_set_text(dateAndTimeAtBar, "21.03.2013 15:12:23");
 
  for (int i = 0; i < 3; i++) {
    labelPMData[i] = lv_label_create(main_scr, NULL);
    labelPMTitle[i] = lv_label_create(main_scr, NULL);
    led[i]  = lv_led_create(main_scr, NULL);
    lv_led_on(led[i]);
    lv_label_set_align(labelPMData[i], LV_LABEL_ALIGN_CENTER);
    lv_obj_set_style_local_text_color(labelPMTitle[i], LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);
    lv_label_set_text(labelPMTitle[i], pm[i]);
    lv_label_set_text(labelPMData[i], " 1");
    lv_obj_set_size(led[i], 10, 10);
    lv_obj_set_pos(labelPMTitle[i], 40, 50+i*25);
    lv_obj_set_pos(labelPMData[i], 120, 50+i*25);
    lv_obj_set_pos(led[i], 20, 55+i*25);
  }
 
  lv_obj_t * lmeter1;
    lmeter1 = lv_linemeter_create(main_scr, NULL);
    lv_linemeter_set_range(lmeter1, 0, 100);                   /*Set the range*/
    lv_linemeter_set_value(lmeter1, 65);                       /*Set the current value*/
    lv_linemeter_set_scale(lmeter1, 0, 0);                  /*Set the angle and number of lines*/
    lv_obj_set_size(lmeter1, 70, 70);
    lv_obj_set_pos(lmeter1, 160, 50);
 
 
 lv_obj_t * led3  = lv_led_create(main_scr, NULL);
 lv_obj_set_size(led3, 10, 10);
 lv_obj_set_pos(led3, 20, 55);
 lv_led_on(led3);
  labelTemp = lv_label_create(main_scr, NULL);
  labelRH = lv_label_create(main_scr, NULL);
}

void wifi_screen()
{
  ssid_label = lv_label_create(wifi_scr, NULL);
  lv_label_set_text(ssid_label, "SSID: ");
  lv_obj_set_pos(ssid_label, 5, 28);

  ssid_ta = lv_textarea_create(wifi_scr, NULL);
  lv_textarea_set_text(ssid_ta, "");
  lv_textarea_set_pwd_mode(ssid_ta, false);
  lv_textarea_set_one_line(ssid_ta, true);
  lv_obj_set_event_cb(ssid_ta, ta_event_cb);
  lv_textarea_set_cursor_hidden(ssid_ta, true);
  lv_obj_set_width(ssid_ta, LV_HOR_RES/2 -20);
  lv_obj_set_pos(ssid_ta, 100, 20);

  pwd_label = lv_label_create(wifi_scr, NULL);
  lv_label_set_text(pwd_label, "Password: ");
  lv_obj_set_pos(pwd_label, 5, 68);
  pwd_ta = lv_textarea_create(wifi_scr, NULL);
  lv_textarea_set_text(pwd_ta, "");
  lv_textarea_set_pwd_mode(pwd_ta, true);
  lv_textarea_set_one_line(pwd_ta, true);
  lv_obj_set_event_cb(pwd_ta, ta_event_cb);
  lv_textarea_set_cursor_hidden(pwd_ta, true);
  lv_obj_set_width(pwd_ta, LV_HOR_RES / 2 - 20);
  lv_obj_set_pos(pwd_ta, 100, 60);

  apply_btn = lv_btn_create(wifi_scr, NULL);
  apply_label = lv_label_create(apply_btn, NULL);
  lv_label_set_text(apply_label, "Connect");
  lv_obj_set_event_cb(apply_btn, btn_connect);
  lv_obj_set_width(apply_btn, 75);
  lv_obj_set_pos(apply_btn, 243, 13);
  cancel_btn = lv_btn_create(wifi_scr, NULL);
  cancel_label = lv_label_create(cancel_btn, NULL);
  lv_label_set_text(cancel_label, "Cancel");
  lv_obj_set_width(cancel_btn, 75);
  lv_obj_set_pos(cancel_btn, 243, 60);
  lv_obj_set_event_cb(cancel_btn, btn_cancel);
}

void setup() {
  pinMode(33, OUTPUT);
  digitalWrite(33, LOW);
  Serial.begin(115200); /* prepare for possible serial debug */
  Serial2.begin(9600, SERIAL_8N1, 16, 17);
  Serial.println(Serial2.available());
  pmsSensor = new PMS5003(&Serial2, &Serial);
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
  lv_task_set_cb(syn_rtc, config_time());
  lv_task_set_period(syn_rtc, 3600000);
  getSample = lv_task_create(getSampleFunc, 300000, LV_TASK_PRIO_HIGH, NULL);
  turnFanOn = lv_task_create(turnFanOnFunc, 240000, LV_TASK_PRIO_HIGH, NULL);
}

void loop() {
  lv_task_handler(); /* let the GUI do its work */
  delay(5);
}