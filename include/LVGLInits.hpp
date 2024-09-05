#include <LVGLTasks.hpp>
#include "ui/widgets/Label.h"
#include "ui/Styles.h"
#include "ui/Symbols.h"

void stylesInits()
{
    lv_style_init(&font16Style);
    lv_style_set_text_font(&font16Style, LV_STATE_DEFAULT, &lv_font_montserrat_16);

    lv_style_init(&hugeFontStyle);
    lv_style_set_text_font(&hugeFontStyle, LV_STATE_DEFAULT, &hugeSymbolsFont48);

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
    backTimeSettingsBtn = Button(timeSettingsScr, backSettingsBtn, 30, 15, 14, 10, timesettings_back_btn);
    backTimeSettingsLabel = lv_label_create(backTimeSettingsBtn, NULL);
    lv_label_set_text(backTimeSettingsLabel, LV_SYMBOL_LEFT);

    timeSettingsLabelAtBar = Label(timeSettingsScr, NULL, 195, 10, "Time Settings");

    timeLabel = Label(timeSettingsScr, timeSettingsLabelAtBar, 5, 70, "Time [HH:MM]");

    timeHour = lv_spinbox_create(timeSettingsScr, NULL);
    lv_textarea_set_cursor_hidden(timeHour, true);
    lv_textarea_set_text_align(timeHour, LV_LABEL_ALIGN_CENTER);
    lv_spinbox_set_range(timeHour, 0, 23);
    lv_spinbox_set_digit_format(timeHour, 2, 0);
    lv_obj_set_size(timeHour, 40, 34);
    lv_obj_set_pos(timeHour, 165, 61);

    timeHourIncrement = Button(timeSettingsScr, NULL, 20, 20, 175, 39, hourIncrement);
    lv_theme_apply(timeHourIncrement, LV_THEME_SPINBOX_BTN);
    lv_obj_add_style(timeHourIncrement, LV_OBJ_PART_MAIN, &Styles::borderlessStyle);
    lv_obj_set_style_local_value_str(timeHourIncrement, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_PLUS);

    timeHourDecrement = Button(timeSettingsScr, timeHourIncrement, 20, 20, 175, 97, hourDecrement);
    lv_obj_set_style_local_value_str(timeHourDecrement, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_MINUS);

    timeColonLabel = Label(timeSettingsScr, timeLabel, 210, 70, ":");
    lv_obj_set_style_local_text_color(timeColonLabel, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    timeMinute = lv_spinbox_create(timeSettingsScr, NULL);
    lv_textarea_set_cursor_hidden(timeMinute, true);
    lv_textarea_set_text_align(timeMinute, LV_LABEL_ALIGN_CENTER);
    lv_spinbox_set_range(timeMinute, 0, 59);
    lv_spinbox_set_digit_format(timeMinute, 2, 0);
    lv_obj_set_size(timeMinute, 40, 34);
    lv_obj_set_pos(timeMinute, 219, 61);

    timeMinuteIncrement = Button(timeSettingsScr, timeHourIncrement, 20, 20, 229, 39, minuteIncrement);
    lv_obj_set_style_local_value_str(timeMinuteIncrement, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_PLUS);

    timeMinuteDecrement = Button(timeSettingsScr, timeHourIncrement, 20, 20, 229, 97, minuteDecrement);
    lv_obj_set_style_local_value_str(timeMinuteDecrement, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_MINUS);

    dateLabel = Label(timeSettingsScr, NULL, 5, 129, "Date ");

    dateBtn = Button(timeSettingsScr, NULL, 95, 43, 165, 119, date_button_func);
    dateBtnLabel = lv_label_create(dateBtn, NULL);
    lv_label_set_text(dateBtnLabel, "99.99.9999");
    lv_obj_set_style_local_border_opa(dateBtn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);
    lv_obj_set_style_local_text_color(dateBtnLabel, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);

    lockScreenLabel = Label(timeSettingsScr, NULL, 5, 170, "Lock screen after ");
    lockScreenDDlist = lv_dropdown_create(timeSettingsScr, NULL);
    lv_dropdown_set_options(lockScreenDDlist, "1 min\n"
                                              "5 min\n"
                                              "10 min\n"
                                              "60 min\n"
                                              "Never");
    lv_obj_set_size(lockScreenDDlist, 120, 34);
    lv_obj_set_pos(lockScreenDDlist, 165, 164);

    timeSettingsBtn = Button(timeSettingsScr, NULL, 75, 33, 240, 200, timesettings_save_btn);
    timeSettingsLabel = lv_label_create(timeSettingsBtn, NULL);
    lv_label_set_text(timeSettingsLabel, "Save");
    lv_obj_add_style(timeSettingsBtn, LV_BTN_PART_MAIN, &whiteButtonStyle);

    syncRtcBtn = Button(timeSettingsScr, NULL, 130, 33, 5, 200, sync_rtc_func);
    syncRtcLabel = lv_label_create(syncRtcBtn, NULL);
    lv_label_set_text(syncRtcLabel, "Sync. Clock");
    lv_obj_add_style(syncRtcBtn, LV_BTN_PART_MAIN, &whiteButtonStyle);
}

void settingsScreen()
{
    backSettingsBtn = Button(settingsScr, NULL, 30, 15, 14, 10, btn_settings_back);
    backSettingsLabel = lv_label_create(backSettingsBtn, NULL);
    lv_label_set_text(backSettingsLabel, LV_SYMBOL_LEFT);
    lv_obj_add_style(backSettingsBtn, LV_OBJ_PART_MAIN, &Styles::transparentBackgroundStyle);
    lv_obj_add_style(backSettingsBtn, LV_OBJ_PART_MAIN, &Styles::borderlessStyle);
    lv_obj_add_style(backSettingsBtn, LV_OBJ_PART_MAIN, &Styles::whiteFontStyle);

    settingsLabelAtBar = Label(settingsScr, NULL, 239, 10, "Settings");

    wifiBtn = Button(settingsScr, NULL, 60, 60, 60, 38, WiFi_btn);
    wifiBtnLabel = lv_label_create(wifiBtn, NULL);
    lv_label_set_text(wifiBtnLabel, Symbols::WIFI_SYMBOL);
    lv_obj_add_style(wifiBtn, LV_OBJ_PART_MAIN, &Styles::transparentBackgroundStyle);
    lv_obj_add_style(wifiBtn, LV_OBJ_PART_MAIN, &Styles::borderlessStyle);
    lv_obj_add_style(wifiBtn, LV_OBJ_PART_MAIN, &Styles::whiteFontStyle);
    lv_obj_add_style(wifiBtn, LV_OBJ_PART_MAIN, &hugeFontStyle);

    wifiBtnName = Label(settingsScr, NULL, 63, 103, "WiFi");
    lv_obj_add_style(wifiBtnName, LV_OBJ_PART_MAIN, &Styles::font20Style);

    infoBtn = Button(settingsScr, wifiBtn, 60, 60, 200, 38, info_btn);
    infoBtnLabel = lv_label_create(infoBtn, NULL);
    lv_label_set_text(infoBtnLabel, Symbols::INFO_SYMBOL);

    infoBtnName = Label(settingsScr, wifiBtnName, 207, 103, "Info");

    timeBtn = Button(settingsScr, wifiBtn, 60, 60, 60, 140, time_settings_btn);
    timeBtnLabel = lv_label_create(timeBtn, NULL);
    lv_label_set_text(timeBtnLabel, Symbols::CLOCK_SYMBOL);

    timeBtnName = Label(settingsScr, wifiBtnName, 65, 200, "Time");

    tempBtn = Button(settingsScr, wifiBtn, 60, 60, 200, 140, temp_settings_btn);
    tempBtnLabel = lv_label_create(tempBtn, wifiBtnName);
    lv_label_set_text(tempBtnLabel, Symbols::COGS_SYMBOL);
    lv_obj_add_style(tempBtnLabel, LV_OBJ_PART_MAIN, &hugeFontStyle);
    tempBtnName = Label(settingsScr, wifiBtnName, 180, 200, "Sampling");
}

void infoScreen()
{
    backInfoBtn = Button(infoScr, backSettingsBtn, 30, 15, 14, 10, setButton_task);
    backInfoLabel = lv_label_create(backInfoBtn, NULL);
    lv_label_set_text(backInfoLabel, LV_SYMBOL_LEFT);

    lcdLabelAtBar = Label(infoScr, NULL, 216, 10, "Device info");

    infoWifiLabel = Label(infoScr, NULL, 5, 53, "WiFi address: ");
    lv_obj_set_style_local_text_font(infoWifiLabel, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_14);

    infoWifiAddressLabel = Label(infoScr, infoWifiLabel, 115, 53, "");

    configLabel = Label(infoScr, NULL, 5, 70, "");
}

void samplingsettingsScreen()
{
    backSamplingSettingsBtn = Button(samplingSettingsScr, backSettingsBtn, 30, 15, 14, 10, sampling_settings_back_btn);
    backSamplingSettingsLabel = lv_label_create(backSamplingSettingsBtn, NULL);
    lv_label_set_text(backSamplingSettingsLabel, LV_SYMBOL_LEFT);

    samplingSaveBtn = Button(samplingSettingsScr, NULL, 75, 25, 231, 10, sampling_settings_save_btn);
    samplingSaveLabel = lv_label_create(samplingSaveBtn, NULL);
    lv_label_set_text(samplingSaveLabel, "Save");
    lv_obj_add_style(samplingSaveBtn, LV_BTN_PART_MAIN, &whiteButtonStyle);

    measurePeriodlabel = Label(samplingSettingsScr, NULL, 5, 52, "Measurements \nsaving time \n[HH:MM:SS]", LV_COLOR_WHITE);
    lv_obj_set_style_local_text_font(measurePeriodlabel, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_14);

    measurePeriodHour = lv_spinbox_create(samplingSettingsScr, NULL);
    lv_textarea_set_cursor_hidden(measurePeriodHour, true);
    lv_textarea_set_text_align(measurePeriodHour, LV_LABEL_ALIGN_CENTER);
    lv_spinbox_set_range(measurePeriodHour, 0, 24);
    lv_spinbox_set_digit_format(measurePeriodHour, 2, 0);
    lv_obj_set_size(measurePeriodHour, 40, 34);
    lv_obj_set_pos(measurePeriodHour, 165, 61);

    measurePeriodHourIncrement = Button(samplingSettingsScr, NULL, 20, 20, 175, 39, sampling_hourIncrement);
    lv_theme_apply(measurePeriodHourIncrement, LV_THEME_SPINBOX_BTN);
    lv_obj_add_style(measurePeriodHourIncrement, LV_OBJ_PART_MAIN, &Styles::borderlessStyle);
    lv_obj_set_style_local_value_str(measurePeriodHourIncrement, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_PLUS);

    measurePeriodHourDecrement = Button(samplingSettingsScr, measurePeriodHourIncrement, 20, 20, 175, 97, sampling_hourDecrement);
    lv_obj_set_style_local_value_str(measurePeriodHourDecrement, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_MINUS);

    measureColonLabel = Label(samplingSettingsScr, NULL, 210, 70, ":", LV_COLOR_WHITE);

    measurePeriodMinute = lv_spinbox_create(samplingSettingsScr, NULL);
    lv_textarea_set_cursor_hidden(measurePeriodMinute, true);
    lv_textarea_set_text_align(measurePeriodMinute, LV_LABEL_ALIGN_CENTER);
    lv_spinbox_set_range(measurePeriodMinute, 0, 59);
    lv_spinbox_set_digit_format(measurePeriodMinute, 2, 0);
    lv_obj_set_size(measurePeriodMinute, 40, 34);
    lv_obj_set_pos(measurePeriodMinute, 219, 61);

    measurePeriodMinuteIncrement = Button(samplingSettingsScr, measurePeriodHourIncrement, 20, 20, 229, 39, sampling_minuteIncrement);
    lv_obj_set_style_local_value_str(measurePeriodMinuteIncrement, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_PLUS);

    measurePeriodMinuteDecrement = Button(samplingSettingsScr, measurePeriodHourIncrement, 20, 20, 229, 97, sampling_minuteDecrement);
    lv_obj_set_style_local_value_str(measurePeriodMinuteDecrement, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_MINUS);

    measureColonLabel2 = Label(samplingSettingsScr, measurePeriodlabel, 265, 70, ":", LV_COLOR_WHITE);

    measurePeriodsecond = lv_spinbox_create(samplingSettingsScr, NULL);
    lv_textarea_set_cursor_hidden(measurePeriodsecond, true);
    lv_textarea_set_text_align(measurePeriodsecond, LV_LABEL_ALIGN_CENTER);
    lv_spinbox_set_range(measurePeriodsecond, 0, 59);
    lv_spinbox_set_digit_format(measurePeriodsecond, 2, 0);
    lv_obj_set_size(measurePeriodsecond, 40, 34);
    lv_obj_set_pos(measurePeriodsecond, 274, 61);

    measurePeriodsecondIncrement = Button(samplingSettingsScr, measurePeriodHourIncrement, 20, 20, 284, 39, sampling_secondIncrement);
    lv_obj_set_style_local_value_str(measurePeriodsecondIncrement, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_PLUS);

    measurePeriodsecondDecrement = Button(samplingSettingsScr, measurePeriodHourIncrement, 20, 20, 284, 97, sampling_secondDecrement);
    lv_obj_set_style_local_value_str(measurePeriodsecondDecrement, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_MINUS);

    measureNumberLabel = Label(samplingSettingsScr, NULL, 5, 127, "Number of samples", LV_COLOR_WHITE);
    lv_obj_set_style_local_text_font(measureNumberLabel, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_14);

    measureNumber = lv_spinbox_create(samplingSettingsScr, NULL);
    lv_textarea_set_cursor_hidden(measureNumber, true);
    lv_textarea_set_text_align(measureNumber, LV_LABEL_ALIGN_CENTER);
    lv_spinbox_set_digit_format(measureNumber, 2, 0);
    lv_spinbox_set_range(measureNumber, 1, 20);
    lv_obj_set_size(measureNumber, 50, 34);
    lv_obj_set_pos(measureNumber, 206, 119);

    measureNumberIncrement = Button(samplingSettingsScr, measurePeriodHourIncrement, 40, 34, 257, 119, measureNumberIncrement_func);
    lv_obj_set_style_local_value_str(measureNumberIncrement, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_PLUS);

    measureNumberDecrement = Button(samplingSettingsScr, measurePeriodHourIncrement, 40, 34, 165, 119, measureNumberDecrement_func);
    lv_obj_set_style_local_value_str(measureNumberDecrement, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_MINUS);

    measureAvPeriodLabel = Label(samplingSettingsScr, measurePeriodlabel, 5, 165, "Time between \nmeasurments [s]:", LV_COLOR_WHITE);

    measureAvPeriod = lv_spinbox_create(samplingSettingsScr, NULL);
    lv_textarea_set_cursor_hidden(measureAvPeriod, true);
    lv_textarea_set_text_align(measureAvPeriod, LV_LABEL_ALIGN_CENTER);
    lv_spinbox_set_digit_format(measureAvPeriod, 3, 0);
    lv_spinbox_set_range(measureAvPeriod, 5, 999);
    lv_obj_set_size(measureAvPeriod, 50, 34);
    lv_obj_set_pos(measureAvPeriod, 206, 160);

    measureAvPeriodIncrement = Button(samplingSettingsScr, measurePeriodHourIncrement, 40, 34, 257, 160, av_periodIncrement);
    lv_obj_set_style_local_value_str(measureAvPeriodIncrement, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_PLUS);

    measureAvPeriodDecrement = Button(samplingSettingsScr, measurePeriodHourIncrement, 40, 34, 165, 160, av_periodDecrement);
    lv_obj_set_style_local_value_str(measureAvPeriodDecrement, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_MINUS);

    turnFanOnTimeLabel = Label(samplingSettingsScr, measurePeriodlabel, 5, 205, "Fan running time \nbefore measure [s]:");

    turnFanOnTime = lv_spinbox_create(samplingSettingsScr, NULL);
    lv_textarea_set_cursor_hidden(turnFanOnTime, true);
    lv_textarea_set_text_align(turnFanOnTime, LV_LABEL_ALIGN_CENTER);
    lv_spinbox_set_digit_format(turnFanOnTime, 3, 0);
    lv_spinbox_set_range(turnFanOnTime, 1, 999);
    lv_obj_set_size(turnFanOnTime, 50, 34);
    lv_obj_set_pos(turnFanOnTime, 206, 200);

    turnFanOnTimeIncrement = Button(samplingSettingsScr, measurePeriodHourIncrement, 40, 34, 257, 200, turnFanOnTimeIncrement_func);
    lv_obj_set_style_local_value_str(turnFanOnTimeIncrement, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_PLUS);

    turnFanOnTimeDecrement = Button(samplingSettingsScr, measurePeriodHourIncrement, 40, 34, 165, 200, turnFanOnTimeDecrement_func);
    lv_obj_set_style_local_value_str(turnFanOnTimeDecrement, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_MINUS);
}

void wifiScreen()
{
    cancelBtn = Button(wifiScr, backSettingsBtn, 30, 15, 14, 10, btn_cancel);
    cancelLabel = lv_label_create(cancelBtn, NULL);
    lv_label_set_text(cancelLabel, LV_SYMBOL_LEFT);
    lv_obj_add_style(cancelBtn, LV_OBJ_PART_MAIN, &Styles::transparentBackgroundStyle);
    lv_obj_add_style(cancelBtn, LV_OBJ_PART_MAIN, &Styles::borderlessStyle);
    lv_obj_add_style(cancelBtn, LV_OBJ_PART_MAIN, &Styles::whiteFontStyle);

    wifiLabelAtBar = Label(wifiScr, NULL, 201, 10, "WiFi settings");

    ssidLabel = Label(wifiScr, NULL, 5, 53, "SSID: ");

    ssidTA = lv_textarea_create(wifiScr, NULL);
    lv_textarea_set_text(ssidTA, "");
    lv_textarea_set_pwd_mode(ssidTA, false);
    lv_textarea_set_one_line(ssidTA, true);
    lv_obj_set_event_cb(ssidTA, ta_event_cb);
    lv_textarea_set_cursor_hidden(ssidTA, true);
    lv_obj_set_size(ssidTA, 140, 34);
    lv_obj_set_pos(ssidTA, 100, 45);
    lv_obj_add_style(ssidTA, LV_OBJ_PART_MAIN, &Styles::borderlessStyle);

    pwdLabel = Label(wifiScr, NULL, 5, 92, "Password: ");

    pwdTA = lv_textarea_create(wifiScr, NULL);
    lv_textarea_set_text(pwdTA, "");
    lv_textarea_set_pwd_show_time(pwdTA, 5000);
    lv_textarea_set_pwd_mode(pwdTA, true);
    lv_textarea_set_one_line(pwdTA, true);
    lv_obj_set_event_cb(pwdTA, ta_event_cb);
    lv_textarea_set_cursor_hidden(pwdTA, true);
    lv_obj_set_size(pwdTA, 140, 34);
    lv_obj_set_pos(pwdTA, 100, 85);
    lv_obj_add_style(pwdTA, LV_OBJ_PART_MAIN, &Styles::borderlessStyle);

    showHideBtn = Button(wifiScr, NULL, 75, 43, 243, 85, showHideBtn_func);
    showHideBtnLabel = lv_label_create(showHideBtn, NULL);
    lv_label_set_text(showHideBtnLabel, LV_SYMBOL_EYE_OPEN);
    lv_obj_add_style(showHideBtn, LV_OBJ_PART_MAIN, &Styles::transparentBackgroundStyle);
    lv_obj_add_style(showHideBtn, LV_OBJ_PART_MAIN, &Styles::borderlessStyle);
    lv_obj_add_style(showHideBtnLabel, LV_OBJ_PART_MAIN, &Styles::whiteFontStyle);

    applyBtn = Button(wifiScr, NULL, 75, 43, 243, 43, btn_connect);
    applyLabel = lv_label_create(applyBtn, NULL);
    lv_label_set_text(applyLabel, "Connect");
    lv_obj_add_style(applyBtn, LV_OBJ_PART_MAIN, &whiteButtonStyle);
    lv_obj_add_style(applyBtn, LV_OBJ_PART_MAIN, &Styles::borderlessStyle);
}

void lockScreen()
{
    contDateTimeAtLock = lv_cont_create(lockScr, NULL);
    lv_obj_set_auto_realign(contDateTimeAtLock, true);
    lv_obj_align(contDateTimeAtLock, NULL, LV_ALIGN_CENTER, 0, -40);
    lv_cont_set_fit(contDateTimeAtLock, LV_FIT_TIGHT);
    lv_cont_set_layout(contDateTimeAtLock, LV_LAYOUT_PRETTY_MID);
    lv_obj_add_style(contDateTimeAtLock, LV_OBJ_PART_MAIN, &Styles::transparentBackgroundStyle);
    lv_obj_add_style(contDateTimeAtLock, LV_OBJ_PART_MAIN, &Styles::borderlessStyle);
    lv_obj_add_style(contDateTimeAtLock, LV_OBJ_PART_MAIN, &Styles::whiteFontStyle);

    unlockButton = lv_btn_create(lockScr, nullptr); // TODO copy was from main screen, was setButton
    labelUnlockButton = lv_label_create(unlockButton, NULL);
    lv_obj_set_style_local_text_font(unlockButton, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, &monte16lock);
    lv_obj_align(unlockButton, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, -25);
    lv_label_set_text(labelUnlockButton, Symbols::UNLOCK_SYMBOL);
    lv_btn_set_fit(unlockButton, LV_FIT_TIGHT);
    lv_obj_set_event_cb(unlockButton, unlockButton_task);

    labelTimeLock = lv_label_create(contDateTimeAtLock, NULL);
    lv_label_set_text(labelTimeLock, "No WiFi connection");
    lv_label_set_align(labelTimeLock, LV_LABEL_ALIGN_CENTER);
    lv_obj_align(labelTimeLock, NULL, LV_ALIGN_CENTER, 0, 30);

    labelDateLock = lv_label_create(contDateTimeAtLock, NULL);
    lv_label_set_text(labelDateLock, "");
    lv_obj_align(labelDateLock, NULL, LV_ALIGN_CENTER, 0, 0);

    wifiStatusAtLock = lv_label_create(lockScr, nullptr); // TODO copy was from main screen, was wifiStatusAtMain
    lv_obj_align(wifiStatusAtLock, NULL, LV_ALIGN_CENTER, -36, 10);
    lv_label_set_text(wifiStatusAtLock, LV_SYMBOL_WIFI);
    wifiStatusAtLockWarning = Label(wifiStatusAtLock, nullptr, 6, 6, LV_SYMBOL_CLOSE, LV_COLOR_RED);

    sdStatusAtLock = lv_label_create(lockScr, nullptr); // TODO copy was from main screen, sdStatusAtMain
    lv_obj_align(sdStatusAtLock, NULL, LV_ALIGN_CENTER, 36, 10);
    lv_label_set_text(sdStatusAtLock, LV_SYMBOL_SD_CARD);
    sdStatusAtLockWarning = Label(sdStatusAtLock, wifiStatusAtLockWarning, 2, 6, LV_SYMBOL_CLOSE, LV_COLOR_RED);

    ledAtLock = lv_led_create(lockScr, NULL);
    lv_obj_set_size(ledAtLock, 13, 13);
    lv_obj_align(ledAtLock, NULL, LV_ALIGN_CENTER, 0, 10);
    lv_led_set_bright(ledAtLock, 200);
    lv_obj_set_style_local_bg_color(ledAtLock, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_YELLOW);
    lv_obj_set_style_local_shadow_color(ledAtLock, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_YELLOW);
    lv_obj_set_style_local_border_opa(ledAtLock, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_0);
}