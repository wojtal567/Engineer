#include <MySD.hpp>
#include <lvgl.h>
#include <RtcDS1307.h>
#include <Wire.h>
#include <PMS5003.hpp>
#include <WEMOS_SHT3X.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <WebServer.h>
#include <TFT_eSPI.h>
#include <ESP32Ping.h>

#define FAN_PIN 33        // * number of PIN which controls the PMS fan
#define MY_SD_CARD_PIN 27 // * pin of SD_CS

// ! CONFIG ============================================|
std::string configFilePath = "/settings.json";

Config config =
    {
        "",
        "",
        60000,
        3600000,
        5000,
        5,
        0};

#define LVGL_TICK_PERIOD 60
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define GMT_OFFSET_SEC 7200

//Include additional font with lock and unlock symbol
extern lv_font_t monte16lock;
#define MY_LOCK_SYMBOL "\xEF\x80\xA3"
#define MY_UNLOCK_SYMBOL "\xEF\x82\x9C"

//RTC, PMS5003 and SHT30 objects declaration
RtcDS1307<TwoWire> Rtc(Wire);
PMS5003 *pmsSensor;
SHT3X sht30(0x45);

std::map<std::string, uint16_t> data;
const char *labels[15] = {
    "framelen",
    "pm10_standard",
    "pm25_standard",
    "pm100_standard",
    "pm10_env",
    "pm25_env",
    "pm100_env",
    "particles_03um",
    "particles_05um",
    "particles_10um",
    "particles_25um",
    "particles_50um",
    "particles_100um",
    "unused",
    "checksum"};

//NTPClient declarations
static const char ntpServerName[] = "europe.pool.ntp.org";
WiFiUDP ntpUDP;
NTPClient dateTimeClient(ntpUDP, ntpServerName, 7200);
bool wasUpdated = false;

//Webserver
WebServer server(80);
String appIpAddress = "";

//TFT display using TFT_eSPI and lvgl library
TFT_eSPI tft = TFT_eSPI();
static lv_disp_buf_t disp_buf;
static lv_color_t buf[LV_HOR_RES_MAX * 10];

//SD Card and sqlite database objects declaration
MySD mySDCard(MY_SD_CARD_PIN);
SQLiteDb sampleDB("/sd/database.db", "/database.db", "samples");

String lastSampleTimestamp;

//Is data synchronized variable
bool date_synchronized = false;
bool in_time_settings = false;
bool time_changed = false;
bool date_changed = false;

//Temperature, relative humidity and pm2.5 per ug/m3 variables
float temp, humi, pm25Aqi;

//declaring photos for settings screen
LV_IMG_DECLARE(wifi);
LV_IMG_DECLARE(info);
LV_IMG_DECLARE(set_time);

String airQualityStates[6] = {"Excellent", "Good", "Moderate", "Unhealthy", "Bad", "Hazardous"};
String particlesSize[6] = {"0.3", "0.5", "1.0", "2.5", "5.0", "10.0"};
float aqiStandards[5] = {21, 61, 101, 141, 201};
int labelParticleSizePosX[6] = {56, 103, 153, 198, 245, 288};

// ? --------------------------------------------------styles
//Basic container with white border and transparent background
static lv_style_t containerStyle;

//Different font sizes using lvgl styles
static lv_style_t font12Style;
static lv_style_t font16Style;
static lv_style_t font20Style;
static lv_style_t font22Style;

//Additional styles with initalization functions
static lv_style_t tinySymbolStyle;
static lv_style_t transparentButtonStyle;
static lv_style_t whiteButtonStyle;
static lv_style_t lineStyle;
static lv_style_t toastListStyle;

// ? --------------------------------------------------main gui
//Main screen objects declaration
lv_obj_t *main_scr; //LVGL Object that represents main screen
lv_obj_t *wifiStatusAtMain;
lv_obj_t *sdStatusAtMain;
lv_obj_t *wifiStatusAtMainWarning;
lv_obj_t *sdStatusAtMainWarning;
lv_obj_t *dateAndTimeAtBar;
lv_obj_t *contBarAtMain;
lv_obj_t *contTemp;
lv_obj_t *contHumi;
lv_obj_t *contPM10;
lv_obj_t *contPM25;
lv_obj_t *contPM100;
lv_obj_t *contAQI;
lv_obj_t *contAQIColorBar;
lv_obj_t *labelLockButton;
lv_obj_t *lockButton;
lv_obj_t *labelSetButton;
lv_obj_t *setButton;
lv_obj_t *labelTemp;
lv_obj_t *labelHumi;
lv_obj_t *labelTempValue;
lv_obj_t *labelHumiValue;
lv_obj_t *labelPM10;
lv_obj_t *labelPM10UpperIndex;
lv_obj_t *labelPM25;
lv_obj_t *labelPM25UpperIndex;
lv_obj_t *labelPM100;
lv_obj_t *labelPM100UpperIndex;
lv_obj_t *labelPM10Data;
lv_obj_t *labelPM25Data;
lv_obj_t *labelPM100Data;
lv_obj_t *labelAQI;
lv_obj_t *labelAQIColorBar;
lv_obj_t *labelSizeTitle;
lv_obj_t *labelNumberTitle;
lv_obj_t *labelParticleSizeum[6];
lv_obj_t *labelParticlesNumber[5];
lv_obj_t *contParticlesNumber[5];
lv_obj_t *ledAtMain;
static lv_point_t mainLinePoints[] = {{65, 210}, {300, 210}};
//An array of points pairs instead of multiple names and declarations
static lv_point_t dividingLinesPoints[][6] = {{{65, 205}, {65, 215}},
                                              {{112, 205}, {112, 215}},
                                              {{159, 205}, {159, 215}},
                                              {{206, 205}, {206, 215}},
                                              {{253, 205}, {253, 215}},
                                              {{300, 205}, {300, 215}}};
//Main line at the bottom declaration
lv_obj_t *mainLine;
//An array of lines dividing main one
lv_obj_t *dividingLines[6];
//An array of colors used depending on actual pm2.5 value
lv_color_t airQualityColors[6] = {LV_COLOR_GREEN, LV_COLOR_GREEN, LV_COLOR_YELLOW, LV_COLOR_ORANGE, LV_COLOR_RED, LV_COLOR_RED};

// ? --------------------------------------------------wifi gui
lv_obj_t *contBarAtMainWiFi;
lv_obj_t *wifiLabelAtBar;
lv_obj_t *wifi_scr;
lv_obj_t *keyboard;
lv_obj_t *ssid_ta;
lv_obj_t *pwd_ta;
lv_obj_t *ssid_label;
lv_obj_t *pwd_label;
lv_obj_t *apply_btn;
lv_obj_t *apply_label;
lv_obj_t *cancel_btn;
lv_obj_t *cancel_label;
lv_obj_t *show_hide_btn;
lv_obj_t *show_hide_btn_label;
// ? -------------------------------------------------- wifilist gui
lv_obj_t *wifilist_scr;
lv_obj_t *contBarWiFiList;
lv_obj_t *wifilistLabelAtBar;
lv_obj_t *wifiList;
lv_obj_t *back_wifilist_btn;
lv_obj_t *back_wifilist_label;
lv_obj_t *loading_bar;
lv_obj_t *refresh_btn;
lv_obj_t *refresh_label;
// ? -------------------------------------------------- info gui
lv_obj_t *info_scr;
lv_obj_t *contBarAtMaininfo;
lv_obj_t *back_info_btn;
lv_obj_t *back_info_label;
lv_obj_t *lcdLabelAtBar;
lv_obj_t *info_wifi_label;
lv_obj_t *info_wifi_address_label;
// ? --------------------------------------------------settings gui
lv_obj_t *settings_scr;
lv_obj_t *contBarAtMainSettings;
lv_obj_t *back_settings_btn;
lv_obj_t *back_settings_label;
lv_obj_t *settingsLabelAtBar;
lv_obj_t *WiFiBtn;
lv_obj_t *infoBtn;
lv_obj_t *timeBtn;
// ? --------------------------------------------------time settings gui
lv_obj_t *time_settings_scr;
lv_obj_t *contBarAtTimeSettings;
lv_obj_t *back_time_settings_btn;
lv_obj_t *back_time_settings_label;
lv_obj_t *timeSettingsLabelAtBar;

lv_obj_t *time_scroll_page;
lv_obj_t *time_label;
lv_obj_t *time_hour;
lv_obj_t *time_hour_increment;
lv_obj_t *time_hour_decrement;

lv_obj_t *time_colon_label;

lv_obj_t *time_minute;
lv_obj_t *time_minute_increment;
lv_obj_t *time_minute_decrement;

lv_obj_t *date_label;
lv_obj_t *date_btn;
lv_obj_t *date_btn_label;

lv_obj_t *calendar;

lv_obj_t *measure_period_label;

lv_obj_t *measure_period_hour;
lv_obj_t *measure_period_hour_increment;
lv_obj_t *measure_period_hour_decrement;

lv_obj_t *measure_colon_label;

lv_obj_t *measure_period_minute;
lv_obj_t *measure_period_minute_increment;
lv_obj_t *measure_period_minute_decrement;

lv_obj_t *measure_number_label;
lv_obj_t *measure_number;
lv_obj_t *measure_number_increment;
lv_obj_t *measure_number_decrement;

lv_obj_t *measure_av_period_label;
lv_obj_t *measure_av_period;
lv_obj_t *measure_av_period_increment;
lv_obj_t *measure_av_period_decrement;

lv_obj_t *lockScreenLabel;
lv_obj_t *lockScreenDDlist;

lv_obj_t *timeSettings_btn;
lv_obj_t *timeSettings_label;
lv_obj_t *sync_rtc_btn;
lv_obj_t *sync_rtc_label;
lv_obj_t *alertBox;
// ? --------------------------------------------------lockscreen gui
lv_obj_t *lock_scr;
lv_obj_t *contDateTimeAtLock;
lv_obj_t *labelUnlockButton;
lv_obj_t *unlockButton;
lv_obj_t *labelDateLock;
lv_obj_t *labelTimeLock;
lv_obj_t *wifiStatusAtLock;
lv_obj_t *sdStatusAtLock;
lv_obj_t *wifiStatusAtLockWarning;
lv_obj_t *sdStatusAtLockWarning;
lv_obj_t *ledAtLock;
// ? --------------------------------------------------tasks
lv_task_t *turnFanOn;
lv_task_t *getSample;
lv_task_t *syn_rtc;
lv_task_t *getAppLastRecordAndSynchronize;
lv_task_t *inactive_time;
lv_task_t *date;
lv_task_t *listNetwork_task;