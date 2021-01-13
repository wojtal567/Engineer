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

#define FAN_PIN 4        // * number of PIN which controls the PMS fan
#define MY_SD_CARD_PIN 27 // * pin of SD_CS

// ! CONFIG ============================================|
std::string configFilePath = "/settings.json";

Config config =
    {
        "",
        "",
        60000,
        3600000,
        30000,
        5,
        0,
        30000};

int ntpTimeOffset = 3600; //poland, winter - 3600, summer (DST) - 7200 

#define LVGL_TICK_PERIOD 60
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define GMT_OFFSET_SEC 7200

//Include additional font with lock and unlock symbol
extern lv_font_t monte16lock;
#define MY_LOCK_SYMBOL "\xEF\x80\xA3"
#define MY_UNLOCK_SYMBOL "\xEF\x82\x9C"

extern lv_font_t hugeSymbolsFont48;
#define MY_INFO_SYMBOL "\xEF\x81\x9A"
#define MY_CLOCK_SYMBOL "\xEF\x80\x97"
#define MY_WIFI_SYMBOL "\xEF\x87\xAB"
#define MY_COGS_SYMBOL "\xEF\x82\x85"

//RTC, PMS5003 and SHT30 objects declaration
RtcDS1307<TwoWire> Rtc(Wire);
PMS5003 *pmsSensor;
SHT3X sht30(0x45);

std::map<std::string, float> data;
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
NTPClient dateTimeClient(ntpUDP, ntpServerName, ntpTimeOffset);

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

bool inTimeSettings = false;
bool timeChanged = false;
bool dateChanged = false;
bool isDefaultTimeOnDisplay=false;
//Temperature, relative humidity and pm2.5 per ug/m3 variables
float temp, humi, pm25Aqi;

// ? --------------------------------------------------styles
//Basic container with white border and transparent background
static lv_style_t containerStyle;

//Different font sizes using lvgl styles
static lv_style_t whiteFontStyle;
static lv_style_t font12Style;
static lv_style_t font16Style;
static lv_style_t font20Style;
static lv_style_t font22Style;

//Additional styles with initalization functions
static lv_style_t warningStyle;
static lv_style_t whiteButtonStyle;
static lv_style_t lineStyle;
static lv_style_t transparentBackgroundStyle;
static lv_style_t borderlessStyle;
static lv_style_t hugeFontStyle;
// ? --------------------------------------------------main gui
//Main screen objects declaration
lv_obj_t *mainScr; //LVGL Object that represents main screen
lv_obj_t *wifiStatusAtMain;
lv_obj_t *sdStatusAtMain;
lv_obj_t *wifiStatusAtMainWarning;
lv_obj_t *sdStatusAtMainWarning;
lv_obj_t *dateAndTimeAtBar;
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
lv_obj_t *labelParticleSizeum[7];
lv_obj_t *labelParticlesNumber[6];
lv_obj_t *contParticlesNumber[6];
lv_obj_t *ledAtMain;

String airQualityStates[6] = {"Excellent", "Good", "Moderate", "Unhealthy", "Bad", "Hazardous"};
String particlesSize[7] = {"0.0", "0.3", "0.5", "1.0", "2.5", "5.0", "10.0"};
float aqiStandards[5] = {21, 61, 101, 141, 201};
int labelParticleSizePosX[7] = {9, 56, 103, 153, 198, 245, 288};
int contParticleNumberPosX[6] = {18, 65, 112, 159, 206, 253};
static lv_point_t mainLinePoints[] = {{18, 210}, {300, 210}};

//An array of points pairs instead of multiple names and declarations
static lv_point_t dividingLinesPoints[][7] = {{{18, 205} , {18, 215}}, 
                                              {{65, 205} , {65, 215}},
                                              {{112, 205}, {112, 215}},
                                              {{159, 205}, {159, 215}},
                                              {{206, 205}, {206, 215}},
                                              {{253, 205}, {253, 215}},
                                              {{300, 205}, {300, 215}}};
//Main line at the bottom declaration
lv_obj_t *mainLine;
//An array of lines dividing main one
lv_obj_t *dividingLines[7];
//An array of colors used depending on actual pm2.5 value
lv_color_t airQualityColors[6] = {LV_COLOR_GREEN, LV_COLOR_GREEN, LV_COLOR_YELLOW, LV_COLOR_ORANGE, LV_COLOR_RED, LV_COLOR_RED};

// ? --------------------------------------------------wifi gui
lv_obj_t *wifiLabelAtBar;
lv_obj_t *wifiScr;
lv_obj_t *keyboard;
lv_obj_t *ssidTA;
lv_obj_t *pwdTA;
lv_obj_t *ssidLabel;
lv_obj_t *pwdLabel;
lv_obj_t *applyBtn;
lv_obj_t *applyLabel;
lv_obj_t *cancelBtn;
lv_obj_t *cancelLabel;
lv_obj_t *showHideBtn;
lv_obj_t *showHideBtnLabel;
// ? -------------------------------------------------- info gui
lv_obj_t *infoScr;
lv_obj_t *backInfoBtn;
lv_obj_t *backInfoLabel;
lv_obj_t *lcdLabelAtBar;
lv_obj_t *infoWifiLabel;
lv_obj_t *infoWifiAddressLabel;
lv_obj_t *configLabel;
// ? --------------------------------------------------settings gui
lv_obj_t *settingsScr;
lv_obj_t *backSettingsBtn;
lv_obj_t *backSettingsLabel;
lv_obj_t *settingsLabelAtBar;
lv_obj_t *wifiBtn;
lv_obj_t *infoBtn;
lv_obj_t *timeBtn;
lv_obj_t *wifiBtnLabel;
lv_obj_t *infoBtnLabel;
lv_obj_t *timeBtnLabel;
lv_obj_t *tempBtn;
lv_obj_t *tempBtnLabel;
lv_obj_t *wifiBtnName;
lv_obj_t *infoBtnName;
lv_obj_t *timeBtnName;
lv_obj_t *tempBtnName;

// ? --------------------------------------------------time settings gui
lv_obj_t *timeSettingsScr;

lv_obj_t *backTimeSettingsBtn;
lv_obj_t *backTimeSettingsLabel;
lv_obj_t *timeSettingsLabelAtBar;

lv_obj_t *timeScrollPage;
lv_obj_t *timeLabel;
lv_obj_t *timeHour;
lv_obj_t *timeHourIncrement;
lv_obj_t *timeHourDecrement;

lv_obj_t *timeColonLabel;

lv_obj_t *timeMinute;
lv_obj_t *timeMinuteIncrement;
lv_obj_t *timeMinuteDecrement;

lv_obj_t *dateLabel;
lv_obj_t *dateBtn;
lv_obj_t *dateBtnLabel;

lv_obj_t *calendar;

lv_obj_t *lockScreenLabel;
lv_obj_t *lockScreenDDlist;

lv_obj_t *timeSettingsBtn;
lv_obj_t *timeSettingsLabel;
lv_obj_t *syncRtcBtn;
lv_obj_t *syncRtcLabel;
lv_obj_t *alertBox;
// ? -------------------------------------------------- second settings gui
lv_obj_t *samplingSettingsScr;
lv_obj_t *backSamplingSettingsBtn;
lv_obj_t *backSamplingSettingsLabel;
lv_obj_t *SamplingSettingsLabelAtBar;

lv_obj_t *measurePeriodlabel;

lv_obj_t *measurePeriodHour;
lv_obj_t *measurePeriodHourIncrement;
lv_obj_t *measurePeriodHourDecrement;

lv_obj_t *measureColonLabel;

lv_obj_t *measurePeriodMinute;
lv_obj_t *measurePeriodMinuteIncrement;
lv_obj_t *measurePeriodMinuteDecrement;

lv_obj_t *measureColonLabel2;

lv_obj_t *measurePeriodsecond;
lv_obj_t *measurePeriodsecondIncrement;
lv_obj_t *measurePeriodsecondDecrement;

lv_obj_t *measureNumberLabel;
lv_obj_t *measureNumber;
lv_obj_t *measureNumberIncrement;
lv_obj_t *measureNumberDecrement;

lv_obj_t *measureAvPeriodLabel;
lv_obj_t *measureAvPeriod;
lv_obj_t *measureAvPeriodIncrement;
lv_obj_t *measureAvPeriodDecrement;

lv_obj_t *turnFanOnTimeLabel;
lv_obj_t *turnFanOnTime;
lv_obj_t *turnFanOnTimeIncrement;
lv_obj_t *turnFanOnTimeDecrement;


lv_obj_t *samplingSaveBtn;
lv_obj_t *samplingSaveLabel;

// ? --------------------------------------------------lockscreen gui
lv_obj_t *lockScr;
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
lv_task_t *getAppLastRecordAndSynchronize;
lv_task_t *inactiveTime;
lv_task_t *date;
lv_task_t *status;