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
const char* remote_host = "www.google.com";
const IPAddress remote_ip(192, 168, 1, 1);
#define LVGL_TICK_PERIOD 60
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define GMT_OFFSET_SEC 7200

RtcDS1307<TwoWire> Rtc(Wire);
static const char ntpServerName[] = "europe.pool.ntp.org";
WiFiUDP ntpUDP;
NTPClient dateTimeClient(ntpUDP, ntpServerName, 7200);
bool wasUpdated = false;

TFT_eSPI tft = TFT_eSPI();
static lv_disp_buf_t disp_buf;
static lv_color_t buf[LV_HOR_RES_MAX * 10];
int measure_period = 300000;

MySD mySDCard(27);
SQLiteDb sampleDB("/sd/database.db", "/database.db", "samples");

bool date_synchronized = false;

String ssid = "";
String password = "";

int temp, humi;
PMS5003 *pmsSensor;
SHT3X sht30(0x45);

String fetchLastRecordURL = "http://192.168.1.130/fetch/last";

//--------------------------------------------------styles

//TODO Style
static lv_style_t tinySymbolStyle;
void tinySymbolStyleInit(void){
	lv_style_init(&tinySymbolStyle);
	lv_style_set_text_font(&tinySymbolStyle, LV_STATE_DEFAULT, &lv_font_montserrat_12);
	lv_style_set_bg_opa(&tinySymbolStyle, LV_STATE_DEFAULT, LV_OPA_0);
	lv_style_set_border_opa(&tinySymbolStyle, LV_STATE_DEFAULT, LV_OPA_0);
	lv_style_set_radius(&tinySymbolStyle, LV_STATE_DEFAULT, 0);
	lv_style_set_text_color(&tinySymbolStyle, LV_STATE_DEFAULT, LV_COLOR_RED);

}

static lv_style_t containerStyle16;
void containerStyle16Init(void){
	lv_style_init(&containerStyle16);
	lv_style_set_text_font(&containerStyle16, LV_STATE_DEFAULT, &lv_font_montserrat_16);
	lv_style_set_bg_opa(&containerStyle16, LV_STATE_DEFAULT, LV_OPA_0);
	lv_style_set_border_color(&containerStyle16, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_radius(&containerStyle16, LV_STATE_DEFAULT, 0);
	lv_style_set_text_color(&containerStyle16, LV_STATE_DEFAULT, LV_COLOR_WHITE);
}

static lv_style_t containerStyle12;
void containerStyle12Init(void){
	lv_style_init(&containerStyle12);
	lv_style_set_text_font(&containerStyle12, LV_STATE_DEFAULT, &lv_font_montserrat_12);
	lv_style_set_bg_opa(&containerStyle12, LV_STATE_DEFAULT, LV_OPA_0);
	lv_style_set_border_color(&containerStyle12, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_radius(&containerStyle12, LV_STATE_DEFAULT, 0);
	lv_style_set_text_color(&containerStyle12, LV_STATE_DEFAULT, LV_COLOR_WHITE);
}

static lv_style_t containerStyle22;
void containerStyle22Init(void){
	lv_style_init(&containerStyle22);
	lv_style_set_text_font(&containerStyle22, LV_STATE_DEFAULT, &lv_font_montserrat_22);
	lv_style_set_bg_opa(&containerStyle22, LV_STATE_DEFAULT, LV_OPA_0);
	lv_style_set_border_color(&containerStyle22, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_radius(&containerStyle22, LV_STATE_DEFAULT, 0);
	lv_style_set_text_color(&containerStyle22, LV_STATE_DEFAULT, LV_COLOR_WHITE);
}

static lv_style_t transparentButtonStyle;
void transparentButtonStyleInit(void){
	lv_style_init(&transparentButtonStyle);
	lv_style_set_bg_opa(&transparentButtonStyle, LV_STATE_DEFAULT, LV_OPA_0);
	lv_style_set_border_opa(&transparentButtonStyle, LV_STATE_DEFAULT, LV_OPA_0);
	lv_style_set_radius(&transparentButtonStyle, LV_STATE_DEFAULT, 0);
	lv_style_set_text_color(&transparentButtonStyle, LV_STATE_DEFAULT, LV_COLOR_WHITE);
}

//--------------------------------------------------main gui

lv_obj_t *main_scr;
lv_obj_t *wifiStatus;
lv_obj_t *sdStatus;
lv_obj_t *wifiStatusWarning;
lv_obj_t *sdStatusWarning;
lv_obj_t *dateAndTimeAtBar;
lv_obj_t *contBar;
lv_obj_t *contTemp;
lv_obj_t *contHumi;
lv_obj_t *contPM10;
lv_obj_t *contPM25;
lv_obj_t *contPM100;
lv_obj_t *contAQI;
lv_obj_t *contTMP;
lv_obj_t *labelLockButton;
lv_obj_t *lockButton;
lv_obj_t *labelSetButton;
lv_obj_t *setButton;
lv_obj_t *labelPMData[3];
lv_obj_t *labelPMParticle[3];
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

//--------------------------------------------------wifi gui
lv_obj_t *contBarWiFi;
lv_obj_t *wifiLabelAtBar;
lv_obj_t *wifiConnect_scr;
lv_obj_t *keyboard;
lv_obj_t *ssid_ta;
lv_obj_t *pwd_ta;
lv_obj_t *ssid_label;
lv_obj_t *pwd_label;
lv_obj_t *apply_btn;
lv_obj_t *apply_label;
lv_obj_t *cancel_btn;
lv_obj_t *cancel_label;
//--------------------------------------------------settings gui
lv_obj_t *settings_scr;
lv_obj_t *contBarSettings;
lv_obj_t *back_settings_btn;
lv_obj_t *back_settings_label;
lv_obj_t *settingsLabelAtBar;
lv_obj_t *settingsMenu;
lv_obj_t *WiFiBtnBar;
lv_obj_t *WiFiBtn;
lv_obj_t *WiFiBtnLabel;

lv_obj_t *placeholder1Bar;
lv_obj_t *placeholder1Btn;
lv_obj_t *placeholder1Label;

lv_obj_t *placeholder2Bar;
lv_obj_t *placeholder2Btn;
lv_obj_t *placeholder2Label;

lv_obj_t *placeholder3Bar;
lv_obj_t *placeholder3Btn;
lv_obj_t *placeholder3Label;
//--------------------------------------------------wifi list gui
lv_obj_t *wifiList_scr;
lv_obj_t *contBarwifiList;
lv_obj_t *wifiListLabelAtBar;
lv_obj_t *back_wifi_btn;
lv_obj_t *back_wifi_label;
lv_obj_t *wifi_menu;

int SSIDnumber;

lv_obj_t** SSIDbar;
lv_obj_t** SSIDBtn;
lv_obj_t** SSIDLabel;
//--------------------------------------------------lockscreen gui
lv_obj_t *lock_scr;
lv_obj_t *contDateTimeLock;
lv_obj_t *labelLockButton1;
lv_obj_t *lockButton1;
lv_obj_t *labelDateLock;
lv_obj_t *labelTimeLock;
lv_obj_t *wifiStatusAtLock;
lv_obj_t *sdStatusAtLock;
//--------------------------------------------------tasks
lv_task_t *turnFanOn;
lv_task_t *getSample;
lv_task_t *syn_rtc;
lv_task_t *getAppLastRecord;
lv_task_t *wifilist;

static void wifi_ssid(lv_obj_t *obj, lv_event_t event)
{
	lv_obj_t * label = lv_obj_get_child(obj, NULL);
	const char * ssid = lv_label_get_text(label);
	Serial.println(ssid);
	lv_textarea_set_text(ssid_ta, ssid);
	lv_disp_load_scr(wifiConnect_scr);
}

void listNetworks(lv_task_t *task) {
  SSIDnumber = WiFi.scanNetworks(false, false, false, 20);
  SSIDbar = new lv_obj_t* [SSIDnumber];
  SSIDBtn = new lv_obj_t* [SSIDnumber];
  SSIDLabel = new lv_obj_t* [SSIDnumber];
  for (int thisNet = 0; thisNet<SSIDnumber; thisNet++) {
	SSIDbar[thisNet] = lv_cont_create(wifi_menu, NULL);
	lv_obj_set_auto_realign(SSIDbar[thisNet], true);
	lv_obj_align(SSIDbar[thisNet], NULL, LV_ALIGN_IN_TOP_MID, 0, thisNet*lv_obj_get_height(SSIDbar[thisNet]));
	lv_cont_set_fit4(SSIDbar[thisNet], LV_FIT_PARENT, LV_FIT_PARENT, LV_FIT_NONE, LV_FIT_NONE);
	SSIDBtn[thisNet] = lv_btn_create(SSIDbar[thisNet], NULL);
	lv_obj_set_size(SSIDBtn[thisNet], lv_obj_get_width(SSIDbar[thisNet]), lv_obj_get_height(SSIDbar[thisNet]));
	SSIDLabel[thisNet] = lv_label_create(SSIDBtn[thisNet], NULL);
	lv_label_set_text(SSIDLabel[thisNet], WiFi.SSID(thisNet).c_str());
	lv_label_set_align(SSIDLabel[thisNet], LV_ALIGN_IN_LEFT_MID);
	lv_obj_set_event_cb(SSIDBtn[thisNet], wifi_ssid);
	lv_obj_add_style(SSIDBtn[thisNet], LV_OBJ_PART_MAIN, &transparentButtonStyle);
	lv_obj_add_style(SSIDbar[thisNet], LV_OBJ_PART_MAIN, &containerStyle16);
  }
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
			if(ta!=ssid_ta){				
				keyboard = lv_keyboard_create(lv_scr_act(), NULL);
				lv_obj_set_size(keyboard, LV_HOR_RES, LV_VER_RES / 2);
				lv_obj_set_event_cb(keyboard, lv_keyboard_def_event_cb);
				lv_keyboard_set_textarea(keyboard, ta);
			}
		}
		else
		{
			if(ta!=ssid_ta){				
				keyboard = lv_keyboard_create(lv_scr_act(), NULL);
				lv_obj_set_size(keyboard, LV_HOR_RES, LV_VER_RES / 2);
				lv_obj_set_event_cb(keyboard, lv_keyboard_def_event_cb);
				lv_keyboard_set_textarea(keyboard, ta);
			}
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

static void setButton_task(lv_obj_t *obj, lv_event_t event)
{
	lv_disp_load_scr(settings_scr);
}

static void lockButton_task(lv_obj_t *obj, lv_event_t event)
{
	lv_disp_load_scr(lock_scr);
}

static void lockButton_task1(lv_obj_t *obj, lv_event_t event)
{
	lv_disp_load_scr(main_scr);
}

static void btn_cancel(lv_obj_t *obj, lv_event_t event)
{
	lv_disp_load_scr(main_scr);
	lv_textarea_set_text(ssid_ta, "");
	lv_textarea_set_text(pwd_ta, "");
}

void getSampleFunc(lv_task_t *task)
{
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
		lv_label_set_text(labelPM25Data, buffer);

		itoa(tmpData["pm100_standard"], buffer, 10);
		lv_label_set_text(labelPM100Data, buffer);
/*
		itoa(tmpData["particles_10um"], buffer, 10);
		lv_label_set_text(labelPMParticle[0], buffer);

		itoa(tmpData["particles_25um"], buffer, 10);
		lv_label_set_text(labelPMParticle[1], buffer);

		itoa(tmpData["particles_100um"], buffer, 10);
		lv_label_set_text(labelPMParticle[2], buffer);
*/		if(wasUpdated !=true)
			mySDCard.save(tmpData, temp, humi, getMainTimestamp(Rtc), &sampleDB, &Serial);
	}
	itoa(temp, buffer, 10);
	lv_label_set_text(labelTempValue, strcat(buffer, "°C"));
	itoa(humi, buffer, 10);
	lv_label_set_text(labelHumiValue, strcat(buffer, "%"));
	lv_task_reset(turnFanOn);
	digitalWrite(33, LOW);
}

void turnFanOnFunc(lv_task_t *task)
{
	digitalWrite(33, HIGH);
}

void date_time(lv_task_t *task)
{
	if (Rtc.GetMemory(1) == 1)
	{
		lv_label_set_text(dateAndTimeAtBar, getMainTimestamp(Rtc).c_str());
		lv_label_set_text(labelTimeLock, getTime(Rtc).c_str());
		lv_label_set_text(labelDateLock, getDate(Rtc).c_str());
	}

	if (WiFi.status() == WL_CONNECTED)
	{
		lv_label_set_text(wifiStatusAtLock, "");
		lv_label_set_text(wifiStatusWarning, "");
	}
	else
	{
		lv_label_set_text(wifiStatusAtLock, "");
		lv_label_set_text(wifiStatusWarning, LV_SYMBOL_CLOSE);
	}

	if (mySDCard.start(&sampleDB, &Serial2))
	{
		lv_label_set_text(sdStatusWarning, "");
		lv_label_set_text(sdStatusAtLock, "");
	}
	else
	{
		lv_label_set_text(sdStatusWarning, LV_SYMBOL_CLOSE);
		lv_label_set_text(sdStatusAtLock, "");
	}
}

static void btn_settings_back(lv_obj_t *obj, lv_event_t event)
{
	lv_disp_load_scr(main_scr);
}

static void wifi_list_btn_back(lv_obj_t *obj, lv_event_t event)
{
	lv_disp_load_scr(settings_scr);
}

static void WiFi_btn(lv_obj_t *obj, lv_event_t event){
	wifilist = lv_task_create(listNetworks, 0, LV_TASK_PRIO_LOWEST, NULL);

	lv_task_set_prio(wifilist, LV_TASK_PRIO_LOWEST);
	lv_task_once(wifilist);
	lv_disp_load_scr(wifiList_scr);
	//lv_disp_load_scr(wifiConnect_scr);
}

void main_screen()
{
	contBar = lv_cont_create(main_scr, NULL);
	lv_obj_set_auto_realign(contBar, true);					/*Auto realign when the size changes*/
	lv_obj_align(contBar, NULL, LV_ALIGN_IN_TOP_MID, 0, -5); /*This parametrs will be sued when realigned*/
	lv_cont_set_fit4(contBar, LV_FIT_PARENT, LV_FIT_PARENT, LV_FIT_NONE, LV_FIT_NONE);
	lv_obj_add_style(contBar, LV_OBJ_PART_MAIN, &containerStyle16);
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
	lv_obj_align(setButton, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, 40, -12);
	lv_label_set_text(labelSetButton, LV_SYMBOL_SETTINGS);
	lv_btn_set_fit(setButton, LV_FIT_TIGHT);
	lv_obj_set_event_cb(setButton, setButton_task);
	lv_obj_add_style(setButton, LV_OBJ_PART_MAIN, &transparentButtonStyle);
	
	wifiStatus = lv_label_create(contBar, NULL);
	lv_label_set_text(wifiStatus, LV_SYMBOL_WIFI);
	lv_obj_align(wifiStatus, NULL, LV_ALIGN_IN_LEFT_MID, 10, 0);

	sdStatus = lv_label_create(contBar, NULL);
	lv_label_set_text(sdStatus, LV_SYMBOL_SD_CARD);
	lv_obj_align(sdStatus, NULL, LV_ALIGN_IN_LEFT_MID, 45, 0);

	wifiStatusWarning = lv_label_create(wifiStatus, NULL);
	lv_label_set_text(wifiStatusWarning, "");
	lv_obj_add_style(wifiStatusWarning, LV_OBJ_PART_MAIN, &tinySymbolStyle);

	sdStatusWarning = lv_label_create(sdStatus, NULL);
	lv_label_set_text(sdStatusWarning, "");
	lv_obj_add_style(sdStatusWarning, LV_OBJ_PART_MAIN, &tinySymbolStyle);

	dateAndTimeAtBar = lv_label_create(contBar, NULL);
	lv_label_set_text(dateAndTimeAtBar, "Hello!");
	lv_obj_align(dateAndTimeAtBar, NULL, LV_ALIGN_IN_RIGHT_MID, -120, 0);
//TODO Styl kontener test

	contTemp = lv_cont_create(main_scr, NULL);
	lv_obj_set_click(contTemp, false);
	lv_obj_add_style(contTemp, LV_OBJ_PART_MAIN, &containerStyle16);

	contHumi = lv_cont_create(main_scr, NULL);
	lv_obj_set_click(contHumi, false);
	lv_obj_add_style(contHumi, LV_OBJ_PART_MAIN, &containerStyle16);

	contPM10 = lv_cont_create(main_scr, NULL);
	lv_obj_set_click(contPM10, false);
	lv_obj_add_style(contPM10, LV_OBJ_PART_MAIN, &containerStyle12);

	contPM25 = lv_cont_create(main_scr, NULL);
	lv_obj_set_click(contPM25, false);
	lv_obj_add_style(contPM25, LV_OBJ_PART_MAIN, &containerStyle16);

	contPM100 = lv_cont_create(main_scr, NULL);
	lv_obj_set_click(contPM100, false);
	lv_obj_add_style(contPM100, LV_OBJ_PART_MAIN, &containerStyle12);
	
	contAQI = lv_cont_create(main_scr, NULL);
	lv_obj_set_click(contAQI, false);
	lv_obj_add_style(contAQI, LV_OBJ_PART_MAIN, &containerStyle16);

	//contTMP = lv_cont_create(main_scr, NULL);
	//lv_obj_set_click(contTMP, false);
	//lv_obj_add_style(contTMP, LV_OBJ_PART_MAIN, &containerStyle);

	labelTemp = lv_label_create(contTemp, NULL);
	labelHumi = lv_label_create(contHumi, NULL);
	labelTempValue = lv_label_create(contTemp, NULL);
	labelHumiValue = lv_label_create(contHumi, NULL);
	labelPM10 = lv_label_create(contPM10, NULL);
	labelPM25 = lv_label_create(contPM25, NULL);
	labelPM100 = lv_label_create(contPM100, NULL);
	labelPM10Data = lv_label_create(contPM10, NULL);
	labelPM25Data = lv_label_create(contPM25, NULL);
	labelPM100Data = lv_label_create(contPM100, NULL);
	lv_obj_add_style(labelPM10Data, LV_OBJ_PART_MAIN, &containerStyle22);
	lv_obj_add_style(labelPM25Data, LV_OBJ_PART_MAIN, &containerStyle22);
	lv_obj_add_style(labelPM100Data, LV_OBJ_PART_MAIN, &containerStyle22);
	//lv_obj_set_pos(contTMP,  10, 30);
	lv_obj_set_pos(contPM10, 10, 148);
	lv_obj_set_pos(contPM25, 10, 30);
	lv_obj_set_pos(contPM100, 99, 148);
	lv_obj_set_pos(contAQI,   188, 148);
	lv_obj_set_pos(contTemp, 188, 30);
	lv_obj_set_pos(contHumi, 188, 89);

	lv_obj_set_pos(labelTemp, 5, 5);
	lv_obj_set_pos(labelHumi, 5, 5);
	lv_obj_set_pos(labelPM10, 5, 5);
	lv_obj_set_pos(labelPM25, 5, 5);
	lv_obj_set_pos(labelPM100, 5, 5);

	lv_obj_set_pos(labelTempValue, 70, 20);
	lv_obj_set_pos(labelHumiValue, 70, 20);
	lv_obj_set_pos(labelPM10Data, 45, 40);
	lv_obj_set_pos(labelPM25Data, 85, 50);
	lv_obj_set_pos(labelPM100Data, 40, 40);

	lv_obj_set_pos(sdStatusWarning, 2, 5);
	lv_obj_set_pos(wifiStatusWarning, 5, 5);
	//lv_obj_set_size(contTMP, 300, 200);
	lv_obj_set_size(contPM10, 91, 82);
	lv_obj_set_size(contPM25, 180, 120);
	lv_obj_set_size(contPM100, 91, 82);
	lv_obj_set_size(contTemp, 122, 61);
	lv_obj_set_size(contHumi, 122, 61);
	lv_obj_set_size(contAQI,  122, 82);

	lv_label_set_align(labelTempValue, LV_LABEL_ALIGN_CENTER);
	lv_label_set_align(labelHumiValue, LV_LABEL_ALIGN_CENTER);
	lv_label_set_text(labelTemp, "Temp");
	lv_label_set_text(labelHumi, "RH");
	lv_label_set_text(labelPM10, "PM 1.0 ug/m3");
	lv_label_set_text(labelPM25, "PM 2.5               ug/m3");
	lv_label_set_text(labelPM100, "PM 10 ug/m3");
	lv_label_set_text(labelTempValue, "-");
	lv_label_set_text(labelHumiValue, "-");
	lv_label_set_text(labelPM10Data, "-");
	lv_label_set_text(labelPM25Data, "-");
	lv_label_set_text(labelPM100Data, "-");

}

void wifiList_screen()
{
	contBarwifiList = lv_cont_create(wifiList_scr, NULL);
	lv_obj_set_auto_realign(contBarwifiList, true);
	lv_obj_align(contBarwifiList, NULL, LV_ALIGN_IN_TOP_MID, 0 ,0);
	lv_cont_set_fit4(contBarwifiList, LV_FIT_PARENT, LV_FIT_PARENT, LV_FIT_NONE, LV_FIT_NONE);
	lv_cont_set_layout(contBarwifiList, LV_LAYOUT_PRETTY_TOP);
	lv_obj_add_style(contBarwifiList, LV_OBJ_PART_MAIN, &containerStyle16);
	lv_obj_set_style_local_border_opa(contBarwifiList, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
	lv_obj_set_click(contBarwifiList, false);
	

	back_wifi_btn=lv_btn_create(contBarwifiList, NULL);
	back_wifi_label = lv_label_create(back_wifi_btn, NULL);
	lv_label_set_text(back_wifi_label, LV_SYMBOL_LEFT);
	lv_obj_set_style_local_bg_opa(back_wifi_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);
	lv_obj_set_style_local_border_opa(back_wifi_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);

	lv_obj_set_size(back_wifi_btn, 30, 15);
	lv_obj_set_event_cb(back_wifi_btn, wifi_list_btn_back);
	wifiListLabelAtBar = lv_label_create(contBarwifiList, NULL);
	lv_label_set_text(wifiListLabelAtBar, "WiFi");

	wifi_menu=lv_page_create(wifiList_scr, NULL);
	lv_obj_set_size(wifi_menu, SCREEN_WIDTH, lv_obj_get_height(wifiList_scr)-lv_obj_get_height_margin(contBarwifiList));
	lv_obj_align(wifi_menu, NULL, LV_ALIGN_CENTER, 0, (lv_obj_get_height_margin(contBarwifiList))/2);
	lv_obj_set_click(wifi_menu, false);
	lv_obj_set_click(wifi_menu, false);
	lv_obj_add_style(wifi_menu, LV_OBJ_PART_MAIN, &containerStyle12);
	lv_obj_set_style_local_border_opa(wifi_menu, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);

}

void settings_screen()
{
	contBarSettings = lv_cont_create(settings_scr, NULL);
	lv_obj_set_auto_realign(contBarSettings, true);					/*Auto realign when the size changes*/
	lv_obj_align(contBarSettings, NULL, LV_ALIGN_IN_TOP_MID, 0, -5); /*This parametrs will be sued when realigned*/
	lv_cont_set_fit4(contBarSettings, LV_FIT_PARENT, LV_FIT_PARENT, LV_FIT_NONE, LV_FIT_NONE);
	lv_cont_set_layout(contBarSettings, LV_LAYOUT_PRETTY_TOP);
	lv_obj_add_style(contBarSettings, LV_OBJ_PART_MAIN, &containerStyle16);
	lv_obj_set_style_local_border_opa(contBarSettings, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
	lv_obj_set_click(contBarSettings, false);

	back_settings_btn = lv_btn_create(contBarSettings, NULL);
	back_settings_label = lv_label_create(back_settings_btn, NULL);
	lv_label_set_text(back_settings_label, LV_SYMBOL_LEFT);
	lv_obj_set_style_local_bg_opa(back_settings_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);
	lv_obj_set_style_local_border_opa(back_settings_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);
	lv_obj_set_size(back_settings_btn, 30, 15);
	lv_obj_set_event_cb(back_settings_btn, btn_settings_back);

	settingsLabelAtBar = lv_label_create (contBarSettings, NULL);
	lv_label_set_text(settingsLabelAtBar, "Settings");

	settingsMenu=lv_page_create(settings_scr, NULL);
	lv_obj_set_size(settingsMenu, SCREEN_WIDTH, lv_obj_get_height(settings_scr)-lv_obj_get_height_margin(contBarSettings));
	lv_obj_align(settingsMenu, NULL, LV_ALIGN_CENTER, 0, (lv_obj_get_height_margin(contBarSettings))/2);
	lv_obj_set_click(settingsMenu, false);
	lv_obj_add_style(settingsMenu, LV_OBJ_PART_MAIN, &containerStyle12);
	lv_obj_set_style_local_border_opa(settingsMenu, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);

	WiFiBtnBar = lv_cont_create(settingsMenu, NULL);
	lv_obj_set_auto_realign(WiFiBtnBar, true);
	lv_obj_align(WiFiBtnBar, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
	lv_cont_set_fit4(WiFiBtnBar, LV_FIT_PARENT, LV_FIT_PARENT, LV_FIT_NONE, LV_FIT_NONE);
	WiFiBtn = lv_btn_create(WiFiBtnBar, NULL);
	lv_obj_set_size(WiFiBtn, lv_obj_get_width(WiFiBtnBar), lv_obj_get_height(WiFiBtnBar));
	WiFiBtnLabel = lv_label_create(WiFiBtn, NULL);
	lv_label_set_text(WiFiBtnLabel, "WiFi");
	lv_label_set_align(WiFiBtnLabel, LV_ALIGN_IN_LEFT_MID);
	lv_obj_set_event_cb(WiFiBtn, WiFi_btn);
	lv_obj_add_style(WiFiBtn, LV_OBJ_PART_MAIN, &transparentButtonStyle);
	lv_obj_add_style(WiFiBtnBar, LV_OBJ_PART_MAIN, &containerStyle16);

	placeholder1Bar = lv_cont_create(settingsMenu, NULL);
	lv_obj_align(placeholder1Bar, NULL, LV_ALIGN_IN_TOP_MID, 0, lv_obj_get_height(WiFiBtnBar));
	lv_cont_set_fit4(placeholder1Bar, LV_FIT_PARENT, LV_FIT_PARENT, LV_FIT_NONE, LV_FIT_NONE);
	placeholder1Btn = lv_btn_create(placeholder1Bar, NULL);
	lv_obj_set_size(placeholder1Btn, lv_obj_get_width(placeholder1Bar), lv_obj_get_height(placeholder1Bar));
	placeholder1Label = lv_label_create(placeholder1Btn, NULL);
	lv_label_set_text(placeholder1Label, "placeholder1");
	lv_label_set_align(placeholder1Label, LV_ALIGN_IN_LEFT_MID);
	lv_obj_add_style(placeholder1Btn, LV_OBJ_PART_MAIN, &transparentButtonStyle);
	lv_obj_add_style(placeholder1Bar, LV_OBJ_PART_MAIN, &containerStyle16);

	placeholder2Bar = lv_cont_create(settingsMenu, NULL);
	lv_obj_align(placeholder2Bar, NULL, LV_ALIGN_IN_TOP_MID, 0, lv_obj_get_height(WiFiBtnBar)*2);
	lv_cont_set_fit4(placeholder2Bar, LV_FIT_PARENT, LV_FIT_PARENT, LV_FIT_NONE, LV_FIT_NONE);
	placeholder2Btn = lv_btn_create(placeholder2Bar, NULL);
	lv_obj_set_size(placeholder2Btn, lv_obj_get_width(placeholder2Bar), lv_obj_get_height(placeholder2Bar));
	placeholder2Label = lv_label_create(placeholder2Btn, NULL);
	lv_label_set_text(placeholder2Label, "placeholder2");
	lv_label_set_align(placeholder2Label, LV_ALIGN_IN_LEFT_MID);
	lv_obj_add_style(placeholder2Btn, LV_OBJ_PART_MAIN, &transparentButtonStyle);
	lv_obj_add_style(placeholder2Bar, LV_OBJ_PART_MAIN, &containerStyle16);

	placeholder3Bar = lv_cont_create(settingsMenu, NULL);
	lv_obj_align(placeholder3Bar, NULL, LV_ALIGN_IN_TOP_MID, 0, lv_obj_get_height(WiFiBtnBar)*3);
	lv_cont_set_fit4(placeholder3Bar, LV_FIT_PARENT, LV_FIT_PARENT, LV_FIT_NONE, LV_FIT_NONE);
	placeholder3Btn = lv_btn_create(placeholder3Bar, NULL);
	lv_obj_set_size(placeholder3Btn, lv_obj_get_width(placeholder3Bar), lv_obj_get_height(placeholder3Bar));
	placeholder3Label = lv_label_create(placeholder3Btn, NULL);
	lv_label_set_text(placeholder3Label, "placeholder3");
	lv_label_set_align(placeholder3Label, LV_ALIGN_IN_LEFT_MID);
	lv_obj_add_style(placeholder3Btn, LV_OBJ_PART_MAIN, &transparentButtonStyle);
	lv_obj_add_style(placeholder3Bar, LV_OBJ_PART_MAIN, &containerStyle16);
}

void wifi_screen()
{
	contBarWiFi = lv_cont_create(wifiConnect_scr, NULL);
	lv_obj_set_auto_realign(contBarWiFi, true);					/*Auto realign when the size changes*/
	lv_obj_align(contBarWiFi, NULL, LV_ALIGN_IN_TOP_MID, 0, 0); /*This parametrs will be sued when realigned*/
	lv_cont_set_fit4(contBarWiFi, LV_FIT_PARENT, LV_FIT_PARENT, LV_FIT_NONE, LV_FIT_NONE);
	lv_cont_set_layout(contBarWiFi, LV_LAYOUT_PRETTY_TOP);
	lv_obj_add_style(contBarWiFi, LV_OBJ_PART_MAIN, &containerStyle16);
	lv_obj_set_style_local_border_opa(contBarWiFi, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
	lv_obj_set_click(contBarWiFi, false);

	cancel_btn = lv_btn_create(contBarWiFi, NULL);
	cancel_label = lv_label_create(cancel_btn, NULL);
	lv_label_set_text(cancel_label, LV_SYMBOL_LEFT);
	lv_obj_set_size(cancel_btn, 30, 15);
	lv_obj_set_event_cb(cancel_btn, btn_cancel);

	wifiLabelAtBar = lv_label_create(contBarWiFi, NULL);
	lv_label_set_text(wifiLabelAtBar, "WiFi settings");

	ssid_label = lv_label_create(wifiConnect_scr, NULL);
	lv_label_set_text(ssid_label, "SSID: ");
	lv_obj_set_pos(ssid_label, 5, 53);

	ssid_ta = lv_textarea_create(wifiConnect_scr, NULL);
	lv_textarea_set_text(ssid_ta, "");
	lv_textarea_set_pwd_mode(ssid_ta, false);
	lv_textarea_set_one_line(ssid_ta, true);
	lv_obj_set_event_cb(ssid_ta, ta_event_cb);
	lv_textarea_set_cursor_hidden(ssid_ta, true);
	lv_obj_set_width(ssid_ta, LV_HOR_RES / 2 - 20);
	lv_obj_set_pos(ssid_ta, 100, 45);

	pwd_label = lv_label_create(wifiConnect_scr, NULL);
	lv_label_set_text(pwd_label, "Password: ");
	lv_obj_set_pos(pwd_label, 5, 92);
	pwd_ta = lv_textarea_create(wifiConnect_scr, NULL);
	lv_textarea_set_text(pwd_ta, "");
	lv_textarea_set_pwd_mode(pwd_ta, true);
	lv_textarea_set_one_line(pwd_ta, true);
	lv_obj_set_event_cb(pwd_ta, ta_event_cb);
	lv_textarea_set_cursor_hidden(pwd_ta, true);
	lv_obj_set_width(pwd_ta, LV_HOR_RES / 2 - 20);
	lv_obj_set_pos(pwd_ta, 100, 85);

	apply_btn = lv_btn_create(wifiConnect_scr, NULL);
	apply_label = lv_label_create(apply_btn, NULL);
	lv_label_set_text(apply_label, "Connect");
	lv_obj_set_style_local_border_opa(apply_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);
	lv_obj_set_event_cb(apply_btn, btn_connect);
	lv_obj_set_width(apply_btn, 75);
	lv_obj_set_pos(apply_btn, 243, 43);
}

void lock_screen()
{
	contDateTimeLock = lv_cont_create(lock_scr, NULL);
	lv_obj_set_auto_realign(contDateTimeLock, true);			   /*Auto realign when the size changes*/
	lv_obj_align(contDateTimeLock, NULL, LV_ALIGN_CENTER, 0, -40); /*This parametrs will be sued when realigned*/
	//lv_cont_set_fit4(contDateTimeLock,   LV_FIT_PARENT, LV_FIT_PARENT, LV_FIT_NONE, LV_FIT_NONE);
	lv_cont_set_fit(contDateTimeLock, LV_FIT_TIGHT);
	lv_cont_set_layout(contDateTimeLock, LV_LAYOUT_PRETTY_MID);

	lockButton1 = lv_btn_create(lock_scr, NULL);
	labelLockButton1 = lv_label_create(lockButton1, NULL);
	lv_obj_align(lockButton1, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);
	lv_label_set_text(labelLockButton1, LV_SYMBOL_POWER);
	lv_btn_set_fit(lockButton1, LV_FIT_TIGHT);
	lv_obj_set_event_cb(lockButton1, lockButton_task1);
	lv_obj_set_style_local_bg_opa(lockButton1, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);
	lv_obj_set_style_local_border_opa(lockButton1, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);
	lv_obj_set_style_local_border_opa(lockButton1, LV_BTN_PART_MAIN, LV_BTN_STATE_PRESSED, LV_OPA_TRANSP);

	labelTimeLock = lv_label_create(contDateTimeLock, NULL);
	lv_label_set_text(labelTimeLock, "Connect\nto wifi");
	lv_obj_align(labelTimeLock, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
	lv_obj_set_size(labelTimeLock, 300, 200);

	labelDateLock = lv_label_create(contDateTimeLock, NULL);
	lv_label_set_text(labelDateLock, "");
	lv_obj_align(labelTimeLock, NULL, LV_ALIGN_CENTER, 0, 0);

	wifiStatusAtLock = lv_label_create(contDateTimeLock, NULL);
	lv_obj_align(wifiStatusAtLock, NULL, LV_ALIGN_IN_BOTTOM_MID, 10, 0);
	lv_label_set_text(wifiStatusAtLock, "");

	sdStatusAtLock = lv_label_create(contDateTimeLock, NULL);
	lv_obj_align(sdStatusAtLock, NULL, LV_ALIGN_IN_BOTTOM_MID, -10, 0);
	lv_label_set_text(sdStatusAtLock, "");
}

void setup()
{
	wasUpdated=false;
	pinMode(33, OUTPUT);
	digitalWrite(33, LOW);
	sqlite3_initialize();
	Serial.begin(115200); /* prepare for possible serial debug */
	Serial2.begin(9600, SERIAL_8N1, 16, 17);

	pmsSensor = new PMS5003(&Serial2, &Serial);

	lv_init();
	tft.begin(); /* TFT init */
	tft.setRotation(3);

	uint16_t calData[5] = {275, 3620, 264, 3532, 1};
	tft.setTouch(calData);

	lv_disp_buf_init(&disp_buf, buf, NULL, LV_HOR_RES_MAX * 10);
	/*Initialize the display*/
	lv_disp_drv_t disp_drv;
	lv_disp_drv_init(&disp_drv);
	disp_drv.hor_res = SCREEN_WIDTH;
	disp_drv.ver_res = SCREEN_HEIGHT;
	disp_drv.flush_cb = my_disp_flush;
	disp_drv.buffer = &disp_buf;
	lv_disp_drv_register(&disp_drv);

	/*Initialize the input device driver*/
	lv_indev_drv_t indev_drv;
	lv_indev_drv_init(&indev_drv);			/*Descriptor of a input device driver*/
	indev_drv.type = LV_INDEV_TYPE_POINTER; /*Touch pad is a pointer-like device*/
	indev_drv.read_cb = my_touchpad_read;	/*Set your driver function*/
	lv_indev_drv_register(&indev_drv);		/*Finally register the driver*/

	//Set the theme..
	lv_theme_t *th = lv_theme_material_init(LV_THEME_DEFAULT_COLOR_PRIMARY, LV_THEME_DEFAULT_COLOR_SECONDARY, LV_THEME_DEFAULT_FLAG, LV_THEME_DEFAULT_FONT_SMALL, LV_THEME_DEFAULT_FONT_NORMAL, LV_THEME_DEFAULT_FONT_SUBTITLE, LV_THEME_DEFAULT_FONT_TITLE);
	lv_theme_set_act(th);
	containerStyle12Init();
	containerStyle16Init();
	containerStyle22Init();
	transparentButtonStyleInit();
	tinySymbolStyleInit();

	main_scr = lv_cont_create(NULL, NULL);
	settings_scr=lv_cont_create(NULL, NULL);
	wifiConnect_scr = lv_cont_create(NULL, NULL);
	wifiList_scr=lv_cont_create(NULL, NULL);
	lock_scr = lv_cont_create(NULL, NULL);
	lv_obj_set_style_local_bg_color(main_scr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
	lv_obj_set_style_local_bg_color(wifiConnect_scr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
	lv_obj_set_style_local_bg_color(lock_scr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
	lv_obj_set_style_local_bg_color(settings_scr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
	lv_obj_set_style_local_bg_color(wifiList_scr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);

	main_screen();
	settings_screen();
	wifi_screen();
	lock_screen();
	wifiList_screen();
	lv_disp_load_scr(main_scr);

	lv_task_t *date = lv_task_create(date_time, 1000, LV_TASK_PRIO_MID, NULL);
	syn_rtc = lv_task_create_basic();
	lv_task_set_cb(syn_rtc, config_time);
	lv_task_set_period(syn_rtc, 3600000);
	getSample = lv_task_create(getSampleFunc, measure_period, LV_TASK_PRIO_HIGH, NULL);
	turnFanOn = lv_task_create(turnFanOnFunc, 240000, LV_TASK_PRIO_HIGH, NULL);

	getAppLastRecord = lv_task_create_basic();
	lv_task_set_cb(getAppLastRecord, fetchLastRecord);
	lv_task_set_period(getAppLastRecord, 36000);

	if (Rtc.GetMemory(53) == 1)
	{
		ssid = getCharArrrayFromRTC(Rtc, 3);
		password = getCharArrrayFromRTC(Rtc, 28);
		WiFi.begin(ssid.c_str(), password.c_str());
	}
	lv_task_ready(syn_rtc);
}

void loop()
{
	lv_task_handler(); /* let the GUI do its work */
	delay(5);
}

//TODO kolorki ustawien
//TODO wiecej ustawien
//TODO zmiana koloru klawiatury