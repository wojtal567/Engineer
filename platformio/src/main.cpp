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
#include <ArduinoJson.h>
#include <time.h>

#define LVGL_TICK_PERIOD 60
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define GMT_OFFSET_SEC 7200

RtcDS1307<TwoWire> Rtc(Wire);
static const char ntpServerName[] = "europe.pool.ntp.org";
WiFiUDP ntpUDP;
NTPClient dateTimeClient(ntpUDP, ntpServerName, 7200);

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

//--------------------------------------------------main gui
char *pm[] = {"PM 1,0", "PM 2,5", "PM 10,0"};
char *pmParticle[] = {">1,0 um", ">2,5 um", ">10,0 um"};
lv_obj_t *main_scr;
lv_obj_t *wifiStatus;
lv_obj_t *dateAndTimeAtBar;
lv_obj_t *contBar;
lv_obj_t *contTemp;
lv_obj_t *contStat;
lv_obj_t *contHumi;
lv_obj_t *labelLockButton;
lv_obj_t *lockButton;
lv_obj_t *labelSetButton;
lv_obj_t *setButton;
lv_obj_t *labelPMTitle[3];
lv_obj_t *labelPMData[3];
lv_obj_t *labelParticles;
lv_obj_t *labelPMTitleParticle[3];
lv_obj_t *labelPMParticle[3];
lv_obj_t *barPMParticle[3];
lv_obj_t *labelTemp;
lv_obj_t *labelHumi;
lv_obj_t *labelTempValue;
lv_obj_t *labelHumiValue;
lv_obj_t *led[3];
lv_obj_t *sdStatus;
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
			StaticJsonDocument<300> doc;
			DeserializationError err = deserializeJson(doc, http.getString());
			Serial.println(err.c_str());
			JsonObject sample = doc.getElement(0);
			String timestamp = sample["timestamp"];
			Serial.println(timestamp);
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
		for (int i = 0; i < 10; i++)
			dateTimeClient.update();
		configTime(Rtc, dateTimeClient);
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

static void setButton_task(lv_obj_t *obj, lv_event_t event)
{
	lv_disp_load_scr(wifi_scr);
	String result = getCharArrrayFromRTC(Rtc, 3);
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

	if (pmsSensor->readData())
	{
		std::map<std::string, uint16_t> tmpData = pmsSensor->returnData();
		pmsSensor->dumpSamples();

		itoa(tmpData["pm10_standard"], buffer, 10);
		lv_label_set_text(labelPMData[0], buffer);

		itoa(tmpData["pm25_standard"], buffer, 10);
		lv_label_set_text(labelPMData[1], buffer);

		itoa(tmpData["pm100_standard"], buffer, 10);
		lv_label_set_text(labelPMData[2], buffer);

		itoa(tmpData["particles_10um"], buffer, 10);
		lv_label_set_text(labelPMParticle[0], buffer);

		lv_bar_set_value(barPMParticle[0], tmpData["particles_10um"], LV_ANIM_ON);

		itoa(tmpData["particles_25um"], buffer, 10);
		lv_label_set_text(labelPMParticle[1], buffer);

		lv_bar_set_value(barPMParticle[1], tmpData["particles_25um"], LV_ANIM_ON);

		itoa(tmpData["particles_100um"], buffer, 10);
		lv_label_set_text(labelPMParticle[2], buffer);

		lv_bar_set_value(barPMParticle[2], tmpData["particles_100um"], LV_ANIM_ON);

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
		lv_label_set_text(wifiStatusAtLock, LV_SYMBOL_WIFI);
		lv_label_set_text(wifiStatus, LV_SYMBOL_WIFI);
	}
	else
	{
		lv_label_set_text(wifiStatusAtLock, "");
		lv_label_set_text(wifiStatus, "");
	}

	if (mySDCard.start(&sampleDB, &Serial2))
	{
		lv_label_set_text(sdStatus, LV_SYMBOL_SD_CARD);
		lv_label_set_text(sdStatusAtLock, LV_SYMBOL_SD_CARD);
	}
	else
	{
		lv_label_set_text(sdStatus, "");
		lv_label_set_text(sdStatusAtLock, "");
	}
}

void main_screen()
{
	contBar = lv_cont_create(main_scr, NULL);
	lv_obj_set_auto_realign(contBar, true);					/*Auto realign when the size changes*/
	lv_obj_align(contBar, NULL, LV_ALIGN_IN_TOP_MID, 0, 0); /*This parametrs will be sued when realigned*/
	lv_cont_set_fit4(contBar, LV_FIT_PARENT, LV_FIT_PARENT, LV_FIT_NONE, LV_FIT_NONE);
	//lv_cont_set_layout(contBar, LV_LAYOUT_PRETTY_MID); - this causes elements to move on seconds tick

	lockButton = lv_btn_create(main_scr, NULL);
	labelLockButton = lv_label_create(lockButton, NULL);
	lv_obj_align(lockButton, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, 40, -55);
	lv_label_set_text(labelLockButton, LV_SYMBOL_POWER);
	lv_btn_set_fit(lockButton, LV_FIT_TIGHT);
	lv_obj_set_event_cb(lockButton, lockButton_task);
	lv_obj_set_style_local_bg_opa(lockButton, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);
	lv_obj_set_style_local_border_opa(lockButton, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);
	lv_obj_set_style_local_border_opa(lockButton, LV_BTN_PART_MAIN, LV_BTN_STATE_PRESSED, LV_OPA_TRANSP);

	setButton = lv_btn_create(main_scr, NULL);
	labelSetButton = lv_label_create(setButton, NULL);
	lv_obj_align(setButton, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, 40, -12);
	lv_label_set_text(labelSetButton, LV_SYMBOL_SETTINGS);
	lv_btn_set_fit(setButton, LV_FIT_TIGHT);
	lv_obj_set_event_cb(setButton, setButton_task);
	lv_obj_set_style_local_bg_opa(setButton, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);
	lv_obj_set_style_local_border_opa(setButton, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);
	lv_obj_set_style_local_border_opa(setButton, LV_BTN_PART_MAIN, LV_BTN_STATE_PRESSED, LV_OPA_TRANSP);
	wifiStatus = lv_label_create(contBar, NULL);
	lv_label_set_text(wifiStatus, "");
	lv_obj_align(wifiStatus, NULL, LV_ALIGN_IN_LEFT_MID, 10, 0);
	sdStatus = lv_label_create(contBar, NULL);
	lv_label_set_text(sdStatus, "");
	lv_obj_align(sdStatus, NULL, LV_ALIGN_IN_LEFT_MID, 45, 0);
	dateAndTimeAtBar = lv_label_create(contBar, NULL);
	lv_label_set_text(dateAndTimeAtBar, "Hello!");
	lv_obj_align(dateAndTimeAtBar, NULL, LV_ALIGN_IN_RIGHT_MID, -120, 0);

	for (int i = 0; i < 3; i++)
	{
		led[i] = lv_led_create(main_scr, NULL);
		labelPMData[i] = lv_label_create(main_scr, NULL);
		labelPMTitle[i] = lv_label_create(main_scr, NULL);
		lv_led_on(led[i]);
		lv_label_set_align(labelPMData[i], LV_LABEL_ALIGN_CENTER);
		lv_obj_set_style_local_text_color(labelPMTitle[i], LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);
		lv_label_set_text(labelPMTitle[i], pm[i]);
		lv_label_set_text(labelPMData[i], "-");
		lv_obj_set_size(led[i], 10, 10);
		lv_obj_set_pos(led[i], 20, 55 + i * 25);
		lv_obj_set_pos(labelPMTitle[i], 40, 50 + i * 25);
		lv_obj_set_pos(labelPMData[i], 130, 50 + i * 25);
	}

	labelParticles = lv_label_create(main_scr, NULL);
	lv_obj_set_pos(labelParticles, 30, 125);
	lv_label_set_text(labelParticles, "Particles");

	for (int i = 0; i < 3; i++)
	{
		labelPMTitleParticle[i] = lv_label_create(main_scr, NULL);
		labelPMParticle[i] = lv_label_create(main_scr, NULL);
		barPMParticle[i] = lv_bar_create(main_scr, NULL);
		lv_label_set_align(labelPMParticle[i], LV_LABEL_ALIGN_CENTER);
		lv_obj_align(barPMParticle[i], NULL, LV_ALIGN_CENTER, 0, 0);
		lv_obj_set_style_local_bg_color(barPMParticle[i], LV_BAR_PART_INDIC, LV_STATE_DEFAULT, LV_COLOR_TEAL);
		lv_label_set_text(labelPMTitleParticle[i], pmParticle[i]);
		lv_label_set_text(labelPMParticle[i], "-");
		lv_obj_set_size(barPMParticle[i], 100, 20);
		lv_bar_set_anim_time(barPMParticle[i], 2000);
		lv_bar_set_range(barPMParticle[i], 0, 100);
		lv_obj_set_pos(labelPMTitleParticle[i], 40, 150 + i * 25);
		lv_obj_set_pos(labelPMParticle[i], 130, 150 + i * 25);
		lv_obj_set_pos(barPMParticle[i], 170, 150 + i * 25);
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
	lv_label_set_text(labelHumi, "Humi");
	lv_label_set_text(labelTempValue, "-");
	lv_label_set_text(labelHumiValue, "-");
}

void wifi_screen()
{
	contBarWiFi = lv_cont_create(wifi_scr, NULL);
	lv_obj_set_auto_realign(contBarWiFi, true);					/*Auto realign when the size changes*/
	lv_obj_align(contBarWiFi, NULL, LV_ALIGN_IN_TOP_MID, 0, 0); /*This parametrs will be sued when realigned*/
	lv_cont_set_fit4(contBarWiFi, LV_FIT_PARENT, LV_FIT_PARENT, LV_FIT_NONE, LV_FIT_NONE);
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
	lv_obj_set_width(ssid_ta, LV_HOR_RES / 2 - 20);
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
	lv_label_set_text(labelTimeLock, "Connect");
	lv_obj_align(labelTimeLock, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
	lv_obj_set_size(labelTimeLock, 300, 200);

	labelDateLock = lv_label_create(contDateTimeLock, NULL);
	lv_label_set_text(labelDateLock, "to wifi");
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

	main_scr = lv_cont_create(NULL, NULL);
	wifi_scr = lv_cont_create(NULL, NULL);
	lock_scr = lv_cont_create(NULL, NULL);
	main_screen();
	wifi_screen();
	lock_screen();
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
		if (WiFi.status() == WL_CONNECTED)
		{
			lv_task_ready(syn_rtc);
		}
	}
}

void loop()
{
	lv_task_handler(); /* let the GUI do its work */
	delay(5);
}

//TODO zrobić chowanie klawiatury
//TODO dokończyć wysyłanie danych na apkę
//TODO ŁADNE GUI
//TODO dodać dark theme (przełącznik w zębatce?)
//TODO Zbierać wszystkie dane i wyświetlać na przesuwanych ekranach?