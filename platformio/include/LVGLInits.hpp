#include <LVGLTasks.hpp>

void stylesInits(void){
    lv_style_init(&whiteFontStyle);
    lv_style_set_text_color(&whiteFontStyle, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    lv_style_init(&font12Style);
    lv_style_set_text_font(&font12Style, LV_STATE_DEFAULT, &lv_font_montserrat_12);

    lv_style_init(&font16Style);
    lv_style_set_text_font(&font16Style, LV_STATE_DEFAULT, &lv_font_montserrat_16);

    lv_style_init(&font20Style);
    lv_style_set_text_font(&font20Style, LV_STATE_DEFAULT, &lv_font_montserrat_20);
    
    lv_style_init(&font22Style);
    lv_style_set_text_font(&font22Style, LV_STATE_DEFAULT, &lv_font_montserrat_22);

    lv_style_init(&transparentBackgroundStyle);
    lv_style_set_bg_opa(&transparentBackgroundStyle, LV_STATE_DEFAULT, LV_OPA_0);

    lv_style_init(&borderlessStyle);
    lv_style_set_border_width(&borderlessStyle, LV_STATE_DEFAULT, 0);
    lv_style_set_outline_width(&borderlessStyle, LV_STATE_DEFAULT, 0);

    lv_style_init(&containerStyle);
    lv_style_set_bg_opa(&containerStyle, LV_STATE_DEFAULT, LV_OPA_0);
    lv_style_set_border_color(&containerStyle, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_radius(&containerStyle, LV_STATE_DEFAULT, 0);
    
    lv_style_init(&hugeFontStyle);
    lv_style_set_text_font(&hugeFontStyle, LV_STATE_DEFAULT, &hugeSymbolsFont48);

    lv_style_init(&lineStyle);
    lv_style_set_line_width(&lineStyle, LV_STATE_DEFAULT, 2);
    lv_style_set_line_color(&lineStyle, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_line_rounded(&lineStyle, LV_STATE_DEFAULT, false);

    lv_style_init(&whiteButtonStyle);
    lv_style_set_bg_color(&whiteButtonStyle, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_radius(&whiteButtonStyle, LV_STATE_DEFAULT, 10);
    lv_style_set_border_width(&whiteButtonStyle, LV_STATE_DEFAULT, 0);
    lv_style_set_outline_width(&whiteButtonStyle, LV_STATE_DEFAULT, 0);
    lv_style_set_text_color(&whiteButtonStyle, LV_STATE_DEFAULT, LV_COLOR_BLACK);

    lv_style_init(&warningStyle);
    lv_style_set_text_font(&warningStyle, LV_STATE_DEFAULT, &lv_font_montserrat_12);
    lv_style_set_bg_opa(&warningStyle, LV_STATE_DEFAULT, LV_OPA_0);
    lv_style_set_border_opa(&warningStyle, LV_STATE_DEFAULT, LV_OPA_0);
    lv_style_set_radius(&warningStyle, LV_STATE_DEFAULT, 0);
    lv_style_set_text_color(&warningStyle, LV_STATE_DEFAULT, LV_COLOR_RED);
}

void timesettingsScreen()
{
    backTimeSettingsBtn = my_lv_btn_create(timeSettingsScr, backSettingsBtn, 30, 15, 14, 10, timesettings_back_btn); 
    backTimeSettingsLabel = lv_label_create(backTimeSettingsBtn, NULL);
    lv_label_set_text(backTimeSettingsLabel, LV_SYMBOL_LEFT);

    timeSettingsLabelAtBar = my_lv_label_create(timeSettingsScr, NULL, 195, 10, "Time Settings");

    timeLabel = my_lv_label_create(timeSettingsScr, timeSettingsLabelAtBar, 5, 70, "Time [HH:MM]");

    timeHour = lv_spinbox_create(timeSettingsScr, NULL);
    lv_textarea_set_cursor_hidden(timeHour, true);
    lv_textarea_set_text_align(timeHour, LV_LABEL_ALIGN_CENTER);
    lv_spinbox_set_range(timeHour, 0, 23);
    lv_spinbox_set_digit_format(timeHour, 2, 0);
    lv_obj_set_size(timeHour, 40, 34);
    lv_obj_set_pos(timeHour, 165, 61);

    timeHourIncrement = my_lv_btn_create(timeSettingsScr, NULL, 20, 20, 175, 39, hourIncrement);
    lv_theme_apply(timeHourIncrement, LV_THEME_SPINBOX_BTN);
    lv_obj_add_style(timeHourIncrement, LV_OBJ_PART_MAIN, &borderlessStyle);
    lv_obj_set_style_local_value_str(timeHourIncrement, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_PLUS);

    timeHourDecrement = my_lv_btn_create(timeSettingsScr, timeHourIncrement, 20, 20, 175, 97, hourDecrement);
    lv_obj_set_style_local_value_str(timeHourDecrement, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_MINUS);

    timeColonLabel = my_lv_label_create(timeSettingsScr, timeLabel, 210, 70, ":");
    lv_obj_set_style_local_text_color(timeColonLabel, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    timeMinute = lv_spinbox_create(timeSettingsScr, NULL);
    lv_textarea_set_cursor_hidden(timeMinute, true);
    lv_textarea_set_text_align(timeMinute, LV_LABEL_ALIGN_CENTER);
    lv_spinbox_set_range(timeMinute, 0, 59);
    lv_spinbox_set_digit_format(timeMinute, 2, 0);
    lv_obj_set_size(timeMinute, 40, 34); 
    lv_obj_set_pos(timeMinute, 219, 61);

    timeMinuteIncrement = my_lv_btn_create(timeSettingsScr, timeHourIncrement, 20, 20, 229, 39, minuteIncrement);
    lv_obj_set_style_local_value_str(timeMinuteIncrement, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_PLUS);

    timeMinuteDecrement = my_lv_btn_create(timeSettingsScr, timeHourIncrement, 20, 20, 229, 97, minuteDecrement);
    lv_obj_set_style_local_value_str(timeMinuteDecrement, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_MINUS);

    dateLabel = my_lv_label_create(timeSettingsScr, NULL, 5, 129, "Date ");

    dateBtn = my_lv_btn_create(timeSettingsScr, NULL, 95, 43, 165, 119, date_button_func);
    dateBtnLabel = lv_label_create(dateBtn, NULL);
    lv_label_set_text(dateBtnLabel, "99.99.9999");
    lv_obj_set_style_local_border_opa(dateBtn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);
    lv_obj_set_style_local_text_color(dateBtnLabel, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);

    lockScreenLabel = my_lv_label_create(timeSettingsScr, NULL, 5, 170, "Lock screen after ");
    lockScreenDDlist = lv_dropdown_create(timeSettingsScr, NULL);
    lv_dropdown_set_options(lockScreenDDlist, "1 min\n"
                                              "5 min\n"
                                              "10 min\n"
                                              "60 min\n"
                                              "Never");
    lv_obj_set_size(lockScreenDDlist, 120, 34); 
    lv_obj_set_pos(lockScreenDDlist, 165, 164);

    timeSettingsBtn = my_lv_btn_create(timeSettingsScr, NULL, 75, 33, 240, 200, timesettings_save_btn);
    timeSettingsLabel = lv_label_create(timeSettingsBtn, NULL);
    lv_label_set_text(timeSettingsLabel, "Save");
    lv_obj_add_style(timeSettingsBtn, LV_BTN_PART_MAIN, &whiteButtonStyle);

    syncRtcBtn = my_lv_btn_create(timeSettingsScr, NULL, 130, 33, 5, 200, sync_rtc_func);
    syncRtcLabel = lv_label_create(syncRtcBtn, NULL);
    lv_label_set_text(syncRtcLabel, "Sync. Clock");
    lv_obj_add_style(syncRtcBtn, LV_BTN_PART_MAIN, &whiteButtonStyle);
}

void settingsScreen()
{
    backSettingsBtn = my_lv_btn_create(settingsScr, NULL, 30, 15, 14, 10, btn_settings_back);
    backSettingsLabel = lv_label_create(backSettingsBtn, NULL);
    lv_label_set_text(backSettingsLabel, LV_SYMBOL_LEFT);
    lv_obj_add_style(backSettingsBtn, LV_OBJ_PART_MAIN, &transparentBackgroundStyle);
    lv_obj_add_style(backSettingsBtn, LV_OBJ_PART_MAIN, &borderlessStyle);
    lv_obj_add_style(backSettingsBtn, LV_OBJ_PART_MAIN, &whiteFontStyle);

    settingsLabelAtBar = my_lv_label_create(settingsScr, NULL, 239, 10, "Settings");

    wifiBtn = my_lv_btn_create(settingsScr, NULL, 60, 60, 60, 38, WiFi_btn);
    wifiBtnLabel = lv_label_create(wifiBtn, NULL);
    lv_label_set_text(wifiBtnLabel, MY_WIFI_SYMBOL);
    lv_obj_add_style(wifiBtn, LV_OBJ_PART_MAIN, &transparentBackgroundStyle);
    lv_obj_add_style(wifiBtn, LV_OBJ_PART_MAIN, &borderlessStyle);
    lv_obj_add_style(wifiBtn, LV_OBJ_PART_MAIN, &whiteFontStyle);
    lv_obj_add_style(wifiBtn, LV_OBJ_PART_MAIN, &hugeFontStyle);

    wifiBtnName = my_lv_label_create(settingsScr, NULL, 63, 103, "WiFi");
    lv_obj_add_style(wifiBtnName, LV_OBJ_PART_MAIN, &font20Style);

    infoBtn = my_lv_btn_create(settingsScr, wifiBtn, 60, 60, 200, 38, info_btn);
    infoBtnLabel = lv_label_create(infoBtn, NULL);
    lv_label_set_text(infoBtnLabel, MY_INFO_SYMBOL);

    infoBtnName = my_lv_label_create(settingsScr, wifiBtnName, 207, 103, "Info");

    timeBtn = my_lv_btn_create(settingsScr, wifiBtn, 60, 60, 60, 140, time_settings_btn);
    timeBtnLabel = lv_label_create(timeBtn, NULL);
    lv_label_set_text(timeBtnLabel, MY_CLOCK_SYMBOL);

    timeBtnName = my_lv_label_create(settingsScr, wifiBtnName, 65, 200, "Time");

    tempBtn = my_lv_btn_create(settingsScr, wifiBtn, 60, 60, 200, 140, temp_settings_btn);
    tempBtnLabel = lv_label_create(tempBtn, wifiBtnName);
    lv_label_set_text(tempBtnLabel, MY_COGS_SYMBOL);
    lv_obj_add_style(tempBtnLabel, LV_OBJ_PART_MAIN, &hugeFontStyle);
    tempBtnName = my_lv_label_create(settingsScr, wifiBtnName, 180, 200, "Sampling");
}

void infoScreen()
{
    backInfoBtn = my_lv_btn_create(infoScr, backSettingsBtn, 30, 15, 14, 10, setButton_task);
    backInfoLabel = lv_label_create(backInfoBtn, NULL);
    lv_label_set_text(backInfoLabel, LV_SYMBOL_LEFT);

    lcdLabelAtBar = my_lv_label_create(infoScr, NULL, 216, 10, "Device info");

    infoWifiLabel = my_lv_label_create(infoScr, NULL, 5, 53, "WiFi address: ");
    lv_obj_set_style_local_text_font(infoWifiLabel, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_14);
    
    infoWifiAddressLabel = my_lv_label_create(infoScr, infoWifiLabel, 115, 53, "");

    configLabel = my_lv_label_create(infoScr, NULL, 5, 70, "");
}

void mainScreen()
{
    setButton = my_lv_btn_create(mainScr, NULL, 16, 18, 32, 7, setButton_task);
    labelSetButton = lv_label_create(setButton, NULL);
    lv_label_set_text(labelSetButton, LV_SYMBOL_SETTINGS);
    lv_obj_add_style(setButton, LV_OBJ_PART_MAIN, &transparentBackgroundStyle);
    lv_obj_add_style(setButton, LV_OBJ_PART_MAIN, &borderlessStyle);
    lv_obj_add_style(setButton, LV_OBJ_PART_MAIN, &whiteFontStyle);

	lockButton = my_lv_btn_create(mainScr, setButton, 14, 18, 95, 7, lockButton_task);
	labelLockButton = lv_label_create(lockButton, NULL);
    lv_obj_set_style_local_text_font(lockButton, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, &monte16lock);
	lv_label_set_text(labelLockButton, MY_LOCK_SYMBOL);

    wifiStatusAtMain = my_lv_label_create(mainScr, NULL, 52, 7, LV_SYMBOL_WIFI, LV_COLOR_WHITE);
    wifiStatusAtMainWarning = my_lv_label_create(wifiStatusAtMain, NULL, 6, 6, LV_SYMBOL_CLOSE, LV_COLOR_RED);
    lv_obj_add_style(wifiStatusAtMainWarning, LV_OBJ_PART_MAIN, &transparentBackgroundStyle);
    lv_obj_add_style(wifiStatusAtMainWarning, LV_OBJ_PART_MAIN, &font12Style);

    sdStatusAtMain = my_lv_label_create(mainScr, wifiStatusAtMain, 77, 7, LV_SYMBOL_SD_CARD, LV_COLOR_WHITE);
    sdStatusAtMainWarning = my_lv_label_create(sdStatusAtMain, wifiStatusAtMainWarning, 2, 6, LV_SYMBOL_CLOSE, LV_COLOR_RED);

    dateAndTimeAtBar = my_lv_label_create(mainScr, NULL, 157, 7, "");

    contPM25 = my_lv_cont_create(mainScr, NULL, 180, 90, 10, 30);
    lv_obj_set_click(contPM25, false);
    lv_obj_add_style(contPM25, LV_OBJ_PART_MAIN, &transparentBackgroundStyle);
    lv_obj_add_style(contPM25, LV_OBJ_PART_MAIN, &containerStyle);

    contTemp = my_lv_cont_create(mainScr, contPM25, 122, 46, 188, 30);
    lv_obj_set_click(contTemp, false);

    contHumi = my_lv_cont_create(mainScr, contPM25, 122, 46, 188, 74);
    lv_obj_set_click(contHumi, false);

    contPM10 = my_lv_cont_create(mainScr, NULL, 91, 62, 10, 118);
    lv_obj_set_click(contPM10, false);
    lv_obj_add_style(contPM10, LV_OBJ_PART_MAIN, &transparentBackgroundStyle);
    lv_obj_add_style(contPM10, LV_OBJ_PART_MAIN, &containerStyle);
    lv_obj_add_style(contPM10, LV_OBJ_PART_MAIN, &font12Style);

    contPM100 = my_lv_cont_create(mainScr, contPM10, 91, 62, 99, 118);
    lv_obj_set_click(contPM100, false);

    contAQI = my_lv_cont_create(mainScr, contPM10, 122, 62, 188, 118);
    lv_obj_set_click(contAQI, false);

    contAQIColorBar = my_lv_cont_create(contAQI, contAQI, 92, 24, 15, 25);
    lv_obj_set_style_local_bg_opa(contAQIColorBar, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_70);
    lv_obj_set_click(contAQIColorBar, false);
    
    labelTemp = my_lv_label_create(contTemp, NULL, 5, 3, "Temp");

    labelTempValue = my_lv_label_create(contTemp, NULL, 16, 22, "         -");
    lv_obj_add_style(labelTempValue, LV_OBJ_PART_MAIN, &font20Style);
    lv_label_set_align(labelTempValue, LV_LABEL_ALIGN_LEFT);

    labelHumi = my_lv_label_create(contHumi, NULL, 5, 3, "RH");

    labelHumiValue = my_lv_label_create(contHumi, NULL, 16, 22, "         -");
    lv_obj_add_style(labelHumiValue, LV_OBJ_PART_MAIN, &font20Style);

    labelPM10 = my_lv_label_create(contPM10, NULL, 5, 5, "PM 1.0 ug/m");

    labelPM10UpperIndex = my_lv_label_create(contPM10, NULL, 80, 3, "3");

    labelPM25 = my_lv_label_create(contPM25, NULL, 5, 5, "PM 2.5                ug/m");

    labelPM25UpperIndex = my_lv_label_create(contPM25, NULL, 167, 3 , "3");
    lv_obj_add_style(labelPM25UpperIndex, LV_OBJ_PART_MAIN, &font12Style);

    labelPM100 = my_lv_label_create(contPM100, NULL, 5, 5, "PM 10 ug/m");

    labelPM100UpperIndex = my_lv_label_create(contPM100, NULL, 77, 3, "3");
    lv_obj_add_style(labelPM100UpperIndex, LV_OBJ_PART_MAIN, &font12Style);

    labelPM10Data = lv_label_create(contPM10, NULL);
    lv_obj_add_style(labelPM10Data, LV_OBJ_PART_MAIN, &font22Style);
    lv_obj_add_style(labelPM10Data, LV_OBJ_PART_MAIN, &whiteFontStyle);
    lv_obj_set_auto_realign(labelPM10Data, true);
    lv_obj_align(labelPM10Data, NULL, LV_ALIGN_CENTER, 0, 5);
    lv_label_set_text(labelPM10Data, "-");

    labelPM25Data = lv_label_create(contPM25, NULL);
    lv_obj_add_style(labelPM25Data, LV_OBJ_PART_MAIN, &font22Style);
    lv_obj_add_style(labelPM25Data, LV_OBJ_PART_MAIN, &whiteFontStyle);
    lv_obj_set_auto_realign(labelPM25Data, true);
    lv_obj_align(labelPM25Data, NULL, LV_ALIGN_CENTER, 0, 10);
    lv_label_set_text(labelPM25Data, "-");

    labelPM100Data = lv_label_create(contPM100, NULL);
    lv_obj_add_style(labelPM100Data, LV_OBJ_PART_MAIN, &font22Style);
    lv_obj_add_style(labelPM100Data, LV_OBJ_PART_MAIN, &whiteFontStyle);
    lv_obj_set_auto_realign(labelPM100Data, true);
    lv_obj_align(labelPM100Data, NULL, LV_ALIGN_CENTER, 0, 5);
    lv_label_set_text(labelPM100Data, "-");

    labelAQI = my_lv_label_create(contAQI, NULL, 5, 5, "Air Quality  PM 2.5");

    labelAQIColorBar = lv_label_create(contAQIColorBar, NULL);
    lv_obj_set_auto_realign(labelAQIColorBar, true);
    lv_obj_align(labelAQIColorBar, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_label_set_text(labelAQIColorBar, "-");
    lv_obj_add_style(labelAQIColorBar, LV_OBJ_PART_MAIN, &whiteFontStyle);

    ledAtMain = lv_led_create(mainScr, NULL);
    lv_obj_set_size(ledAtMain, 13, 13);
    lv_obj_set_pos(ledAtMain, 13, 10);
    lv_led_set_bright(ledAtMain, 200);
    lv_obj_set_style_local_bg_color(ledAtMain, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_YELLOW);
    lv_obj_set_style_local_shadow_color(ledAtMain, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_YELLOW);
    lv_obj_set_style_local_border_opa(ledAtMain, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_0);
    //Function that draws lines and st text above those
    drawParticlesIndicator();
}

void samplingsettingsScreen()
{
    backSamplingSettingsBtn = my_lv_btn_create(samplingSettingsScr, backSettingsBtn, 30, 15, 14, 10, sampling_settings_back_btn); 
    backSamplingSettingsLabel = lv_label_create(backSamplingSettingsBtn, NULL);
    lv_label_set_text(backSamplingSettingsLabel, LV_SYMBOL_LEFT);

    samplingSaveBtn = my_lv_btn_create(samplingSettingsScr, NULL, 75, 25, 231, 10, sampling_settings_save_btn);
    samplingSaveLabel = lv_label_create(samplingSaveBtn, NULL);
    lv_label_set_text(samplingSaveLabel, "Save");
    lv_obj_add_style(samplingSaveBtn, LV_BTN_PART_MAIN, &whiteButtonStyle);

    measurePeriodlabel = my_lv_label_create(samplingSettingsScr, NULL, 5, 52, "Measurements \nsaving time \n[HH:MM:SS]", LV_COLOR_WHITE);
    lv_obj_set_style_local_text_font(measurePeriodlabel, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_14);

    measurePeriodHour = lv_spinbox_create(samplingSettingsScr, NULL);
    lv_textarea_set_cursor_hidden(measurePeriodHour, true);
    lv_textarea_set_text_align(measurePeriodHour, LV_LABEL_ALIGN_CENTER);
    lv_spinbox_set_range(measurePeriodHour, 0, 24);
    lv_spinbox_set_digit_format(measurePeriodHour, 2, 0);
    lv_obj_set_size(measurePeriodHour, 40, 34);
    lv_obj_set_pos(measurePeriodHour, 165, 61);

    measurePeriodHourIncrement  = my_lv_btn_create(samplingSettingsScr, NULL, 20, 20, 175, 39, sampling_hourIncrement);
    lv_theme_apply(measurePeriodHourIncrement , LV_THEME_SPINBOX_BTN);
    lv_obj_add_style(measurePeriodHourIncrement , LV_OBJ_PART_MAIN, &borderlessStyle);
    lv_obj_set_style_local_value_str(measurePeriodHourIncrement, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_PLUS);

    measurePeriodHourDecrement = my_lv_btn_create(samplingSettingsScr, measurePeriodHourIncrement , 20, 20, 175, 97, sampling_hourDecrement);
    lv_obj_set_style_local_value_str(measurePeriodHourDecrement, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_MINUS);

    measureColonLabel = my_lv_label_create(samplingSettingsScr, NULL, 210, 70, ":", LV_COLOR_WHITE);

    measurePeriodMinute = lv_spinbox_create(samplingSettingsScr, NULL);
    lv_textarea_set_cursor_hidden(measurePeriodMinute, true);
    lv_textarea_set_text_align(measurePeriodMinute, LV_LABEL_ALIGN_CENTER);
    lv_spinbox_set_range(measurePeriodMinute, 0, 59);
    lv_spinbox_set_digit_format(measurePeriodMinute, 2, 0);
    lv_obj_set_size(measurePeriodMinute, 40, 34);
    lv_obj_set_pos(measurePeriodMinute, 219, 61);

    measurePeriodMinuteIncrement = my_lv_btn_create(samplingSettingsScr, measurePeriodHourIncrement , 20, 20, 229, 39, sampling_minuteIncrement);
    lv_obj_set_style_local_value_str(measurePeriodMinuteIncrement, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_PLUS);

    measurePeriodMinuteDecrement = my_lv_btn_create(samplingSettingsScr, measurePeriodHourIncrement , 20, 20, 229, 97, sampling_minuteDecrement);
    lv_obj_set_style_local_value_str(measurePeriodMinuteDecrement, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_MINUS);

    measureColonLabel2 = my_lv_label_create(samplingSettingsScr, measurePeriodlabel, 265, 70, ":", LV_COLOR_WHITE);

    measurePeriodsecond = lv_spinbox_create(samplingSettingsScr, NULL);
    lv_textarea_set_cursor_hidden(measurePeriodsecond, true);
    lv_textarea_set_text_align(measurePeriodsecond, LV_LABEL_ALIGN_CENTER);
    lv_spinbox_set_range(measurePeriodsecond, 0, 59);
    lv_spinbox_set_digit_format(measurePeriodsecond, 2, 0);
    lv_obj_set_size(measurePeriodsecond, 40, 34);
    lv_obj_set_pos(measurePeriodsecond, 274, 61);

    measurePeriodsecondIncrement = my_lv_btn_create(samplingSettingsScr, measurePeriodHourIncrement , 20, 20, 284, 39, sampling_secondIncrement);
    lv_obj_set_style_local_value_str(measurePeriodsecondIncrement, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_PLUS);

    measurePeriodsecondDecrement = my_lv_btn_create(samplingSettingsScr, measurePeriodHourIncrement , 20, 20, 284, 97, sampling_secondDecrement);
    lv_obj_set_style_local_value_str(measurePeriodsecondDecrement, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_MINUS);

    measureNumberLabel = my_lv_label_create(samplingSettingsScr, NULL, 5, 127, "Number of samples", LV_COLOR_WHITE);
    lv_obj_set_style_local_text_font(measureNumberLabel, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_14);

    measureNumber = lv_spinbox_create(samplingSettingsScr, NULL);
    lv_textarea_set_cursor_hidden(measureNumber, true);
    lv_textarea_set_text_align(measureNumber, LV_LABEL_ALIGN_CENTER);
    lv_spinbox_set_digit_format(measureNumber, 2, 0);
    lv_spinbox_set_range(measureNumber, 1, 20);
    lv_obj_set_size(measureNumber, 50, 34);
    lv_obj_set_pos(measureNumber, 206, 119);

    measureNumberIncrement = my_lv_btn_create(samplingSettingsScr, measurePeriodHourIncrement , 40, 34, 257, 119, measureNumberIncrement_func);
    lv_obj_set_style_local_value_str(measureNumberIncrement, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_PLUS);

    measureNumberDecrement = my_lv_btn_create(samplingSettingsScr, measurePeriodHourIncrement , 40, 34, 165, 119, measureNumberDecrement_func);
    lv_obj_set_style_local_value_str(measureNumberDecrement, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_MINUS);

    measureAvPeriodLabel = my_lv_label_create(samplingSettingsScr, measurePeriodlabel, 5, 165, "Time between \nmeasurments [s]:", LV_COLOR_WHITE);

    measureAvPeriod = lv_spinbox_create(samplingSettingsScr, NULL);
    lv_textarea_set_cursor_hidden(measureAvPeriod, true);
    lv_textarea_set_text_align(measureAvPeriod, LV_LABEL_ALIGN_CENTER);
    lv_spinbox_set_digit_format(measureAvPeriod, 3, 0);
    lv_spinbox_set_range(measureAvPeriod, 5, 999);
    lv_obj_set_size(measureAvPeriod, 50, 34);
    lv_obj_set_pos(measureAvPeriod, 206, 160);

    measureAvPeriodIncrement = my_lv_btn_create(samplingSettingsScr, measurePeriodHourIncrement , 40, 34, 257, 160, av_periodIncrement);
    lv_obj_set_style_local_value_str(measureAvPeriodIncrement, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_PLUS);

    measureAvPeriodDecrement = my_lv_btn_create(samplingSettingsScr, measurePeriodHourIncrement , 40, 34, 165, 160, av_periodDecrement);
    lv_obj_set_style_local_value_str(measureAvPeriodDecrement, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_MINUS);

    turnFanOnTimeLabel = my_lv_label_create(samplingSettingsScr, measurePeriodlabel, 5, 205, "Fan running time \nbefore measure [s]:");

    turnFanOnTime = lv_spinbox_create(samplingSettingsScr, NULL);
    lv_textarea_set_cursor_hidden(turnFanOnTime, true);
    lv_textarea_set_text_align(turnFanOnTime, LV_LABEL_ALIGN_CENTER);
    lv_spinbox_set_digit_format(turnFanOnTime, 3, 0);
    lv_spinbox_set_range(turnFanOnTime, 1, 999);
    lv_obj_set_size(turnFanOnTime, 50, 34);
    lv_obj_set_pos(turnFanOnTime, 206, 200);

    turnFanOnTimeIncrement = my_lv_btn_create(samplingSettingsScr, measurePeriodHourIncrement , 40, 34, 257, 200, turnFanOnTimeIncrement_func);
    lv_obj_set_style_local_value_str(turnFanOnTimeIncrement, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_PLUS);

    turnFanOnTimeDecrement = my_lv_btn_create(samplingSettingsScr, measurePeriodHourIncrement , 40, 34, 165, 200, turnFanOnTimeDecrement_func);
    lv_obj_set_style_local_value_str(turnFanOnTimeDecrement, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_MINUS);
}

//Zrobione
void wifiScreen()
{
    cancelBtn = my_lv_btn_create(wifiScr, backSettingsBtn, 30, 15, 14, 10, btn_cancel);
    cancelLabel = lv_label_create(cancelBtn, NULL);
    lv_label_set_text(cancelLabel, LV_SYMBOL_LEFT);
    lv_obj_add_style(cancelBtn, LV_OBJ_PART_MAIN, &transparentBackgroundStyle);
    lv_obj_add_style(cancelBtn, LV_OBJ_PART_MAIN, &borderlessStyle);
    lv_obj_add_style(cancelBtn, LV_OBJ_PART_MAIN, &whiteFontStyle);

    wifiLabelAtBar = my_lv_label_create(wifiScr, NULL, 201, 10, "WiFi settings");
 
    ssidLabel = my_lv_label_create(wifiScr, NULL, 5, 53, "SSID: ");
   
    ssidTA = lv_textarea_create(wifiScr, NULL);
    lv_textarea_set_text(ssidTA, "");
    lv_textarea_set_pwd_mode(ssidTA, false);
    lv_textarea_set_one_line(ssidTA, true);    
    lv_obj_set_event_cb(ssidTA, ta_event_cb);
    lv_textarea_set_cursor_hidden(ssidTA, true);
    lv_obj_set_size(ssidTA, 140, 34);
    lv_obj_set_pos(ssidTA, 100, 45);
    lv_obj_add_style(ssidTA, LV_OBJ_PART_MAIN, &borderlessStyle);

    pwdLabel = my_lv_label_create(wifiScr, NULL, 5, 92, "Password: ");

    pwdTA = lv_textarea_create(wifiScr, NULL);
    lv_textarea_set_text(pwdTA, "");
    lv_textarea_set_pwd_show_time(pwdTA, 5000);
    lv_textarea_set_pwd_mode(pwdTA, true);
    lv_textarea_set_one_line(pwdTA, true);
    lv_obj_set_event_cb(pwdTA, ta_event_cb);
    lv_textarea_set_cursor_hidden(pwdTA, true);
    lv_obj_set_size(pwdTA, 140, 34); 
    lv_obj_set_pos(pwdTA, 100, 85);
    lv_obj_add_style(pwdTA, LV_OBJ_PART_MAIN, &borderlessStyle);

    showHideBtn = my_lv_btn_create(wifiScr, NULL, 75, 43, 243, 85, showHideBtn_func);
    showHideBtnLabel = lv_label_create(showHideBtn, NULL);
    lv_label_set_text(showHideBtnLabel, LV_SYMBOL_EYE_OPEN);
    lv_obj_add_style(showHideBtn, LV_OBJ_PART_MAIN, &transparentBackgroundStyle);
    lv_obj_add_style(showHideBtn, LV_OBJ_PART_MAIN, &borderlessStyle);
    lv_obj_add_style(showHideBtnLabel, LV_OBJ_PART_MAIN, &whiteFontStyle);

    applyBtn = my_lv_btn_create(wifiScr, NULL, 75, 43, 243, 43, btn_connect);
    applyLabel = lv_label_create(applyBtn, NULL);
    lv_label_set_text(applyLabel, "Connect");
    lv_obj_add_style(applyBtn, LV_OBJ_PART_MAIN, &whiteButtonStyle);
    lv_obj_add_style(applyBtn, LV_OBJ_PART_MAIN, &borderlessStyle);
}

void lockScreen()
{
    contDateTimeAtLock = lv_cont_create(lockScr, NULL);
    lv_obj_set_auto_realign(contDateTimeAtLock, true);
    lv_obj_align(contDateTimeAtLock, NULL, LV_ALIGN_CENTER, 0, -40);
    lv_cont_set_fit(contDateTimeAtLock, LV_FIT_TIGHT);
    lv_cont_set_layout(contDateTimeAtLock, LV_LAYOUT_PRETTY_MID);
    lv_obj_add_style(contDateTimeAtLock, LV_OBJ_PART_MAIN, &transparentBackgroundStyle);
    lv_obj_add_style(contDateTimeAtLock, LV_OBJ_PART_MAIN, &borderlessStyle);
    lv_obj_add_style(contDateTimeAtLock, LV_OBJ_PART_MAIN, &whiteFontStyle);

    unlockButton = lv_btn_create(lockScr, setButton);//TODO BRAK POS
    labelUnlockButton = lv_label_create(unlockButton, NULL);
    lv_obj_set_style_local_text_font(unlockButton, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, &monte16lock);
    lv_obj_align(unlockButton, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, -25);
    lv_label_set_text(labelUnlockButton, MY_UNLOCK_SYMBOL);
    lv_btn_set_fit(unlockButton, LV_FIT_TIGHT);
    lv_obj_set_event_cb(unlockButton, unlockButton_task);

    labelTimeLock = lv_label_create(contDateTimeAtLock, NULL);
    lv_label_set_text(labelTimeLock,  "No WiFi connection");
    lv_label_set_align(labelTimeLock, LV_LABEL_ALIGN_CENTER);
    lv_obj_align(labelTimeLock, NULL, LV_ALIGN_CENTER, 0, 30);

    labelDateLock = lv_label_create(contDateTimeAtLock, NULL);
    lv_label_set_text(labelDateLock, "");
    lv_obj_align(labelDateLock, NULL, LV_ALIGN_CENTER, 0, 0);

    wifiStatusAtLock = lv_label_create(lockScr, wifiStatusAtMain);
    lv_obj_align(wifiStatusAtLock, NULL, LV_ALIGN_CENTER, -36, 10);
    lv_label_set_text(wifiStatusAtLock, LV_SYMBOL_WIFI);
    wifiStatusAtLockWarning = my_lv_label_create(wifiStatusAtLock,  wifiStatusAtMainWarning, 6, 6, LV_SYMBOL_CLOSE, LV_COLOR_RED);

    sdStatusAtLock = lv_label_create(lockScr, sdStatusAtMain);
    lv_obj_align(sdStatusAtLock, NULL, LV_ALIGN_CENTER, 36, 10);
    lv_label_set_text(sdStatusAtLock, LV_SYMBOL_SD_CARD);
    sdStatusAtLockWarning = my_lv_label_create(sdStatusAtLock, wifiStatusAtLockWarning, 2, 6, LV_SYMBOL_CLOSE, LV_COLOR_RED);

    ledAtLock = lv_led_create(lockScr, NULL);
    lv_obj_set_size(ledAtLock, 13, 13);
    lv_obj_align(ledAtLock, NULL, LV_ALIGN_CENTER, 0, 10);
    lv_led_set_bright(ledAtLock, 200);
    lv_obj_set_style_local_bg_color(ledAtLock, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_YELLOW);
    lv_obj_set_style_local_shadow_color(ledAtLock, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_YELLOW);
    lv_obj_set_style_local_border_opa(ledAtLock, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_0);
}