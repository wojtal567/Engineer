#pragma once
#include <lvgl.h>

#include <GlobalVariables.hpp>

#include "globals/Peripherals.h"
#include "rtc.hpp"
#include "ui/Screens.h"
#include "ui/widgets/Button.h"

void set_spinbox_digit_format(lv_obj_t *spinbox, int32_t range_min, int32_t range_max, int offset) {
    int _spinbox_value = lv_spinbox_get_value(spinbox) + offset;
    if (_spinbox_value > 9) {
        if (_spinbox_value > 99) {
            lv_spinbox_set_digit_format(spinbox, 3, 0);
        } else {
            lv_spinbox_set_digit_format(spinbox, 2, 0);
        }
    } else {
        lv_spinbox_set_digit_format(spinbox, 1, 0);
    }
    lv_spinbox_set_range(spinbox, range_min, range_max);
}

int getDDListIndexBasedOnLcdLockTime(int lcdLockTime) {
    switch (lcdLockTime) {
        case -1: {
            return 7;
            break;
        }

        case 30000: {
            return 0;
            break;
        }
        case 60000: {
            return 1;
            break;
        }
        case 120000: {
            return 2;
            break;
        }
        case 300000: {
            return 3;
            break;
        }
        case 600000: {
            return 4;
            break;
        }
        case 1800000: {
            return 5;
            break;
        }
        case 3600000: {
            return 6;
            break;
        }
        default: {
            return 1;
        }
    }
}

// Function that turns fan on
void turnFanOnFunc(lv_task_t *task) {
    digitalWrite(PinConfig::fanPin, HIGH);
    lv_task_set_prio(turnFanOn, LV_TASK_PRIO_OFF);
}

void config_time() {
    if (WiFi.status() == WL_CONNECTED) {
        for (int i = 0; i < 500; i++) dateTimeClient.update();
        configTime(Peripherals::Rtc, dateTimeClient);
        Serial.println("Succesfully updated time on RTC.");
    }
}

bool isLastSampleSaved() {
    StaticJsonDocument<600> docA;
    JsonArray lastRecordToCheck = docA.to<JsonArray>();
    Peripherals::mySDCard.getLastRecord(&Peripherals::sampleDB, &Serial, &lastRecordToCheck);
    Serial.print("Global: ");
    Serial.print(lastSampleTimestamp);
    Serial.print(" Baza: ");
    Serial.print(lastRecordToCheck[0]["timestamp"].as<String>());
    if (lastSampleTimestamp == lastRecordToCheck[0]["timestamp"].as<String>()) {
        Serial.println("Last sample has been saved correctly - return true.");
        return true;
    } else {
        Serial.println("Something went wrong saving last sample - return false");
        return false;
    }
}

// Get single sample and set text
void getSampleFunc(lv_task_t *task) {
    Peripherals::sht30.get();
    if (Global::config.currentSampleNumber != 0 &&
        Global::config.currentSampleNumber < Global::config.numberOfSamples) {
        if (Peripherals::pmsSensor->readData()) {
            Serial.println("Succesfully read data from dust sensor.");
            std::map<std::string, float> tmpData = Peripherals::pmsSensor->returnData();
            Peripherals::pmsSensor->dumpSamples();
            for (uint8_t i = 0; i < 15; i++) {
                Global::data[Global::labels[i]] += tmpData[Global::labels[i]];
            }
            Global::config.currentSampleNumber++;
            temp += Peripherals::sht30.cTemp;
            humi += Peripherals::sht30.humidity;
        }
    }
    if (Global::config.currentSampleNumber == 0) {
        lv_task_set_period(getSample, Global::config.measurePeriod);
        if (Peripherals::pmsSensor->readData()) {
            Serial.println("Succesfully read data from dust sensor.");
            std::map<std::string, float> tmpData = Peripherals::pmsSensor->returnData();
            Peripherals::pmsSensor->dumpSamples();
            Global::data = tmpData;
            Global::config.currentSampleNumber++;
            temp = Peripherals::sht30.cTemp;
            humi = Peripherals::sht30.humidity;
        }
    }
    if (Global::config.currentSampleNumber == Global::config.numberOfSamples) {
        for (uint8_t i = 0; i < 15; i++)
            Global::data[Global::labels[i]] = Global::data[Global::labels[i]] / Global::config.numberOfSamples;
        Global::config.currentSampleNumber = 0;
        temp = temp / Global::config.numberOfSamples;
        humi = humi / Global::config.numberOfSamples;
        lv_task_set_period(getSample, (Global::config.timeBetweenSavingSamples -
                                       (Global::config.numberOfSamples - 1) * Global::config.measurePeriod));

        pm25Aqi = Global::data["pm25_standard"];

        Screens::mainScr->setAqiStateNColor(pm25Aqi);
        Screens::mainScr->updateSampleData(Global::data["pm10_standard"], Global::data["pm25_standard"],
                                           Global::data["pm100_standard"], Global::data["particles_03um"],
                                           Global::data["particles_05um"], Global::data["particles_10um"],
                                           Global::data["particles_25um"], Global::data["particles_50um"],
                                           Global::data["particles_100um"], temp, humi);
        if (Peripherals::Rtc.GetIsRunning()) {
            lastSampleTimestamp = getMainTimestamp(Peripherals::Rtc);
            Serial.print("lastSampleTimestamp przed wrzuceniem do bazy: " + lastSampleTimestamp);
            Peripherals::mySDCard.save(Global::data, temp, humi, lastSampleTimestamp, &Peripherals::sampleDB, &Serial);
        } else {
            Serial.println("RTC is not running, not saving");
        }
        lv_task_reset(turnFanOn);
        lv_task_set_prio(turnFanOn, LV_TASK_PRIO_MID);
        digitalWrite(PinConfig::fanPin, LOW);
        const bool sampleSaveError = !isLastSampleSaved();
        if (!sampleSaveError) {
            lv_obj_set_style_local_bg_color(ledAtLock, LV_LED_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_GREEN);
            lv_obj_set_style_local_shadow_color(ledAtLock, LV_LED_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_GREEN);
        } else {
            lv_obj_set_style_local_bg_color(ledAtLock, LV_LED_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);
            lv_obj_set_style_local_shadow_color(ledAtLock, LV_LED_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);
        }

        Screens::mainScr->setSampleSaveError(sampleSaveError);
    }
}

// Unlocking button clicked
static void unlockButton_task(lv_obj_t *obj, lv_event_t event) {
    Serial.print(lv_btn_get_state(unlockButton));
    if (event == LV_EVENT_CLICKED) lv_disp_load_scr(Screens::mainScr->getScreen());
}

static void btn_settings_back(lv_obj_t *obj, lv_event_t event) {
    if (event == LV_EVENT_CLICKED) lv_disp_load_scr(Screens::mainScr->getScreen());
}

void timesettings_back_btn(lv_obj_t *obj, lv_event_t event) {
    if (event == LV_EVENT_CLICKED) {
        switch (Global::config.lcdLockTime) {
            case -1:
                lv_dropdown_set_selected(lockScreenDDlist, 4);
                break;
            case 60000:
                lv_dropdown_set_selected(lockScreenDDlist, 0);
                break;
            case 300000:
                lv_dropdown_set_selected(lockScreenDDlist, 1);
                break;
            case 600000:
                lv_dropdown_set_selected(lockScreenDDlist, 2);
                break;
            case 3600000:
                lv_dropdown_set_selected(lockScreenDDlist, 3);
                break;
            default:
                lv_dropdown_set_selected(lockScreenDDlist, 0);
                break;
        }
        lv_scr_load(Screens::settingsScr->getScreen());
        inTimeSettings = false;
        timeChanged = false;
        dateChanged = false;
    }
}

void timesettings_save_btn(lv_obj_t *obj, lv_event_t event) {
    if (event == LV_EVENT_CLICKED) {
        switch (lv_dropdown_get_selected(lockScreenDDlist)) {
            case 0:
                Global::config.lcdLockTime = 60000;
                break;
            case 1:
                Global::config.lcdLockTime = 300000;
                break;
            case 2:
                Global::config.lcdLockTime = 600000;
                break;
            case 3:
                Global::config.lcdLockTime = 3600000;
                break;
            case 4:
                Global::config.lcdLockTime = -1;
                break;
            default:
                Global::config.lcdLockTime = 60000;
                break;
        }
        Peripherals::mySDCard.saveConfig(Global::config, Global::configFilePath);
        Peripherals::mySDCard.printConfig(Global::configFilePath);
        if (timeChanged == true) {
            String datet = lv_label_get_text(dateBtnLabel) + (String)lv_textarea_get_text(timeHour) + ":" +
                           (String)lv_textarea_get_text(timeMinute);
            Serial.println(datet);
            RtcDateTime *dt = new RtcDateTime(atoi(datet.substring(6, 10).c_str()), atoi(datet.substring(3, 6).c_str()),
                                              atoi(datet.substring(0, 2).c_str()), datet.substring(10, 12).toDouble(),
                                              datet.substring(13, 15).toDouble(), 0);
            Peripherals::Rtc.SetDateTime(*dt);
            Peripherals::Rtc.SetIsRunning(true);
        }
        if (dateChanged == true) {
            RtcDateTime ori = Peripherals::Rtc.GetDateTime();
            String date = lv_label_get_text(dateBtnLabel);
            RtcDateTime *dt =
                new RtcDateTime(atoi(date.substring(6).c_str()), atoi(date.substring(3, 6).c_str()),
                                atoi(date.substring(0, 2).c_str()), ori.Hour(), ori.Minute(), ori.Second());
            Peripherals::Rtc.SetDateTime(*dt);
            Peripherals::Rtc.SetIsRunning(true);
        }
        lv_disp_load_scr(Screens::mainScr->getScreen());
        inTimeSettings = false;
        timeChanged = false;
        dateChanged = false;
    }
}

static void hourIncrement(lv_obj_t *btn, lv_event_t e) {
    if (e == LV_EVENT_SHORT_CLICKED || e == LV_EVENT_LONG_PRESSED_REPEAT) {
        if (lv_spinbox_get_value(timeHour) == 23) {
            lv_spinbox_set_value(timeHour, 0);
        } else {
            lv_spinbox_increment(timeHour);
        }
        timeChanged = true;
    }
}

static void hourDecrement(lv_obj_t *btn, lv_event_t e) {
    if (e == LV_EVENT_SHORT_CLICKED || e == LV_EVENT_LONG_PRESSED_REPEAT) {
        if (lv_spinbox_get_value(timeHour) == 0)
            lv_spinbox_set_value(timeHour, 23);
        else
            lv_spinbox_decrement(timeHour);
        timeChanged = true;
    }
}

static void minuteIncrement(lv_obj_t *btn, lv_event_t e) {
    if (e == LV_EVENT_SHORT_CLICKED || e == LV_EVENT_LONG_PRESSED_REPEAT) {
        if (lv_spinbox_get_value(timeMinute) == 59) {
            if (lv_spinbox_get_value(timeHour) == 23) {
                lv_spinbox_set_value(timeHour, 0);
            } else {
                lv_spinbox_increment(timeHour);
            }
            lv_spinbox_set_value(timeMinute, 0);
        } else
            lv_spinbox_increment(timeMinute);
        timeChanged = true;
    }
}

static void minuteDecrement(lv_obj_t *btn, lv_event_t e) {
    if (e == LV_EVENT_SHORT_CLICKED || e == LV_EVENT_LONG_PRESSED_REPEAT) {
        if (lv_spinbox_get_value(timeMinute) == 00) {
            if (lv_spinbox_get_value(timeHour) == 0) {
                lv_spinbox_set_value(timeHour, 23);
            } else {
                lv_spinbox_decrement(timeHour);
            }
            lv_spinbox_set_value(timeMinute, 59);
        } else
            lv_spinbox_decrement(timeMinute);
        timeChanged = true;
    }
}

static void temp_settings_btn(lv_obj_t *obj, lv_event_t event) {
    if (event == LV_EVENT_CLICKED) lv_scr_load(samplingSettingsScr);
}

static void sampling_hourIncrement(lv_obj_t *btn, lv_event_t e) {
    if (e == LV_EVENT_SHORT_CLICKED || e == LV_EVENT_LONG_PRESSED_REPEAT) {
        lv_spinbox_increment(measurePeriodHour);
    }
}

static void sampling_secondIncrement(lv_obj_t *btn, lv_event_t e) {
    if (e == LV_EVENT_SHORT_CLICKED || e == LV_EVENT_LONG_PRESSED_REPEAT) {
        if (lv_spinbox_get_value(measurePeriodsecond) == 59) {
            if (lv_spinbox_get_value(measurePeriodMinute) == 59) {
                if (lv_spinbox_get_value(measurePeriodHour) != 24) {
                    lv_spinbox_set_value(measurePeriodMinute, 0);
                    lv_spinbox_set_value(measurePeriodsecond, 0);
                    lv_spinbox_increment(measurePeriodHour);
                }
            } else {
                lv_spinbox_set_value(measurePeriodsecond, 0);
                lv_spinbox_increment(measurePeriodMinute);
            }
        } else {
            lv_spinbox_increment(measurePeriodsecond);
        }
    }
}

static void sampling_secondDecrement(lv_obj_t *btn, lv_event_t e) {
    if (e == LV_EVENT_SHORT_CLICKED || e == LV_EVENT_LONG_PRESSED_REPEAT) {
        if (lv_spinbox_get_value(measurePeriodsecond) == 0 && lv_spinbox_get_value(measurePeriodMinute) != 0) {
            if ((lv_spinbox_get_value(measurePeriodHour) * 3600 + lv_spinbox_get_value(measurePeriodMinute) * 60 +
                 lv_spinbox_get_value(measurePeriodsecond) - 1) >=
                (lv_spinbox_get_value(turnFanOnTime) +
                 (lv_spinbox_get_value(measureNumber) - 1) * lv_spinbox_get_value(measureAvPeriod))) {
                lv_spinbox_decrement(measurePeriodMinute);
                lv_spinbox_set_value(measurePeriodsecond, 59);
            }
        } else {
            if (lv_spinbox_get_value(measurePeriodsecond) == 0 && lv_spinbox_get_value(measurePeriodMinute) == 0) {
                if (lv_spinbox_get_value(measurePeriodHour) != 0) {
                    if ((lv_spinbox_get_value(measurePeriodHour) * 3600 +
                         lv_spinbox_get_value(measurePeriodMinute) * 60 + lv_spinbox_get_value(measurePeriodsecond) -
                         1) >= (lv_spinbox_get_value(turnFanOnTime) +
                                (lv_spinbox_get_value(measureNumber) - 1) * lv_spinbox_get_value(measureAvPeriod))) {
                        lv_spinbox_decrement(measurePeriodHour);
                        lv_spinbox_set_value(measurePeriodMinute, 59);
                        lv_spinbox_set_value(measurePeriodsecond, 59);
                    }
                }
            } else {
                if ((lv_spinbox_get_value(measurePeriodHour) * 3600 + lv_spinbox_get_value(measurePeriodMinute) * 60 +
                     lv_spinbox_get_value(measurePeriodsecond) - 1) >=
                    (lv_spinbox_get_value(turnFanOnTime) +
                     (lv_spinbox_get_value(measureNumber) - 1) * lv_spinbox_get_value(measureAvPeriod)))
                    lv_spinbox_decrement(measurePeriodsecond);
            }
        }
    }
}

static void sampling_hourDecrement(lv_obj_t *btn, lv_event_t e) {
    if (e == LV_EVENT_SHORT_CLICKED || e == LV_EVENT_LONG_PRESSED_REPEAT) {
        if (((lv_spinbox_get_value(measurePeriodHour) - 1) * 3600) + (lv_spinbox_get_value(measurePeriodMinute) * 60) +
                lv_spinbox_get_value(measurePeriodsecond) >=
            1) {
            if ((lv_spinbox_get_value(measurePeriodHour) * 3600 + lv_spinbox_get_value(measurePeriodMinute) * 60 +
                 lv_spinbox_get_value(measurePeriodsecond) - 3600) >=
                (lv_spinbox_get_value(turnFanOnTime) +
                 (lv_spinbox_get_value(measureNumber) - 1) * lv_spinbox_get_value(measureAvPeriod)))
                lv_spinbox_decrement(measurePeriodHour);
        }
    }
}

static void sampling_minuteIncrement(lv_obj_t *btn, lv_event_t e) {
    if (e == LV_EVENT_SHORT_CLICKED || e == LV_EVENT_LONG_PRESSED_REPEAT) {
        if (lv_spinbox_get_value(measurePeriodMinute) == 59) {
            if (lv_spinbox_get_value(measurePeriodHour) != 24) {
                lv_spinbox_set_value(measurePeriodMinute, 0);
                lv_spinbox_increment(measurePeriodHour);
            }
        } else {
            lv_spinbox_increment(measurePeriodMinute);
        }
    }
}

static void sampling_minuteDecrement(lv_obj_t *btn, lv_event_t e) {
    if (e == LV_EVENT_SHORT_CLICKED || e == LV_EVENT_LONG_PRESSED_REPEAT) {
        if (lv_spinbox_get_value(measurePeriodMinute) == 0) {
            if (lv_spinbox_get_value(measurePeriodHour) != 0)
                if ((lv_spinbox_get_value(measurePeriodHour) * 3600 + lv_spinbox_get_value(measurePeriodMinute) * 60 +
                     lv_spinbox_get_value(measurePeriodsecond) - 60) >=
                    (lv_spinbox_get_value(turnFanOnTime) +
                     (lv_spinbox_get_value(measureNumber) - 1) * lv_spinbox_get_value(measureAvPeriod))) {
                    lv_spinbox_set_value(measurePeriodMinute, 59);
                    lv_spinbox_decrement(measurePeriodHour);
                }
        } else {
            if ((lv_spinbox_get_value(measurePeriodHour) * 3600 + lv_spinbox_get_value(measurePeriodMinute) * 60 +
                 lv_spinbox_get_value(measurePeriodsecond) - 60) >=
                (lv_spinbox_get_value(turnFanOnTime) +
                 (lv_spinbox_get_value(measureNumber) - 1) * lv_spinbox_get_value(measureAvPeriod)))
                lv_spinbox_decrement(measurePeriodMinute);
        }
    }
}

static void sync_rtc_func(lv_obj_t *btn, lv_event_t event) {
    if (event == LV_EVENT_CLICKED) {
        if (WiFi.status() == WL_CONNECTED) config_time();
    }
}

static void calendar_event(lv_obj_t *obj, lv_event_t event) {
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_calendar_date_t *date = lv_calendar_get_pressed_date(obj);
        if (date) {
            Serial.printf("Clicked date: %02d.%02d.%d\n", date->day, date->month, date->year);
            lv_calendar_set_today_date(calendar, date);
            lv_calendar_set_showed_date(calendar, date);
            char buffer[16];
            itoa(date->day, buffer, 10);
            String label;
            if (atoi(buffer) < 10)
                label = '0' + (String)buffer + '.';
            else
                label = (String)buffer + '.';
            itoa(date->month, buffer, 10);
            if (atoi(buffer) < 10)
                label += '0' + (String)buffer + '.';
            else
                label += (String)buffer + '.';
            Serial.println(label);
            itoa(date->year, buffer, 10);
            label += (String)buffer;
            lv_label_set_text(dateBtnLabel, label.c_str());
            lv_obj_del(calendar);
            calendar = NULL;
            dateChanged = true;
        }
    }
}

static void date_button_func(lv_obj_t *btn, lv_event_t event) {
    if (event == LV_EVENT_CLICKED) {
        calendar = lv_calendar_create(timeSettingsScr, NULL);
        lv_obj_set_size(calendar, 235, 235);
        lv_obj_align(calendar, NULL, LV_ALIGN_CENTER, 0, 0);
        lv_obj_set_event_cb(calendar, calendar_event);
        lv_obj_set_style_local_text_font(calendar, LV_CALENDAR_PART_DATE, LV_STATE_DEFAULT, lv_theme_get_font_small());
        String now = lv_label_get_text(dateBtnLabel);
        Serial.println(now);
        Serial.println(now.substring(6).toInt());
        Serial.println(now.substring(4, 5).toInt());
        Serial.println(now.substring(0, 2).toInt());
        lv_calendar_date_t today;
        today.year = atoi(now.substring(6, 10).c_str());
        Serial.println(now.substring(3, 6).toInt());
        today.month = atoi(now.substring(3, 6).c_str());
        today.day = atoi(now.substring(0, 2).c_str());
        lv_calendar_set_today_date(calendar, &today);
        lv_calendar_set_showed_date(calendar, &today);
    }
}

static void measureNumberIncrement_func(lv_obj_t *btn, lv_event_t event) {
    if (event == LV_EVENT_SHORT_CLICKED || event == LV_EVENT_LONG_PRESSED_REPEAT) {
        if ((lv_spinbox_get_value(measurePeriodHour) * 3600 + lv_spinbox_get_value(measurePeriodMinute) * 60 +
             lv_spinbox_get_value(measurePeriodsecond)) >=
            (lv_spinbox_get_value(turnFanOnTime) +
             ((lv_spinbox_get_value(measureNumber) - 1) * lv_spinbox_get_value(measureAvPeriod)))) {
            set_spinbox_digit_format(measureNumber, MIN_RANGE, MAX_RANGE, 1);
            lv_spinbox_increment(measureNumber);
        }
    }
}

static void measureNumberDecrement_func(lv_obj_t *btn, lv_event_t event) {
    if (event == LV_EVENT_SHORT_CLICKED || event == LV_EVENT_LONG_PRESSED_REPEAT) {
        set_spinbox_digit_format(measureNumber, MIN_RANGE, MAX_RANGE, -1);
        lv_spinbox_decrement(measureNumber);
    }
}

static void turnFanOnTimeIncrement_func(lv_obj_t *btn, lv_event_t event) {
    if (event == LV_EVENT_SHORT_CLICKED || event == LV_EVENT_LONG_PRESSED_REPEAT) {
        if ((lv_spinbox_get_value(measurePeriodHour) * 3600 + lv_spinbox_get_value(measurePeriodMinute) * 60 +
             lv_spinbox_get_value(measurePeriodsecond)) >=
            (lv_spinbox_get_value(turnFanOnTime) + 1 +
             (lv_spinbox_get_value(measureNumber) - 1) * lv_spinbox_get_value(measureAvPeriod))) {
            set_spinbox_digit_format(turnFanOnTime, MIN_RANGE, MAX_RANGE, 1);
            lv_spinbox_increment(turnFanOnTime);
        }
    }
}

static void turnFanOnTimeDecrement_func(lv_obj_t *btn, lv_event_t event) {
    if (event == LV_EVENT_SHORT_CLICKED || event == LV_EVENT_LONG_PRESSED_REPEAT) {
        set_spinbox_digit_format(turnFanOnTime, MIN_RANGE, MAX_RANGE, -1);
        lv_spinbox_decrement(turnFanOnTime);
    }
}

static void av_periodIncrement(lv_obj_t *btn, lv_event_t event) {
    if (event == LV_EVENT_SHORT_CLICKED || event == LV_EVENT_LONG_PRESSED_REPEAT) {
        if ((lv_spinbox_get_value(measurePeriodHour) * 3600 + lv_spinbox_get_value(measurePeriodMinute) * 60 +
             lv_spinbox_get_value(measurePeriodsecond)) >=
            (lv_spinbox_get_value(turnFanOnTime) +
             (lv_spinbox_get_value(measureNumber) - 1) * (lv_spinbox_get_value(measureAvPeriod) + 1))) {
            set_spinbox_digit_format(measureAvPeriod, MIN_RANGE, MAX_RANGE, 1);
            lv_spinbox_increment(measureAvPeriod);
        }
    }
}

static void av_periodDecrement(lv_obj_t *btn, lv_event_t event) {
    if (event == LV_EVENT_SHORT_CLICKED || event == LV_EVENT_LONG_PRESSED_REPEAT) {
        set_spinbox_digit_format(measureAvPeriod, MIN_RANGE, MAX_RANGE, -1);
        lv_spinbox_decrement(measureAvPeriod);
    }
}

static void sampling_settings_save_btn(lv_obj_t *btn, lv_event_t event) {
    if (event == LV_EVENT_SHORT_CLICKED || event == LV_EVENT_LONG_PRESSED_REPEAT) {
        int get_value = lv_spinbox_get_value(measurePeriodHour) * 60 * 60000 +
                        lv_spinbox_get_value(measurePeriodMinute) * 60000 +
                        lv_spinbox_get_value(measurePeriodsecond) * 1000;
        Global::config.timeBetweenSavingSamples = get_value;
        Global::config.numberOfSamples = lv_spinbox_get_value(measureNumber);
        Global::config.measurePeriod = lv_spinbox_get_value(measureAvPeriod) * 1000;
        Global::config.turnFanTime = lv_spinbox_get_value(turnFanOnTime) * 1000;
        getSample = lv_task_create(getSampleFunc,
                                   (Global::config.timeBetweenSavingSamples -
                                    (Global::config.numberOfSamples - 1) * Global::config.measurePeriod),
                                   LV_TASK_PRIO_MID, NULL);
        turnFanOn = lv_task_create(turnFanOnFunc, Global::config.timeBetweenSavingSamples - Global::config.turnFanTime,
                                   LV_TASK_PRIO_MID, NULL);
        Peripherals::mySDCard.saveConfig(Global::config, Global::configFilePath);
        Peripherals::mySDCard.printConfig(Global::configFilePath);
        lv_scr_load(Screens::mainScr->getScreen());
    }
}

static void sampling_settings_back_btn(lv_obj_t *btn, lv_event_t event) {
    if (event == LV_EVENT_SHORT_CLICKED || event == LV_EVENT_LONG_PRESSED_REPEAT) {
        lv_spinbox_set_value(measurePeriodHour, ((Global::config.timeBetweenSavingSamples / 60000) / 60));
        lv_spinbox_set_value(measurePeriodMinute, ((Global::config.timeBetweenSavingSamples / 60000) % 60));
        lv_scr_load(Screens::settingsScr->getScreen());
    }
}