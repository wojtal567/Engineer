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

void timesettings_screen()
{
    back_time_settings_btn = my_lv_btn_create(time_settings_scr, back_settings_btn, 30, 15, 14, 10, timesettings_back_btn); 
    back_time_settings_label = lv_label_create(back_time_settings_btn, NULL);
    lv_label_set_text(back_time_settings_label, LV_SYMBOL_LEFT);

    timeSettingsLabelAtBar = my_lv_label_create(time_settings_scr, NULL, 195, 10, "Time Settings");

    time_label = my_lv_label_create(time_settings_scr, timeSettingsLabelAtBar, 5, 70, "Time [HH:MM]");

    time_hour = lv_spinbox_create(time_settings_scr, NULL);
    lv_textarea_set_cursor_hidden(time_hour, true);
    lv_textarea_set_text_align(time_hour, LV_LABEL_ALIGN_CENTER);
    lv_spinbox_set_range(time_hour, 0, 23);
    lv_spinbox_set_digit_format(time_hour, 2, 0);
    lv_obj_set_size(time_hour, 40, 34);
    lv_obj_set_pos(time_hour, 165, 61);

    time_hour_increment = my_lv_btn_create(time_settings_scr, NULL, 20, 20, 175, 39, hour_increment);
    lv_theme_apply(time_hour_increment, LV_THEME_SPINBOX_BTN);
    lv_obj_add_style(time_hour_increment, LV_OBJ_PART_MAIN, &borderlessStyle);
    lv_obj_set_style_local_value_str(time_hour_increment, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_PLUS);

    time_hour_decrement = my_lv_btn_create(time_settings_scr, time_hour_increment, 20, 20, 175, 97, hour_decrement);
    lv_obj_set_style_local_value_str(time_hour_decrement, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_MINUS);

    time_colon_label = my_lv_label_create(time_settings_scr, time_label, 210, 70, ":");
    lv_obj_set_style_local_text_color(time_colon_label, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    time_minute = lv_spinbox_create(time_settings_scr, NULL);
    lv_textarea_set_cursor_hidden(time_minute, true);
    lv_textarea_set_text_align(time_minute, LV_LABEL_ALIGN_CENTER);
    lv_spinbox_set_range(time_minute, 0, 59);
    lv_spinbox_set_digit_format(time_minute, 2, 0);
    lv_obj_set_size(time_minute, 40, 34); 
    lv_obj_set_pos(time_minute, 219, 61);

    time_minute_increment = my_lv_btn_create(time_settings_scr, time_hour_increment, 20, 20, 229, 39, minute_increment);
    lv_obj_set_style_local_value_str(time_minute_increment, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_PLUS);

    time_minute_decrement = my_lv_btn_create(time_settings_scr, time_hour_increment, 20, 20, 229, 97, minute_decrement);
    lv_obj_set_style_local_value_str(time_minute_decrement, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_MINUS);

    date_label = my_lv_label_create(time_settings_scr, NULL, 5, 129, "Date ");

    date_btn = my_lv_btn_create(time_settings_scr, NULL, 95, 43, 165, 119, date_button_func);
    date_btn_label = lv_label_create(date_btn, NULL);
    lv_label_set_text(date_btn_label, "99.99.9999");
    lv_obj_set_style_local_border_opa(date_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);
    lv_obj_set_style_local_text_color(date_btn_label, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);

    lockScreenLabel = my_lv_label_create(time_settings_scr, NULL, 5, 170, "Lock screen after ");
    lockScreenDDlist = lv_dropdown_create(time_settings_scr, NULL);
    lv_dropdown_set_options(lockScreenDDlist, "1 min\n"
                                              "5 min\n"
                                              "10 min\n"
                                              "60 min\n"
                                              "Never");
    lv_obj_set_size(lockScreenDDlist, 120, 34); 
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
    back_settings_btn = my_lv_btn_create(settings_scr, NULL, 30, 15, 14, 10, btn_settings_back);
    back_settings_label = lv_label_create(back_settings_btn, NULL);
    lv_label_set_text(back_settings_label, LV_SYMBOL_LEFT);
    lv_obj_add_style(back_settings_btn, LV_OBJ_PART_MAIN, &transparentBackgroundStyle);
    lv_obj_add_style(back_settings_btn, LV_OBJ_PART_MAIN, &borderlessStyle);
    lv_obj_add_style(back_settings_btn, LV_OBJ_PART_MAIN, &whiteFontStyle);

    settingsLabelAtBar = my_lv_label_create(settings_scr, NULL, 239, 10, "Settings");

    wifiBtn = my_lv_btn_create(settings_scr, NULL, 60, 60, 60, 38, WiFi_btn);
    wifiBtnLabel = lv_label_create(wifiBtn, NULL);
    lv_label_set_text(wifiBtnLabel, MY_WIFI_SYMBOL);
    lv_obj_add_style(wifiBtn, LV_OBJ_PART_MAIN, &transparentBackgroundStyle);
    lv_obj_add_style(wifiBtn, LV_OBJ_PART_MAIN, &borderlessStyle);
    lv_obj_add_style(wifiBtn, LV_OBJ_PART_MAIN, &whiteFontStyle);
    lv_obj_add_style(wifiBtn, LV_OBJ_PART_MAIN, &hugeFontStyle);

    wifiBtnName = my_lv_label_create(settings_scr, NULL, 63, 103, "WiFi");
    lv_obj_add_style(wifiBtnName, LV_OBJ_PART_MAIN, &font20Style);

    infoBtn = my_lv_btn_create(settings_scr, wifiBtn, 60, 60, 200, 38, info_btn);
    infoBtnLabel = lv_label_create(infoBtn, NULL);
    lv_label_set_text(infoBtnLabel, MY_INFO_SYMBOL);

    infoBtnName = my_lv_label_create(settings_scr, wifiBtnName, 207, 103, "Info");

    timeBtn = my_lv_btn_create(settings_scr, wifiBtn, 60, 60, 60, 140, time_settings_btn);
    timeBtnLabel = lv_label_create(timeBtn, NULL);
    lv_label_set_text(timeBtnLabel, MY_CLOCK_SYMBOL);

    timeBtnName = my_lv_label_create(settings_scr, wifiBtnName, 65, 200, "Time");

    tempBtn = my_lv_btn_create(settings_scr, wifiBtn, 60, 60, 200, 140, temp_settings_btn);
    tempBtnLabel = lv_label_create(tempBtn, wifiBtnName);
    lv_label_set_text(tempBtnLabel, MY_COGS_SYMBOL);
    lv_obj_add_style(tempBtnLabel, LV_OBJ_PART_MAIN, &hugeFontStyle);
    tempBtnName = my_lv_label_create(settings_scr, wifiBtnName, 180, 200, "Sampling");
}

void info_screen()
{
    back_info_btn = my_lv_btn_create(info_scr, back_settings_btn, 30, 15, 14, 10, setButton_task);
    back_info_label = lv_label_create(back_info_btn, NULL);
    lv_label_set_text(back_info_label, LV_SYMBOL_LEFT);

    lcdLabelAtBar = my_lv_label_create(info_scr, NULL, 216, 10, "Device info");

    info_wifi_label = my_lv_label_create(info_scr, NULL, 5, 53, "WiFi address: ");

    info_wifi_address_label = my_lv_label_create(info_scr, NULL, 115, 53, "");

    config_label = my_lv_label_create(info_scr, NULL, 5, 70, "");

}

void main_screen()
{
    setButton = my_lv_btn_create(main_scr, NULL, 16, 18, 32, 7, setButton_task);
    labelSetButton = lv_label_create(setButton, NULL);
    lv_label_set_text(labelSetButton, LV_SYMBOL_SETTINGS);
    lv_obj_add_style(setButton, LV_OBJ_PART_MAIN, &transparentBackgroundStyle);
    lv_obj_add_style(setButton, LV_OBJ_PART_MAIN, &borderlessStyle);
    lv_obj_add_style(setButton, LV_OBJ_PART_MAIN, &whiteFontStyle);

	lockButton = my_lv_btn_create(main_scr, setButton, 14, 18, 95, 7, lockButton_task);
	labelLockButton = lv_label_create(lockButton, NULL);
    lv_obj_set_style_local_text_font(lockButton, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, &monte16lock);
	lv_label_set_text(labelLockButton, MY_LOCK_SYMBOL);

    wifiStatusAtMain = my_lv_label_create(main_scr, NULL, 52, 7, LV_SYMBOL_WIFI, LV_COLOR_WHITE);
    wifiStatusAtMainWarning = my_lv_label_create(wifiStatusAtMain, NULL, 6, 6, LV_SYMBOL_CLOSE, LV_COLOR_RED);
    lv_obj_add_style(wifiStatusAtMainWarning, LV_OBJ_PART_MAIN, &transparentBackgroundStyle);
    lv_obj_add_style(wifiStatusAtMainWarning, LV_OBJ_PART_MAIN, &font12Style);

    sdStatusAtMain = my_lv_label_create(main_scr, wifiStatusAtMain, 77, 7, LV_SYMBOL_SD_CARD, LV_COLOR_WHITE);
    sdStatusAtMainWarning = my_lv_label_create(sdStatusAtMain, wifiStatusAtMainWarning, 2, 6, LV_SYMBOL_CLOSE, LV_COLOR_RED);

    dateAndTimeAtBar = my_lv_label_create(main_scr, NULL, 157, 7, "");

    contPM25 = my_lv_cont_create(main_scr, NULL, 180, 90, 10, 30);
    lv_obj_set_click(contPM25, false);
    lv_obj_add_style(contPM25, LV_OBJ_PART_MAIN, &transparentBackgroundStyle);
    lv_obj_add_style(contPM25, LV_OBJ_PART_MAIN, &containerStyle);

    contTemp = my_lv_cont_create(main_scr, contPM25, 122, 46, 188, 30);
    lv_obj_set_click(contTemp, false);

    contHumi = my_lv_cont_create(main_scr, contPM25, 122, 46, 188, 74);
    lv_obj_set_click(contHumi, false);

    contPM10 = my_lv_cont_create(main_scr, NULL, 91, 62, 10, 118);
    lv_obj_set_click(contPM10, false);
    lv_obj_add_style(contPM10, LV_OBJ_PART_MAIN, &transparentBackgroundStyle);
    lv_obj_add_style(contPM10, LV_OBJ_PART_MAIN, &containerStyle);
    lv_obj_add_style(contPM10, LV_OBJ_PART_MAIN, &font12Style);

    contPM100 = my_lv_cont_create(main_scr, contPM10, 91, 62, 99, 118);
    lv_obj_set_click(contPM100, false);

    contAQI = my_lv_cont_create(main_scr, contPM10, 122, 62, 188, 118);
    lv_obj_set_click(contAQI, false);

    contAQIColorBar = my_lv_cont_create(contAQI, contAQI, 92, 24, 15, 25);
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

void samplingSettings_screen()
{
    back_sampling_settings_btn = my_lv_btn_create(sampling_settings_scr, back_settings_btn, 30, 15, 14, 10, sampling_settings_back_btn); 
    back_sampling_settings_label = lv_label_create(back_sampling_settings_btn, NULL);
    lv_label_set_text(back_sampling_settings_label, LV_SYMBOL_LEFT);

    sampling_save_btn = my_lv_btn_create(sampling_settings_scr, NULL, 75, 25, 231, 10, sampling_settings_save_btn);
    sampling_save_label = lv_label_create(sampling_save_btn, NULL);
    lv_label_set_text(sampling_save_label, "Save");
    lv_obj_add_style(sampling_save_btn, LV_BTN_PART_MAIN, &whiteButtonStyle);

    measure_period_label = my_lv_label_create(sampling_settings_scr, NULL, 5, 52, "Measurements \nsaving time \n[HH:MM:SS]", LV_COLOR_WHITE);
    lv_obj_set_style_local_text_font(measure_period_label, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_14);

    measure_period_hour = lv_spinbox_create(sampling_settings_scr, NULL);
    lv_textarea_set_cursor_hidden(measure_period_hour, true);
    lv_textarea_set_text_align(measure_period_hour, LV_LABEL_ALIGN_CENTER);
    lv_spinbox_set_range(measure_period_hour, 0, 24);
    lv_spinbox_set_digit_format(measure_period_hour, 2, 0);
    lv_obj_set_size(measure_period_hour, 40, 34);
    lv_obj_set_pos(measure_period_hour, 165, 61);

    measure_period_hour_increment  = my_lv_btn_create(sampling_settings_scr, NULL, 20, 20, 175, 39, sampling_hour_increment);
    lv_theme_apply(measure_period_hour_increment , LV_THEME_SPINBOX_BTN);
    lv_obj_add_style(measure_period_hour_increment , LV_OBJ_PART_MAIN, &borderlessStyle);
    lv_obj_set_style_local_value_str(measure_period_hour_increment, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_PLUS);

    measure_period_hour_decrement = my_lv_btn_create(sampling_settings_scr, measure_period_hour_increment , 20, 20, 175, 97, sampling_hour_decrement);
    lv_obj_set_style_local_value_str(measure_period_hour_decrement, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_MINUS);

    measure_colon_label = my_lv_label_create(sampling_settings_scr, NULL, 210, 70, ":", LV_COLOR_WHITE);

    measure_period_minute = lv_spinbox_create(sampling_settings_scr, NULL);
    lv_textarea_set_cursor_hidden(measure_period_minute, true);
    lv_textarea_set_text_align(measure_period_minute, LV_LABEL_ALIGN_CENTER);
    lv_spinbox_set_range(measure_period_minute, 0, 59);
    lv_spinbox_set_digit_format(measure_period_minute, 2, 0);
    lv_obj_set_size(measure_period_minute, 40, 34);
    lv_obj_set_pos(measure_period_minute, 219, 61);

    measure_period_minute_increment = my_lv_btn_create(sampling_settings_scr, measure_period_hour_increment , 20, 20, 229, 39, sampling_minute_increment);
    lv_obj_set_style_local_value_str(measure_period_minute_increment, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_PLUS);

    measure_period_minute_decrement = my_lv_btn_create(sampling_settings_scr, measure_period_hour_increment , 20, 20, 229, 97, sampling_minute_decrement);
    lv_obj_set_style_local_value_str(measure_period_minute_decrement, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_MINUS);

    measure_colon_label2 = my_lv_label_create(sampling_settings_scr, NULL, 265, 70, ":", LV_COLOR_WHITE);

    measure_period_second = lv_spinbox_create(sampling_settings_scr, NULL);
    lv_textarea_set_cursor_hidden(measure_period_second, true);
    lv_textarea_set_text_align(measure_period_second, LV_LABEL_ALIGN_CENTER);
    lv_spinbox_set_range(measure_period_second, 0, 59);
    lv_spinbox_set_digit_format(measure_period_second, 2, 0);
    lv_obj_set_size(measure_period_second, 40, 34);
    lv_obj_set_pos(measure_period_second, 274, 61);

    measure_period_second_increment = my_lv_btn_create(sampling_settings_scr, measure_period_hour_increment , 20, 20, 284, 39, sampling_second_increment);
    lv_obj_set_style_local_value_str(measure_period_second_increment, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_PLUS);

    measure_period_second_decrement = my_lv_btn_create(sampling_settings_scr, measure_period_hour_increment , 20, 20, 284, 97, sampling_second_decrement);
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
    lv_obj_set_size(measure_number, 50, 34);
    lv_obj_set_pos(measure_number, 206, 119);

    measure_number_increment = my_lv_btn_create(sampling_settings_scr, measure_period_hour_increment , 40, 34, 257, 119, measure_number_increment_func);
    lv_obj_set_style_local_value_str(measure_number_increment, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_PLUS);

    measure_number_decrement = my_lv_btn_create(sampling_settings_scr, measure_period_hour_increment , 40, 34, 165, 119, measure_number_decrement_func);
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
    lv_obj_set_size(measure_av_period, 50, 34);
    lv_obj_set_pos(measure_av_period, 206, 160);

    measure_av_period_increment = my_lv_btn_create(sampling_settings_scr, measure_period_hour_increment , 40, 34, 257, 160, av_period_increment);
    lv_obj_set_style_local_value_str(measure_av_period_increment, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_PLUS);

    measure_av_period_decrement = my_lv_btn_create(sampling_settings_scr, measure_period_hour_increment , 40, 34, 165, 160, av_period_decrement);
    lv_obj_set_style_local_value_str(measure_av_period_decrement, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_MINUS);

    turn_fan_on_time_label = my_lv_label_create(sampling_settings_scr, NULL, 5, 205, "Fan running time \nbefore measure [s]:");

    lv_obj_set_style_local_text_font(turn_fan_on_time_label, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_14);

    turn_fan_on_time = lv_spinbox_create(sampling_settings_scr, NULL);
    lv_textarea_set_cursor_hidden(turn_fan_on_time, true);
    lv_textarea_set_text_align(turn_fan_on_time, LV_LABEL_ALIGN_CENTER);
    lv_spinbox_set_digit_format(turn_fan_on_time, 3, 0);
    lv_spinbox_set_range(turn_fan_on_time, 1, 999);
    lv_obj_set_size(turn_fan_on_time, 50, 34);
    lv_obj_set_pos(turn_fan_on_time, 206, 200);

    turn_fan_on_time_increment = my_lv_btn_create(sampling_settings_scr, measure_period_hour_increment , 40, 34, 257, 200, turn_fan_on_time_increment_func);
    lv_obj_set_style_local_value_str(turn_fan_on_time_increment, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_PLUS);

    turn_fan_on_time_decrement = my_lv_btn_create(sampling_settings_scr, measure_period_hour_increment , 40, 34, 165, 200, turn_fan_on_time_decrement_func);
    lv_obj_set_style_local_value_str(turn_fan_on_time_decrement, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_MINUS);
}

//Zrobione
void wifi_screen()
{
    cancel_btn = my_lv_btn_create(wifi_scr, back_settings_btn, 30, 15, 14, 10, btn_cancel);
    cancel_label = lv_label_create(cancel_btn, NULL);
    lv_label_set_text(cancel_label, LV_SYMBOL_LEFT);
    lv_obj_add_style(cancel_btn, LV_OBJ_PART_MAIN, &transparentBackgroundStyle);
    lv_obj_add_style(cancel_btn, LV_OBJ_PART_MAIN, &borderlessStyle);
    lv_obj_add_style(cancel_btn, LV_OBJ_PART_MAIN, &whiteFontStyle);

    wifiLabelAtBar = my_lv_label_create(wifi_scr, NULL, 201, 10, "WiFi settings");
 
    ssid_label = my_lv_label_create(wifi_scr, NULL, 5, 53, "SSID: ");
   
    ssid_ta = lv_textarea_create(wifi_scr, NULL);
    lv_textarea_set_text(ssid_ta, "");
    lv_textarea_set_pwd_mode(ssid_ta, false);
    lv_textarea_set_one_line(ssid_ta, true);    
    lv_obj_set_event_cb(ssid_ta, ta_event_cb);
    lv_textarea_set_cursor_hidden(ssid_ta, true);
    lv_obj_set_size(ssid_ta, 140, 34);
    lv_obj_set_pos(ssid_ta, 100, 45);
    lv_obj_add_style(ssid_ta, LV_OBJ_PART_MAIN, &borderlessStyle);

    pwd_label = my_lv_label_create(wifi_scr, NULL, 5, 92, "Password: ");

    pwd_ta = lv_textarea_create(wifi_scr, NULL);
    lv_textarea_set_text(pwd_ta, "");
    lv_textarea_set_pwd_show_time(pwd_ta, 5000);
    lv_textarea_set_pwd_mode(pwd_ta, true);
    lv_textarea_set_one_line(pwd_ta, true);
    lv_obj_set_event_cb(pwd_ta, ta_event_cb);
    lv_textarea_set_cursor_hidden(pwd_ta, true);
    lv_obj_set_size(pwd_ta, 140, 34); 
    lv_obj_set_pos(pwd_ta, 100, 85);
    lv_obj_add_style(pwd_ta, LV_OBJ_PART_MAIN, &borderlessStyle);

    show_hide_btn = my_lv_btn_create(wifi_scr, NULL, 75, 43, 243, 85, show_hide_btn_func);
    show_hide_btn_label = lv_label_create(show_hide_btn, NULL);
    lv_label_set_text(show_hide_btn_label, LV_SYMBOL_EYE_OPEN);
    lv_obj_add_style(show_hide_btn, LV_OBJ_PART_MAIN, &transparentBackgroundStyle);
    lv_obj_add_style(show_hide_btn, LV_OBJ_PART_MAIN, &borderlessStyle);
    lv_obj_add_style(show_hide_btn_label, LV_OBJ_PART_MAIN, &whiteFontStyle);

    apply_btn = my_lv_btn_create(wifi_scr, NULL, 75, 43, 243, 43, btn_connect);
    apply_label = lv_label_create(apply_btn, NULL);
    lv_label_set_text(apply_label, "Connect");
    lv_obj_add_style(apply_btn, LV_OBJ_PART_MAIN, &whiteButtonStyle);
    lv_obj_add_style(apply_btn, LV_OBJ_PART_MAIN, &borderlessStyle);
}

void lock_screen()
{
    contDateTimeAtLock = lv_cont_create(lock_scr, NULL);
    lv_obj_set_auto_realign(contDateTimeAtLock, true);
    lv_obj_align(contDateTimeAtLock, NULL, LV_ALIGN_CENTER, 0, -40);
    lv_cont_set_fit(contDateTimeAtLock, LV_FIT_TIGHT);
    lv_cont_set_layout(contDateTimeAtLock, LV_LAYOUT_PRETTY_MID);
    lv_obj_add_style(contDateTimeAtLock, LV_OBJ_PART_MAIN, &transparentBackgroundStyle);
    lv_obj_add_style(contDateTimeAtLock, LV_OBJ_PART_MAIN, &borderlessStyle);
    lv_obj_add_style(contDateTimeAtLock, LV_OBJ_PART_MAIN, &whiteFontStyle);

    unlockButton = lv_btn_create(lock_scr, setButton);//TODO BRAK POS
    labelUnlockButton = lv_label_create(unlockButton, NULL);
    lv_obj_set_style_local_text_font(unlockButton, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, &monte16lock);
    lv_obj_align(unlockButton, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, -25);
    lv_label_set_text(labelUnlockButton, MY_UNLOCK_SYMBOL);
    lv_btn_set_fit(unlockButton, LV_FIT_TIGHT);
    lv_obj_set_event_cb(unlockButton, unlockButton_task);


    labelTimeLock = lv_label_create(contDateTimeAtLock, NULL);
    lv_label_set_text(labelTimeLock, "Connect to wifi");
    lv_label_set_align(labelTimeLock, LV_LABEL_ALIGN_CENTER);
    lv_obj_align(labelTimeLock, NULL, LV_ALIGN_CENTER, 0, 30);

    labelDateLock = lv_label_create(contDateTimeAtLock, NULL);
    lv_label_set_text(labelDateLock, "");
    lv_obj_align(labelDateLock, NULL, LV_ALIGN_CENTER, 0, 0);

    wifiStatusAtLock = lv_label_create(lock_scr, wifiStatusAtMain);
    lv_obj_align(wifiStatusAtLock, NULL, LV_ALIGN_CENTER, -36, 10);
    lv_label_set_text(wifiStatusAtLock, LV_SYMBOL_WIFI);
    wifiStatusAtLockWarning = my_lv_label_create(wifiStatusAtLock,  wifiStatusAtMainWarning, 6, 6, LV_SYMBOL_CLOSE, LV_COLOR_RED);

    sdStatusAtLock = lv_label_create(lock_scr, sdStatusAtMain);
    lv_obj_align(sdStatusAtLock, NULL, LV_ALIGN_CENTER, 36, 10);
    lv_label_set_text(sdStatusAtLock, LV_SYMBOL_SD_CARD);
    sdStatusAtLockWarning = my_lv_label_create(sdStatusAtLock, wifiStatusAtLockWarning, 2, 6, LV_SYMBOL_CLOSE, LV_COLOR_RED);

    ledAtLock = lv_led_create(lock_scr, NULL);
    lv_obj_set_size(ledAtLock, 13, 13);
    lv_obj_align(ledAtLock, NULL, LV_ALIGN_CENTER, 0, 10);
    lv_led_set_bright(ledAtLock, 200);
    lv_obj_set_style_local_bg_color(ledAtLock, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_YELLOW);
    lv_obj_set_style_local_shadow_color(ledAtLock, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_YELLOW);
    lv_obj_set_style_local_border_opa(ledAtLock, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_0);
}