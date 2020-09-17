#include <Arduino.h>
#include <Wire.h>
#include <lvgl.h>
#include <TFT_eSPI.h>
#include <RtcDS1307.h>
#include <WiFi.h>
#include <WEMOS_SHT3X.h>
#include <PMS5003.hpp>
#include <rtc.hpp>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <MySD.hpp>
#include <HTTPClient.h>
#include <time.h>
#include <ESP32Ping.h>

#include <Adafruit_GFX.h>
#include <Adafruit_BusIO_Register.h>

//checking internet connection
const IPAddress remote_ip(192, 168, 1, 1);

#define LVGL_TICK_PERIOD 60
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define GMT_OFFSET_SEC 7200

//RTC, PMS5003 and SHT30 objects declaration
RtcDS1307<TwoWire> Rtc(Wire);
PMS5003 *pmsSensor;
SHT3X sht30(0x45);

//NTPClient declarations 
static const char ntpServerName[] = "europe.pool.ntp.org";
WiFiUDP ntpUDP;
NTPClient dateTimeClient(ntpUDP, ntpServerName, 7200);
bool wasUpdated = false;

//TFT display using TFT_eSPI and lvgl library
TFT_eSPI tft = TFT_eSPI();
static lv_disp_buf_t disp_buf;
static lv_color_t buf[LV_HOR_RES_MAX * 10];

//Time between sampling
int measure_period;
//inactive time
int lcd_lock_time;
//SD Card and sqlite database objects declaration
MySD mySDCard(27);
SQLiteDb sampleDB("/sd/database.db", "/database.db", "samples");

//Is data synchronized variable
bool date_synchronized = false;

//Wifi connection strings
String ssid = "";
String password = "";

//Temperature, relative humidity and pm2.5 per ug/m3 variables
float temp, humi, pm25Aqi;

//declaring photos for settings screen
LV_IMG_DECLARE(wifi);
LV_IMG_DECLARE(lcd);
LV_IMG_DECLARE(set_time);
//
String fetchLastRecordURL = "";

String airQualityStates[6] = { "Excellent", "Good", "Moderate", "Unhealthy", "Very unhealthy", "Hazardous" };
String particlesSize[6] = {"0.3", "0.5", "1.0", "2.5", "5.0", "10.0"};
float aqiStandards[5] = {21, 61, 101, 141, 201};

//--------------------------------------------------styles
//Basic container with white border and transparent background
static lv_style_t containerStyle;
void containerStyleInit(void){
	lv_style_init(&containerStyle);
	lv_style_set_text_font(&containerStyle, LV_STATE_DEFAULT, &lv_font_montserrat_16);
	lv_style_set_text_color(&containerStyle, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_bg_opa(&containerStyle, LV_STATE_DEFAULT, LV_OPA_0);
	lv_style_set_border_color(&containerStyle, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_radius(&containerStyle, LV_STATE_DEFAULT, 0);
}
//Different font sizes using lvgl styles
static lv_style_t font12Style;
static lv_style_t font16Style;
static lv_style_t font20Style;
static lv_style_t font22Style;

//Style initialization functions
void font20StyleInit(void){
	lv_style_init(&font20Style);
	lv_style_set_text_font(&font20Style, LV_STATE_DEFAULT, &lv_font_montserrat_20);
	lv_style_set_text_color(&font20Style, LV_STATE_DEFAULT, LV_COLOR_WHITE);
}

void font12StyleInit(void){
	lv_style_init(&font12Style);
	lv_style_set_text_font(&font12Style, LV_STATE_DEFAULT, &lv_font_montserrat_12);
	lv_style_set_text_color(&font12Style, LV_STATE_DEFAULT, LV_COLOR_WHITE);
}
void font16StyleInit(void){
	lv_style_init(&font16Style);
	lv_style_set_text_font(&font16Style, LV_STATE_DEFAULT, &lv_font_montserrat_16);
	lv_style_set_text_color(&font16Style, LV_STATE_DEFAULT, LV_COLOR_WHITE);
}
void font22StyleInit(void){
	lv_style_init(&font22Style);
	lv_style_set_text_font(&font22Style, LV_STATE_DEFAULT, &lv_font_montserrat_22);
	lv_style_set_text_color(&font22Style, LV_STATE_DEFAULT, LV_COLOR_WHITE);
}

//Additional styles with initalization functions
static lv_style_t tinySymbolStyle;
static lv_style_t transparentButtonStyle;
static lv_style_t lineStyle;

//Tiny symbols to signalize wifi and sd card status
void tinySymbolStyleInit(void){
	lv_style_init(&tinySymbolStyle);
	lv_style_set_text_font(&tinySymbolStyle, LV_STATE_DEFAULT, &lv_font_montserrat_12);
	lv_style_set_bg_opa(&tinySymbolStyle, LV_STATE_DEFAULT, LV_OPA_0);
	lv_style_set_border_opa(&tinySymbolStyle, LV_STATE_DEFAULT, LV_OPA_0);
	lv_style_set_radius(&tinySymbolStyle, LV_STATE_DEFAULT, 0);
	lv_style_set_text_color(&tinySymbolStyle, LV_STATE_DEFAULT, LV_COLOR_RED);
}

void transparentButtonStyleInit(void){
	lv_style_init(&transparentButtonStyle);
	lv_style_set_bg_opa(&transparentButtonStyle, LV_STATE_DEFAULT, LV_OPA_0);
	lv_style_set_border_opa(&transparentButtonStyle, LV_STATE_DEFAULT, LV_OPA_0);
	lv_style_set_radius(&transparentButtonStyle, LV_STATE_DEFAULT, 0);
	lv_style_set_text_color(&transparentButtonStyle, LV_STATE_DEFAULT, LV_COLOR_WHITE);
}

void lineStyleInit(void){
	lv_style_init(&lineStyle);
    lv_style_set_line_width(&lineStyle, LV_STATE_DEFAULT, 2);
    lv_style_set_line_color(&lineStyle, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_line_rounded(&lineStyle, LV_STATE_DEFAULT, false);
}

//--------------------------------------------------main gui
//Main screen objects declaration
lv_obj_t *main_scr;
lv_obj_t *wifiStatusAtMain;
lv_obj_t *sdStatusAtMain;
lv_obj_t *wifiStatusAtMainWarning;
lv_obj_t *sdStatusAtMainWarning;
lv_obj_t *dateAndTimeAtBar;
lv_obj_t *contBar;
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
lv_obj_t *labelPM25;
lv_obj_t *labelPM100;
lv_obj_t *labelPM10Data;
lv_obj_t *labelPM25Data;
lv_obj_t *labelPM100Data;
lv_obj_t *labelAQI;
lv_obj_t *labelAQIColorBar;
lv_obj_t *labelSizeTitle;
lv_obj_t *labelNumberTitle;
lv_obj_t *labelSizes[6];
lv_obj_t *labelParticlesNumber[5];
lv_obj_t *contParticlesNumber[5];
static lv_point_t mainLinePoints[] = {{20, 210}, {270, 210}};
//An array of points pairs instead of multiple names and declarations
static lv_point_t dividingLinesPoints[][6] = 	{{{20,205}, {20, 215}},
												{{70,205}, {70, 215}},
												{{120,205}, {120, 215}},
												{{170,205}, {170, 215}},
												{{220,205}, {220, 215}},
												{{270,205}, {270, 215}}};
//Main line at the bottom declaration
lv_obj_t *mainLine;
//An array of lines dividing main one 
lv_obj_t *dividingLines[6];
//An array of colors used depending on actual pm2.5 value
lv_color_t airQualityColors[6] = { LV_COLOR_GREEN, LV_COLOR_GREEN, LV_COLOR_YELLOW, LV_COLOR_ORANGE, LV_COLOR_RED, LV_COLOR_RED};
//--------------------------------------------------wifi gui
lv_obj_t *contBarWiFi;
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
//-------------------------------------------------- lcd gui
lv_obj_t *lcd_scr;
lv_obj_t *contBarlcd;
lv_obj_t *back_lcd_btn;
lv_obj_t *back_lcd_label;
lv_obj_t *lcdLabelAtBar;
lv_obj_t *lockScreenLabel;
lv_obj_t *lockScreenDDlist;
lv_obj_t *lcd_save_btn;
lv_obj_t *lcd_save_label;
//--------------------------------------------------settings gui
lv_obj_t *settings_scr;
lv_obj_t *contBarSettings;
lv_obj_t *back_settings_btn;
lv_obj_t *back_settings_label;
lv_obj_t *settingsLabelAtBar;
lv_obj_t *WiFiBtn;
lv_obj_t *LcdBtn;
lv_obj_t *placeholder2Btn;
//--------------------------------------------------time settings gui
lv_obj_t *time_settings_scr;
lv_obj_t *contBarTime;
lv_obj_t *back_time_settings_btn;
lv_obj_t *back_time_settings_label;
lv_obj_t *timeSettingsLabelAtBar;
lv_obj_t *placeholder;
//--------------------------------------------------lockscreen gui
lv_obj_t *lock_scr;
lv_obj_t *contDateTimeAtLock;
lv_obj_t *labelLockButton1;
lv_obj_t *lockButton1;
lv_obj_t *labelDateLock;
lv_obj_t *labelTimeLock;
lv_obj_t *wifiStatusAtLock;
lv_obj_t *sdStatusAtLock;
lv_obj_t *wifiStatusAtLockWarning;
lv_obj_t *sdStatusAtLockWarning;
//--------------------------------------------------tasks
lv_task_t *turnFanOn;
lv_task_t *getSample;
lv_task_t *syn_rtc;
lv_task_t *getAppLastRecord;
lv_task_t *inactive_time;


void inactive_screen(lv_task_t *task)
{
	if(lv_disp_get_inactive_time(NULL)>lcd_lock_time)
	{
		if(lv_scr_act()!=lock_scr)
			lv_disp_load_scr(lock_scr);
	}
}

//Check pm2,5ug/m3 value and set status (text and color at main screen)
void setAqiStateNColor(){
	for(int i=0;i<6;i++){
		if(i==5 or pm25Aqi<aqiStandards[i]){
			lv_label_set_text(labelAQIColorBar, airQualityStates[i].c_str());
			lv_obj_set_style_local_bg_color(contAQIColorBar, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, airQualityColors[i]);
			return;
		}
	}
}

//Draw a line-like thing
void drawSomeLines(){
		for(int i=0;i<6;i++){
		dividingLines[i] = lv_line_create(main_scr, NULL);
		lv_line_set_points(dividingLines[i], dividingLinesPoints[i], 2);
		lv_obj_add_style(dividingLines[i], LV_LINE_PART_MAIN, &lineStyle);
		labelSizes[i] = lv_label_create(main_scr, NULL);
		lv_label_set_text(labelSizes[i], particlesSize[i].c_str());
		lv_obj_add_style(labelSizes[i], LV_LABEL_PART_MAIN, &font12Style);
		lv_obj_set_pos(labelSizes[i], 12+i*49, 190);
	}

	for(int j=0; j<5;j++){
		contParticlesNumber[j] = lv_cont_create(main_scr, NULL);
		lv_obj_add_style(contParticlesNumber[j], LV_OBJ_PART_MAIN, &containerStyle);
		lv_obj_set_style_local_border_opa(contParticlesNumber[j], LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_0);
		lv_obj_set_click(contParticlesNumber[j], false);
		labelParticlesNumber[j] = lv_label_create(contParticlesNumber[j], NULL);
		lv_obj_set_size(contParticlesNumber[j], 50, 14);
		lv_obj_set_pos(contParticlesNumber[j], 20+j*50, 215);
		lv_label_set_align(labelParticlesNumber[j], LV_LABEL_ALIGN_CENTER);
		lv_obj_set_auto_realign(labelParticlesNumber[j], true);
		lv_label_set_text(labelParticlesNumber[j], "-");
		lv_obj_add_style(labelParticlesNumber[j], LV_LABEL_PART_MAIN, &font12Style);
	}
	mainLine = lv_line_create(main_scr, NULL);
	lv_line_set_points(mainLine, mainLinePoints, 2);
	lv_line_set_auto_size(mainLine, true);
	lv_obj_add_style(mainLine, LV_LINE_PART_MAIN, &lineStyle);
}

void fetchLastRecord(lv_task_t *task)
{
	if(WiFi.status() == WL_CONNECTED)
	{
		HTTPClient http;
		http.begin(fetchLastRecordURL.c_str());

		int responseCode = http.GET();

		if(responseCode == 200)
		{
			Serial.println("HTTP RESPONSE CODE: " + (String)responseCode);
			StaticJsonDocument<50> doc;
			DynamicJsonDocument samplesToSend(2056);
			DeserializationError err = deserializeJson(doc, http.getString());
			Serial.println(err.c_str());
			JsonObject sample = doc.getElement(0);
			int id = sample["id"];
			Serial.println(id);
		}
		else
		{
			Serial.println("ERROR FETCHING DATA CODE: " + (String)responseCode);
		}
	}
}

void config_time(lv_task_t *task)
{
	if (WiFi.status() == WL_CONNECTED)
	{
		if(Ping.ping(remote_ip, 1)) {
    		for (int i = 0; i < 500; i++)
				dateTimeClient.update();
			configTime(Rtc, dateTimeClient);
			wasUpdated=true;
  		} else {
    		wasUpdated=false;
 		}
		
	}else
	{
		wasUpdated=false;
	}
}

//Get single sample and set text
void getSampleFunc(lv_task_t *task)
{
	Serial.println("Get Sample");
	sht30.get();
	temp = sht30.cTemp;
	humi = sht30.humidity;
	Serial.println(temp);
	Serial.println(humi);
	char buffer[7];
	if(wasUpdated != true)
	{
		lv_task_ready(syn_rtc);
		wasUpdated = true;
	} 
	if (pmsSensor->readData())
	{
		std::map<std::string, uint16_t> tmpData = pmsSensor->returnData();
		pmsSensor->dumpSamples();

		itoa(tmpData["pm10_standard"], buffer, 10);
		lv_label_set_text(labelPM10Data, buffer);

		itoa(tmpData["pm25_standard"], buffer, 10);
		pm25Aqi = tmpData["pm25_standard"];
		lv_label_set_text(labelPM25Data, buffer);
		setAqiStateNColor();

		itoa(tmpData["pm100_standard"], buffer, 10);
		lv_label_set_text(labelPM100Data, buffer);

		itoa(tmpData["particles_05um"], buffer, 10);
		lv_label_set_text(labelParticlesNumber[0], buffer);

		itoa(tmpData["particles_10um"], buffer, 10);
		lv_label_set_text(labelParticlesNumber[1], buffer);

		itoa(tmpData["particles_25um"], buffer, 10);
		lv_label_set_text(labelParticlesNumber[2], buffer);

		itoa(tmpData["particles_50um"], buffer, 10);
		lv_label_set_text(labelParticlesNumber[3], buffer);

		itoa(tmpData["particles_100um"], buffer, 10);
		lv_label_set_text(labelParticlesNumber[4], buffer);

		mySDCard.save(tmpData, temp, humi, getMainTimestamp(Rtc), &sampleDB, &Serial);
	}
	itoa(temp, buffer, 10);
	lv_label_set_text(labelTempValue, strcat(buffer, "°C"));
	itoa(humi, buffer, 10);
	lv_label_set_text(labelHumiValue, strcat(buffer, "%"));
	lv_task_reset(turnFanOn);
	lv_task_set_prio(turnFanOn, LV_TASK_PRIO_HIGHEST);
	digitalWrite(33, LOW);
}

void dateTimeStatusFunc(lv_task_t *task)
{
	if (Rtc.GetMemory(1) == 1)
	{
		lv_label_set_text(dateAndTimeAtBar, getMainTimestamp(Rtc).c_str());
		lv_label_set_text(labelTimeLock, getTime(Rtc).c_str());
		lv_label_set_text(labelDateLock, getDate(Rtc).c_str());
	}

	if (WiFi.status() == WL_CONNECTED)
	{
		lv_label_set_text(wifiStatusAtLockWarning, "");
		lv_label_set_text(wifiStatusAtMainWarning, "");
	}
	else
	{
		lv_label_set_text(wifiStatusAtLockWarning, LV_SYMBOL_CLOSE);
		lv_label_set_text(wifiStatusAtMainWarning, LV_SYMBOL_CLOSE);
	}

	if (mySDCard.start(&sampleDB, &Serial2))
	{
		lv_label_set_text(sdStatusAtMainWarning, "");
		lv_label_set_text(sdStatusAtLockWarning, "");
	}
	else
	{
		lv_label_set_text(sdStatusAtMainWarning, LV_SYMBOL_CLOSE);
		lv_label_set_text(sdStatusAtLockWarning, LV_SYMBOL_CLOSE);
	}
}

void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
	uint16_t c;

	tft.startWrite();																			 /* Start new TFT transaction */
	tft.setAddrWindow(area->x1, area->y1, (area->x2 - area->x1 + 1), (area->y2 - area->y1 + 1)); /* set the working window */
	for (int y = area->y1; y <= area->y2; y++)
	{
		for (int x = area->x1; x <= area->x2; x++)
		{
			c = color_p->full;
			tft.writeColor(c, 1);
			color_p++;
		}
	}
	tft.endWrite();			   /* terminate TFT transaction */
	lv_disp_flush_ready(disp); /* tell lvgl that flushing is done */
}

bool my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
	uint16_t touchX, touchY;

	bool touched = tft.getTouch(&touchX, &touchY, 600);

	if (!touched)
	{
		return false;
	}

	if (touchX > SCREEN_WIDTH || touchY > SCREEN_HEIGHT)
	{
		//Serial.println("Y or y outside of expected parameters..");
		//Serial.print("y:");
		//Serial.print(touchX);
		//Serial.print(" x:");
		//Serial.print(touchY);
	}
	else
	{

		data->state = touched ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL;

		/*Save the state and save the pressed coordinate*/
		//if(data->state == LV_INDEV_STATE_PR) touchpad_get_xy(&last_x, &last_y);

		/*Set the coordinates (if released use the last pressed coordinates)*/
		data->point.x = touchX;
		data->point.y = touchY;

		//Serial.print("Data x");
		//Serial.println(touchX);
		//
		//Serial.print("Data y");
		//Serial.println(touchY);
	}

	return false; /*Return `false` because we are not buffering and no more data to read*/
}

static void ta_event_cb(lv_obj_t *ta, lv_event_t event)
{
	if (event == LV_EVENT_CLICKED)
	{
		if (keyboard == NULL)
		{
			keyboard = lv_keyboard_create(lv_scr_act(), NULL);
			lv_obj_set_size(keyboard, LV_HOR_RES, LV_VER_RES / 2);
			lv_obj_set_event_cb(keyboard, lv_keyboard_def_event_cb);
			lv_keyboard_set_textarea(keyboard, ta);
		}
		else
		{
			keyboard = lv_keyboard_create(lv_scr_act(), NULL);
			lv_obj_set_size(keyboard, LV_HOR_RES, LV_VER_RES / 2);
			lv_obj_set_event_cb(keyboard, lv_keyboard_def_event_cb);
			lv_keyboard_set_textarea(keyboard, ta);
		}
	}
}

static void btn_connect(lv_obj_t *obj, lv_event_t event)
{
	if (event == LV_EVENT_RELEASED and ((lv_textarea_get_text(ssid_ta) != NULL and lv_textarea_get_text(ssid_ta) != '\0') or (lv_textarea_get_text(pwd_ta) != NULL and lv_textarea_get_text(pwd_ta) != '\0')))
	{
		uint8_t wifiAttempts = 10;

		ssid = lv_textarea_get_text(ssid_ta);
		Serial.println(ssid);
		password = lv_textarea_get_text(pwd_ta);

		saveWiFiToRtcMemory(Rtc, ssid, password);
		WiFi.begin(ssid.c_str(), password.c_str());
		while (WiFi.status() != WL_CONNECTED and wifiAttempts > 0)
		{
			Serial.print(".");
			delay(500);
			wifiAttempts--;
		}

		if (WiFi.status() == WL_CONNECTED)
		{
			dateTimeClient.begin();
			for (int i = 0; i < 3; i++)
				dateTimeClient.update();
			lv_task_ready(syn_rtc);
		}
		lv_disp_load_scr(main_scr);
		lv_textarea_set_text(ssid_ta, "");
		lv_textarea_set_text(pwd_ta, "");
	}
}

//Settings button clicked
static void setButton_task(lv_obj_t *obj, lv_event_t event)
{
	lv_disp_load_scr(settings_scr);
	String result = getCharArrrayFromRTC(Rtc, 3);
	delay(20);
}

//Locking button clicked
static void lockButton_task(lv_obj_t *obj, lv_event_t event)
{
	lv_disp_load_scr(lock_scr);
}

//Unlocking button clicked
static void unlockButton_task(lv_obj_t *obj, lv_event_t event)
{
	lv_disp_load_scr(main_scr);
	delay(20);
}

//Exit from wifi settings button clicked
static void btn_cancel(lv_obj_t *obj, lv_event_t event)
{
	lv_disp_load_scr(main_scr);
	lv_textarea_set_text(ssid_ta, "");
	lv_textarea_set_text(pwd_ta, "");
	delay(20);
}

static void btn_settings_back(lv_obj_t *obj, lv_event_t event)
{
	lv_disp_load_scr(main_scr);
	delay(20);
}

static void WiFi_btn(lv_obj_t *obj, lv_event_t event){
	lv_scr_load(wifi_scr);
	delay(20);
}

static void Lcd_save_btn(lv_obj_t *obj, lv_event_t event){
	switch(lv_dropdown_get_selected(lockScreenDDlist))
	{
		case 0: 
			lcd_lock_time = 30000;
			break;
		case 1: 
			lcd_lock_time = 60000;
			break;
		case 2:
			lcd_lock_time = 120000;
			break;
		case 3:
			lcd_lock_time = 300000;
			break;
		case 4:
			lcd_lock_time = 600000;
			break;
		case 5:
			lcd_lock_time = 1800000;
			break;
		case 6:
			lcd_lock_time = 3600000;
			break;
	}
	if(mySDCard.begin())
	{
		File settings = SD.open("/settings.csv", FILE_WRITE);
		String stn = "";
		stn+=(String)measure_period;
		stn+="%";
		stn+=(String)lcd_lock_time;
		settings.print(stn);
		settings.close();
	}
	lv_disp_load_scr(main_scr);
	delay(20);
}

static void Lcd_btn(lv_obj_t *obj, lv_event_t event){
	lv_scr_load(lcd_scr);
	delay(20);
}

static void time_settings_btn(lv_obj_t *obj, lv_event_t event){
	lv_scr_load(time_settings_scr);
	delay(20);	
}

//Function that turns fan on
void turnFanOnFunc(lv_task_t *task)
{
	digitalWrite(33, HIGH);
	lv_task_set_prio(turnFanOn, LV_TASK_PRIO_OFF);
}

void timesettings_screen()
{	
	contBarTime = lv_cont_create(time_settings_scr, NULL);
	lv_obj_set_auto_realign(contBarTime, true);					/*Auto realign when the size changes*/
	lv_obj_align(contBarTime, NULL, LV_ALIGN_IN_TOP_MID, 0, 0); /*This parametrs will be sued when realigned*/
	lv_cont_set_fit4(contBarTime, LV_FIT_PARENT, LV_FIT_PARENT, LV_FIT_NONE, LV_FIT_NONE);
	lv_cont_set_layout(contBarTime, LV_LAYOUT_PRETTY_TOP);
	lv_obj_add_style(contBarTime, LV_OBJ_PART_MAIN, &containerStyle);
	lv_obj_set_style_local_border_opa(contBarTime, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
	lv_obj_set_click(contBarTime, false);

	back_time_settings_btn = lv_btn_create(contBarTime, NULL);
	back_time_settings_label = lv_label_create(back_time_settings_btn, NULL);
	lv_label_set_text(back_time_settings_label, LV_SYMBOL_LEFT);
	lv_obj_set_size(back_time_settings_btn, 30, 15);
	lv_obj_set_event_cb(back_time_settings_btn, btn_settings_back);
	lv_obj_add_style(back_time_settings_btn, LV_OBJ_PART_MAIN, &transparentButtonStyle);

	timeSettingsLabelAtBar = lv_label_create (contBarTime, NULL);
	lv_label_set_text(timeSettingsLabelAtBar, "Work in progress");
}

void settings_screen()
{
	contBarSettings = lv_cont_create(settings_scr, NULL);
	lv_obj_set_auto_realign(contBarSettings, true);					/*Auto realign when the size changes*/
	lv_obj_align(contBarSettings, NULL, LV_ALIGN_IN_TOP_MID, 0, -5); /*This parametrs will be sued when realigned*/
	lv_cont_set_fit4(contBarSettings, LV_FIT_PARENT, LV_FIT_PARENT, LV_FIT_NONE, LV_FIT_NONE);
	lv_cont_set_layout(contBarSettings, LV_LAYOUT_PRETTY_TOP);
	lv_obj_add_style(contBarSettings, LV_OBJ_PART_MAIN, &containerStyle);
	lv_obj_set_style_local_border_opa(contBarSettings, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
	lv_obj_set_click(contBarSettings, false);

	back_settings_btn = lv_btn_create(contBarSettings, NULL);
	back_settings_label = lv_label_create(back_settings_btn, NULL);
	lv_label_set_text(back_settings_label, LV_SYMBOL_LEFT);
	lv_obj_set_size(back_settings_btn, 30, 15);
	lv_obj_set_event_cb(back_settings_btn, btn_settings_back);
	lv_obj_add_style(back_settings_btn, LV_OBJ_PART_MAIN, &transparentButtonStyle);

	settingsLabelAtBar = lv_label_create (contBarSettings, NULL);
	lv_label_set_text(settingsLabelAtBar, "Settings");

	WiFiBtn = lv_imgbtn_create(settings_scr, NULL);
	lv_imgbtn_set_src(WiFiBtn, LV_BTN_STATE_RELEASED, &wifi);
    lv_imgbtn_set_src(WiFiBtn, LV_BTN_STATE_PRESSED, &wifi);
    lv_imgbtn_set_src(WiFiBtn, LV_BTN_STATE_CHECKED_RELEASED, &wifi);
    lv_imgbtn_set_src(WiFiBtn, LV_BTN_STATE_CHECKED_PRESSED, &wifi);
    lv_imgbtn_set_checkable(WiFiBtn, true);
    lv_obj_set_pos(WiFiBtn, 10, 55);
	lv_obj_set_event_cb(WiFiBtn, WiFi_btn);

	LcdBtn = lv_imgbtn_create(settings_scr, NULL);
	lv_imgbtn_set_src(LcdBtn, LV_BTN_STATE_RELEASED, &lcd);
    lv_imgbtn_set_src(LcdBtn, LV_BTN_STATE_PRESSED, &lcd);
    lv_imgbtn_set_src(LcdBtn, LV_BTN_STATE_CHECKED_RELEASED, &lcd);
    lv_imgbtn_set_src(LcdBtn, LV_BTN_STATE_CHECKED_PRESSED, &lcd);
    lv_imgbtn_set_checkable(LcdBtn, true);
    lv_obj_set_pos(LcdBtn, 110, 55);
	lv_obj_set_event_cb(LcdBtn, Lcd_btn);

	placeholder2Btn = lv_imgbtn_create(settings_scr, NULL);
	lv_imgbtn_set_src(placeholder2Btn, LV_BTN_STATE_RELEASED, &set_time);
    lv_imgbtn_set_src(placeholder2Btn, LV_BTN_STATE_PRESSED, &set_time);
    lv_imgbtn_set_src(placeholder2Btn, LV_BTN_STATE_CHECKED_RELEASED, &set_time);
    lv_imgbtn_set_src(placeholder2Btn, LV_BTN_STATE_CHECKED_PRESSED, &set_time);
    lv_imgbtn_set_checkable(placeholder2Btn, true);
    lv_obj_set_pos(placeholder2Btn, 210, 55);
	lv_obj_set_event_cb(placeholder2Btn, time_settings_btn);	
}

void lcd_screen()
{	
	contBarlcd = lv_cont_create(lcd_scr, NULL);
	lv_obj_set_auto_realign(contBarlcd, true);					/*Auto realign when the size changes*/
	lv_obj_align(contBarlcd, NULL, LV_ALIGN_IN_TOP_MID, 0, 0); /*This parametrs will be sued when realigned*/
	lv_cont_set_fit4(contBarlcd, LV_FIT_PARENT, LV_FIT_PARENT, LV_FIT_NONE, LV_FIT_NONE);
	lv_cont_set_layout(contBarlcd, LV_LAYOUT_PRETTY_TOP);
	lv_obj_add_style(contBarlcd, LV_OBJ_PART_MAIN, &containerStyle);
	lv_obj_set_style_local_border_opa(contBarlcd, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
	lv_obj_set_click(contBarlcd, false);

	back_lcd_btn = lv_btn_create(contBarlcd, NULL);
	back_lcd_label = lv_label_create(back_lcd_btn, NULL);
	lv_label_set_text(back_lcd_label, LV_SYMBOL_LEFT);
	lv_obj_set_size(back_lcd_btn, 30, 15);
	lv_obj_set_event_cb(back_lcd_btn, btn_settings_back);
	lv_obj_add_style(back_lcd_btn, LV_OBJ_PART_MAIN, &transparentButtonStyle);

	lcdLabelAtBar = lv_label_create (contBarlcd, NULL);
	lv_label_set_text(lcdLabelAtBar, "LCD Settings");

	lockScreenLabel = lv_label_create(lcd_scr, NULL);
	lv_obj_set_pos(lockScreenLabel, 5, 58);
	lv_label_set_text(lockScreenLabel, "Lock screen after: ");
	lv_obj_set_style_local_text_color(lockScreenLabel, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lockScreenDDlist = lv_dropdown_create(lcd_scr, NULL);
	lv_dropdown_set_options(lockScreenDDlist, "30 sec\n"
	"1 min\n"
	"2 min\n"
	"5 min\n"
	"10 min\n"
	"30 min\n"
	"60 min\n" );
	lv_obj_set_width(lockScreenDDlist, LV_HOR_RES/2-20);
	lv_obj_set_pos(lockScreenDDlist, 180, 53);

	lcd_save_btn = lv_btn_create(lcd_scr, NULL);
	lcd_save_label = lv_label_create(lcd_save_btn, NULL);
	lv_label_set_text(lcd_save_label, "Save");
	lv_obj_set_size(lcd_save_btn, 50, 15);
	lv_obj_set_pos(lcd_save_btn, 250, 200);
	lv_obj_add_style(lcd_save_btn, LV_OBJ_PART_MAIN, &transparentButtonStyle);
	lv_obj_set_event_cb(lcd_save_btn, Lcd_save_btn);
	//TODO przycisk styl bo go nie widac
}

void main_screen()
{
	contBar = lv_cont_create(main_scr, NULL);
	lv_obj_set_auto_realign(contBar, true);					/*Auto realign when the size changes*/
	lv_obj_align(contBar, NULL, LV_ALIGN_IN_TOP_MID, 0, -5); /*This parametrs will be sued when realigned*/
	lv_cont_set_fit4(contBar, LV_FIT_PARENT, LV_FIT_PARENT, LV_FIT_NONE, LV_FIT_NONE);
	lv_obj_add_style(contBar, LV_OBJ_PART_MAIN, &containerStyle);
	lv_obj_set_style_local_border_opa(contBar, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
	lv_obj_set_click(contBar, false);

	lockButton = lv_btn_create(main_scr, NULL);
	labelLockButton = lv_label_create(lockButton, NULL);
	lv_obj_align(lockButton, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, 40, -55);
	lv_label_set_text(labelLockButton, LV_SYMBOL_POWER);
	lv_btn_set_fit(lockButton, LV_FIT_TIGHT);
	lv_obj_set_event_cb(lockButton, lockButton_task);
	lv_obj_add_style(lockButton, LV_OBJ_PART_MAIN, &transparentButtonStyle);

	setButton = lv_btn_create(main_scr, NULL);
	labelSetButton = lv_label_create(setButton, NULL);
	lv_obj_align(setButton, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, 45, -8);
	lv_label_set_text(labelSetButton, LV_SYMBOL_SETTINGS);
	lv_btn_set_fit(setButton, LV_FIT_TIGHT);
	lv_obj_set_event_cb(setButton, setButton_task);
	lv_obj_add_style(setButton, LV_OBJ_PART_MAIN, &transparentButtonStyle);
	
	wifiStatusAtMain = lv_label_create(contBar, NULL);
	lv_label_set_text(wifiStatusAtMain, LV_SYMBOL_WIFI);
	lv_obj_align(wifiStatusAtMain, NULL, LV_ALIGN_IN_LEFT_MID, 10, 0);

	sdStatusAtMain = lv_label_create(contBar, NULL);
	lv_label_set_text(sdStatusAtMain, LV_SYMBOL_SD_CARD);
	lv_obj_align(sdStatusAtMain, NULL, LV_ALIGN_IN_LEFT_MID, 45, 0);

	wifiStatusAtMainWarning = lv_label_create(wifiStatusAtMain, NULL);
	lv_label_set_text(wifiStatusAtMainWarning, "");
	lv_obj_add_style(wifiStatusAtMainWarning, LV_OBJ_PART_MAIN, &tinySymbolStyle);
	lv_obj_set_pos(wifiStatusAtMainWarning, 5, 5);

	sdStatusAtMainWarning = lv_label_create(sdStatusAtMain, NULL);
	lv_label_set_text(sdStatusAtMainWarning, "");
	lv_obj_add_style(sdStatusAtMainWarning, LV_OBJ_PART_MAIN, &tinySymbolStyle);
	lv_obj_set_pos(sdStatusAtMainWarning, 2, 5);

	dateAndTimeAtBar = lv_label_create(contBar, NULL);
	lv_label_set_text(dateAndTimeAtBar, "Hello!");
	lv_obj_align(dateAndTimeAtBar, NULL, LV_ALIGN_IN_RIGHT_MID, -120, 0);

	contTemp = lv_cont_create(main_scr, NULL);
	lv_obj_set_click(contTemp, false);
	lv_obj_add_style(contTemp, LV_OBJ_PART_MAIN, &containerStyle);
	lv_obj_set_pos(contTemp, 188, 30);
	lv_obj_set_size(contTemp, 122, 46);

	contHumi = lv_cont_create(main_scr, NULL);
	lv_obj_set_click(contHumi, false);
	lv_obj_add_style(contHumi, LV_OBJ_PART_MAIN, &containerStyle);
	lv_obj_set_pos(contHumi, 188, 74);
	lv_obj_set_size(contHumi, 122, 46);

	contPM10 = lv_cont_create(main_scr, NULL);
	lv_obj_set_click(contPM10, false);
	lv_obj_add_style(contPM10, LV_OBJ_PART_MAIN, &containerStyle);
	lv_obj_add_style(contPM10, LV_OBJ_PART_MAIN, &font12Style);
	lv_obj_set_pos(contPM10, 10, 118);
	lv_obj_set_size(contPM10, 91, 62);

	contPM25 = lv_cont_create(main_scr, NULL);
	lv_obj_set_click(contPM25, false);
	lv_obj_add_style(contPM25, LV_OBJ_PART_MAIN, &containerStyle);
	lv_obj_set_pos(contPM25, 10, 30);
	lv_obj_set_size(contPM25, 180, 90);

	contPM100 = lv_cont_create(main_scr, NULL);
	lv_obj_set_click(contPM100, false);
	lv_obj_add_style(contPM100, LV_OBJ_PART_MAIN, &containerStyle);
	lv_obj_add_style(contPM100, LV_OBJ_PART_MAIN, &font12Style);
	lv_obj_set_pos(contPM100, 99, 118);
	lv_obj_set_size(contPM100, 91, 62);

	contAQI = lv_cont_create(main_scr, NULL);
	lv_obj_set_click(contAQI, false);
	lv_obj_add_style(contAQI, LV_OBJ_PART_MAIN, &containerStyle);
	lv_obj_add_style(contAQI, LV_OBJ_PART_MAIN, &font12Style);
	lv_obj_set_pos(contAQI,   188, 118);
	lv_obj_set_size(contAQI,  122, 62);

	contAQIColorBar = lv_cont_create(contAQI, NULL);
	lv_obj_set_click(contAQIColorBar, false);
	lv_obj_add_style(contAQIColorBar, LV_OBJ_PART_MAIN, &containerStyle);
	lv_obj_add_style(contAQIColorBar, LV_OBJ_PART_MAIN, &font12Style);
	lv_obj_set_style_local_bg_opa(contAQIColorBar, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_70);
	lv_obj_set_style_local_bg_color(contAQIColorBar, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_GREEN);
	lv_obj_set_pos(contAQIColorBar,   15, 25);
	lv_obj_set_size(contAQIColorBar,  92, 24);

	labelTemp = lv_label_create(contTemp, NULL);
	lv_obj_set_pos(labelTemp, 5, 5);
	lv_label_set_text(labelTemp, "Temp");

	labelHumi = lv_label_create(contHumi, NULL);
	lv_obj_set_pos(labelHumi, 5, 5);
	lv_label_set_text(labelHumi, "RH");

	labelPM10 = lv_label_create(contPM10, NULL);
	lv_obj_set_pos(labelPM10, 5, 5);
	lv_label_set_text(labelPM10, "PM 1.0 ug/m3");

	labelPM25 = lv_label_create(contPM25, NULL);
	lv_obj_set_pos(labelPM25, 5, 5);
	lv_label_set_text(labelPM25, "PM 2.5               ug/m3");

	labelPM100 = lv_label_create(contPM100, NULL);
	lv_obj_set_pos(labelPM100, 5, 5);
	lv_label_set_text(labelPM100, "PM 10 ug/m3");

	labelTempValue = lv_label_create(contTemp, NULL);
	lv_obj_add_style(labelTempValue, LV_OBJ_PART_MAIN, &font20Style);
	lv_obj_set_pos(labelTempValue, 70, 20);
	lv_label_set_text(labelTempValue, "-");

	labelHumiValue = lv_label_create(contHumi, NULL);
	lv_obj_add_style(labelHumiValue, LV_OBJ_PART_MAIN, &font20Style);
	lv_obj_set_pos(labelHumiValue, 70, 20);
	lv_label_set_text(labelHumiValue, "-");

	labelPM10Data = lv_label_create(contPM10, NULL);
	lv_obj_add_style(labelPM10Data, LV_OBJ_PART_MAIN, &font22Style);
	lv_obj_set_auto_realign(labelPM10Data, true);					
	lv_obj_align(labelPM10Data, NULL, LV_ALIGN_CENTER, 0, 5);
	lv_label_set_text(labelPM10Data, "-");

	labelPM25Data = lv_label_create(contPM25, NULL);
	lv_obj_add_style(labelPM25Data, LV_OBJ_PART_MAIN, &font22Style);
	lv_obj_set_auto_realign(labelPM25Data, true);					
	lv_obj_align(labelPM25Data, NULL, LV_ALIGN_CENTER, 0, 10);
	lv_label_set_text(labelPM25Data, "-");

	labelPM100Data = lv_label_create(contPM100, NULL);
	lv_obj_add_style(labelPM100Data, LV_OBJ_PART_MAIN, &font22Style);
	lv_obj_set_auto_realign(labelPM100Data, true);					
	lv_obj_align(labelPM100Data, NULL, LV_ALIGN_CENTER, 0, 5);
	lv_label_set_text(labelPM100Data, "-");
	
	labelAQI = lv_label_create(contAQI, NULL);
	lv_obj_set_pos(labelAQI, 5, 5);
	lv_label_set_text(labelAQI, "Air Quality  PM 2.5");

	labelAQIColorBar = lv_label_create(contAQIColorBar, NULL);
	lv_obj_set_auto_realign(labelAQIColorBar, true);					
	lv_obj_align(labelAQIColorBar, NULL, LV_ALIGN_CENTER, 0, 0);
	lv_label_set_text(labelAQIColorBar, "-"); 

	//Function that draws lines and set text above those
	drawSomeLines();
}

void wifi_screen()
{
	contBarWiFi = lv_cont_create(wifi_scr, NULL);
	lv_obj_set_auto_realign(contBarWiFi, true);					
	lv_obj_align(contBarWiFi, NULL, LV_ALIGN_IN_TOP_MID, 0, 0); 
	lv_cont_set_fit4(contBarWiFi, LV_FIT_PARENT, LV_FIT_PARENT, LV_FIT_NONE, LV_FIT_NONE);
	lv_cont_set_layout(contBarWiFi, LV_LAYOUT_PRETTY_TOP);
	lv_obj_add_style(contBarWiFi, LV_OBJ_PART_MAIN, &containerStyle);
	lv_obj_set_style_local_border_opa(contBarWiFi, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
	lv_obj_set_click(contBarWiFi, false);

	cancel_btn = lv_btn_create(contBarWiFi, NULL);
	cancel_label = lv_label_create(cancel_btn, NULL);
	lv_label_set_text(cancel_label, LV_SYMBOL_LEFT);
	lv_obj_set_size(cancel_btn, 30, 15);
	lv_obj_set_event_cb(cancel_btn, btn_cancel);
	lv_obj_add_style(cancel_btn, LV_OBJ_PART_MAIN, &transparentButtonStyle);

	wifiLabelAtBar = lv_label_create(contBarWiFi, NULL);
	lv_label_set_text(wifiLabelAtBar, "WiFi settings");

	ssid_label = lv_label_create(wifi_scr, NULL);
	lv_label_set_text(ssid_label, "SSID: ");
	lv_obj_set_pos(ssid_label, 5, 53);
	lv_obj_set_style_local_text_color(ssid_label, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

	ssid_ta = lv_textarea_create(wifi_scr, NULL);
	lv_textarea_set_text(ssid_ta, "");
	lv_textarea_set_pwd_mode(ssid_ta, false);
	lv_textarea_set_one_line(ssid_ta, true);
	lv_obj_set_event_cb(ssid_ta, ta_event_cb);
	lv_textarea_set_cursor_hidden(ssid_ta, true);
	lv_obj_set_width(ssid_ta, LV_HOR_RES / 2 - 20);
	lv_obj_set_pos(ssid_ta, 100, 45);

	pwd_label = lv_label_create(wifi_scr, NULL);
	lv_label_set_text(pwd_label, "Password: ");
	lv_obj_set_pos(pwd_label, 5, 92);
	lv_obj_set_style_local_text_color(pwd_label, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
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
	lv_obj_set_style_local_text_color(apply_label, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
	lv_obj_set_event_cb(apply_btn, btn_connect);
	lv_obj_set_width(apply_btn, 75);
	lv_obj_set_pos(apply_btn, 243, 43);
}

void lock_screen()
{
	contDateTimeAtLock = lv_cont_create(lock_scr, NULL);
	lv_obj_set_auto_realign(contDateTimeAtLock, true);			   
	lv_obj_align(contDateTimeAtLock, NULL, LV_ALIGN_CENTER, 0, -40);
	//lv_cont_set_fit4(contDateTimeLock,   LV_FIT_PARENT, LV_FIT_PARENT, LV_FIT_NONE, LV_FIT_NONE);
	lv_cont_set_fit(contDateTimeAtLock, LV_FIT_TIGHT);
	lv_cont_set_layout(contDateTimeAtLock, LV_LAYOUT_PRETTY_MID);
	lv_obj_add_style(contDateTimeAtLock, LV_OBJ_PART_MAIN, &containerStyle);
	lv_obj_set_style_local_border_opa(contDateTimeAtLock, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_0);

	lockButton1 = lv_btn_create(lock_scr, NULL);
	labelLockButton1 = lv_label_create(lockButton1, NULL);
	lv_obj_align(lockButton1, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);
	lv_label_set_text(labelLockButton1, LV_SYMBOL_POWER);
	lv_btn_set_fit(lockButton1, LV_FIT_TIGHT);
	lv_obj_set_event_cb(lockButton1, unlockButton_task);
	lv_obj_add_style(lockButton1, LV_OBJ_PART_MAIN, &transparentButtonStyle);

	labelTimeLock = lv_label_create(contDateTimeAtLock, NULL);
	lv_label_set_text(labelTimeLock, "Connect\nto wifi");
	lv_obj_align(labelTimeLock, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
	lv_obj_set_size(labelTimeLock, 300, 200);

	labelDateLock = lv_label_create(contDateTimeAtLock, NULL);
	lv_label_set_text(labelDateLock, "");
	lv_obj_align(labelTimeLock, NULL, LV_ALIGN_CENTER, 0, 0);

	wifiStatusAtLock = lv_label_create(contDateTimeAtLock, NULL);
	lv_obj_align(wifiStatusAtLock, NULL, LV_ALIGN_IN_BOTTOM_MID, 10, 0);
	lv_label_set_text(wifiStatusAtLock, LV_SYMBOL_WIFI);

	sdStatusAtLock = lv_label_create(contDateTimeAtLock, NULL);
	lv_obj_align(sdStatusAtLock, NULL, LV_ALIGN_IN_BOTTOM_MID, -10, 0);
	lv_label_set_text(sdStatusAtLock, LV_SYMBOL_SD_CARD);

	wifiStatusAtLockWarning = lv_label_create(wifiStatusAtLock, NULL);
	lv_label_set_text(wifiStatusAtLockWarning, "");
	lv_obj_add_style(wifiStatusAtLockWarning, LV_OBJ_PART_MAIN, &tinySymbolStyle);
	lv_obj_set_pos(wifiStatusAtLockWarning, 5, 5);

	sdStatusAtLockWarning = lv_label_create(sdStatusAtLock, NULL);
	lv_label_set_text(sdStatusAtLockWarning, "");
	lv_obj_add_style(sdStatusAtLockWarning, LV_OBJ_PART_MAIN, &tinySymbolStyle);
	lv_obj_set_pos(sdStatusAtLockWarning, 2, 5);
}

void load_settings()
{
	File settings;
	String stn="";
	if(mySDCard.begin())
	{
		if(SD.exists("/settings.csv"))
		{
			settings = SD.open("/settings.csv", FILE_READ);
			Serial.println("som");
			while(settings.available())
			{
				stn += (char)settings.read();
			}
			measure_period=stn.substring(0,stn.indexOf("%")).toInt();
			lcd_lock_time=stn.substring(stn.indexOf("%")+1).toInt();
			switch(lcd_lock_time)
			{
				case 30000: 
					lv_dropdown_set_selected(lockScreenDDlist, 0);
					break;
				case 60000: 
					lv_dropdown_set_selected(lockScreenDDlist, 1);
					break;
				case 120000:
					lv_dropdown_set_selected(lockScreenDDlist, 2);
					break;
				case 300000:
					lv_dropdown_set_selected(lockScreenDDlist, 3);
					break;
				case 600000:
					lv_dropdown_set_selected(lockScreenDDlist, 4);
					break;
				case 1800000:
					lv_dropdown_set_selected(lockScreenDDlist, 5);
					break;
				case 3600000:
					lv_dropdown_set_selected(lockScreenDDlist, 6);
					break;
			}
		}else
		{
			settings = SD.open("/settings.csv", FILE_WRITE);
			settings.print("3600000%60000");
			settings.close();
		}
		
	}
}

void setup()
{
	pinMode(33, OUTPUT);
	digitalWrite(33, LOW);
	sqlite3_initialize();
	//Serial debug
	Serial.begin(115200); 
	Serial2.begin(9600, SERIAL_8N1, 16, 17);
	//PMS sensor initialization
	pmsSensor = new PMS5003(&Serial2, &Serial);
	
	lv_init();
	tft.begin(); /* TFT init */
	tft.setRotation(3); 

	uint16_t calData[5] = {275, 3620, 264, 3532, 1};
	tft.setTouch(calData);

	lv_disp_buf_init(&disp_buf, buf, NULL, LV_HOR_RES_MAX * 10);
	//Initialize the display
	lv_disp_drv_t disp_drv;
	lv_disp_drv_init(&disp_drv);
	disp_drv.hor_res = SCREEN_WIDTH;
	disp_drv.ver_res = SCREEN_HEIGHT;
	disp_drv.flush_cb = my_disp_flush;
	disp_drv.buffer = &disp_buf;
	lv_disp_drv_register(&disp_drv);

	//Initialize the input device driver
	lv_indev_drv_t indev_drv;
	lv_indev_drv_init(&indev_drv);			/*Descriptor of a input device driver*/
	indev_drv.type = LV_INDEV_TYPE_POINTER; /*Touch pad is a pointer-like device*/
	indev_drv.read_cb = my_touchpad_read;	/*Set your driver function*/
	lv_indev_drv_register(&indev_drv);		/*Finally register the driver*/

	//Set theme
	lv_theme_t *th = lv_theme_material_init(LV_THEME_DEFAULT_COLOR_PRIMARY, LV_THEME_DEFAULT_COLOR_SECONDARY, LV_THEME_DEFAULT_FLAG, LV_THEME_DEFAULT_FONT_SMALL, LV_THEME_DEFAULT_FONT_NORMAL, LV_THEME_DEFAULT_FONT_SUBTITLE, LV_THEME_DEFAULT_FONT_TITLE);
	lv_theme_set_act(th);
	//Styles initialization functions
	containerStyleInit();
	font12StyleInit();
	font16StyleInit();
	font20StyleInit();
	font22StyleInit();
	transparentButtonStyleInit();
	tinySymbolStyleInit();
	lineStyleInit();
	main_scr = lv_cont_create(NULL, NULL);
	lv_obj_set_style_local_bg_color(main_scr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
	settings_scr=lv_cont_create(NULL, NULL);
	lv_obj_set_style_local_bg_color(settings_scr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
	lcd_scr = lv_cont_create(NULL, NULL);
	lv_obj_set_style_local_bg_color(lcd_scr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
	time_settings_scr = lv_cont_create(NULL, NULL);
	lv_obj_set_style_local_bg_color(time_settings_scr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
	wifi_scr = lv_cont_create(NULL, NULL);
	lv_obj_set_style_local_bg_color(wifi_scr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
	lock_scr = lv_cont_create(NULL, NULL);
	lv_obj_set_style_local_bg_color(lock_scr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
	//Screens initialization function
	main_screen();
	wifi_screen();
	lock_screen();
	settings_screen();
	lcd_screen();
	timesettings_screen();
	lv_disp_load_scr(main_scr);
	load_settings();
	lv_task_t *date = lv_task_create(dateTimeStatusFunc, 900, LV_TASK_PRIO_MID, NULL);
	syn_rtc = lv_task_create_basic();
	lv_task_set_cb(syn_rtc, config_time);
	lv_task_set_period(syn_rtc, 3600000);
	getSample = lv_task_create(getSampleFunc, measure_period, LV_TASK_PRIO_HIGH, NULL);
	turnFanOn = lv_task_create(turnFanOnFunc, measure_period-300000, LV_TASK_PRIO_HIGHEST, NULL);
	inactive_time = lv_task_create(inactive_screen, 1, LV_TASK_PRIO_HIGH, NULL);


	getAppLastRecord = lv_task_create_basic();
	lv_task_set_cb(getAppLastRecord, fetchLastRecord);
	lv_task_set_period(getAppLastRecord, 36000);

	if (Rtc.GetMemory(53) == 1)
	{
		ssid = getCharArrrayFromRTC(Rtc, 3);
		password = getCharArrrayFromRTC(Rtc, 28);
		WiFi.begin(ssid.c_str(), password.c_str());
	}
	delay(500);
	lv_task_ready(syn_rtc);
}

void loop()
{
	lv_task_handler(); /* let the GUI do its work */
	delay(5);
}

