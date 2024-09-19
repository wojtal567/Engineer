#pragma once

#include <lvgl.h>

class SettingsScreen {
   public:
    SettingsScreen();
    ~SettingsScreen();
    lv_obj_t *getScreen() const;

   private:
    lv_obj_t *settingsScr;
    lv_obj_t *backSettingsLabel;
    lv_obj_t *wifiBtnLabel;
    lv_obj_t *infoBtnLabel;
    lv_obj_t *timeBtnLabel;
    lv_obj_t *tempBtnLabel;
};