#include <GlobalVariables.hpp>

void set_spinbox_digit_format(lv_obj_t *spinbox, int32_t range_min, int32_t range_max, int offset)
{
    int _spinbox_value = lv_spinbox_get_value(spinbox) + offset;
    if(_spinbox_value>9){
        if(_spinbox_value>99){
        lv_spinbox_set_digit_format(spinbox, 3, 0);
        }
        else{
        lv_spinbox_set_digit_format(spinbox, 2, 0);
        }
    }
    else
    {
    lv_spinbox_set_digit_format(spinbox, 1, 0);
    }
    lv_spinbox_set_range(spinbox, range_min, range_max);
}

lv_obj_t * my_lv_btn_create(lv_obj_t * par, const lv_obj_t * copy, lv_coord_t width, lv_coord_t height, lv_coord_t x_position, lv_coord_t y_position, lv_event_cb_t event_cb)
{
    lv_obj_t * btn = lv_btn_create(par, copy);
    lv_obj_set_size(btn, width, height);
    lv_obj_set_pos(btn, x_position, y_position);
    lv_obj_set_event_cb(btn, event_cb);
    return btn;
}

lv_obj_t * my_lv_cont_create(lv_obj_t * par, const lv_obj_t * copy, lv_coord_t width, lv_coord_t height, lv_coord_t x_position, lv_coord_t y_position)
{
    lv_obj_t * cont = lv_cont_create(par, copy);
    lv_obj_set_size(cont, width, height);
    lv_obj_set_pos(cont, x_position, y_position);
    return cont;
}

lv_obj_t * my_lv_label_create(lv_obj_t * par, const lv_obj_t * copy, lv_coord_t x_position, lv_coord_t y_position, const char * text="", lv_color_t color=LV_COLOR_WHITE)
{
    lv_obj_t * new_label = lv_label_create(par, copy);
    lv_obj_set_pos(new_label, x_position, y_position);
    lv_label_set_text(new_label, text);
    lv_obj_set_style_local_text_color(new_label, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, color);

    return new_label;
}



int getDDListIndexBasedOnLcdLockTime(int lcdLockTime)
{
    switch (lcdLockTime)
    {
    case -1:
    {
        return 7;
        break;
    }

    case 30000:
    {
        return 0;
        break;
    }
    case 60000:
    {
        return 1;
        break;
    }
    case 120000:
    {
        return 2;
        break;
    }
    case 300000:
    {
        return 3;
        break;
    }
    case 600000:
    {
        return 4;
        break;
    }
    case 1800000:
    {
        return 5;
        break;
    }
    case 3600000:
    {
        return 6;
        break;
    }
    default:
    {
        return 1;
    }
    }
}

void display_current_config()
{
    String current_config = (String) "SSID: " + config.ssid.c_str();
    current_config += (String) "\nNumber of samples: " + (String)config.countOfSamples;
    if (config.lcdLockTime == -1)
        current_config += "\nLCD lock time: Never";
    if (config.lcdLockTime == 30000)
        current_config += "\nLCD lock time: 30s";
    if (config.lcdLockTime > 30000)
        current_config += "\nLCD lock time: " + (String)(config.lcdLockTime / 60000) + "m";
    current_config += (String) "\nFan running time before measure: " + config.turnFanTime / 1000 + "s\n";
    current_config += (String) "Time between measurments: " + config.measurePeriod / 1000 + "s\nMeasurements saving time: ";
    if (config.timeBetweenSavingSample >= 3600000)
        current_config += config.timeBetweenSavingSample / 60000 / 60 + (String) "h" + (config.timeBetweenSavingSample / 60000) % 60 + (String) "m" + (config.timeBetweenSavingSample / 1000) % 60 + "s";
    else if (config.timeBetweenSavingSample >= 1000)
    {
        current_config += (config.timeBetweenSavingSample / 60000) % 60 + (String) "m " + (config.timeBetweenSavingSample / 1000) % 60 + "s";
    }
    else
    {
        current_config += (config.timeBetweenSavingSample / 1000) + (String) "s";
    }
    lv_obj_set_style_local_text_font(configLabel, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_14);
    lv_label_set_text(configLabel, current_config.c_str());

}

//Function that turns fan on
void turnFanOnFunc(lv_task_t *task)
{
    digitalWrite(FAN_PIN, HIGH);
    lv_task_set_prio(turnFanOn, LV_TASK_PRIO_OFF);
}

void config_time()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        for (int i = 0; i < 500; i++)
            dateTimeClient.update();
        configTime(Rtc, dateTimeClient);
        Serial.println("Succesfully updated time on RTC.");
    }
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
    {
        Serial.println("Last sample has been saved correctly - return true.");
        return true;
    }
    else
    {
        Serial.println("Something went wrong saving last sample - return false");
        return false;
    }
}

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
    if (config.currentSampleNumber != 0 && config.currentSampleNumber < config.countOfSamples)
    {
        if(pmsSensor->readData())
        {
            Serial.println("Succesfully read data from dust sensor.");
            std::map<std::string, float> tmpData = pmsSensor->returnData();
            pmsSensor->dumpSamples();
            for (uint8_t i = 0; i < 15; i++)
                {
                    data[labels[i]] += tmpData[labels[i]];
                }
                config.currentSampleNumber++;
                temp += sht30.cTemp;
                humi += sht30.humidity;
        }
    }
    if (config.currentSampleNumber == 0)
    {
        lv_task_set_period(getSample, config.measurePeriod);
        if (pmsSensor->readData())
        {
            Serial.println("Succesfully read data from dust sensor.");
            std::map<std::string, float> tmpData = pmsSensor->returnData();
            pmsSensor->dumpSamples();
            data = tmpData;
            config.currentSampleNumber++;
            temp = sht30.cTemp;
            humi = sht30.humidity;
        }
    }
    if (config.currentSampleNumber == config.countOfSamples)
    {
        char buffer[7];
        for (uint8_t i = 0; i < 15; i++)
            data[labels[i]] = data[labels[i]] / config.countOfSamples;
        config.currentSampleNumber = 0;
        temp = temp / config.countOfSamples;
        humi = humi / config.countOfSamples;
        lv_task_set_period(getSample, (config.timeBetweenSavingSample-config.countOfSamples*config.measurePeriod));

        itoa(data["pm10_standard"], buffer, 10);
        lv_label_set_text(labelPM10Data, buffer);

        itoa(data["pm25_standard"], buffer, 10);
        pm25Aqi = data["pm25_standard"];
        lv_label_set_text(labelPM25Data, buffer);
        setAqiStateNColor();

        itoa(data["pm100_standard"], buffer, 10);
        lv_label_set_text(labelPM100Data, buffer);

        itoa(data["particles_03um"], buffer, 10);
        lv_label_set_text(labelParticlesNumber[0], buffer);

        itoa(data["particles_05um"], buffer, 10);
        lv_label_set_text(labelParticlesNumber[1], buffer);

        itoa(data["particles_10um"], buffer, 10);
        lv_label_set_text(labelParticlesNumber[2], buffer);

        itoa(data["particles_25um"], buffer, 10);
        lv_label_set_text(labelParticlesNumber[3], buffer);

        itoa(data["particles_50um"], buffer, 10);
        lv_label_set_text(labelParticlesNumber[4], buffer);

        itoa(data["particles_100um"], buffer, 10);
        lv_label_set_text(labelParticlesNumber[5], buffer);

        dtostrf(temp, 10, 2, buffer);
        lv_label_set_text(labelTempValue, strcat(buffer, "°C"));

        dtostrf(humi, 10, 2, buffer);
        lv_label_set_text(labelHumiValue, strcat(buffer, "%"));
        if(Rtc.GetIsRunning())
        {
            lastSampleTimestamp = getMainTimestamp(Rtc);
            Serial.print("lastSampleTimestamp przed wrzuceniem do bazy: " + lastSampleTimestamp);
            mySDCard.save(data, temp, humi, lastSampleTimestamp, &sampleDB, &Serial);
        }
        else
            Serial.println("RTC is not running, not saving");        
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
    for (int i = 0; i < 7; i++)
    {
        dividingLines[i] = lv_line_create(mainScr, NULL);
        lv_line_set_points(dividingLines[i], dividingLinesPoints[i], 2);
        lv_obj_add_style(dividingLines[i], LV_LINE_PART_MAIN, &lineStyle);

        labelParticleSizeum[i] = lv_label_create(mainScr, NULL);
        lv_label_set_text(labelParticleSizeum[i], particlesSize[i].c_str());
        lv_obj_add_style(labelParticleSizeum[i], LV_LABEL_PART_MAIN, &font12Style);
        lv_obj_add_style(labelParticleSizeum[i], LV_LABEL_PART_MAIN, &whiteFontStyle);
        lv_obj_set_pos(labelParticleSizeum[i], labelParticleSizePosX[i], 190); //12
    }

    for (int j = 0; j < 6; j++)
    {
        
        contParticlesNumber[j] = lv_cont_create(mainScr, NULL);
        lv_obj_add_style(contParticlesNumber[j], LV_OBJ_PART_MAIN, &containerStyle);
        lv_obj_set_style_local_border_opa(contParticlesNumber[j], LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_0);
        lv_obj_set_click(contParticlesNumber[j], false);
        lv_obj_set_size(contParticlesNumber[j], 47, 14);
        labelParticlesNumber[j] = lv_label_create(contParticlesNumber[j], NULL);
        lv_obj_set_pos(contParticlesNumber[j], contParticleNumberPosX[j], 215); //20
        lv_label_set_align(labelParticlesNumber[j], LV_LABEL_ALIGN_CENTER);
        lv_obj_set_auto_realign(labelParticlesNumber[j], true);
        lv_label_set_text(labelParticlesNumber[j], "-");
        lv_obj_add_style(labelParticlesNumber[j], LV_LABEL_PART_MAIN, &font12Style);
        lv_obj_add_style(labelParticlesNumber[j], LV_LABEL_PART_MAIN, &whiteFontStyle);
    }

    mainLine = lv_line_create(mainScr, NULL);
    lv_line_set_points(mainLine, mainLinePoints, 2);
    lv_line_set_auto_size(mainLine, true);
    lv_obj_add_style(mainLine, LV_LINE_PART_MAIN, &lineStyle);
}

static void ta_event_cb(lv_obj_t *ta, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED)
    {
        if (ta == ssidTA)
        {
            lv_textarea_set_cursor_hidden(ssidTA, false);
            lv_textarea_set_cursor_hidden(pwdTA, true);
        }
        if (ta == pwdTA)
        {
            lv_textarea_set_cursor_hidden(pwdTA, false);
            lv_textarea_set_cursor_hidden(ssidTA, true);
        }

        if (keyboard == NULL)
        {
            keyboard = lv_keyboard_create(lv_scr_act(), NULL);
            lv_obj_set_size(keyboard, LV_HOR_RES, LV_VER_RES / 2);
            lv_obj_set_event_cb(keyboard, lv_keyboard_def_event_cb);
            lv_keyboard_set_textarea(keyboard, ta);
        }
        else
        {
            lv_keyboard_set_textarea(keyboard, ta);
        }
    }
}

static void btn_connect(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED and ((lv_textarea_get_text(ssidTA) != NULL and lv_textarea_get_text(ssidTA) != '\0') or (lv_textarea_get_text(pwdTA) != NULL and lv_textarea_get_text(pwdTA) != '\0')))
    {
        uint8_t wifiAttempts = 10;

        config.ssid = lv_textarea_get_text(ssidTA);
        Serial.println(config.ssid.c_str());
        config.password = lv_textarea_get_text(pwdTA);

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
            Serial.println("btn_connect -> connected to Wi-Fi! IP: " + WiFi.localIP().toString());
            Rtc.SetIsRunning(true);
            dateTimeClient.begin();
            for (int i = 0; i < 3; i++)
                dateTimeClient.update();
            config_time();
        }
        else if (WiFi.status() == WL_DISCONNECTED)
            Serial.println("btn_connect -> can't connect. Probably you have entered wrong credentials.");
        lv_disp_load_scr(mainScr);
        lv_textarea_set_text(ssidTA, "");
        lv_textarea_set_text(pwdTA, "");
    }
}

//Settings button clicked
static void setButton_task(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED)
        lv_disp_load_scr(settingsScr);
}

//Locking button clicked
static void lockButton_task(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED)
        lv_disp_load_scr(lockScr);
}

//Unlocking button clicked
static void unlockButton_task(lv_obj_t *obj, lv_event_t event)
{
    Serial.print(lv_btn_get_state(unlockButton));
    if (event == LV_EVENT_CLICKED)
        lv_disp_load_scr(mainScr);
}

//Exit from wifi settings button clicked
static void btn_cancel(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED)
    {
        lv_disp_load_scr(settingsScr);
        lv_textarea_set_text(ssidTA, "");
        lv_textarea_set_text(pwdTA, "");
    }
}

static void btn_settings_back(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED)
        lv_disp_load_scr(mainScr);
}

static void WiFi_btn(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED)
    {
        lv_scr_load(wifiScr);
    }
}

static void info_btn(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED)
        lv_scr_load(infoScr);
}

static void time_settings_btn(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED)
    {
        lv_scr_load(timeSettingsScr);
        inTimeSettings = true;
    }
}

void timesettings_back_btn(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED)
    {
        switch (config.lcdLockTime)
        {
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
        lv_scr_load(settingsScr);
        inTimeSettings = false;
        timeChanged = false;
        dateChanged = false;
    }
}

void timesettings_save_btn(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED)
    {
        switch (lv_dropdown_get_selected(lockScreenDDlist))
        {
        case 0:
            config.lcdLockTime = 60000;
            break;
        case 1:
            config.lcdLockTime = 300000;
            break;
        case 2:
            config.lcdLockTime = 600000;
            break;
        case 3:
            config.lcdLockTime = 3600000;
            break;
        case 4:
            config.lcdLockTime = -1;
            break;
        default:
            config.lcdLockTime = 60000;
            break;
        }
        mySDCard.saveConfig(config, configFilePath);
        mySDCard.printConfig(configFilePath);
        if (timeChanged == true)
        {
            String datet = lv_label_get_text(dateBtnLabel) + (String)lv_textarea_get_text(timeHour) + ":" + (String)lv_textarea_get_text(timeMinute);
            Serial.println(datet);
            RtcDateTime *dt = new RtcDateTime(atoi(datet.substring(6, 10).c_str()), atoi(datet.substring(3, 6).c_str()), atoi(datet.substring(0, 2).c_str()), datet.substring(10, 12).toDouble(), datet.substring(13, 15).toDouble(), 0);
            Rtc.SetDateTime(*dt);
            Rtc.SetIsRunning(true);
        }
        if (dateChanged == true)
        {
            RtcDateTime ori = Rtc.GetDateTime();
            String date = lv_label_get_text(dateBtnLabel);
            RtcDateTime *dt = new RtcDateTime(atoi(date.substring(6).c_str()), atoi(date.substring(3, 6).c_str()), atoi(date.substring(0, 2).c_str()), ori.Hour(), ori.Minute(), ori.Second());
            Rtc.SetDateTime(*dt);
            Rtc.SetIsRunning(true);
        }
        lv_disp_load_scr(mainScr);
        inTimeSettings = false;
        timeChanged = false;
        dateChanged = false;
        display_current_config();
    }
}

static void hourIncrement(lv_obj_t *btn, lv_event_t e)
{
    if (e == LV_EVENT_SHORT_CLICKED || e == LV_EVENT_LONG_PRESSED_REPEAT)
    {
        if (lv_spinbox_get_value(timeHour) == 23)
        {
            lv_spinbox_set_value(timeHour, 0);
        }
        else
        {
            lv_spinbox_increment(timeHour);
        }
        timeChanged = true;
    }
}

static void hourDecrement(lv_obj_t *btn, lv_event_t e)
{
    if (e == LV_EVENT_SHORT_CLICKED || e == LV_EVENT_LONG_PRESSED_REPEAT)
    {
        if (lv_spinbox_get_value(timeHour) == 0)
            lv_spinbox_set_value(timeHour, 23);
        else
            lv_spinbox_decrement(timeHour);
        timeChanged = true;
    }
}

static void minuteIncrement(lv_obj_t *btn, lv_event_t e)
{
    if (e == LV_EVENT_SHORT_CLICKED || e == LV_EVENT_LONG_PRESSED_REPEAT)
    {
        if (lv_spinbox_get_value(timeMinute) == 59)
        {
            if (lv_spinbox_get_value(timeHour) == 23)
            {
                lv_spinbox_set_value(timeHour, 0);
            }
            else
            {
                lv_spinbox_increment(timeHour);
            }
            lv_spinbox_set_value(timeMinute, 0);
        }
        else
            lv_spinbox_increment(timeMinute);
        timeChanged = true;
    }
}

static void minuteDecrement(lv_obj_t *btn, lv_event_t e)
{
    if (e == LV_EVENT_SHORT_CLICKED || e == LV_EVENT_LONG_PRESSED_REPEAT)
    {
        if (lv_spinbox_get_value(timeMinute) == 00)
        {
            if (lv_spinbox_get_value(timeHour) == 0)
            {
                lv_spinbox_set_value(timeHour, 23);
            }
            else
            {
                lv_spinbox_decrement(timeHour);
            }
            lv_spinbox_set_value(timeMinute, 59);
        }
        else
            lv_spinbox_decrement(timeMinute);
        timeChanged = true;
    }
}

static void temp_settings_btn(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED)
        lv_scr_load(samplingSettingsScr);
}

static void sampling_hourIncrement(lv_obj_t *btn, lv_event_t e)
{
    if (e == LV_EVENT_SHORT_CLICKED || e == LV_EVENT_LONG_PRESSED_REPEAT)
    {
        lv_spinbox_increment(measurePeriodHour);
    }
}

static void sampling_secondIncrement(lv_obj_t *btn, lv_event_t e)
{
    if (e == LV_EVENT_SHORT_CLICKED || e == LV_EVENT_LONG_PRESSED_REPEAT)
    {
        if (lv_spinbox_get_value(measurePeriodsecond) == 59)
        {
            if (lv_spinbox_get_value(measurePeriodMinute) == 59)
            {
                if (lv_spinbox_get_value(measurePeriodHour) != 24)
                {
                    lv_spinbox_set_value(measurePeriodMinute, 0);
                    lv_spinbox_set_value(measurePeriodsecond, 0);
                    lv_spinbox_increment(measurePeriodHour);
                }
            }
            else
            {
                lv_spinbox_set_value(measurePeriodsecond, 0);
                lv_spinbox_increment(measurePeriodMinute);
            }
        }
        else
        {
            lv_spinbox_increment(measurePeriodsecond);
        }
    }
}

static void sampling_secondDecrement(lv_obj_t *btn, lv_event_t e)
{
    if (e == LV_EVENT_SHORT_CLICKED || e == LV_EVENT_LONG_PRESSED_REPEAT)
    {
        if (lv_spinbox_get_value(measurePeriodsecond) == 0 && lv_spinbox_get_value(measurePeriodMinute) != 0)
        {
            if(!(lv_spinbox_get_value(measurePeriodsecond)==0 && lv_spinbox_get_value(measurePeriodMinute)==1 && lv_spinbox_get_value(measurePeriodHour)==0))
            {
                if((lv_spinbox_get_value(measurePeriodHour)*3600+lv_spinbox_get_value(measurePeriodMinute)*60+lv_spinbox_get_value(measurePeriodsecond)-1)>=(lv_spinbox_get_value(turnFanOnTime)+lv_spinbox_get_value(measureNumber)*lv_spinbox_get_value(measureAvPeriod)))
                {
                    lv_spinbox_decrement(measurePeriodMinute);
                    lv_spinbox_set_value(measurePeriodsecond, 59);
                }
            }
            
        }
        else
        {
            if (lv_spinbox_get_value(measurePeriodsecond) == 0 && lv_spinbox_get_value(measurePeriodMinute) == 0)
            {
                if (lv_spinbox_get_value(measurePeriodHour) != 0)
                {
                    if((lv_spinbox_get_value(measurePeriodHour)*3600+lv_spinbox_get_value(measurePeriodMinute)*60+lv_spinbox_get_value(measurePeriodsecond)-1)>=(lv_spinbox_get_value(turnFanOnTime)+lv_spinbox_get_value(measureNumber)*lv_spinbox_get_value(measureAvPeriod)))
                    {
                        lv_spinbox_decrement(measurePeriodHour);
                        lv_spinbox_set_value(measurePeriodMinute, 59);
                        lv_spinbox_set_value(measurePeriodsecond, 59);
                    }
                }
            }
            else
            {
                if((lv_spinbox_get_value(measurePeriodHour)*3600+lv_spinbox_get_value(measurePeriodMinute)*60+lv_spinbox_get_value(measurePeriodsecond)-1)>=(lv_spinbox_get_value(turnFanOnTime)+lv_spinbox_get_value(measureNumber)*lv_spinbox_get_value(measureAvPeriod)))
                    lv_spinbox_decrement(measurePeriodsecond);
            }
        }
    }
}

static void sampling_hourDecrement(lv_obj_t *btn, lv_event_t e)
{
    if (e == LV_EVENT_SHORT_CLICKED || e == LV_EVENT_LONG_PRESSED_REPEAT)
    {
        if(((lv_spinbox_get_value(measurePeriodHour)-1)*3600)+(lv_spinbox_get_value(measurePeriodMinute)*60)+lv_spinbox_get_value(measurePeriodsecond)<60)
            return;
        if(!((lv_spinbox_get_value(measurePeriodsecond)==0)&&(lv_spinbox_get_value(measurePeriodMinute)==0)))
        {
            if((lv_spinbox_get_value(measurePeriodHour)*3600+lv_spinbox_get_value(measurePeriodMinute)*60+lv_spinbox_get_value(measurePeriodsecond)-3600)>=(lv_spinbox_get_value(turnFanOnTime)+lv_spinbox_get_value(measureNumber)*lv_spinbox_get_value(measureAvPeriod)))
                lv_spinbox_decrement(measurePeriodHour);   
        }
    }
}

static void sampling_minuteIncrement(lv_obj_t *btn, lv_event_t e)
{
    if (e == LV_EVENT_SHORT_CLICKED || e == LV_EVENT_LONG_PRESSED_REPEAT)
    {
        if (lv_spinbox_get_value(measurePeriodMinute) == 59)
        {
            if (lv_spinbox_get_value(measurePeriodHour) != 24)
            {
                lv_spinbox_set_value(measurePeriodMinute, 0);
                lv_spinbox_increment(measurePeriodHour);
            }
        }
        else
        {
            lv_spinbox_increment(measurePeriodMinute);
        }
    }
}

static void sampling_minuteDecrement(lv_obj_t *btn, lv_event_t e)
{
    if (e == LV_EVENT_SHORT_CLICKED || e == LV_EVENT_LONG_PRESSED_REPEAT)
    {
        if (lv_spinbox_get_value(measurePeriodMinute) == 0)
        {
            if((lv_spinbox_get_value(measurePeriodHour)*3600+lv_spinbox_get_value(measurePeriodMinute)*60+lv_spinbox_get_value(measurePeriodsecond)-60)>=(lv_spinbox_get_value(turnFanOnTime)+lv_spinbox_get_value(measureNumber)*lv_spinbox_get_value(measureAvPeriod)))
            {
                lv_spinbox_set_value(measurePeriodMinute, 59);
                lv_spinbox_decrement(measurePeriodHour);
            }
        }
        if (!(lv_spinbox_get_value(measurePeriodMinute) == 1 && lv_spinbox_get_value(measurePeriodHour) == 0))
        {
            if((lv_spinbox_get_value(measurePeriodHour)*3600+lv_spinbox_get_value(measurePeriodMinute)*60+lv_spinbox_get_value(measurePeriodsecond)-60)>=(lv_spinbox_get_value(turnFanOnTime)+lv_spinbox_get_value(measureNumber)*lv_spinbox_get_value(measureAvPeriod)))
                lv_spinbox_decrement(measurePeriodMinute);
        }
    }
}

static void sync_rtc_func(lv_obj_t *btn, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED)
    {
        if (WiFi.status() == WL_CONNECTED)
            config_time();
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
            lv_label_set_text(dateBtnLabel, label.c_str());
            lv_obj_del(calendar);
            calendar = NULL;
            dateChanged = true;
        }
    }
}

static void date_button_func(lv_obj_t *btn, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED)
    {
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

static void showHideBtn_func(lv_obj_t *btn, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED)
    {
        if (lv_textarea_get_pwd_mode(pwdTA))
        {
            lv_textarea_set_pwd_mode(pwdTA, false);
            lv_label_set_text(showHideBtnLabel, LV_SYMBOL_EYE_CLOSE);
        }
        else
        {
            lv_textarea_set_pwd_mode(pwdTA, true);
            lv_textarea_set_pwd_show_time(pwdTA, 1);
            lv_textarea_set_text(pwdTA, lv_textarea_get_text(pwdTA));
            lv_textarea_set_pwd_show_time(pwdTA, 5000);
            lv_label_set_text(showHideBtnLabel, LV_SYMBOL_EYE_OPEN);
        }
    }
}

static void measureNumberIncrement_func(lv_obj_t *btn, lv_event_t event)
{
    if (event == LV_EVENT_SHORT_CLICKED || event == LV_EVENT_LONG_PRESSED_REPEAT)
    {
        if((lv_spinbox_get_value(measurePeriodHour)*3600+lv_spinbox_get_value(measurePeriodMinute)*60+lv_spinbox_get_value(measurePeriodsecond))>=(lv_spinbox_get_value(turnFanOnTime)+(lv_spinbox_get_value(measureNumber)+1)*lv_spinbox_get_value(measureAvPeriod)))
        {
            set_spinbox_digit_format(measureNumber, MIN_RANGE, MAX_RANGE, 1);
            lv_spinbox_increment(measureNumber);
        }
    }
}

static void measureNumberDecrement_func(lv_obj_t *btn, lv_event_t event)
{
    if (event == LV_EVENT_SHORT_CLICKED || event == LV_EVENT_LONG_PRESSED_REPEAT)
    {
        set_spinbox_digit_format(measureNumber, MIN_RANGE, MAX_RANGE, -1);
        lv_spinbox_decrement(measureNumber);
    }
}

static void turnFanOnTimeIncrement_func(lv_obj_t *btn, lv_event_t event)
{
    if (event == LV_EVENT_SHORT_CLICKED || event == LV_EVENT_LONG_PRESSED_REPEAT)
    {
        if((lv_spinbox_get_value(measurePeriodHour)*3600+lv_spinbox_get_value(measurePeriodMinute)*60+lv_spinbox_get_value(measurePeriodsecond))>=(lv_spinbox_get_value(turnFanOnTime)+1+lv_spinbox_get_value(measureNumber)*lv_spinbox_get_value(measureAvPeriod)))        
        {  
            set_spinbox_digit_format(turnFanOnTime, MIN_RANGE, MAX_RANGE, 1);
            lv_spinbox_increment(turnFanOnTime);
        }
    }        
}

static void turnFanOnTimeDecrement_func(lv_obj_t *btn, lv_event_t event)
{
    if (event == LV_EVENT_SHORT_CLICKED || event == LV_EVENT_LONG_PRESSED_REPEAT)
    {    
        set_spinbox_digit_format(turnFanOnTime, MIN_RANGE, MAX_RANGE, -1);
        lv_spinbox_decrement(turnFanOnTime);
    }
}

static void av_periodIncrement(lv_obj_t *btn, lv_event_t event)
{
    if (event == LV_EVENT_SHORT_CLICKED || event == LV_EVENT_LONG_PRESSED_REPEAT)
    {  
        if((lv_spinbox_get_value(measurePeriodHour)*3600+lv_spinbox_get_value(measurePeriodMinute)*60+lv_spinbox_get_value(measurePeriodsecond))>=(lv_spinbox_get_value(turnFanOnTime)+lv_spinbox_get_value(measureNumber)*(lv_spinbox_get_value(measureAvPeriod)+1)))
        {
            set_spinbox_digit_format(measureAvPeriod, MIN_RANGE, MAX_RANGE, 1);
            lv_spinbox_increment(measureAvPeriod);
        }
    }
}

static void av_periodDecrement(lv_obj_t *btn, lv_event_t event)
{
    if (event == LV_EVENT_SHORT_CLICKED || event == LV_EVENT_LONG_PRESSED_REPEAT){
        set_spinbox_digit_format(measureAvPeriod, MIN_RANGE, MAX_RANGE, -1);
        lv_spinbox_decrement(measureAvPeriod);
    }
}

static void sampling_settings_save_btn(lv_obj_t *btn, lv_event_t event)
{
    if (event == LV_EVENT_SHORT_CLICKED || event == LV_EVENT_LONG_PRESSED_REPEAT)
    {
        int get_value = lv_spinbox_get_value(measurePeriodHour) * 60 * 60000 + lv_spinbox_get_value(measurePeriodMinute) * 60000 + lv_spinbox_get_value(measurePeriodsecond) * 1000;
        config.timeBetweenSavingSample = get_value;
        config.countOfSamples = lv_spinbox_get_value(measureNumber);
        config.measurePeriod = lv_spinbox_get_value(measureAvPeriod) * 1000;
        config.turnFanTime = lv_spinbox_get_value(turnFanOnTime) * 1000;
        getSample = lv_task_create(getSampleFunc, (config.timeBetweenSavingSample-config.countOfSamples*config.measurePeriod), LV_TASK_PRIO_HIGH, NULL);
        turnFanOn = lv_task_create(turnFanOnFunc, config.timeBetweenSavingSample - config.turnFanTime, LV_TASK_PRIO_HIGHEST, NULL);
        mySDCard.saveConfig(config, configFilePath);
        mySDCard.printConfig(configFilePath);
        lv_scr_load(mainScr);
        display_current_config();
    }
}

static void sampling_settings_back_btn(lv_obj_t *btn, lv_event_t event)
{
    if (event == LV_EVENT_SHORT_CLICKED || event == LV_EVENT_LONG_PRESSED_REPEAT)
    {
        lv_spinbox_set_value(measurePeriodHour, ((config.timeBetweenSavingSample / 60000) / 60));
        lv_spinbox_set_value(measurePeriodMinute, ((config.timeBetweenSavingSample / 60000) % 60));
        lv_scr_load(settingsScr);
    }
}