#pragma once

#include <../configs./pin_conf.h>
#include <NTPClient.h>
#include <RtcDS1307.h>
#include <TFT_eSPI.h>
#include <WEMOS_SHT3X.h>
#include <WebServer.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include <lvgl.h>

#include <MySD.hpp>
#include <PMS5003.hpp>

// ! CONFIG ============================================|
std::string configFilePath = "/settings.json";

Config config = {"", "", 60000, 3600000, 30000, 5, 0, 30000};

int ntpTimeOffset = 3600;  // poland, winter - 3600, summer (DST) - 7200

#define LVGL_TICK_PERIOD 60
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define GMT_OFFSET_SEC 7200
#define MIN_RANGE 1
#define MAX_RANGE 999

// RTC, PMS5003 and SHT30 objects declaration
RtcDS1307<TwoWire> Rtc(Wire);
PMS5003 *pmsSensor;
SHT3X sht30(0x44);  // TODO move address for some config file

std::map<std::string, float> data;
const char *labels[15] = {"framelen",       "pm10_standard",  "pm25_standard",   "pm100_standard", "pm10_env",
                          "pm25_env",       "pm100_env",      "particles_03um",  "particles_05um", "particles_10um",
                          "particles_25um", "particles_50um", "particles_100um", "unused",         "checksum"};

// NTPClient declarations
static const char ntpServerName[] = "europe.pool.ntp.org";
WiFiUDP ntpUDP;
NTPClient dateTimeClient(ntpUDP, ntpServerName, ntpTimeOffset);

// Webserver
WebServer server(80);
String appIpAddress = "";
int fetchPeriod = 30000;

// TFT display using TFT_eSPI and lvgl library
TFT_eSPI tft = TFT_eSPI();
static lv_disp_buf_t disp_buf;
static lv_color_t buf[LV_HOR_RES_MAX * 10];

// SD Card and sqlite database objects declaration
MySD mySDCard(PinConfig::sdCardPin);
SQLiteDb sampleDB("/sd/database.db", "/database.db", "samples");

String lastSampleTimestamp;

bool inTimeSettings = false;
bool timeChanged = false;
bool dateChanged = false;
// Temperature, relative humidity and pm2.5 per ug/m3 variables
float temp, humi, pm25Aqi;

// ? --------------------------------------------------styles

static lv_style_t font16Style;

static lv_style_t warningStyle;
static lv_style_t whiteButtonStyle;

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