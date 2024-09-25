#pragma once

#include <lvgl.h>

// TODO rethink this class, singleton etc.

class WifiSettingsScreen {
   public:
    static WifiSettingsScreen &getInstance() {
        static WifiSettingsScreen instance;
        return instance;
    }
    lv_obj_t *getScreen() const;

   private:
    WifiSettingsScreen();
    ~WifiSettingsScreen();

    WifiSettingsScreen(const WifiSettingsScreen &) = delete;
    WifiSettingsScreen &operator=(const WifiSettingsScreen &) = delete;

    lv_obj_t *wifiScr;
    lv_obj_t *keyboard;
    lv_obj_t *ssidTA;
    lv_obj_t *pwdTA;
    lv_obj_t *applyLabel;
    lv_obj_t *cancelLabel;
    lv_obj_t *showHideBtnLabel;

    static void textAreaEventCallback(lv_obj_t *ta, lv_event_t event);
    static void keyboardCallback(lv_obj_t *kb, lv_event_t event);
    static WifiSettingsScreen *instance;
};