#include <GlobalVariables.hpp>


void display_current_config()
{
    String current_config= (String)"SSID: " + config.ssid.c_str();
    current_config+= (String)"\nCount of Samples: " + (String)config.countOfSamples ;
    if(config.lcdLockTime==-1)
        current_config+= "\nLCD lock time: Never" ; 
    if(config.lcdLockTime==30000)
        current_config+= "\nLCD lock time: 30 secs";
    if(config.lcdLockTime>30000)
        current_config += "\nLCD lock time: " + (String)(config.lcdLockTime/60000) + " mins";
    current_config += (String)"\nMeasure period: " + config.measurePeriod/1000 + " sec\nTime between saving sample: ";
    if(config.timeBetweenSavingSample>=3600000)
        current_config+= config.timeBetweenSavingSample/ 60000 / 60 + (String)"h " + (config.timeBetweenSavingSample/60000 )%60 + (String)"min\nTime offset: ";
    else
        current_config+= (config.timeBetweenSavingSample/60000 )%60 + (String)"min\nTime offset: ";
    if(ntpTimeOffset<0)
        current_config+="-";
    if(ntpTimeOffset>0)
        current_config+="+";   
    current_config += ntpTimeOffset/3600;
    lv_label_set_text(config_label, current_config.c_str());
}

//Function that turns fan on
void turnFanOnFunc(lv_task_t *task)
{
    digitalWrite(FAN_PIN, HIGH);
    lv_task_set_prio(turnFanOn, LV_TASK_PRIO_OFF);
}

bool isLastSampleSaved()
{
    StaticJsonDocument<600> docA;
    JsonArray lastRecordToCheck = docA.to<JsonArray>();
    mySDCard.getLastRecord(&sampleDB, &Serial, &lastRecordToCheck);
    Serial.print("Global: ");
    Serial.print(lastSampleTimestamp);
    Serial.print(" Baza: ");
    Serial.print(lastRecordToCheck[0]["timestamp"].as<String>());
    if (lastSampleTimestamp == lastRecordToCheck[0]["timestamp"].as<String>())
        return true;
    else
        return false;
}

//Check pm2,5ug/m3 value and set status (text and color at main screen)
void setAqiStateNColor()
{
    for (int i = 0; i < 6; i++)
    {
        if (i == 5 or pm25Aqi < aqiStandards[i])
        {
            lv_label_set_text(labelAQIColorBar, airQualityStates[i].c_str());
            lv_obj_set_style_local_bg_color(contAQIColorBar, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, airQualityColors[i]);
            return;
        }
    }
}

//Get single sample and set text
void getSampleFunc(lv_task_t *task)
{
    sht30.get();
    if (wasUpdated != true)
    {
        lv_task_ready(syn_rtc);
        wasUpdated = true;
    }
    if (config.currentSampleNumber == 0)
    {
        if (pmsSensor->readData())
        {
            std::map<std::string, uint16_t> tmpData = pmsSensor->returnData();
            pmsSensor->dumpSamples();
            data = tmpData;
            config.currentSampleNumber++;
            lv_task_set_period(getSample, config.measurePeriod);
            temp = sht30.cTemp;
            humi = sht30.humidity;
        }
    }
    if (config.currentSampleNumber != 0 && config.currentSampleNumber < config.countOfSamples)
    {
        std::map<std::string, uint16_t> tmpData = pmsSensor->returnData();
        pmsSensor->dumpSamples();
        for (uint8_t i = 0; i < 15; i++)
        {
            data[labels[i]] += tmpData[labels[i]];
        }
        config.currentSampleNumber++;
        temp += sht30.cTemp;
        humi += sht30.humidity;
    }
    if (config.currentSampleNumber == config.countOfSamples)
    {
        char buffer[7];
        for (uint8_t i = 0; i < 15; i++)
            data[labels[i]] = data[labels[i]] / config.countOfSamples;
        config.currentSampleNumber = 0;
        temp = temp / config.countOfSamples;
        humi = humi / config.countOfSamples;
        lv_task_set_period(getSample, config.timeBetweenSavingSample);

        itoa(data["pm10_standard"], buffer, 10);
        lv_label_set_text(labelPM10Data, buffer);

        itoa(data["pm25_standard"], buffer, 10);
        pm25Aqi = data["pm25_standard"];
        lv_label_set_text(labelPM25Data, buffer);
        setAqiStateNColor();

        itoa(data["pm100_standard"], buffer, 10);
        lv_label_set_text(labelPM100Data, buffer);

        itoa(data["particles_05um"], buffer, 10);
        lv_label_set_text(labelParticlesNumber[0], buffer);

        itoa(data["particles_10um"], buffer, 10);
        lv_label_set_text(labelParticlesNumber[1], buffer);

        itoa(data["particles_25um"], buffer, 10);
        lv_label_set_text(labelParticlesNumber[2], buffer);

        itoa(data["particles_50um"], buffer, 10);
        lv_label_set_text(labelParticlesNumber[3], buffer);

        itoa(data["particles_100um"], buffer, 10);
        lv_label_set_text(labelParticlesNumber[4], buffer);

        dtostrf(temp, 10, 2, buffer);
        lv_label_set_text(labelTempValue, strcat(buffer, "°C"));

        dtostrf(humi, 10, 2, buffer);
        lv_label_set_text(labelHumiValue, strcat(buffer, "%"));
        lastSampleTimestamp = getMainTimestamp(Rtc);
        Serial.print("lastSampleTimestamp przed wrzuceniem do bazy: " + lastSampleTimestamp);
        mySDCard.save(data, temp, humi, lastSampleTimestamp, &sampleDB, &Serial);
        lv_task_reset(turnFanOn);
        lv_task_set_prio(turnFanOn, LV_TASK_PRIO_HIGHEST);
        digitalWrite(FAN_PIN, LOW);
        if (isLastSampleSaved())
        {
            lv_obj_set_style_local_bg_color(ledAtLock, LV_LED_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_GREEN);
            lv_obj_set_style_local_bg_color(ledAtMain, LV_LED_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_GREEN);
            lv_obj_set_style_local_shadow_color(ledAtLock, LV_LED_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_GREEN);
            lv_obj_set_style_local_shadow_color(ledAtMain, LV_LED_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_GREEN);
        }
        else
        {
            lv_obj_set_style_local_bg_color(ledAtLock, LV_LED_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);
            lv_obj_set_style_local_bg_color(ledAtMain, LV_LED_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);
            lv_obj_set_style_local_shadow_color(ledAtLock, LV_LED_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);
            lv_obj_set_style_local_shadow_color(ledAtMain, LV_LED_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);
        }
    }
}

//Draw a line-like thing
void drawParticlesIndicator()
{
    for (int i = 0; i < 6; i++)
    {
        dividingLines[i] = lv_line_create(main_scr, NULL);
        lv_line_set_points(dividingLines[i], dividingLinesPoints[i], 2);
        lv_obj_add_style(dividingLines[i], LV_LINE_PART_MAIN, &lineStyle);
        labelParticleSizeum[i] = lv_label_create(main_scr, NULL);
        lv_label_set_text(labelParticleSizeum[i], particlesSize[i].c_str());
        lv_obj_add_style(labelParticleSizeum[i], LV_LABEL_PART_MAIN, &font12Style);
        //lv_obj_set_auto_realign(labelParticleSizeum[i], true);
        //lv_obj_align_origo(labelParticleSizeum[i], dividingLines[i], LV_ALIGN_CENTER, 0, 0);
        lv_obj_set_pos(labelParticleSizeum[i], labelParticleSizePosX[i], 190); //12
    }

    for (int j = 0; j < 5; j++)
    {
        contParticlesNumber[j] = lv_cont_create(main_scr, NULL);
        lv_obj_add_style(contParticlesNumber[j], LV_OBJ_PART_MAIN, &containerStyle);
        lv_obj_set_style_local_border_opa(contParticlesNumber[j], LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_0);
        lv_obj_set_click(contParticlesNumber[j], false);
        lv_obj_set_size(contParticlesNumber[j], 47, 14);
        labelParticlesNumber[j] = lv_label_create(contParticlesNumber[j], NULL);
        lv_obj_set_pos(contParticlesNumber[j], 65 + j * 46, 215); //20
        lv_label_set_align(labelParticlesNumber[j], LV_LABEL_ALIGN_CENTER);
        lv_obj_set_auto_realign(labelParticlesNumber[j], true);
        lv_label_set_text(labelParticlesNumber[j], "-");
        lv_obj_add_style(labelParticlesNumber[j], LV_LABEL_PART_MAIN, &font12Style);
    }

    mainLine = lv_line_create(main_scr, NULL);
    lv_line_set_points(mainLine, mainLinePoints, 2);
    lv_line_set_auto_size(mainLine, true);
    lv_obj_add_style(mainLine, LV_LINE_PART_MAIN, &lineStyle);

    labelSizeTitle = lv_label_create(main_scr, NULL);
    lv_obj_set_pos(labelSizeTitle, 10, 190);
    lv_label_set_text(labelSizeTitle, "Size");
    lv_obj_add_style(labelSizeTitle, LV_OBJ_PART_MAIN, &font12Style);

    labelNumberTitle = lv_label_create(main_scr, NULL);
    lv_obj_set_pos(labelNumberTitle, 10, 215);
    lv_label_set_text(labelNumberTitle, "Number");
    lv_obj_add_style(labelNumberTitle, LV_OBJ_PART_MAIN, &font12Style);
}

static void WiFi_SSID(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED)
    {
        lv_textarea_set_text(ssid_ta, lv_list_get_btn_text(obj));
        lv_scr_load(wifi_scr);
    }
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
    if (event == LV_EVENT_CLICKED and ((lv_textarea_get_text(ssid_ta) != NULL and lv_textarea_get_text(ssid_ta) != '\0') or (lv_textarea_get_text(pwd_ta) != NULL and lv_textarea_get_text(pwd_ta) != '\0')))
    {
        uint8_t wifiAttempts = 10;

        config.ssid = lv_textarea_get_text(ssid_ta);
        Serial.println(config.ssid.c_str());
        config.password = lv_textarea_get_text(pwd_ta);

        mySDCard.saveConfig(config, configFilePath);
        mySDCard.printConfig(configFilePath);
        WiFi.begin(config.ssid.c_str(), config.password.c_str());
        while (WiFi.status() != WL_CONNECTED and wifiAttempts > 0)
        {
            delay(500);
            wifiAttempts--;
        }

        if (WiFi.status() == WL_CONNECTED)
        {
            Rtc.SetIsRunning(true);
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
    if (event == LV_EVENT_CLICKED)
        lv_disp_load_scr(settings_scr);
}

//Locking button clicked
static void lockButton_task(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED)
        lv_disp_load_scr(lock_scr);
}

//Unlocking button clicked
static void unlockButton_task(lv_obj_t *obj, lv_event_t event)
{
    Serial.print(lv_btn_get_state(unlockButton));
    if (event == LV_EVENT_CLICKED)
        lv_disp_load_scr(main_scr);
}

//Exit from wifi settings button clicked
static void btn_cancel(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED)
    {
        lv_disp_load_scr(wifilist_scr);
        lv_textarea_set_text(ssid_ta, "");
        lv_textarea_set_text(pwd_ta, "");
    }
}

static void btn_settings_back(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED)
        lv_disp_load_scr(main_scr);
}

void startbar()
{
    loading_bar = lv_bar_create(wifilist_scr, NULL);
    lv_obj_set_size(loading_bar, 175, 20);
    lv_obj_set_pos(loading_bar, 5, 205);
    lv_bar_set_anim_time(loading_bar, 10500);
    lv_bar_set_value(loading_bar, 100, LV_ANIM_ON);
}

static void WiFi_btn(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED)
    {
        lv_list_clean(wifiList);
        lv_scr_load(wifilist_scr);
        lv_task_set_prio(listNetwork_task, LV_TASK_PRIO_MID);
        lv_task_reset(listNetwork_task);
        startbar();
    }
}

static void refresh_btn_task(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED)
    {
        lv_list_clean(wifiList);
        lv_task_set_prio(listNetwork_task, LV_TASK_PRIO_MID);
        lv_task_reset(listNetwork_task);
        startbar();
    }
}

static void info_btn(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED)
        lv_scr_load(info_scr);
}

static void time_settings_btn(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED)
    {
        lv_scr_load(time_settings_scr);
        in_time_settings = true;
    }
}

void timesettings_back_btn(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED)
    {
        switch (config.lcdLockTime)
        {
        case -1:
            lv_dropdown_set_selected(lockScreenDDlist, 7);
            break;
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
        default:
            lv_dropdown_set_selected(lockScreenDDlist, 1);
            break;
        }
        lv_scr_load(settings_scr);
        in_time_settings = false;
        time_changed = false;
        date_changed = false;
    }
}

void timesettings_save_btn(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED)
    {
        switch (lv_dropdown_get_selected(lockScreenDDlist))
            {
            case 0:
                config.lcdLockTime = 30000;
                break;
            case 1:
                config.lcdLockTime = 60000;
                break;
            case 2:
                config.lcdLockTime = 120000;
                break;
            case 3:
                config.lcdLockTime = 300000;
                break;
            case 4:
                config.lcdLockTime = 600000;
                break;
            case 5:
                config.lcdLockTime = 1800000;
                break;
            case 6:
                config.lcdLockTime = 3600000;
                break;
            case 7:
                config.lcdLockTime = -1;
                break;
            default:
                config.lcdLockTime = 60000;
                break;
            }
            mySDCard.saveConfig(config, configFilePath);
            mySDCard.printConfig(configFilePath);
            if (time_changed == true)
            {
                String datet = lv_label_get_text(date_btn_label) + (String)lv_textarea_get_text(time_hour) + ":" + (String)lv_textarea_get_text(time_minute);
                Serial.println(datet);
                RtcDateTime *dt = new RtcDateTime(atoi(datet.substring(6, 10).c_str()), atoi(datet.substring(3, 6).c_str()), atoi(datet.substring(0, 2).c_str()), datet.substring(10, 12).toDouble(), datet.substring(13, 15).toDouble(), 0);
                Rtc.SetDateTime(*dt);
                Rtc.SetMemory(1, 1);
            }
            if (date_changed == true)
            {
                RtcDateTime ori = Rtc.GetDateTime();
                String date = lv_label_get_text(date_btn_label);
                RtcDateTime *dt = new RtcDateTime(atoi(date.substring(6).c_str()), atoi(date.substring(3, 6).c_str()), atoi(date.substring(0, 2).c_str()), ori.Hour(), ori.Minute(), ori.Second());
                Rtc.SetDateTime(*dt);
                Rtc.SetMemory(1, 1);
            }
            lv_disp_load_scr(main_scr);
            in_time_settings = false;
            time_changed = false;
            date_changed = false;
            display_current_config();
    }
}

static void hour_increment(lv_obj_t *btn, lv_event_t e)
{
    if (e == LV_EVENT_SHORT_CLICKED || e == LV_EVENT_LONG_PRESSED_REPEAT)
    {
        if (lv_spinbox_get_value(time_hour) == 23)
        {
            lv_spinbox_set_value(time_hour, 0);
        }
        else
        {
            lv_spinbox_increment(time_hour);
        }
        time_changed = true;
    }
}

static void hour_decrement(lv_obj_t *btn, lv_event_t e)
{
    if (e == LV_EVENT_SHORT_CLICKED || e == LV_EVENT_LONG_PRESSED_REPEAT)
    {
        if (lv_spinbox_get_value(time_hour) == 0)
            lv_spinbox_set_value(time_hour, 23);
        else
            lv_spinbox_decrement(time_hour);
        time_changed = true;
    }
}

static void minute_increment(lv_obj_t *btn, lv_event_t e)
{
    if (e == LV_EVENT_SHORT_CLICKED || e == LV_EVENT_LONG_PRESSED_REPEAT)
    {
        if (lv_spinbox_get_value(time_minute) == 59)
        {
            if (lv_spinbox_get_value(time_hour) == 23)
            {
                lv_spinbox_set_value(time_hour, 0);
            }
            else
            {
                lv_spinbox_increment(time_hour);
            }
            lv_spinbox_set_value(time_minute, 0);
        }
        else
            lv_spinbox_increment(time_minute);
        time_changed = true;
    }
}

static void minute_decrement(lv_obj_t *btn, lv_event_t e)
{
    if (e == LV_EVENT_SHORT_CLICKED || e == LV_EVENT_LONG_PRESSED_REPEAT)
    {
        if (lv_spinbox_get_value(time_minute) == 00)
        {
            if (lv_spinbox_get_value(time_hour) == 0)
            {
                lv_spinbox_set_value(time_hour, 23);
            }
            else
            {
                lv_spinbox_decrement(time_hour);
            }
            lv_spinbox_set_value(time_minute, 59);
        }
        else
            lv_spinbox_decrement(time_minute);
        time_changed = true;
    }
}

static void temp_settings_btn(lv_obj_t *obj, lv_event_t event)
{
    if(event == LV_EVENT_CLICKED)
        lv_scr_load(sampling_settings_scr);
}

static void sampling_hour_increment(lv_obj_t *btn, lv_event_t e)
{
    if (e == LV_EVENT_SHORT_CLICKED || e == LV_EVENT_LONG_PRESSED_REPEAT)
    {
        lv_spinbox_increment(measure_period_hour);
    }
}

static void sampling_hour_decrement(lv_obj_t *btn, lv_event_t e)
{
    if (e == LV_EVENT_SHORT_CLICKED || e == LV_EVENT_LONG_PRESSED_REPEAT)
    {
        lv_spinbox_decrement(measure_period_hour);
    }
}

static void sampling_minute_increment(lv_obj_t *btn, lv_event_t e)
{
    if (e == LV_EVENT_SHORT_CLICKED || e == LV_EVENT_LONG_PRESSED_REPEAT)
    {
        lv_spinbox_increment(measure_period_minute);
    }
}

static void sampling_minute_decrement(lv_obj_t *btn, lv_event_t e)
{
    if (e == LV_EVENT_SHORT_CLICKED || e == LV_EVENT_LONG_PRESSED_REPEAT)
    {
        lv_spinbox_decrement(measure_period_minute);
    }
}

static void sync_rtc_func(lv_obj_t *btn, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED)
    {
        if (WiFi.status() == WL_CONNECTED)
        {
            if (Ping.ping(ntpServerName, 1))
            {
                alertBox = lv_msgbox_create(time_settings_scr, NULL);
                lv_obj_add_style(alertBox, LV_STATE_DEFAULT, &toastListStyle);
                lv_msgbox_set_text(alertBox, "Time synchronized");
                lv_msgbox_set_anim_time(alertBox, 0);
                lv_msgbox_start_auto_close(alertBox, 5000);
                lv_obj_align(alertBox, NULL, LV_ALIGN_CENTER, 0, 0);
                lv_task_ready(syn_rtc);
            }
            else
            {
                alertBox = lv_msgbox_create(time_settings_scr, NULL);
                lv_obj_add_style(alertBox, LV_STATE_DEFAULT, &toastListStyle);
                lv_msgbox_set_text(alertBox, "No internet connection.");
                lv_msgbox_set_anim_time(alertBox, 0);
                lv_msgbox_start_auto_close(alertBox, 5000);
                lv_obj_align(alertBox, NULL, LV_ALIGN_CENTER, 0, 0);
            }
        }
        else
        {
            alertBox = lv_msgbox_create(time_settings_scr, NULL);
            lv_obj_add_style(alertBox, LV_STATE_DEFAULT, &toastListStyle);
            lv_msgbox_set_text(alertBox, "No WiFi connection.");
            lv_msgbox_set_anim_time(alertBox, 0);
            lv_msgbox_start_auto_close(alertBox, 5000);
            lv_obj_align(alertBox, NULL, LV_ALIGN_CENTER, 0, 0);
        }
    }
}

static void calendar_event(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_VALUE_CHANGED)
    {
        lv_calendar_date_t *date = lv_calendar_get_pressed_date(obj);
        if (date)
        {
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
            lv_label_set_text(date_btn_label, label.c_str());
            lv_obj_del(calendar);
            calendar = NULL;
            date_changed = true;
        }
    }
}

static void date_button_func(lv_obj_t *btn, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED)
    {
        calendar = lv_calendar_create(time_settings_scr, NULL);
        lv_obj_set_size(calendar, 235, 235);
        lv_obj_align(calendar, NULL, LV_ALIGN_CENTER, 0, 0);
        lv_obj_set_event_cb(calendar, calendar_event);
        lv_obj_set_style_local_text_font(calendar, LV_CALENDAR_PART_DATE, LV_STATE_DEFAULT, lv_theme_get_font_small());
        String now = lv_label_get_text(date_btn_label);
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

static void show_hide_btn_func(lv_obj_t *btn, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED)
    {
        if (lv_textarea_get_pwd_mode(pwd_ta))
        {
            lv_textarea_set_pwd_mode(pwd_ta, false);
            lv_label_set_text(show_hide_btn_label, LV_SYMBOL_EYE_CLOSE);
        }
        else
        {
            lv_textarea_set_pwd_mode(pwd_ta, true);
            lv_textarea_set_pwd_show_time(pwd_ta, 1);
            lv_textarea_set_text(pwd_ta, lv_textarea_get_text(pwd_ta));
            lv_textarea_set_pwd_show_time(pwd_ta, 5000);
            lv_label_set_text(show_hide_btn_label, LV_SYMBOL_EYE_OPEN);
        }
    }
}

static void measure_number_increment_func(lv_obj_t *btn, lv_event_t event)
{
    if (event == LV_EVENT_SHORT_CLICKED || event == LV_EVENT_LONG_PRESSED_REPEAT)
        lv_spinbox_increment(measure_number);
}

static void measure_number_decrement_func(lv_obj_t *btn, lv_event_t event)
{
    if (event == LV_EVENT_SHORT_CLICKED || event == LV_EVENT_LONG_PRESSED_REPEAT)
        lv_spinbox_decrement(measure_number);
}

static void av_period_increment(lv_obj_t *btn, lv_event_t event)
{
    if (event == LV_EVENT_SHORT_CLICKED || event == LV_EVENT_LONG_PRESSED_REPEAT)
        lv_spinbox_increment(measure_av_period);
}

static void av_period_decrement(lv_obj_t *btn, lv_event_t event)
{
    if (event == LV_EVENT_SHORT_CLICKED || event == LV_EVENT_LONG_PRESSED_REPEAT)
        lv_spinbox_decrement(measure_av_period);
}

static void sampling_settings_save_btn(lv_obj_t *btn, lv_event_t event)
{
    if (event == LV_EVENT_SHORT_CLICKED || event == LV_EVENT_LONG_PRESSED_REPEAT)
    {
        int get_value = lv_spinbox_get_value(measure_period_hour) * 60 * 60000 + lv_spinbox_get_value(measure_period_minute) * 60000;
        if (get_value < 300000)
        {
            alertBox = lv_msgbox_create(sampling_settings_scr, NULL);
            lv_obj_add_style(alertBox, LV_STATE_DEFAULT, &toastListStyle);
            lv_msgbox_set_text(alertBox, "The minimum required sampling time is 5 mins.");
            lv_msgbox_set_anim_time(alertBox, 0);
            lv_msgbox_start_auto_close(alertBox, 5000);
            lv_obj_align(alertBox, NULL, LV_ALIGN_CENTER, 0, 0);
        }
        else
        {
            config.timeBetweenSavingSample = get_value;
            config.countOfSamples = lv_spinbox_get_value(measure_number);
            config.measurePeriod = lv_spinbox_get_value(measure_av_period) * 1000;
            getSample = lv_task_create(getSampleFunc, config.timeBetweenSavingSample, LV_TASK_PRIO_HIGH, NULL);
            turnFanOn = lv_task_create(turnFanOnFunc, config.timeBetweenSavingSample - turnFanTime, LV_TASK_PRIO_HIGHEST, NULL);
            mySDCard.saveConfig(config, configFilePath);
            mySDCard.printConfig(configFilePath);
            lv_scr_load(main_scr);
        }
    }
}

static void sampling_settings_back_btn(lv_obj_t *btn, lv_event_t event)
{
    if (event == LV_EVENT_SHORT_CLICKED || event == LV_EVENT_LONG_PRESSED_REPEAT)
    {
        lv_spinbox_set_value(measure_period_hour, ((config.timeBetweenSavingSample / 60000) / 60));
        lv_spinbox_set_value(measure_period_minute, ((config.timeBetweenSavingSample / 60000) % 60));
        lv_scr_load(settings_scr);
    }
}