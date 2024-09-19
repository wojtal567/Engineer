#include "ui/screens/SettingsScreen.h"

#include "ui/Screens.h"
#include "ui/Styles.h"
#include "ui/Symbols.h"
#include "ui/widgets/Button.h"
#include "ui/widgets/Label.h"

SettingsScreen::SettingsScreen() {
    settingsScr = lv_cont_create(NULL, NULL);
    lv_obj_set_style_local_bg_color(settingsScr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);

    Button backSettingsBtn = Button(settingsScr, NULL, 30, 15, 14, 10, [](lv_obj_t *, lv_event_t event) {
        if (event == LV_EVENT_CLICKED) {
            lv_disp_load_scr(Screens::mainScr->getScreen());
        }
    });

    backSettingsLabel = lv_label_create(backSettingsBtn, NULL);
    lv_label_set_text(backSettingsLabel, LV_SYMBOL_LEFT);
    lv_obj_add_style(backSettingsBtn, LV_OBJ_PART_MAIN, &Styles::transparentBackgroundStyle);
    lv_obj_add_style(backSettingsBtn, LV_OBJ_PART_MAIN, &Styles::borderlessStyle);
    lv_obj_add_style(backSettingsBtn, LV_OBJ_PART_MAIN, &Styles::whiteFontStyle);

    Label settingsLabelAtBar = Label(settingsScr, NULL, 239, 10, "Settings");

    Button wifiBtn = Button(settingsScr, NULL, 60, 60, 60, 38, [](lv_obj_t *, lv_event_t event) {
        if (event == LV_EVENT_CLICKED) {
            // lv_scr_load(wifiScr);
        }
    });
    wifiBtnLabel = lv_label_create(wifiBtn, NULL);
    lv_label_set_text(wifiBtnLabel, Symbols::WIFI_SYMBOL);
    lv_obj_add_style(wifiBtn, LV_OBJ_PART_MAIN, &Styles::transparentBackgroundStyle);
    lv_obj_add_style(wifiBtn, LV_OBJ_PART_MAIN, &Styles::borderlessStyle);
    lv_obj_add_style(wifiBtn, LV_OBJ_PART_MAIN, &Styles::whiteFontStyle);
    lv_obj_add_style(wifiBtn, LV_OBJ_PART_MAIN, &Styles::hugeFontStyle);

    Label wifiBtnName = Label(settingsScr, NULL, 63, 103, "WiFi");
    lv_obj_add_style(wifiBtnName, LV_OBJ_PART_MAIN, &Styles::font20Style);

    Button infoBtn = Button(settingsScr, wifiBtn, 60, 60, 200, 38, [](lv_obj_t *, lv_event_t event) {
        if (event == LV_EVENT_CLICKED) {
            // lv_scr_load(infoScr);
        }
    });
    infoBtnLabel = lv_label_create(infoBtn, NULL);
    lv_label_set_text(infoBtnLabel, Symbols::INFO_SYMBOL);

    Label infoBtnName = Label(settingsScr, wifiBtnName, 207, 103, "Info");

    Button timeBtn = Button(settingsScr, wifiBtn, 60, 60, 60, 140, [](lv_obj_t *, lv_event_t event) {
        if (event == LV_EVENT_CLICKED) {
            // lv_scr_load(timeSettingsScr);
            // inTimeSettings = true; // TODO check what was that???
        }
    });
    timeBtnLabel = lv_label_create(timeBtn, NULL);
    lv_label_set_text(timeBtnLabel, Symbols::CLOCK_SYMBOL);

    Label timeBtnName = Label(settingsScr, wifiBtnName, 65, 200, "Time");

    Button tempBtn = Button(settingsScr, wifiBtn, 60, 60, 200, 140, [](lv_obj_t *, lv_event_t event) {
        if (event == LV_EVENT_CLICKED) {
            // lv_scr_load(samplingSettingsScr);
        }
    });
    tempBtnLabel = lv_label_create(tempBtn, wifiBtnName);
    lv_label_set_text(tempBtnLabel, Symbols::COGS_SYMBOL);
    lv_obj_add_style(tempBtnLabel, LV_OBJ_PART_MAIN, &Styles::hugeFontStyle);
    Label tempBtnName = Label(settingsScr, wifiBtnName, 180, 200, "Sampling");
}

SettingsScreen::~SettingsScreen() {
    lv_obj_del(settingsScr);
    settingsScr = nullptr;
    lv_obj_del(backSettingsLabel);
    backSettingsLabel = nullptr;
    lv_obj_del(wifiBtnLabel);
    wifiBtnLabel = nullptr;
    lv_obj_del(infoBtnLabel);
    infoBtnLabel = nullptr;
    lv_obj_del(timeBtnLabel);
    timeBtnLabel = nullptr;
    lv_obj_del(tempBtnLabel);
    timeBtnLabel = nullptr;
}

lv_obj_t *SettingsScreen::getScreen() const { return settingsScr; }