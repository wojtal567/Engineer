#include "ui/screens/WifiSettingsScreen.h"

#include <HardwareSerial.h>
#include <WiFi.h>

#include "globals/Global.h"
#include "globals/Peripherals.h"
#include "ui/Screens.h"
#include "ui/Styles.h"
#include "ui/widgets/Button.h"
#include "ui/widgets/Label.h"

WifiSettingsScreen *WifiSettingsScreen::instance = nullptr;

WifiSettingsScreen::WifiSettingsScreen() {
    wifiScr = lv_cont_create(NULL, NULL);
    lv_obj_set_style_local_bg_color(wifiScr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);

    instance = this;

    Button cancelBtn = Button(wifiScr, NULL, 30, 15, 14, 10, [](lv_obj_t *, lv_event_t event) {
        if (event == LV_EVENT_CLICKED) {
            lv_disp_load_scr(Screens::settingsScr->getScreen());
            lv_textarea_set_text(instance->ssidTA, "");
            lv_textarea_set_text(instance->pwdTA, "");
        }
    });

    cancelLabel = lv_label_create(cancelBtn, NULL);
    lv_label_set_text(cancelLabel, LV_SYMBOL_LEFT);
    lv_obj_add_style(cancelBtn, LV_OBJ_PART_MAIN, &Styles::transparentBackgroundStyle);
    lv_obj_add_style(cancelBtn, LV_OBJ_PART_MAIN, &Styles::borderlessStyle);
    lv_obj_add_style(cancelBtn, LV_OBJ_PART_MAIN, &Styles::whiteFontStyle);

    Label wifiLabelAtBar = Label(wifiScr, NULL, 201, 10, "WiFi settings");

    Label ssidLabel = Label(wifiScr, NULL, 5, 53, "SSID: ");

    ssidTA = lv_textarea_create(wifiScr, NULL);
    lv_textarea_set_text(ssidTA, "");
    lv_textarea_set_pwd_mode(ssidTA, false);
    lv_textarea_set_one_line(ssidTA, true);
    lv_obj_set_event_cb(ssidTA, textAreaEventCallback);

    lv_textarea_set_cursor_hidden(ssidTA, true);
    lv_obj_set_size(ssidTA, 140, 34);
    lv_obj_set_pos(ssidTA, 100, 45);
    lv_obj_add_style(ssidTA, LV_OBJ_PART_MAIN, &Styles::borderlessStyle);

    Label pwdLabel = Label(wifiScr, NULL, 5, 92, "Password: ");

    pwdTA = lv_textarea_create(wifiScr, NULL);
    lv_textarea_set_text(pwdTA, "");
    lv_textarea_set_pwd_show_time(pwdTA, 5000);
    lv_textarea_set_pwd_mode(pwdTA, true);
    lv_textarea_set_one_line(pwdTA, true);
    lv_obj_set_event_cb(pwdTA, textAreaEventCallback);
    lv_textarea_set_cursor_hidden(pwdTA, true);
    lv_obj_set_size(pwdTA, 140, 34);
    lv_obj_set_pos(pwdTA, 100, 85);
    lv_obj_add_style(pwdTA, LV_OBJ_PART_MAIN, &Styles::borderlessStyle);

    Button showHideBtn = Button(wifiScr, NULL, 75, 43, 243, 85, [](lv_obj_t *, lv_event_t event) {
        if (event == LV_EVENT_CLICKED) {
            if (lv_textarea_get_pwd_mode(instance->pwdTA)) {
                lv_textarea_set_pwd_mode(instance->pwdTA, false);
                lv_label_set_text(instance->showHideBtnLabel, LV_SYMBOL_EYE_CLOSE);
            } else {
                lv_textarea_set_pwd_mode(instance->pwdTA, true);
                lv_textarea_set_pwd_show_time(instance->pwdTA, 1);
                lv_textarea_set_text(instance->pwdTA, lv_textarea_get_text(instance->pwdTA));
                lv_textarea_set_pwd_show_time(instance->pwdTA, 5000);
                lv_label_set_text(instance->showHideBtnLabel, LV_SYMBOL_EYE_OPEN);
            }
        }
    });
    showHideBtnLabel = lv_label_create(showHideBtn, NULL);
    lv_label_set_text(showHideBtnLabel, LV_SYMBOL_EYE_OPEN);
    lv_obj_add_style(showHideBtn, LV_OBJ_PART_MAIN, &Styles::transparentBackgroundStyle);
    lv_obj_add_style(showHideBtn, LV_OBJ_PART_MAIN, &Styles::borderlessStyle);
    lv_obj_add_style(showHideBtnLabel, LV_OBJ_PART_MAIN, &Styles::whiteFontStyle);

    // TODO add some optimization to this handler
    Button applyBtn = Button(wifiScr, NULL, 75, 43, 243, 43, [](lv_obj_t *, lv_event_t event) {
        if (event == LV_EVENT_CLICKED and
            ((lv_textarea_get_text(instance->ssidTA) != NULL and lv_textarea_get_text(instance->ssidTA)[0] != '\0') or
             (lv_textarea_get_text(instance->pwdTA) != NULL and lv_textarea_get_text(instance->pwdTA)[0] != '\0'))) {
            uint8_t wifiAttempts = 10;

            Global::config.ssid = lv_textarea_get_text(instance->ssidTA);
            Serial.println(Global::config.ssid.c_str());
            Global::config.password = lv_textarea_get_text(instance->pwdTA);

            Peripherals::mySDCard.saveConfig(Global::config, Global::configFilePath);
            Peripherals::mySDCard.printConfig(Global::configFilePath);
            WiFi.begin(Global::config.ssid.c_str(), Global::config.password.c_str());
            while (WiFi.status() != WL_CONNECTED and wifiAttempts > 0) {
                delay(500);
                wifiAttempts--;
            }

            if (WiFi.status() == WL_CONNECTED)
                Serial.println("btn_connect -> connected to Wi-Fi! IP: " + WiFi.localIP().toString());
            else if (WiFi.status() == WL_DISCONNECTED)
                Serial.println("btn_connect -> can't connect. Probably you have entered wrong credentials.");
            lv_disp_load_scr(Screens::mainScr->getScreen());
            lv_textarea_set_text(instance->ssidTA, "");
            lv_textarea_set_text(instance->pwdTA, "");
        }
    });
    applyLabel = lv_label_create(applyBtn, NULL);
    lv_label_set_text(applyLabel, "Connect");
    lv_obj_add_style(applyBtn, LV_OBJ_PART_MAIN, &Styles::whiteButtonStyle);
    lv_obj_add_style(applyBtn, LV_OBJ_PART_MAIN, &Styles::borderlessStyle);
}

WifiSettingsScreen::~WifiSettingsScreen() {
    lv_obj_del(wifiScr);
    wifiScr = nullptr;
    lv_obj_del(keyboard);
    keyboard = nullptr;
    lv_obj_del(ssidTA);
    ssidTA = nullptr;
    lv_obj_del(pwdTA);
    pwdTA = nullptr;
    lv_obj_del(applyLabel);
    applyLabel = nullptr;
    lv_obj_del(cancelLabel);
    cancelLabel = nullptr;
    lv_obj_del(showHideBtnLabel);
    showHideBtnLabel = nullptr;
}

void WifiSettingsScreen::textAreaEventCallback(lv_obj_t *textArea, lv_event_t event) {
    if (event == LV_EVENT_CLICKED) {
        if (textArea == instance->ssidTA) {
            lv_textarea_set_cursor_hidden(instance->ssidTA, false);
            lv_textarea_set_cursor_hidden(instance->pwdTA, true);
        } else if (textArea == instance->pwdTA) {
            lv_textarea_set_cursor_hidden(instance->pwdTA, false);
            lv_textarea_set_cursor_hidden(instance->ssidTA, true);
        }
    }

    if (instance->keyboard == NULL) {
        instance->keyboard = lv_keyboard_create(lv_scr_act(), NULL);
        lv_obj_set_size(instance->keyboard, LV_HOR_RES, LV_VER_RES / 2);
        lv_obj_set_event_cb(instance->keyboard, keyboardCallback);
        lv_keyboard_set_textarea(instance->keyboard, textArea);
    } else {
        lv_keyboard_set_textarea(instance->keyboard, textArea);
    }
}

void WifiSettingsScreen::keyboardCallback(lv_obj_t *kb, lv_event_t event) {
    if (event != LV_EVENT_CANCEL) {
        lv_keyboard_def_event_cb(kb, event);
    }
}

lv_obj_t *WifiSettingsScreen::getScreen() const { return wifiScr; }
