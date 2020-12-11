#include <LVGLTasks.hpp>

void containerStyleInit(void)
{
    lv_style_init(&containerStyle);
    lv_style_set_text_font(&containerStyle, LV_STATE_DEFAULT, &lv_font_montserrat_16);
    lv_style_set_text_color(&containerStyle, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_bg_opa(&containerStyle, LV_STATE_DEFAULT, LV_OPA_0);
    lv_style_set_border_color(&containerStyle, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_radius(&containerStyle, LV_STATE_DEFAULT, 0);
}

//Style initialization functions
void font20StyleInit(void)
{
    lv_style_init(&font20Style);
    lv_style_set_text_font(&font20Style, LV_STATE_DEFAULT, &lv_font_montserrat_20);
    lv_style_set_text_color(&font20Style, LV_STATE_DEFAULT, LV_COLOR_WHITE);
}

void font12StyleInit(void)
{
    lv_style_init(&font12Style);
    lv_style_set_text_font(&font12Style, LV_STATE_DEFAULT, &lv_font_montserrat_12);
    lv_style_set_text_color(&font12Style, LV_STATE_DEFAULT, LV_COLOR_WHITE);
}
void font16StyleInit(void)
{
    lv_style_init(&font16Style);
    lv_style_set_text_font(&font16Style, LV_STATE_DEFAULT, &lv_font_montserrat_16);
    lv_style_set_text_color(&font16Style, LV_STATE_DEFAULT, LV_COLOR_WHITE);
}
void font22StyleInit(void)
{
    lv_style_init(&font22Style);
    lv_style_set_text_font(&font22Style, LV_STATE_DEFAULT, &lv_font_montserrat_22);
    lv_style_set_text_color(&font22Style, LV_STATE_DEFAULT, LV_COLOR_WHITE);
}

//Tiny symbols to signalize wifi and sd card status
void tinySymbolStyleInit(void)
{
    lv_style_init(&tinySymbolStyle);
    lv_style_set_text_font(&tinySymbolStyle, LV_STATE_DEFAULT, &lv_font_montserrat_12);
    lv_style_set_bg_opa(&tinySymbolStyle, LV_STATE_DEFAULT, LV_OPA_0);
    lv_style_set_border_opa(&tinySymbolStyle, LV_STATE_DEFAULT, LV_OPA_0);
    lv_style_set_radius(&tinySymbolStyle, LV_STATE_DEFAULT, 0);
    lv_style_set_text_color(&tinySymbolStyle, LV_STATE_DEFAULT, LV_COLOR_RED);
}

void transparentButtonStyleInit(void)
{
    lv_style_init(&transparentButtonStyle);
    lv_style_set_bg_opa(&transparentButtonStyle, LV_BTN_STATE_RELEASED, LV_OPA_0);
    lv_style_set_border_width(&transparentButtonStyle, LV_STATE_DEFAULT, 0);
    lv_style_set_outline_width(&transparentButtonStyle, LV_STATE_DEFAULT, 0);
    lv_style_set_radius(&transparentButtonStyle, LV_STATE_DEFAULT, 0);
    lv_style_set_text_color(&transparentButtonStyle, LV_STATE_DEFAULT, LV_COLOR_WHITE);
}

void whiteButtonStyleInit(void)
{
    lv_style_init(&whiteButtonStyle);
    lv_style_set_bg_color(&whiteButtonStyle, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_border_width(&whiteButtonStyle, LV_STATE_DEFAULT, 0);
    lv_style_set_outline_width(&whiteButtonStyle, LV_STATE_DEFAULT, 0);
    lv_style_set_radius(&whiteButtonStyle, LV_STATE_DEFAULT, 10);
    lv_style_set_text_color(&whiteButtonStyle, LV_STATE_DEFAULT, LV_COLOR_BLACK);
}

void hugeTransparentButtonStyleInit(void)
{
    lv_style_init(&hugeTransparentButtonStyle);
    lv_style_set_bg_opa(&hugeTransparentButtonStyle, LV_BTN_STATE_RELEASED, LV_OPA_0);
    lv_style_set_border_width(&hugeTransparentButtonStyle, LV_STATE_DEFAULT, 0);
    lv_style_set_outline_width(&hugeTransparentButtonStyle, LV_STATE_DEFAULT, 0);
    lv_style_set_radius(&hugeTransparentButtonStyle, LV_STATE_DEFAULT, 0);
    lv_style_set_text_color(&hugeTransparentButtonStyle, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_text_font(&hugeTransparentButtonStyle, LV_STATE_DEFAULT, &hugeSymbolsFont48);
}

void lineStyleInit(void)
{
    lv_style_init(&lineStyle);
    lv_style_set_line_width(&lineStyle, LV_STATE_DEFAULT, 2);
    lv_style_set_line_color(&lineStyle, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_line_rounded(&lineStyle, LV_STATE_DEFAULT, false);
}

void toastListStyleInit(void)
{
    lv_style_init(&toastListStyle);
    lv_style_set_bg_color(&toastListStyle, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x32, 0x32, 0x32));
    lv_style_set_text_color(&toastListStyle, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_radius(&toastListStyle, LV_STATE_DEFAULT, 0);
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

    back_time_settings_btn = lv_btn_create(contBarAtTimeSettings, NULL);
    back_time_settings_label = lv_label_create(back_time_settings_btn, NULL);
    lv_label_set_text(back_time_settings_label, LV_SYMBOL_LEFT);
    lv_obj_set_size(back_time_settings_btn, 30, 15);
    lv_obj_set_event_cb(back_time_settings_btn, timesettings_back_btn);
    lv_obj_add_style(back_time_settings_btn, LV_OBJ_PART_MAIN, &transparentButtonStyle);

    timeSettingsLabelAtBar = lv_label_create(contBarAtTimeSettings, NULL);
    lv_label_set_text(timeSettingsLabelAtBar, "Time Settings");

    time_label = lv_label_create(time_settings_scr, NULL);
    lv_obj_set_pos(time_label, 5, 70);
    lv_label_set_text(time_label, "Time (Hrs:Min)");
    lv_obj_set_style_local_text_color(time_label, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    time_hour = lv_spinbox_create(time_settings_scr, NULL);
    lv_textarea_set_cursor_hidden(time_hour, true);
    lv_textarea_set_text_align(time_hour, LV_LABEL_ALIGN_CENTER);
    lv_spinbox_set_range(time_hour, 0, 23);
    lv_spinbox_set_digit_format(time_hour, 2, 0);
    lv_obj_set_width(time_hour, 40);
    lv_obj_set_pos(time_hour, 165, 61);

    time_hour_increment = lv_btn_create(time_settings_scr, NULL);
    lv_obj_set_size(time_hour_increment, 20, 20);
    lv_obj_set_pos(time_hour_increment, 175, 39);
    lv_theme_apply(time_hour_increment, LV_THEME_SPINBOX_BTN);
    lv_obj_set_style_local_value_str(time_hour_increment, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_PLUS);
    lv_obj_set_event_cb(time_hour_increment, hour_increment);

    time_hour_decrement = lv_btn_create(time_settings_scr, NULL);
    lv_obj_set_size(time_hour_decrement, 20, 20);
    lv_obj_set_pos(time_hour_decrement, 175, 97);
    lv_theme_apply(time_hour_decrement, LV_THEME_SPINBOX_BTN);
    lv_obj_set_style_local_value_str(time_hour_decrement, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_MINUS);
    lv_obj_set_event_cb(time_hour_decrement, hour_decrement);

    time_colon_label = lv_label_create(time_settings_scr, NULL);
    lv_obj_set_pos(time_colon_label, 210, 70);
    lv_label_set_text(time_colon_label, ":");
    lv_obj_set_style_local_text_color(time_colon_label, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    time_minute = lv_spinbox_create(time_settings_scr, NULL);
    lv_textarea_set_cursor_hidden(time_minute, true);
    lv_textarea_set_text_align(time_minute, LV_LABEL_ALIGN_CENTER);
    lv_spinbox_set_range(time_minute, 0, 59);
    lv_spinbox_set_digit_format(time_minute, 2, 0);
    lv_obj_set_width(time_minute, 40);
    lv_obj_set_pos(time_minute, 219, 61);

    time_minute_increment = lv_btn_create(time_settings_scr, NULL);
    lv_obj_set_size(time_minute_increment, 20, 20);
    lv_obj_set_pos(time_minute_increment, 229, 39);
    lv_theme_apply(time_minute_increment, LV_THEME_SPINBOX_BTN);
    lv_obj_set_style_local_value_str(time_minute_increment, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_PLUS);
    lv_obj_set_event_cb(time_minute_increment, minute_increment);

    time_minute_decrement = lv_btn_create(time_settings_scr, NULL);
    lv_obj_set_size(time_minute_decrement, 20, 20);
    lv_obj_set_pos(time_minute_decrement, 229, 97);
    lv_theme_apply(time_minute_decrement, LV_THEME_SPINBOX_BTN);
    lv_obj_set_style_local_value_str(time_minute_decrement, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_MINUS);
    lv_obj_set_event_cb(time_minute_decrement, minute_decrement);

    date_label = lv_label_create(time_settings_scr, NULL);
    lv_obj_set_pos(date_label, 5, 129);
    lv_label_set_text(date_label, "Date: ");
    lv_obj_set_style_local_text_color(date_label, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    date_btn = lv_btn_create(time_settings_scr, NULL);
    date_btn_label = lv_label_create(date_btn, NULL);
    lv_label_set_text(date_btn_label, "99.99.9999");
    lv_obj_set_style_local_border_opa(date_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);
    lv_obj_set_style_local_text_color(date_btn_label, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_obj_set_width(date_btn, 95);
    lv_obj_set_pos(date_btn, 165, 119);
    lv_obj_set_event_cb(date_btn, date_button_func);

    lockScreenLabel = lv_label_create(time_settings_scr, NULL);
    lv_obj_set_pos(lockScreenLabel, 5, 170);
    lv_label_set_text(lockScreenLabel, "Lock screen after ");
    lv_obj_set_style_local_text_color(lockScreenLabel, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lockScreenDDlist = lv_dropdown_create(time_settings_scr, NULL);
    lv_dropdown_set_options(lockScreenDDlist, "30 sec\n"
                                              "1 min\n"
                                              "2 min\n"
                                              "5 min\n"
                                              "10 min\n"
                                              "30 min\n"
                                              "60 min\n"
                                              "Never");
    lv_obj_set_width(lockScreenDDlist, 120);
    lv_obj_set_pos(lockScreenDDlist, 165, 164);

    timeSettings_btn = lv_btn_create(time_settings_scr, NULL);
    timeSettings_label = lv_label_create(timeSettings_btn, NULL);
    lv_label_set_text(timeSettings_label, "Save");
    lv_obj_set_size(timeSettings_btn, 75, 33);
    lv_obj_set_pos(timeSettings_btn, 240, 200);
    lv_obj_set_event_cb(timeSettings_btn, timesettings_save_btn);
    lv_obj_add_style(timeSettings_btn, LV_BTN_PART_MAIN, &whiteButtonStyle);

    sync_rtc_btn = lv_btn_create(time_settings_scr, NULL);
    sync_rtc_label = lv_label_create(sync_rtc_btn, NULL);
    lv_obj_set_height(sync_rtc_btn, 33);
    lv_label_set_text(sync_rtc_label, "Sync. Clock");
    lv_obj_add_style(sync_rtc_btn, LV_BTN_PART_MAIN, &whiteButtonStyle);
    lv_obj_set_pos(sync_rtc_btn, 5, 200);
    lv_obj_set_event_cb(sync_rtc_btn, sync_rtc_func);
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

    back_settings_btn = lv_btn_create(contBarAtMainSettings, NULL);
    back_settings_label = lv_label_create(back_settings_btn, NULL);
    lv_label_set_text(back_settings_label, LV_SYMBOL_LEFT);
    lv_obj_set_size(back_settings_btn, 30, 15);
    lv_obj_set_event_cb(back_settings_btn, btn_settings_back);
    lv_obj_add_style(back_settings_btn, LV_OBJ_PART_MAIN, &transparentButtonStyle);

    settingsLabelAtBar = lv_label_create(contBarAtMainSettings, NULL);
    lv_label_set_text(settingsLabelAtBar, "Settings");

    wifiBtn = lv_btn_create(settings_scr, NULL);
    wifiBtnLabel = lv_label_create(wifiBtn, NULL);
    lv_obj_set_size(wifiBtn, 60, 60);
    lv_obj_set_pos(wifiBtn, 60, 38);
    lv_label_set_text(wifiBtnLabel, MY_WIFI_SYMBOL);
    lv_obj_set_event_cb(wifiBtn, WiFi_btn);
    lv_obj_add_style(wifiBtn, LV_OBJ_PART_MAIN, &hugeTransparentButtonStyle);

    wifiBtnName = lv_label_create(settings_scr, NULL);
    lv_label_set_text(wifiBtnName, "WiFi");
    lv_obj_set_pos(wifiBtnName, 63, 103);
    lv_obj_add_style(wifiBtnName, LV_OBJ_PART_MAIN, &font20Style);

    infoBtn = lv_btn_create(settings_scr, NULL);
    infoBtnLabel = lv_label_create(infoBtn, NULL);
    lv_obj_set_size(infoBtn, 60, 60);
    lv_obj_set_pos(infoBtn, 200, 38);
    lv_label_set_text(infoBtnLabel, MY_INFO_SYMBOL);
    lv_obj_set_event_cb(infoBtn, info_btn);
    lv_obj_add_style(infoBtn, LV_OBJ_PART_MAIN, &hugeTransparentButtonStyle);
    
    infoBtnName = lv_label_create(settings_scr, NULL);
    lv_label_set_text(infoBtnName, "Info");
    lv_obj_set_pos(infoBtnName, 207, 103);
    lv_obj_add_style(infoBtnName, LV_OBJ_PART_MAIN, &font20Style);

    timeBtn = lv_btn_create(settings_scr, NULL);
    timeBtnLabel = lv_label_create(timeBtn, NULL);
    lv_obj_set_size(timeBtn, 60, 60);
    lv_obj_set_pos(timeBtn, 60, 140);
    lv_label_set_text(timeBtnLabel, MY_CLOCK_SYMBOL);
    lv_obj_set_event_cb(timeBtn, time_settings_btn);
    lv_obj_add_style(timeBtn, LV_OBJ_PART_MAIN, &hugeTransparentButtonStyle);

    timeBtnName = lv_label_create(settings_scr, NULL);
    lv_label_set_text(timeBtnName, "Time");
    lv_obj_set_pos(timeBtnName, 65, 200);
    lv_obj_add_style(timeBtnName, LV_OBJ_PART_MAIN, &font20Style);

    tempBtn = lv_btn_create(settings_scr, NULL);
    tempBtnLabel = lv_label_create(tempBtn, NULL);
    lv_obj_set_size(tempBtn, 60, 60);
    lv_obj_set_pos(tempBtn, 200, 140);
    lv_label_set_text(tempBtnLabel, MY_COGS_SYMBOL);
    lv_obj_set_event_cb(tempBtn, temp_settings_btn);
    lv_obj_add_style(tempBtn, LV_OBJ_PART_MAIN, &hugeTransparentButtonStyle);
    
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

    back_info_btn = lv_btn_create(contBarAtMaininfo, NULL);
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
 
	lockButton = lv_btn_create(main_scr, NULL);
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

    setButton = lv_btn_create(main_scr, NULL);
    labelSetButton = lv_label_create(setButton, NULL);
    lv_label_set_text(labelSetButton, LV_SYMBOL_SETTINGS);
    lv_obj_align(setButton, NULL, LV_ALIGN_IN_TOP_LEFT, -25, -5);
    lv_btn_set_fit(setButton, LV_FIT_TIGHT);
    lv_obj_set_event_cb(setButton, setButton_task);
    lv_obj_add_style(setButton, LV_OBJ_PART_MAIN, &transparentButtonStyle);


    wifiStatusAtMainWarning = lv_label_create(wifiStatusAtMain, NULL);
    lv_label_set_text(wifiStatusAtMainWarning, LV_SYMBOL_CLOSE);
    lv_obj_add_style(wifiStatusAtMainWarning, LV_OBJ_PART_MAIN, &tinySymbolStyle);
    lv_obj_set_pos(wifiStatusAtMainWarning, 5, 5);

    sdStatusAtMainWarning = lv_label_create(sdStatusAtMain, NULL);
    lv_label_set_text(sdStatusAtMainWarning, LV_SYMBOL_CLOSE);
    lv_obj_add_style(sdStatusAtMainWarning, LV_OBJ_PART_MAIN, &tinySymbolStyle);
    lv_obj_set_pos(sdStatusAtMainWarning, 2, 5);

    dateAndTimeAtBar = lv_label_create(contBarAtMain, NULL);
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
    lv_obj_set_pos(contAQI, 188, 118);
    lv_obj_set_size(contAQI, 122, 62);

    contAQIColorBar = lv_cont_create(contAQI, NULL);
    lv_obj_set_click(contAQIColorBar, false);
    lv_obj_add_style(contAQIColorBar, LV_OBJ_PART_MAIN, &containerStyle);
    lv_obj_add_style(contAQIColorBar, LV_OBJ_PART_MAIN, &font12Style);
    lv_obj_set_style_local_bg_opa(contAQIColorBar, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_70);
    lv_obj_set_style_local_bg_color(contAQIColorBar, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_GREEN);
    lv_obj_set_pos(contAQIColorBar, 15, 25);
    lv_obj_set_size(contAQIColorBar, 92, 24);

    labelTemp = lv_label_create(contTemp, NULL);
    lv_obj_set_pos(labelTemp, 5, 3);
    lv_label_set_text(labelTemp, "Temp");

    labelTempValue = lv_label_create(contTemp, NULL);
    lv_obj_add_style(labelTempValue, LV_OBJ_PART_MAIN, &font20Style);
    lv_obj_set_pos(labelTempValue, 16, 22);
    lv_label_set_text(labelTempValue, "         -");
    lv_label_set_align(labelTempValue, LV_LABEL_ALIGN_LEFT);
    labelHumi = lv_label_create(contHumi, NULL);
    lv_obj_set_pos(labelHumi, 5, 3);
    lv_label_set_text(labelHumi, "RH");

    labelHumiValue = lv_label_create(contHumi, NULL);
    lv_obj_add_style(labelHumiValue, LV_OBJ_PART_MAIN, &font20Style);
    lv_obj_set_pos(labelHumiValue, 16, 22);
    lv_label_set_text(labelHumiValue, "         -");

    labelPM10 = lv_label_create(contPM10, NULL);
    lv_obj_set_pos(labelPM10, 5, 5);
    lv_label_set_text(labelPM10, "PM 1.0 ug/m");

    labelPM10UpperIndex = lv_label_create(contPM10, NULL);
    lv_obj_set_pos(labelPM10UpperIndex, 80, 3);
    lv_label_set_text(labelPM10UpperIndex, "3");

    labelPM25 = lv_label_create(contPM25, NULL);
    lv_obj_set_pos(labelPM25, 5, 5);
    lv_label_set_text(labelPM25, "PM 2.5                ug/m");

    labelPM25UpperIndex = lv_label_create(contPM25, NULL);
    //	lv_obj_align(labelPM25, labelPM25UpperIndex, LV_ALIGN_OUT_RIGHT_TOP, 0, 0);
    lv_obj_set_pos(labelPM25UpperIndex, 167, 3);
    lv_label_set_text(labelPM25UpperIndex, "3");
    lv_obj_add_style(labelPM25UpperIndex, LV_OBJ_PART_MAIN, &font12Style);

    labelPM100 = lv_label_create(contPM100, NULL);
    lv_obj_set_pos(labelPM100, 5, 5);
    lv_label_set_text(labelPM100, "PM 10 ug/m");

    labelPM100UpperIndex = lv_label_create(contPM100, NULL);
    lv_obj_set_pos(labelPM100UpperIndex, 77, 3);
    lv_label_set_text(labelPM100UpperIndex, "3");
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

    cancel_btn = lv_btn_create(contBarAtMainWiFi, NULL);
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
    lv_obj_set_width(ssid_ta, LV_HOR_RES / 2 - 20);
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
    lv_obj_set_width(pwd_ta, LV_HOR_RES / 2 - 20);
    lv_obj_set_pos(pwd_ta, 100, 85);

    show_hide_btn = lv_btn_create(wifi_scr, NULL);
    show_hide_btn_label = lv_label_create(show_hide_btn, NULL);
    lv_label_set_text(show_hide_btn_label, LV_SYMBOL_EYE_OPEN);
    lv_obj_set_pos(show_hide_btn, 243, 85);
    lv_obj_set_width(show_hide_btn, 75);
    lv_obj_add_style(show_hide_btn, LV_OBJ_PART_MAIN, &transparentButtonStyle);
    lv_obj_set_style_local_text_color(show_hide_btn_label, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_event_cb(show_hide_btn, show_hide_btn_func);
    apply_btn = lv_btn_create(wifi_scr, NULL);
    apply_label = lv_label_create(apply_btn, NULL);
    lv_label_set_text(apply_label, "Connect");
    lv_obj_add_style(apply_btn, LV_OBJ_PART_MAIN, &whiteButtonStyle);
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

    unlockButton = lv_btn_create(lock_scr, NULL);
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

    wifiStatusAtLockWarning = lv_label_create(wifiStatusAtLock, NULL);
    lv_label_set_text(wifiStatusAtLockWarning, LV_SYMBOL_CLOSE);
    lv_obj_add_style(wifiStatusAtLockWarning, LV_OBJ_PART_MAIN, &tinySymbolStyle);
    lv_obj_set_pos(wifiStatusAtLockWarning, 5, 5);

    sdStatusAtLockWarning = lv_label_create(sdStatusAtLock, NULL);
    lv_label_set_text(sdStatusAtLockWarning, LV_SYMBOL_CLOSE);
    lv_obj_add_style(sdStatusAtLockWarning, LV_OBJ_PART_MAIN, &tinySymbolStyle);
    lv_obj_set_pos(sdStatusAtLockWarning, 2, 5);

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

    back_sampling_settings_btn = lv_btn_create(contBarAtSampling, NULL);
    back_sampling_settings_label = lv_label_create(back_sampling_settings_btn, NULL);
    lv_label_set_text(back_sampling_settings_label, LV_SYMBOL_LEFT);
    lv_obj_set_size(back_sampling_settings_btn, 30, 15);
    lv_obj_set_event_cb(back_sampling_settings_btn, sampling_settings_back_btn);
    lv_obj_add_style(back_sampling_settings_btn, LV_OBJ_PART_MAIN, &transparentButtonStyle);

    SamplingSettingsLabelAtBar = lv_label_create(contBarAtSampling, NULL);
    lv_label_set_text(SamplingSettingsLabelAtBar, "Sampling settings");

    sampling_save_btn = lv_btn_create(contBarAtSampling, NULL);
    sampling_save_label = lv_label_create(sampling_save_btn, NULL);
    lv_label_set_text(sampling_save_label, "Save");
    lv_obj_set_size(sampling_save_btn, 75, 20);
    //lv_obj_set_pos(sampling_save_btn, 240, 200);
    lv_obj_set_event_cb(sampling_save_btn, sampling_settings_save_btn);
    lv_obj_add_style(sampling_save_btn, LV_BTN_PART_MAIN, &whiteButtonStyle);

    measure_period_label = lv_label_create(sampling_settings_scr, NULL);
    lv_obj_set_pos(measure_period_label, 5, 61);
    lv_label_set_text(measure_period_label, "Time between \nsaving sample");
    lv_obj_set_style_local_text_color(measure_period_label, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    measure_period_hour = lv_spinbox_create(sampling_settings_scr, NULL);
    lv_textarea_set_cursor_hidden(measure_period_hour, true);
    lv_textarea_set_text_align(measure_period_hour, LV_LABEL_ALIGN_CENTER);
    lv_spinbox_set_range(measure_period_hour, 0, 24);
    lv_spinbox_set_digit_format(measure_period_hour, 2, 0);
    lv_obj_set_width(measure_period_hour, 40);
    lv_obj_set_pos(measure_period_hour, 165, 61);

    measure_period_hour_increment = lv_btn_create(sampling_settings_scr, NULL);
    lv_obj_set_size(measure_period_hour_increment, 20, 20);
    lv_obj_set_pos(measure_period_hour_increment, 175, 39);
    lv_theme_apply(measure_period_hour_increment, LV_THEME_SPINBOX_BTN);
    lv_obj_set_style_local_value_str(measure_period_hour_increment, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_PLUS);
    lv_obj_set_event_cb(measure_period_hour_increment, sampling_hour_increment);

    measure_period_hour_decrement = lv_btn_create(sampling_settings_scr, NULL);
    lv_obj_set_size(measure_period_hour_decrement, 20, 20);
    lv_obj_set_pos(measure_period_hour_decrement, 175, 97);
    lv_theme_apply(measure_period_hour_decrement, LV_THEME_SPINBOX_BTN);
    lv_obj_set_style_local_value_str(measure_period_hour_decrement, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_MINUS);
    lv_obj_set_event_cb(measure_period_hour_decrement, sampling_hour_decrement);

    measure_colon_label = lv_label_create(sampling_settings_scr, NULL);
    lv_obj_set_pos(measure_colon_label, 210, 70);
    lv_label_set_text(measure_colon_label, ":");
    lv_obj_set_style_local_text_color(measure_colon_label, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    measure_period_minute = lv_spinbox_create(sampling_settings_scr, NULL);
    lv_textarea_set_cursor_hidden(measure_period_minute, true);
    lv_textarea_set_text_align(measure_period_minute, LV_LABEL_ALIGN_CENTER);
    lv_spinbox_set_range(measure_period_minute, 0, 59);
    lv_spinbox_set_digit_format(measure_period_minute, 2, 0);
    lv_obj_set_width(measure_period_minute, 40);
    lv_obj_set_pos(measure_period_minute, 219, 61);

    measure_period_minute_increment = lv_btn_create(sampling_settings_scr, NULL);
    lv_obj_set_size(measure_period_minute_increment, 20, 20);
    lv_obj_set_pos(measure_period_minute_increment, 229, 39);
    lv_theme_apply(measure_period_minute_increment, LV_THEME_SPINBOX_BTN);
    lv_obj_set_style_local_value_str(measure_period_minute_increment, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_PLUS);
    lv_obj_set_event_cb(measure_period_minute_increment, sampling_minute_increment);

    measure_period_minute_decrement = lv_btn_create(sampling_settings_scr, NULL);
    lv_obj_set_size(measure_period_minute_decrement, 20, 20);
    lv_obj_set_pos(measure_period_minute_decrement, 229, 97);
    lv_theme_apply(measure_period_minute_decrement, LV_THEME_SPINBOX_BTN);
    lv_obj_set_style_local_value_str(measure_period_minute_decrement, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_MINUS);
    lv_obj_set_event_cb(measure_period_minute_decrement, sampling_minute_decrement);

    measure_number_label = lv_label_create(sampling_settings_scr, NULL);
    lv_obj_set_pos(measure_number_label, 5, 129);
    lv_label_set_text(measure_number_label, "Averaged samples");
    lv_obj_set_style_local_text_color(measure_number_label, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    measure_number = lv_spinbox_create(sampling_settings_scr, NULL);
    lv_textarea_set_cursor_hidden(measure_number, true);
    lv_textarea_set_text_align(measure_number, LV_LABEL_ALIGN_CENTER);
    lv_spinbox_set_digit_format(measure_number, 2, 0);
    lv_spinbox_set_range(measure_number, 1, 20);
    lv_obj_set_width(measure_number, 40);
    lv_obj_set_pos(measure_number, 206, 119);

    measure_number_increment = lv_btn_create(sampling_settings_scr, NULL);
    lv_obj_set_size(measure_number_increment, 40, 34);
    lv_obj_set_pos(measure_number_increment, 247, 119);
    lv_theme_apply(measure_number_increment, LV_THEME_SPINBOX_BTN);
    lv_obj_set_style_local_value_str(measure_number_increment, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_PLUS);
    lv_obj_set_event_cb(measure_number_increment, measure_number_increment_func);

    measure_number_decrement = lv_btn_create(sampling_settings_scr, NULL);
    lv_obj_set_size(measure_number_decrement, 40, 34);
    lv_obj_set_pos(measure_number_decrement, 165, 119);
    lv_theme_apply(measure_number_decrement, LV_THEME_SPINBOX_BTN);
    lv_obj_set_style_local_value_str(measure_number_decrement, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_MINUS);
    lv_obj_set_event_cb(measure_number_decrement, measure_number_decrement_func);

    measure_av_period_label = lv_label_create(sampling_settings_scr, NULL);
    lv_obj_set_pos(measure_av_period_label, 5, 170);
    lv_label_set_text(measure_av_period_label, "Measure period:");
    lv_obj_set_style_local_text_color(measure_av_period_label, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    measure_av_period = lv_spinbox_create(sampling_settings_scr, NULL);
    lv_textarea_set_cursor_hidden(measure_av_period, true);
    lv_textarea_set_text_align(measure_av_period, LV_LABEL_ALIGN_CENTER);
    lv_spinbox_set_digit_format(measure_av_period, 2, 0);
    lv_spinbox_set_range(measure_av_period, 5, 30);
    lv_obj_set_width(measure_av_period, 40);
    lv_obj_set_pos(measure_av_period, 206, 160);

    measure_av_period_increment = lv_btn_create(sampling_settings_scr, NULL);
    lv_obj_set_size(measure_av_period_increment, 40, 34);
    lv_obj_set_pos(measure_av_period_increment, 247, 160);
    lv_theme_apply(measure_av_period_increment, LV_THEME_SPINBOX_BTN);
    lv_obj_set_style_local_value_str(measure_av_period_increment, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_PLUS);
    lv_obj_set_event_cb(measure_av_period_increment, av_period_increment);

    measure_av_period_decrement = lv_btn_create(sampling_settings_scr, NULL);
    lv_obj_set_size(measure_av_period_decrement, 40, 34);
    lv_obj_set_pos(measure_av_period_decrement, 165, 160);
    lv_theme_apply(measure_av_period_decrement, LV_THEME_SPINBOX_BTN);
    lv_obj_set_style_local_value_str(measure_av_period_decrement, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_MINUS);
    lv_obj_set_event_cb(measure_av_period_decrement, av_period_decrement);

    turn_fan_on_time_label = lv_label_create(sampling_settings_scr, NULL);
    lv_obj_set_pos(turn_fan_on_time_label, 5, 210);
    lv_label_set_text(turn_fan_on_time_label, "Turn fan on time [s]:");
    lv_obj_set_style_local_text_color(turn_fan_on_time_label, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    turn_fan_on_time = lv_spinbox_create(sampling_settings_scr, NULL);
    lv_textarea_set_cursor_hidden(turn_fan_on_time, true);
    lv_textarea_set_text_align(turn_fan_on_time, LV_LABEL_ALIGN_CENTER);
    lv_spinbox_set_digit_format(turn_fan_on_time, 2, 0);
    lv_spinbox_set_range(turn_fan_on_time, 1, 60);
    lv_obj_set_width(turn_fan_on_time, 40);
    lv_obj_set_pos(turn_fan_on_time, 206, 200);

    turn_fan_on_time_increment = lv_btn_create(sampling_settings_scr, NULL);
    lv_obj_set_size(turn_fan_on_time_increment, 40, 34);
    lv_obj_set_pos(turn_fan_on_time_increment, 247, 200);
    lv_theme_apply(turn_fan_on_time_increment, LV_THEME_SPINBOX_BTN);
    lv_obj_set_style_local_value_str(turn_fan_on_time_increment, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_PLUS);
    lv_obj_set_event_cb(turn_fan_on_time_increment, turn_fan_on_time_increment_func);

    turn_fan_on_time_decrement = lv_btn_create(sampling_settings_scr, NULL);
    lv_obj_set_size(turn_fan_on_time_decrement, 40, 34);
    lv_obj_set_pos(turn_fan_on_time_decrement, 165, 200);
    lv_theme_apply(turn_fan_on_time_decrement, LV_THEME_SPINBOX_BTN);
    lv_obj_set_style_local_value_str(turn_fan_on_time_decrement, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_MINUS);
    lv_obj_set_event_cb(turn_fan_on_time_decrement, turn_fan_on_time_decrement_func);    
}