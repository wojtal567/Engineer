#include "ui/screens/MainScreen.h"

#include <lvgl.h>

#include "ui/widgets/Button.h"
#include "ui/widgets/Label.h"
#include "ui/Styles.h"

#include <Wire.h>
#include <Arduino.h>
#include "ui/Symbols.h"
lv_point_t MainScreen::dividingLinesPoints[][7] = {{{18, 205}, {18, 215}},
                                                   {{65, 205}, {65, 215}},
                                                   {{112, 205}, {112, 215}},
                                                   {{159, 205}, {159, 215}},
                                                   {{206, 205}, {206, 215}},
                                                   {{253, 205}, {253, 215}},
                                                   {{300, 205}, {300, 215}}};

lv_point_t MainScreen::mainLinePoints[] = {{18, 210}, {300, 210}};

MainScreen::MainScreen()
{
    mainScr = lv_cont_create(NULL, NULL);
    lv_obj_set_style_local_bg_color(mainScr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);

    Button setButton = Button(mainScr, NULL, 16, 18, 32, 7, [](lv_obj_t *, lv_event_t event)
                              {
        if (event == LV_EVENT_CLICKED) {
            // lv_disp_load_scr(Screens::settings);
        } });

    lv_obj_t *labelSetButton = lv_label_create(setButton, NULL);
    lv_label_set_text(labelSetButton, LV_SYMBOL_SETTINGS);
    lv_obj_add_style(setButton, LV_OBJ_PART_MAIN, &Styles::transparentBackgroundStyle);
    lv_obj_add_style(setButton, LV_OBJ_PART_MAIN, &Styles::borderlessStyle);
    lv_obj_add_style(setButton, LV_OBJ_PART_MAIN, &Styles::whiteFontStyle);

    Button lockButton = Button(mainScr, setButton, 14, 18, 95, 7, [](lv_obj_t *, lv_event_t event)
                               {
                                if (event == LV_EVENT_CLICKED) {
                            // lv_disp_load_scr(Screens::lock);
                                } });

    lv_obj_t *labelLockButton = lv_label_create(lockButton, NULL);
    lv_obj_set_style_local_text_font(lockButton, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, &monte16lock);
    lv_label_set_text(labelLockButton, Symbols::LOCK_SYMBOL);

    Label wifiStatus = Label(mainScr, NULL, 52, 7, LV_SYMBOL_WIFI, LV_COLOR_WHITE);
    wifiStatusWarning = Label(wifiStatus, NULL, 6, 6, LV_SYMBOL_CLOSE, LV_COLOR_RED);
    lv_obj_add_style(wifiStatusWarning, LV_OBJ_PART_MAIN, &Styles::transparentBackgroundStyle);
    lv_obj_add_style(wifiStatusWarning, LV_OBJ_PART_MAIN, &Styles::font12Style);

    Label sdStatus = Label(mainScr, wifiStatus, 77, 7, LV_SYMBOL_SD_CARD, LV_COLOR_WHITE);
    sdStatusWarning = Label(sdStatus, wifiStatusWarning, 2, 6, LV_SYMBOL_CLOSE, LV_COLOR_RED);

    dateAndTime = Label(mainScr, NULL, 157, 7, "");

    Container contPM25 = Container(mainScr, NULL, 180, 90, 10, 30);
    lv_obj_set_click(contPM25, false);
    lv_obj_add_style(contPM25, LV_OBJ_PART_MAIN, &Styles::transparentBackgroundStyle);
    lv_obj_add_style(contPM25, LV_OBJ_PART_MAIN, &Styles::containerStyle);

    Container contTemp = Container(mainScr, contPM25, 122, 46, 188, 30);
    lv_obj_set_click(contTemp, false);

    Container contHumi = Container(mainScr, contPM25, 122, 46, 188, 74);
    lv_obj_set_click(contHumi, false);

    Container contPM10 = Container(mainScr, NULL, 91, 62, 10, 118);
    lv_obj_set_click(contPM10, false);
    lv_obj_add_style(contPM10, LV_OBJ_PART_MAIN, &Styles::transparentBackgroundStyle);
    lv_obj_add_style(contPM10, LV_OBJ_PART_MAIN, &Styles::containerStyle);
    lv_obj_add_style(contPM10, LV_OBJ_PART_MAIN, &Styles::font12Style);

    Container contPM100 = Container(mainScr, contPM10, 91, 62, 99, 118);
    lv_obj_set_click(contPM100, false);

    Container contAQI = Container(mainScr, contPM10, 122, 62, 188, 118);
    lv_obj_set_click(contAQI, false);

    contAQIColorBar = Container(contAQI, contAQI, 92, 24, 15, 25);
    lv_obj_set_style_local_bg_opa(contAQIColorBar, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_70);
    lv_obj_set_click(contAQIColorBar, false);

    Label labelTemp = Label(contTemp, NULL, 5, 3, "Temp");

    labelTempValue = Label(contTemp, NULL, 16, 22, "         -");
    lv_obj_add_style(labelTempValue, LV_OBJ_PART_MAIN, &Styles::font20Style);
    lv_label_set_align(labelTempValue, LV_LABEL_ALIGN_LEFT);

    Label labelHumi = Label(contHumi, NULL, 5, 3, "RH");

    labelHumiValue = Label(contHumi, NULL, 16, 22, "         -");
    lv_obj_add_style(labelHumiValue, LV_OBJ_PART_MAIN, &Styles::font20Style);

    Label labelPM10 = Label(contPM10, NULL, 5, 5, "PM 1.0 ug/m");

    Label labelPM10UpperIndex = Label(contPM10, NULL, 80, 3, "3");

    Label labelPM25 = Label(contPM25, NULL, 5, 5, "PM 2.5                ug/m");

    Label labelPM25UpperIndex = Label(contPM25, NULL, 167, 3, "3");
    lv_obj_add_style(labelPM25UpperIndex, LV_OBJ_PART_MAIN, &Styles::font12Style);

    Label labelPM100 = Label(contPM100, NULL, 5, 5, "PM 10 ug/m");

    Label labelPM100UpperIndex = Label(contPM100, NULL, 77, 3, "3");
    lv_obj_add_style(labelPM100UpperIndex, LV_OBJ_PART_MAIN, &Styles::font12Style);

    labelPM10Data = lv_label_create(contPM10, NULL);
    lv_obj_add_style(labelPM10Data, LV_OBJ_PART_MAIN, &Styles::font22Style);
    lv_obj_add_style(labelPM10Data, LV_OBJ_PART_MAIN, &Styles::whiteFontStyle);
    lv_obj_set_auto_realign(labelPM10Data, true);
    lv_obj_align(labelPM10Data, NULL, LV_ALIGN_CENTER, 0, 5);
    lv_label_set_text(labelPM10Data, "-");

    labelPM25Data = lv_label_create(contPM25, NULL);
    lv_obj_add_style(labelPM25Data, LV_OBJ_PART_MAIN, &Styles::font22Style);
    lv_obj_add_style(labelPM25Data, LV_OBJ_PART_MAIN, &Styles::whiteFontStyle);
    lv_obj_set_auto_realign(labelPM25Data, true);
    lv_obj_align(labelPM25Data, NULL, LV_ALIGN_CENTER, 0, 10);
    lv_label_set_text(labelPM25Data, "-");

    labelPM100Data = lv_label_create(contPM100, NULL);
    lv_obj_add_style(labelPM100Data, LV_OBJ_PART_MAIN, &Styles::font22Style);
    lv_obj_add_style(labelPM100Data, LV_OBJ_PART_MAIN, &Styles::whiteFontStyle);
    lv_obj_set_auto_realign(labelPM100Data, true);
    lv_obj_align(labelPM100Data, NULL, LV_ALIGN_CENTER, 0, 5);
    lv_label_set_text(labelPM100Data, "-");

    Label labelAQI = Label(contAQI, NULL, 5, 5, "Air Quality  PM 2.5");

    labelAQIColorBar = lv_label_create(contAQIColorBar, NULL);
    lv_obj_set_auto_realign(labelAQIColorBar, true);
    lv_obj_align(labelAQIColorBar, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_label_set_text(labelAQIColorBar, "-");
    lv_obj_add_style(labelAQIColorBar, LV_OBJ_PART_MAIN, &Styles::whiteFontStyle);

    led = lv_led_create(mainScr, NULL);
    lv_obj_set_size(led, 13, 13);
    lv_obj_set_pos(led, 13, 10);
    lv_led_set_bright(led, 200);
    lv_obj_set_style_local_bg_color(led, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_YELLOW);
    lv_obj_set_style_local_shadow_color(led, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_YELLOW);
    lv_obj_set_style_local_border_opa(led, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_0);
    // Function that draws lines and st text above those
    drawParticlesIndicator();
}

// Draw a line-like thing
void MainScreen::drawParticlesIndicator()
{
    for (int i = 0; i < 7; i++)
    {
        dividingLines[i] = lv_line_create(mainScr, NULL);
        lv_line_set_points(dividingLines[i], dividingLinesPoints[i], 2);
        lv_obj_add_style(dividingLines[i], LV_LINE_PART_MAIN, &Styles::lineStyle);

        labelParticleSizeum[i] = lv_label_create(mainScr, NULL);
        lv_label_set_text(labelParticleSizeum[i], particlesSize[i].c_str());
        lv_obj_add_style(labelParticleSizeum[i], LV_LABEL_PART_MAIN, &Styles::font12Style);
        lv_obj_add_style(labelParticleSizeum[i], LV_LABEL_PART_MAIN, &Styles::whiteFontStyle);
        lv_obj_set_pos(labelParticleSizeum[i], labelParticleSizePosX[i], 190); // 12
    }

    for (int j = 0; j < 6; j++)
    {
        contParticlesNumber[j] = lv_cont_create(mainScr, NULL);
        lv_obj_add_style(contParticlesNumber[j], LV_OBJ_PART_MAIN, &Styles::containerStyle);
        lv_obj_set_style_local_border_opa(contParticlesNumber[j], LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_0);
        lv_obj_set_click(contParticlesNumber[j], false);
        lv_obj_set_size(contParticlesNumber[j], 47, 14);
        labelParticlesNumber[j] = lv_label_create(contParticlesNumber[j], NULL);
        lv_obj_set_pos(contParticlesNumber[j], contParticleNumberPosX[j], 215); // 20
        lv_label_set_align(labelParticlesNumber[j], LV_LABEL_ALIGN_CENTER);
        lv_obj_set_auto_realign(labelParticlesNumber[j], true);
        lv_label_set_text(labelParticlesNumber[j], "-");
        lv_obj_add_style(labelParticlesNumber[j], LV_LABEL_PART_MAIN, &Styles::font12Style);
        lv_obj_add_style(labelParticlesNumber[j], LV_LABEL_PART_MAIN, &Styles::whiteFontStyle);
    }

    mainLine = lv_line_create(mainScr, NULL);
    lv_line_set_points(mainLine, mainLinePoints, 2);
    lv_line_set_auto_size(mainLine, true);
    lv_obj_add_style(mainLine, LV_LINE_PART_MAIN, &Styles::lineStyle);
}

_lv_obj_t *MainScreen::getScreen() const
{
    return mainScr;
}

void MainScreen::setAqiStateNColor(float pm25Aqi)
{
    for (int i = 0; i < 6; i++)
    {
        if (i == 5 or pm25Aqi < aqiStandards[i])
        {
            lv_label_set_text(labelAQIColorBar, airQualityStates[i].c_str());
            lv_obj_set_style_local_bg_color(contAQIColorBar, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, airQualityColors[i]);
            return;
        }
    }
}

void MainScreen::updateSampleData(const float pm10, const float pm25, const float pm100,
                                  const float particles03, const float particles05,
                                  const float particles10, const float particles25,
                                  const float particles50, const float particles100,
                                  const float temp, const float humidity)
{

    char buffer[7];

    itoa(pm10, buffer, 10);
    lv_label_set_text(labelPM10Data, buffer);
    itoa(pm25, buffer, 10);
    lv_label_set_text(labelPM25Data, buffer);
    itoa(pm100, buffer, 10);
    lv_label_set_text(labelPM100Data, buffer);

    itoa(particles03, buffer, 10);
    lv_label_set_text(labelParticlesNumber[0], buffer);

    itoa(particles05, buffer, 10);
    lv_label_set_text(labelParticlesNumber[1], buffer);

    itoa(particles10, buffer, 10);
    lv_label_set_text(labelParticlesNumber[2], buffer);

    itoa(particles25, buffer, 10);
    lv_label_set_text(labelParticlesNumber[3], buffer);

    itoa(particles50, buffer, 10);
    lv_label_set_text(labelParticlesNumber[4], buffer);

    itoa(particles100, buffer, 10);
    lv_label_set_text(labelParticlesNumber[5], buffer);

    char tempBuffer[16];
    char humidityBuffer[16];

    dtostrf(temp, 10, 2, tempBuffer);
    lv_label_set_text(labelTempValue, strcat(tempBuffer, "°C"));
    dtostrf(humidity, 10, 2, humidityBuffer);
    lv_label_set_text(labelHumiValue, strcat(humidityBuffer, "%"));
}

void MainScreen::setSampleSaveError(bool error)
{
    lv_obj_set_style_local_bg_color(led, LV_LED_PART_MAIN, LV_STATE_DEFAULT,
                                    error ? LV_COLOR_RED : LV_COLOR_GREEN);
    lv_obj_set_style_local_shadow_color(led, LV_LED_PART_MAIN, LV_STATE_DEFAULT,
                                        error ? LV_COLOR_RED : LV_COLOR_GREEN);
}

void MainScreen::updateDateTime(const String &dateTime)
{
    lv_label_set_text(dateAndTime, dateTime.c_str());
}

void MainScreen::setWifiWarning(const bool isWarning)
{
    lv_obj_set_hidden(wifiStatusWarning, !isWarning);
}

void MainScreen::setSdCardWarning(const bool isWarning)
{
    lv_obj_set_hidden(sdStatusWarning, isWarning);
}
