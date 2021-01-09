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
    lv_style_set_text_color(&whiteButtonStyle, LV_STATE_DEFAULT, LV_COLOR_BLACK);
}

void timesettings_screen()
{
    contBarAtTimeSettings = lv_cont_create(time_settings_scr, NULL);
    lv_obj_set_auto_realign(contBarAtTimeSettings, true);                  /*Auto realign when the size changes*/
    lv_obj_align(contBarAtTimeSettings, NULL, LV_ALIGN_IN_TOP_MID, 0, -5); /*This parametrs will be sued when realigned*/
    lv_cont_set_fit4(contBarAtTimeSettings, LV_FIT_PARENT, LV_FIT_PARENT, LV_FIT_NONE, LV_FIT_NONE);
    lv_cont_set_layout(contBarAtTimeSettings, LV_LAYOUT_PRETTY_TOP);
    lv_obj_add_style(contBarAtTimeSettings, LV_OBJ_PART_MAIN, &containerStyle);
    lv_obj_set_style_local_border_opa(contBarAtTimeSettings, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_click(contBarAtTimeSettings, false);

    back_time_settings_btn = lv_btn_create(contBarAtTimeSettings, NULL); //TODO BRAK POS
    back_time_settings_label = lv_label_create(back_time_settings_btn, NULL);
    lv_label_set_text(back_time_settings_label, LV_SYMBOL_LEFT);
    lv_obj_set_size(back_time_settings_btn, 30, 15);
    lv_obj_set_event_cb(back_time_settings_btn, timesettings_back_btn);
    lv_obj_add_style(back_time_settings_btn, LV_OBJ_PART_MAIN, &transparentBackgroundStyle);
    lv_obj_add_style(back_time_settings_btn, LV_OBJ_PART_MAIN, &borderlessStyle);
    lv_obj_add_style(back_time_settings_btn, LV_OBJ_PART_MAIN, &hugeFontStyle);

    timeSettingsLabelAtBar = lv_label_create(contBarAtTimeSettings, NULL);
    lv_label_set_text(timeSettingsLabelAtBar, "Time Settings");

    time_label = lv_label_create(time_settings_scr, NULL);
    lv_obj_set_pos(time_label, 5, 70);
    lv_label_set_text(time_label, "Time [HH:MM]");
    lv_obj_set_style_local_text_color(time_label, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    time_hour = lv_spinbox_create(time_settings_scr, NULL);
    lv_textarea_set_cursor_hidden(time_hour, true);
    lv_textarea_set_text_align(time_hour, LV_LABEL_ALIGN_CENTER);
    lv_spinbox_set_range(time_hour, 0, 23);
    lv_spinbox_set_digit_format(time_hour, 2, 0);
    lv_obj_set_size(time_hour, 40, 34); //TODO ZMIENIONED
    lv_obj_set_pos(time_hour, 165, 61);

    time_hour_increment = my_lv_btn_create(time_settings_scr, NULL, 20, 20, 175, 39, hour_increment);
    lv_theme_apply(time_hour_increment, LV_THEME_SPINBOX_BTN);
    lv_obj_set_style_local_value_str(time_hour_increment, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_PLUS);

    time_hour_decrement = my_lv_btn_create(time_settings_scr, NULL, 20, 20, 175, 97, hour_decrement);
    lv_theme_apply(time_hour_decrement, LV_THEME_SPINBOX_BTN);
    lv_obj_set_style_local_value_str(time_hour_decrement, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_MINUS);

    time_colon_label = lv_label_create(time_settings_scr, NULL);
    lv_obj_set_pos(time_colon_label, 210, 70);
    lv_label_set_text(time_colon_label, ":");
    lv_obj_set_style_local_text_color(time_colon_label, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    time_minute = lv_spinbox_create(time_settings_scr, NULL);
    lv_textarea_set_cursor_hidden(time_minute, true);
    lv_textarea_set_text_align(time_minute, LV_LABEL_ALIGN_CENTER);
    lv_spinbox_set_range(time_minute, 0, 59);
    lv_spinbox_set_digit_format(time_minute, 2, 0);
    lv_obj_set_size(time_minute, 40, 34); //TODO ZMIENIONED
    lv_obj_set_pos(time_minute, 219, 61);

    time_minute_increment = my_lv_btn_create(time_settings_scr, NULL, 20, 20, 229, 39, minute_increment);
    lv_theme_apply(time_minute_increment, LV_THEME_SPINBOX_BTN);
    lv_obj_set_style_local_value_str(time_minute_increment, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_PLUS);

    time_minute_decrement = my_lv_btn_create(time_settings_scr, NULL, 20, 20, 229, 97, minute_decrement);
    lv_theme_apply(time_minute_decrement, LV_THEME_SPINBOX_BTN);
    lv_obj_set_style_local_value_str(time_minute_decrement, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_MINUS);

    date_label = lv_label_create(time_settings_scr, NULL);
    lv_obj_set_pos(date_label, 5, 129);
    lv_label_set_text(date_label, "Date ");
    lv_obj_set_style_local_text_color(date_label, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    date_btn = my_lv_btn_create(time_settings_scr, NULL, 95, 43, 165, 19, date_button_func);

    date_btn_label = lv_label_create(date_btn, NULL);
    lv_label_set_text(date_btn_label, "99.99.9999");
    lv_obj_set_style_local_border_opa(date_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);
    lv_obj_set_style_local_text_color(date_btn_label, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);

    lockScreenLabel = lv_label_create(time_settings_scr, NULL);
    lv_obj_set_pos(lockScreenLabel, 5, 170);
    lv_label_set_text(lockScreenLabel, "Lock screen after ");
    lv_obj_set_style_local_text_color(lockScreenLabel, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lockScreenDDlist = lv_dropdown_create(time_settings_scr, NULL);
    lv_dropdown_set_options(lockScreenDDlist, "1 min\n"
                                              "5 min\n"
                                              "10 min\n"
                                              "60 min\n"
                                              "Never");
    lv_obj_set_size(lockScreenDDlist, 120, 34); //TODO ZMIENIONED
    lv_obj_set_pos(lockScreenDDlist, 165, 164);

    timeSettings_btn = my_lv_btn_create(time_settings_scr, NULL, 75, 33, 240, 200, timesettings_save_btn);
    timeSettings_label = lv_label_create(timeSettings_btn, NULL);
    lv_label_set_text(timeSettings_label, "Save");
    lv_obj_add_style(timeSettings_btn, LV_BTN_PART_MAIN, &whiteButtonStyle);

    sync_rtc_btn = my_lv_btn_create(time_settings_scr, NULL, 130, 33, 5, 200, sync_rtc_func);
    sync_rtc_label = lv_label_create(sync_rtc_btn, NULL);
    lv_label_set_text(sync_rtc_label, "Sync. Clock");
    lv_obj_add_style(sync_rtc_btn, LV_BTN_PART_MAIN, &whiteButtonStyle);
}

void settings_screen()
{
    contBarAtMainSettings = lv_cont_create(settings_scr, NULL);
    lv_obj_set_auto_realign(contBarAtMainSettings, true);                  /*Auto realign when the size changes*/
    lv_obj_align(contBarAtMainSettings, NULL, LV_ALIGN_IN_TOP_MID, 0, -5); /*This parametrs will be sued when realigned*/
    lv_cont_set_fit4(contBarAtMainSettings, LV_FIT_PARENT, LV_FIT_PARENT, LV_FIT_NONE, LV_FIT_NONE);
    lv_cont_set_layout(contBarAtMainSettings, LV_LAYOUT_PRETTY_TOP);
    lv_obj_add_style(contBarAtMainSettings, LV_OBJ_PART_MAIN, &containerStyle);
    lv_obj_set_style_local_border_opa(contBarAtMainSettings, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_click(contBarAtMainSettings, false);

    back_settings_btn = lv_btn_create(contBarAtMainSettings, NULL); //TODO BRAK POS
    back_settings_label = lv_label_create(back_settings_btn, NULL);
    lv_label_set_text(back_settings_label, LV_SYMBOL_LEFT);
    lv_obj_set_size(back_settings_btn, 30, 15);
    lv_obj_set_event_cb(back_settings_btn, btn_settings_back);
    lv_obj_add_style(back_settings_btn, LV_OBJ_PART_MAIN, &transparentButtonStyle);

    settingsLabelAtBar = lv_label_create(contBarAtMainSettings, NULL);
    lv_label_set_text(settingsLabelAtBar, "Settings");

    wifiBtn = my_lv_btn_create(settings_scr, NULL, 60, 60, 60, 38, WiFi_btn);

    wifiBtnLabel = lv_label_create(wifiBtn, NULL);
    lv_label_set_text(wifiBtnLabel, MY_WIFI_SYMBOL);
    lv_obj_add_style(wifiBtn, LV_OBJ_PART_MAIN, &hugeTransparentButtonStyle);
    lv_obj_add_style(wifiBtn, LV_OBJ_PART_MAIN, &whiteFontStyle);

    wifiBtnName = lv_label_create(settings_scr, NULL);
    lv_label_set_text(wifiBtnName, "WiFi");
    lv_obj_set_pos(wifiBtnName, 63, 103);
    lv_obj_add_style(wifiBtnName, LV_OBJ_PART_MAIN, &font20Style);

    infoBtn = my_lv_btn_create(settings_scr, NULL, 60, 60, 200, 38, info_btn);
    infoBtnLabel = lv_label_create(infoBtn, NULL);
    lv_label_set_text(infoBtnLabel, MY_INFO_SYMBOL);
    lv_obj_add_style(infoBtn, LV_OBJ_PART_MAIN, &hugeTransparentButtonStyle);
    lv_obj_add_style(infoBtn, LV_OBJ_PART_MAIN, &whiteFontStyle);

    infoBtnName = lv_label_create(settings_scr, NULL);//TODO DUDUDUDU
    lv_label_set_text(infoBtnName, "Info");
    lv_obj_set_pos(infoBtnName, 207, 103);
    lv_obj_add_style(infoBtnName, LV_OBJ_PART_MAIN, &font20Style);

    timeBtn = my_lv_btn_create(settings_scr, NULL, 60, 60, 60, 140, time_settings_btn);
    timeBtnLabel = lv_label_create(timeBtn, NULL);
    lv_label_set_text(timeBtnLabel, MY_CLOCK_SYMBOL);
    lv_obj_add_style(timeBtn, LV_OBJ_PART_MAIN, &hugeTransparentButtonStyle);
    lv_obj_add_style(timeBtn, LV_OBJ_PART_MAIN, &whiteFontStyle);

    timeBtnName = lv_label_create(settings_scr, NULL);
    lv_label_set_text(timeBtnName, "Time");
    lv_obj_set_pos(timeBtnName, 65, 200);
    lv_obj_add_style(timeBtnName, LV_OBJ_PART_MAIN, &font20Style);

    tempBtn = my_lv_btn_create(settings_scr, NULL, 60, 60, 200, 140, temp_settings_btn);

    tempBtnLabel = lv_label_create(tempBtn, NULL);
    lv_label_set_text(tempBtnLabel, MY_COGS_SYMBOL);
    lv_obj_add_style(tempBtn, LV_OBJ_PART_MAIN, &hugeTransparentButtonStyle);
    lv_obj_add_style(tempBtn, LV_OBJ_PART_MAIN, &whiteFontStyle);

    tempBtnName = lv_label_create(settings_scr, NULL);
    lv_label_set_text(tempBtnName, "Sampling");
    lv_obj_set_pos(tempBtnName, 180, 200);
    lv_obj_add_style(tempBtnName, LV_OBJ_PART_MAIN, &font20Style);
}

void info_screen()
{
    contBarAtMaininfo = lv_cont_create(info_scr, NULL);
    lv_obj_set_auto_realign(contBarAtMaininfo, true);                  /*Auto realign when the size changes*/
    lv_obj_align(contBarAtMaininfo, NULL, LV_ALIGN_IN_TOP_MID, 0, -5); /*This parametrs will be sued when realigned*/
    lv_cont_set_fit4(contBarAtMaininfo, LV_FIT_PARENT, LV_FIT_PARENT, LV_FIT_NONE, LV_FIT_NONE);
    lv_cont_set_layout(contBarAtMaininfo, LV_LAYOUT_PRETTY_TOP);
    lv_obj_add_style(contBarAtMaininfo, LV_OBJ_PART_MAIN, &containerStyle);
    lv_obj_set_style_local_border_opa(contBarAtMaininfo, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_click(contBarAtMaininfo, false);

    back_info_btn = lv_btn_create(contBarAtMaininfo, NULL); //TODO BRAK POS
    back_info_label = lv_label_create(back_info_btn, NULL);
    lv_label_set_text(back_info_label, LV_SYMBOL_LEFT);
    lv_obj_set_size(back_info_btn, 30, 15);
    lv_obj_set_event_cb(back_info_btn, setButton_task);
    lv_obj_add_style(back_info_btn, LV_OBJ_PART_MAIN, &transparentButtonStyle);

    lcdLabelAtBar = lv_label_create(contBarAtMaininfo, NULL);
    lv_label_set_text(lcdLabelAtBar, "Device info");

    info_wifi_label = lv_label_create(info_scr, NULL);
    lv_label_set_text(info_wifi_label, "WiFi address: ");
    lv_obj_set_style_local_text_color(info_wifi_label, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    lv_obj_set_pos(info_wifi_label, 5, 53);
    info_wifi_address_label = lv_label_create(info_scr, NULL);
    lv_obj_set_pos(info_wifi_address_label, 115, 53);
    lv_obj_set_style_local_text_color(info_wifi_address_label, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    config_label = lv_label_create(info_scr, NULL);
    lv_obj_set_pos(config_label, 5, 70);
    lv_obj_set_style_local_text_color(config_label, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
}

void main_screen()
{
    contBarAtMain = lv_cont_create(main_scr, NULL);
    lv_obj_set_auto_realign(contBarAtMain, true);                  /*Auto realign when the size changes*/
    lv_obj_align(contBarAtMain, NULL, LV_ALIGN_IN_TOP_MID, 0, -5); /*This parametrs will be sued when realigned*/
    lv_cont_set_fit4(contBarAtMain, LV_FIT_PARENT, LV_FIT_PARENT, LV_FIT_NONE, LV_FIT_NONE);
    lv_obj_add_style(contBarAtMain, LV_OBJ_PART_MAIN, &containerStyle);
    lv_obj_set_style_local_border_opa(contBarAtMain, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_click(contBarAtMain, false);
 
	lockButton = lv_btn_create(main_scr, NULL); //TODO BRAK POS
	lv_obj_add_style(lockButton, LV_OBJ_PART_MAIN, &transparentButtonStyle);
    lv_obj_set_style_local_text_font(lockButton, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, &monte16lock);
	labelLockButton = lv_label_create(lockButton, NULL);
    lv_obj_align(lockButton, NULL, LV_ALIGN_IN_TOP_LEFT, 37, -5);
	lv_label_set_text(labelLockButton, MY_LOCK_SYMBOL);
	lv_btn_set_fit(lockButton, LV_FIT_TIGHT);
	lv_obj_set_event_cb(lockButton, lockButton_task);
    

    wifiStatusAtMain = lv_label_create(contBarAtMain, NULL);
    lv_label_set_text(wifiStatusAtMain, LV_SYMBOL_WIFI);
    lv_obj_align(wifiStatusAtMain, NULL, LV_ALIGN_IN_LEFT_MID, 52, 0);

    sdStatusAtMain = lv_label_create(contBarAtMain, NULL);
    lv_label_set_text(sdStatusAtMain, LV_SYMBOL_SD_CARD);
    lv_obj_align(sdStatusAtMain, NULL, LV_ALIGN_IN_LEFT_MID, 77, 0);

    setButton = lv_btn_create(main_scr, NULL); //TODO BRAK POS
    labelSetButton = lv_label_create(setButton, NULL);
    lv_label_set_text(labelSetButton, LV_SYMBOL_SETTINGS);
    lv_obj_align(setButton, NULL, LV_ALIGN_IN_TOP_LEFT, -25, -5);
    lv_btn_set_fit(setButton, LV_FIT_TIGHT);
    lv_obj_set_event_cb(setButton, setButton_task);
    lv_obj_add_style(setButton, LV_OBJ_PART_MAIN, &transparentButtonStyle);


    wifiStatusAtMainWarning = lv_label_create(wifiStatusAtMain, NULL);
    lv_label_set_text(wifiStatusAtMainWarning, LV_SYMBOL_CLOSE);
    lv_obj_add_style(wifiStatusAtMainWarning, LV_OBJ_PART_MAIN, &tinySymbolStyle);
    lv_obj_add_style(wifiStatusAtMainWarning, LV_OBJ_PART_MAIN, &font12Style);
    lv_obj_set_pos(wifiStatusAtMainWarning, 5, 5);

    sdStatusAtMainWarning = lv_label_create(sdStatusAtMain, NULL);
    lv_label_set_text(sdStatusAtMainWarning, LV_SYMBOL_CLOSE);
    lv_obj_add_style(sdStatusAtMainWarning, LV_OBJ_PART_MAIN, &tinySymbolStyle);
    lv_obj_add_style(sdStatusAtMainWarning, LV_OBJ_PART_MAIN, &font12Style);
    lv_obj_set_pos(sdStatusAtMainWarning, 2, 5);

    dateAndTimeAtBar = lv_label_create(contBarAtMain, NULL);
    lv_label_set_text(dateAndTimeAtBar, "");
    lv_obj_set_pos(dateAndTimeAtBar, 157, 13);
    //lv_obj_align(dateAndTimeAtBar, NULL, LV_ALIGN_IN_RIGHT_MID, -120, 0);

    contTemp = my_lv_cont_create(main_scr, NULL, 122, 46, 188, 30);
    lv_obj_set_click(contTemp, false);
    lv_obj_add_style(contTemp, LV_OBJ_PART_MAIN, &containerStyle);

    contHumi = my_lv_cont_create(main_scr, NULL, 122, 46, 188, 74);
    lv_obj_set_click(contHumi, false);
    lv_obj_add_style(contHumi, LV_OBJ_PART_MAIN, &containerStyle);

    contPM10 = my_lv_cont_create(main_scr, NULL, 91, 62, 10, 118);
    lv_obj_set_click(contPM10, false);
    lv_obj_add_style(contPM10, LV_OBJ_PART_MAIN, &containerStyle);
    lv_obj_add_style(contPM10, LV_OBJ_PART_MAIN, &font12Style);

    contPM25 = my_lv_cont_create(main_scr, NULL, 180, 90, 10, 30);
    lv_obj_set_click(contPM25, false);
    lv_obj_add_style(contPM25, LV_OBJ_PART_MAIN, &containerStyle);

    contPM100 = my_lv_cont_create(main_scr, NULL, 91, 62, 99, 118);
    lv_obj_set_click(contPM100, false);
    lv_obj_add_style(contPM100, LV_OBJ_PART_MAIN, &containerStyle);
    lv_obj_add_style(contPM100, LV_OBJ_PART_MAIN, &font12Style);

    contAQI = my_lv_cont_create(main_scr, NULL, 122, 62, 188, 118);
    lv_obj_set_click(contAQI, false);
    lv_obj_add_style(contAQI, LV_OBJ_PART_MAIN, &containerStyle);
    lv_obj_add_style(contAQI, LV_OBJ_PART_MAIN, &font12Style);

    contAQIColorBar = my_lv_cont_create(contAQI, NULL, 92, 24, 15, 25);
    lv_obj_set_click(contAQIColorBar, false);
    lv_obj_add_style(contAQIColorBar, LV_OBJ_PART_MAIN, &containerStyle);
    lv_obj_add_style(contAQIColorBar, LV_OBJ_PART_MAIN, &font12Style);
    lv_obj_set_style_local_bg_opa(contAQIColorBar, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_70);
    lv_obj_set_style_local_bg_color(contAQIColorBar, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_GREEN);

    labelTemp = lv_label_create(contTemp, NULL);
    lv_obj_set_pos(labelTemp, 5, 3);
    lv_label_set_text(labelTemp, "Temp");

    labelTempValue = lv_label_create(contTemp, NULL);
    lv_obj_add_style(labelTempValue, LV_OBJ_PART_MAIN, &font20Style);
    lv_obj_set_pos(labelTempValue, 16, 22);
    lv_label_set_text(labelTempValue, "         -");
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

    ledAtMain = lv_led_create(main_scr, NULL);
    lv_obj_set_size(ledAtMain, 13, 13);
    lv_obj_set_pos(ledAtMain, 13, 10);
    lv_led_set_bright(ledAtMain, 200);
    lv_obj_set_style_local_bg_color(ledAtMain, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_YELLOW);
    lv_obj_set_style_local_shadow_color(ledAtMain, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_YELLOW);
    lv_obj_set_style_local_border_opa(ledAtMain, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_0);
    //Function that draws lines and st text above those
    drawParticlesIndicator();
}

void wifi_screen()
{
    contBarAtMainWiFi = lv_cont_create(wifi_scr, NULL);
    lv_obj_set_auto_realign(contBarAtMainWiFi, true);
    lv_obj_align(contBarAtMainWiFi, NULL, LV_ALIGN_IN_TOP_MID, 0, -5);
    lv_cont_set_fit4(contBarAtMainWiFi, LV_FIT_PARENT, LV_FIT_PARENT, LV_FIT_NONE, LV_FIT_NONE);
    lv_cont_set_layout(contBarAtMainWiFi, LV_LAYOUT_PRETTY_TOP);
    lv_obj_add_style(contBarAtMainWiFi, LV_OBJ_PART_MAIN, &containerStyle);
    lv_obj_set_style_local_border_opa(contBarAtMainWiFi, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_click(contBarAtMainWiFi, false);

    cancel_btn = lv_btn_create(contBarAtMainWiFi, NULL); //TODO brak pos
    cancel_label = lv_label_create(cancel_btn, NULL);
    lv_label_set_text(cancel_label, LV_SYMBOL_LEFT);
    lv_obj_set_size(cancel_btn, 30, 15);
    lv_obj_set_event_cb(cancel_btn, btn_cancel);
    lv_obj_add_style(cancel_btn, LV_OBJ_PART_MAIN, &transparentButtonStyle);

    wifiLabelAtBar = lv_label_create(contBarAtMainWiFi, NULL);
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
    lv_obj_set_size(ssid_ta, 140, 34); //TODO ZMIENIONED
    lv_obj_set_pos(ssid_ta, 100, 45);

    pwd_label = lv_label_create(wifi_scr, NULL);
    lv_label_set_text(pwd_label, "Password: ");
    lv_obj_set_pos(pwd_label, 5, 92);
    lv_obj_set_style_local_text_color(pwd_label, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    pwd_ta = lv_textarea_create(wifi_scr, NULL);
    lv_textarea_set_text(pwd_ta, "");
    lv_textarea_set_pwd_show_time(pwd_ta, 5000);
    lv_textarea_set_pwd_mode(pwd_ta, true);
    lv_textarea_set_one_line(pwd_ta, true);
    lv_obj_set_event_cb(pwd_ta, ta_event_cb);
    lv_textarea_set_cursor_hidden(pwd_ta, true);
    lv_obj_set_size(pwd_ta, 140, 34); //TODO ZMIENIONED
    lv_obj_set_pos(pwd_ta, 100, 85);

    show_hide_btn = my_lv_btn_create(wifi_scr, NULL, 75, 43, 243, 85, show_hide_btn_func);
    show_hide_btn_label = lv_label_create(show_hide_btn, NULL);
    lv_label_set_text(show_hide_btn_label, LV_SYMBOL_EYE_OPEN);
    lv_obj_add_style(show_hide_btn, LV_OBJ_PART_MAIN, &transparentButtonStyle);
    lv_obj_set_style_local_text_color(show_hide_btn_label, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    apply_btn = my_lv_btn_create(wifi_scr, NULL, 75, 43, 243, 43, btn_connect);
    apply_label = lv_label_create(apply_btn, NULL);
    lv_label_set_text(apply_label, "Connect");
    lv_obj_add_style(apply_btn, LV_OBJ_PART_MAIN, &whiteButtonStyle);
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

    unlockButton = lv_btn_create(lock_scr, NULL);//TODO BRAK POS
    labelUnlockButton = lv_label_create(unlockButton, NULL);
    lv_obj_align(unlockButton, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);
    lv_label_set_text(labelUnlockButton, MY_UNLOCK_SYMBOL);
    lv_btn_set_fit(unlockButton, LV_FIT_TIGHT);
    lv_obj_set_event_cb(unlockButton, unlockButton_task);
    lv_obj_add_style(unlockButton, LV_OBJ_PART_MAIN, &transparentButtonStyle);
    lv_obj_set_style_local_text_font(unlockButton, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, &monte16lock);

    labelTimeLock = lv_label_create(contDateTimeAtLock, NULL);
    lv_label_set_text(labelTimeLock, "Connect to wifi");
    lv_label_set_align(labelTimeLock, LV_LABEL_ALIGN_CENTER);
    lv_obj_align(labelTimeLock, NULL, LV_ALIGN_CENTER, 0, 30);

    labelDateLock = lv_label_create(contDateTimeAtLock, NULL);
    lv_label_set_text(labelDateLock, "");
    lv_obj_align(labelDateLock, NULL, LV_ALIGN_CENTER, 0, 0);

    wifiStatusAtLock = lv_label_create(lock_scr, NULL);
    lv_obj_align(wifiStatusAtLock, NULL, LV_ALIGN_CENTER, 42, 10);
    lv_label_set_text(wifiStatusAtLock, LV_SYMBOL_WIFI);
    lv_obj_add_style(wifiStatusAtLock, LV_OBJ_PART_MAIN, &font16Style);

    sdStatusAtLock = lv_label_create(lock_scr, NULL);
    lv_obj_align(sdStatusAtLock, NULL, LV_ALIGN_CENTER, -30, 10);
    lv_label_set_text(sdStatusAtLock, LV_SYMBOL_SD_CARD);
    lv_obj_add_style(sdStatusAtLock, LV_OBJ_PART_MAIN, &font16Style);

    wifiStatusAtLockWarning = my_lv_label_create(wifiStatusAtLock, NULL, 5, 5, LV_SYMBOL_CLOSE);
    lv_obj_add_style(wifiStatusAtLockWarning, LV_OBJ_PART_MAIN, &tinySymbolStyle);
    lv_obj_add_style(wifiStatusAtLockWarning, LV_OBJ_PART_MAIN, &font12Style);

    sdStatusAtLockWarning = my_lv_label_create(sdStatusAtLock, NULL, 2, 5, LV_SYMBOL_CLOSE);
    lv_obj_add_style(sdStatusAtLockWarning, LV_OBJ_PART_MAIN, &tinySymbolStyle);
    lv_obj_add_style(sdStatusAtLockWarning, LV_OBJ_PART_MAIN, &font12Style);

    ledAtLock = lv_led_create(lock_scr, NULL);
    lv_obj_set_size(ledAtLock, 13, 13);
    lv_obj_set_pos(ledAtLock, 13, 10);
    lv_led_set_bright(ledAtLock, 200);
    lv_obj_set_style_local_bg_color(ledAtLock, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_YELLOW);
    lv_obj_set_style_local_shadow_color(ledAtLock, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_YELLOW);
    lv_obj_set_style_local_border_opa(ledAtLock, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_0);
}

void samplingSettings_screen()
{
    contBarAtSampling = lv_cont_create(sampling_settings_scr, NULL);
    lv_obj_set_auto_realign(contBarAtSampling, true);                  /*Auto realign when the size changes*/
    lv_obj_align(contBarAtSampling, NULL, LV_ALIGN_IN_TOP_MID, 0, -5); /*This parametrs will be sued when realigned*/
    lv_cont_set_fit4(contBarAtSampling, LV_FIT_PARENT, LV_FIT_PARENT, LV_FIT_NONE, LV_FIT_NONE);
    lv_cont_set_layout(contBarAtSampling, LV_LAYOUT_PRETTY_TOP);
    lv_obj_add_style(contBarAtSampling, LV_OBJ_PART_MAIN, &containerStyle);
    lv_obj_set_style_local_border_opa(contBarAtSampling, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_click(contBarAtSampling, false);

    back_sampling_settings_btn = lv_btn_create(contBarAtSampling, NULL); //TODO BRAK POS
    back_sampling_settings_label = lv_label_create(back_sampling_settings_btn, NULL);
    lv_label_set_text(back_sampling_settings_label, LV_SYMBOL_LEFT);
    lv_obj_set_size(back_sampling_settings_btn, 30, 15);
    lv_obj_set_event_cb(back_sampling_settings_btn, sampling_settings_back_btn);
    lv_obj_add_style(back_sampling_settings_btn, LV_OBJ_PART_MAIN, &transparentButtonStyle);

    sampling_save_btn = lv_btn_create(contBarAtSampling, NULL); //TODO BRAK POS
    sampling_save_label = lv_label_create(sampling_save_btn, NULL);
    lv_label_set_text(sampling_save_label, "Save");
    lv_obj_set_size(sampling_save_btn, 75, 25);
    lv_obj_set_event_cb(sampling_save_btn, sampling_settings_save_btn);
    lv_obj_add_style(sampling_save_btn, LV_BTN_PART_MAIN, &whiteButtonStyle);

    measure_period_label = my_lv_label_create(sampling_settings_scr, NULL, 5, 52, "Measurements \nsaving time \n[HH:MM:SS]", LV_COLOR_WHITE);
    lv_obj_set_style_local_text_font(measure_period_label, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_14);

    measure_period_hour = lv_spinbox_create(sampling_settings_scr, NULL);
    lv_textarea_set_cursor_hidden(measure_period_hour, true);
    lv_textarea_set_text_align(measure_period_hour, LV_LABEL_ALIGN_CENTER);
    lv_spinbox_set_range(measure_period_hour, 0, 24);
    lv_spinbox_set_digit_format(measure_period_hour, 2, 0);
    lv_obj_set_size(measure_period_hour, 40, 34); //TODO ZMIENIONED
    lv_obj_set_pos(measure_period_hour, 165, 61);

    measure_period_hour_increment = my_lv_btn_create(sampling_settings_scr, NULL, 20, 20, 175, 39, sampling_hour_increment);
    lv_theme_apply(measure_period_hour_increment, LV_THEME_SPINBOX_BTN);
    lv_obj_set_style_local_value_str(measure_period_hour_increment, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_PLUS);

    measure_period_hour_decrement = my_lv_btn_create(sampling_settings_scr, NULL, 20, 20, 175, 97, sampling_hour_decrement);
    lv_theme_apply(measure_period_hour_decrement, LV_THEME_SPINBOX_BTN);
    lv_obj_set_style_local_value_str(measure_period_hour_decrement, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_MINUS);

    measure_colon_label = my_lv_label_create(sampling_settings_scr, NULL, 210, 70, ":", LV_COLOR_WHITE);

    measure_period_minute = lv_spinbox_create(sampling_settings_scr, NULL);
    lv_textarea_set_cursor_hidden(measure_period_minute, true);
    lv_textarea_set_text_align(measure_period_minute, LV_LABEL_ALIGN_CENTER);
    lv_spinbox_set_range(measure_period_minute, 0, 59);
    lv_spinbox_set_digit_format(measure_period_minute, 2, 0);
    lv_obj_set_size(measure_period_minute, 40, 34); //TODO ZMIENIONED
    lv_obj_set_pos(measure_period_minute, 219, 61);

    measure_period_minute_increment = my_lv_btn_create(sampling_settings_scr, NULL, 20, 20, 229, 39, sampling_minute_increment);
    lv_theme_apply(measure_period_minute_increment, LV_THEME_SPINBOX_BTN);
    lv_obj_set_style_local_value_str(measure_period_minute_increment, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_PLUS);

    measure_period_minute_decrement = my_lv_btn_create(sampling_settings_scr, NULL, 20, 20, 229, 97, sampling_minute_decrement);
    lv_theme_apply(measure_period_minute_decrement, LV_THEME_SPINBOX_BTN);
    lv_obj_set_style_local_value_str(measure_period_minute_decrement, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_MINUS);

    measure_colon_label2 = my_lv_label_create(sampling_settings_scr, NULL, 265, 70, ":", LV_COLOR_WHITE);

    measure_period_second = lv_spinbox_create(sampling_settings_scr, NULL);
    lv_textarea_set_cursor_hidden(measure_period_second, true);
    lv_textarea_set_text_align(measure_period_second, LV_LABEL_ALIGN_CENTER);
    lv_spinbox_set_range(measure_period_second, 0, 59);
    lv_spinbox_set_digit_format(measure_period_second, 2, 0);
    lv_obj_set_size(measure_period_second, 40, 34); //TODO ZMIENIONED
    lv_obj_set_pos(measure_period_second, 274, 61);

    measure_period_second_increment = my_lv_btn_create(sampling_settings_scr, NULL, 20, 20, 284, 39, sampling_second_increment);
    lv_theme_apply(measure_period_second_increment, LV_THEME_SPINBOX_BTN);
    lv_obj_set_style_local_value_str(measure_period_second_increment, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_PLUS);

    measure_period_second_decrement = my_lv_btn_create(sampling_settings_scr, NULL, 20, 20, 284, 97, sampling_second_decrement);
    lv_theme_apply(measure_period_second_decrement, LV_THEME_SPINBOX_BTN);
    lv_obj_set_style_local_value_str(measure_period_second_decrement, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_MINUS);

    measure_number_label = lv_label_create(sampling_settings_scr, NULL);
    lv_obj_set_pos(measure_number_label, 5, 127);
    lv_obj_set_style_local_text_font(measure_number_label, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_14);
    lv_label_set_text(measure_number_label, "Number of samples");
    lv_obj_set_style_local_text_color(measure_number_label, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    measure_number = lv_spinbox_create(sampling_settings_scr, NULL);
    lv_textarea_set_cursor_hidden(measure_number, true);
    lv_textarea_set_text_align(measure_number, LV_LABEL_ALIGN_CENTER);
    lv_spinbox_set_digit_format(measure_number, 2, 0);
    lv_spinbox_set_range(measure_number, 1, 20);
    lv_obj_set_size(measure_number, 50, 34); //TODO ZMIENIONED
    lv_obj_set_pos(measure_number, 206, 119);

    measure_number_increment = my_lv_btn_create(sampling_settings_scr, NULL, 40, 34, 257, 119, measure_number_increment_func);
    lv_theme_apply(measure_number_increment, LV_THEME_SPINBOX_BTN);
    lv_obj_set_style_local_value_str(measure_number_increment, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_PLUS);

    measure_number_decrement = my_lv_btn_create(sampling_settings_scr, NULL, 40, 34, 165, 119, measure_number_decrement_func);
    lv_theme_apply(measure_number_decrement, LV_THEME_SPINBOX_BTN);
    lv_obj_set_style_local_value_str(measure_number_decrement, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_MINUS);

    measure_av_period_label = lv_label_create(sampling_settings_scr, NULL);
    lv_obj_set_pos(measure_av_period_label, 5, 165);
    lv_obj_set_style_local_text_font(measure_av_period_label, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_14);
    lv_label_set_text(measure_av_period_label, "Time between \nmeasurments [s]:");
    lv_obj_set_style_local_text_color(measure_av_period_label, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    measure_av_period = lv_spinbox_create(sampling_settings_scr, NULL);
    lv_textarea_set_cursor_hidden(measure_av_period, true);
    lv_textarea_set_text_align(measure_av_period, LV_LABEL_ALIGN_CENTER);
    lv_spinbox_set_digit_format(measure_av_period, 3, 0);
    lv_spinbox_set_range(measure_av_period, 5, 999);
    lv_obj_set_size(measure_av_period, 50, 34); //TODO ZMIENIONED
    lv_obj_set_pos(measure_av_period, 206, 160);

    measure_av_period_increment = my_lv_btn_create(sampling_settings_scr, NULL, 40, 34, 257, 160, av_period_increment);
    lv_theme_apply(measure_av_period_increment, LV_THEME_SPINBOX_BTN);
    lv_obj_set_style_local_value_str(measure_av_period_increment, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_PLUS);

    measure_av_period_decrement = my_lv_btn_create(sampling_settings_scr, NULL, 40, 34, 165, 160, av_period_decrement);
    lv_theme_apply(measure_av_period_decrement, LV_THEME_SPINBOX_BTN);
    lv_obj_set_style_local_value_str(measure_av_period_decrement, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_MINUS);

    turn_fan_on_time_label = lv_label_create(sampling_settings_scr, NULL);
    lv_obj_set_pos(turn_fan_on_time_label, 5, 205);
    lv_obj_set_style_local_text_font(turn_fan_on_time_label, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_14);
    lv_label_set_text(turn_fan_on_time_label, "Fan running time \nbefore measure [s]:");
    lv_obj_set_style_local_text_color(turn_fan_on_time_label, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    turn_fan_on_time = lv_spinbox_create(sampling_settings_scr, NULL);
    lv_textarea_set_cursor_hidden(turn_fan_on_time, true);
    lv_textarea_set_text_align(turn_fan_on_time, LV_LABEL_ALIGN_CENTER);
    lv_spinbox_set_digit_format(turn_fan_on_time, 3, 0);
    lv_spinbox_set_range(turn_fan_on_time, 1, 999);
    lv_obj_set_size(turn_fan_on_time, 50, 34); //TODO ZMIENIONED
    lv_obj_set_pos(turn_fan_on_time, 206, 200);

    turn_fan_on_time_increment = my_lv_btn_create(sampling_settings_scr, NULL, 40, 34, 257, 200, turn_fan_on_time_increment_func);
    lv_theme_apply(turn_fan_on_time_increment, LV_THEME_SPINBOX_BTN);
    lv_obj_set_style_local_value_str(turn_fan_on_time_increment, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_PLUS);

    turn_fan_on_time_decrement = my_lv_btn_create(sampling_settings_scr, NULL, 40, 34, 165, 200, turn_fan_on_time_decrement_func);
    lv_theme_apply(turn_fan_on_time_decrement, LV_THEME_SPINBOX_BTN);
    lv_obj_set_style_local_value_str(turn_fan_on_time_decrement, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_MINUS);
}