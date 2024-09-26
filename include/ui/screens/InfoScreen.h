#pragma once

#include <WString.h>
#include <lvgl.h>

class InfoScreen {
   public:
    InfoScreen();
    ~InfoScreen();
    lv_obj_t *getScreen() const;
    void setWifiAddress(const char *text);
    void display_current_config();

   private:
    String format_time(unsigned long timeMs) const;

    lv_obj_t *infoScr;
    lv_obj_t *backInfoLabel;
    lv_obj_t *infoWifiAddressLabel;
    lv_obj_t *configLabel;
};