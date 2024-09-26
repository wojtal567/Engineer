#include "ui/screens/InfoScreen.h"

#include <globals/Global.h>

#include "ui/Screens.h"
#include "ui/Styles.h"
#include "ui/Symbols.h"
#include "ui/widgets/Button.h"
#include "ui/widgets/Label.h"

InfoScreen::InfoScreen() {
    infoScr = lv_cont_create(NULL, NULL);
    lv_obj_set_style_local_bg_color(infoScr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);

    Button backInfoBtn = Button(infoScr, NULL, 30, 15, 14, 10, [](lv_obj_t *, lv_event_t event) {
        if (event == LV_EVENT_CLICKED) {
            lv_disp_load_scr(Screens::settingsScr->getScreen());
        }
    });
    lv_obj_add_style(backInfoBtn, LV_OBJ_PART_MAIN, &Styles::transparentBackgroundStyle);
    lv_obj_add_style(backInfoBtn, LV_OBJ_PART_MAIN, &Styles::borderlessStyle);
    lv_obj_add_style(backInfoBtn, LV_OBJ_PART_MAIN, &Styles::whiteFontStyle);

    backInfoLabel = lv_label_create(backInfoBtn, NULL);
    lv_label_set_text(backInfoLabel, LV_SYMBOL_LEFT);

    Label lcdLabelAtBar = Label(infoScr, NULL, 216, 10, "Device info");

    Label infoWifiLabel = Label(infoScr, NULL, 5, 53, "WiFi address: ");
    lv_obj_set_style_local_text_font(infoWifiLabel, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_14);

    infoWifiAddressLabel = Label(infoScr, infoWifiLabel, 115, 53, "");

    configLabel = Label(infoScr, NULL, 5, 70, "");
}

void InfoScreen::display_current_config() {
    String current_config = "SSID: " + String(Global::config.ssid.c_str());
    current_config += "\nNumber of samples: " + String(Global::config.numberOfSamples);

    switch (Global::config.lcdLockTime) {
        case -1:
            current_config += "\nLCD lock time: Never";
            break;
        case 30000:
            current_config += "\nLCD lock time: 30s";
            break;
        default:
            if (Global::config.lcdLockTime > 30000) {
                current_config += "\nLCD lock time: " + String(Global::config.lcdLockTime / 60000) + "m";
            }
            break;
    }

    current_config += "\nFan running time before measure: " + String(Global::config.turnFanTime / 1000) + "s";

    current_config += "\nTime between measurements: " + String(Global::config.measurePeriod / 1000) + "s";

    current_config += "\nMeasurements saving time: " + format_time(Global::config.timeBetweenSavingSamples);

    lv_obj_set_style_local_text_font(configLabel, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_14);
    lv_label_set_text(configLabel, current_config.c_str());
}

String InfoScreen::format_time(unsigned long timeMs) const {
    if (timeMs >= 3600000) {
        int hours = timeMs / 3600000;
        int minutes = (timeMs / 60000) % 60;
        int seconds = (timeMs / 1000) % 60;
        return String(hours) + "h " + String(minutes) + "m " + String(seconds) + "s";
    } else if (timeMs >= 60000) {
        int minutes = (timeMs / 60000) % 60;
        int seconds = (timeMs / 1000) % 60;
        return String(minutes) + "m " + String(seconds) + "s";
    } else {
        return String(timeMs / 1000) + "s";
    }
}

InfoScreen::~InfoScreen() {
    lv_obj_del(infoScr);
    infoScr = nullptr;
    lv_obj_del(backInfoLabel);
    backInfoLabel = nullptr;
    lv_obj_del(infoWifiAddressLabel);
    infoWifiAddressLabel = nullptr;
    lv_obj_del(configLabel);
    configLabel = nullptr;
}

void InfoScreen::setWifiAddress(const char *text) { lv_label_set_text(infoWifiAddressLabel, text); }

lv_obj_t *InfoScreen::getScreen() const { return infoScr; }