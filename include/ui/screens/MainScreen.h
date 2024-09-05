#pragma once

#include <lvgl.h>
#include <Wire.h>

#include "ui/widgets/Container.h"

class MainScreen : public lv_obj_t
{
public:
    MainScreen();
    ~MainScreen();
    lv_obj_t *getScreen() const;

    void setAqiStateNColor(float pm25Aqi);
    void updateSampleData(const float pm10, const float pm25, const float pm100,
                          const float particles03, const float particles05,
                          const float particles10, const float particles25,
                          const float particles50, const float particles100,
                          const float temp, const float humidity);
    void setSampleSaveError(const bool error);
    void updateDateTime(const String &dateTime);
    void setWifiWarning(const bool isWarning);
    void setSdCardWarning(const bool isWarning);

private:
    void drawParticlesIndicator();

    lv_obj_t *mainScr;
    lv_obj_t *labelSetButton;
    lv_obj_t *labelLockButton;
    lv_obj_t *labelParticleSizeum[7];
    lv_obj_t *contParticlesNumber[6];
    lv_obj_t *labelParticlesNumber[6];

    lv_obj_t *labelAQIColorBar;
    lv_obj_t *contAQIColorBar;
    lv_obj_t *labelPM10Data;
    lv_obj_t *labelPM25Data;
    lv_obj_t *labelPM100Data;
    lv_obj_t *labelTempValue;
    lv_obj_t *labelHumiValue;
    lv_obj_t *led;
    lv_obj_t *dateAndTime;
    lv_obj_t *wifiStatusWarning;
    lv_obj_t *sdStatusWarning;

    // An array of points pairs instead of multiple names and declarations
    static lv_point_t dividingLinesPoints[][7];

    // An array of lines dividing main one
    lv_obj_t *dividingLines[7];

    const String particlesSize[7] = {"0.0", "0.3", "0.5", "1.0", "2.5", "5.0", "10.0"};

    const int labelParticleSizePosX[7] = {9, 56, 103, 153, 198, 245, 288};
    const int contParticleNumberPosX[6] = {18, 65, 112, 159, 206, 253};

    lv_obj_t *mainLine;
    static lv_point_t mainLinePoints[];

    // todo move somewhere else, this air quality standars
    const float aqiStandards[5] = {21, 61, 101, 141, 201};
    const String airQualityStates[6] = {"Excellent", "Good", "Moderate", "Unhealthy", "Bad", "Hazardous"};
    // An array of colors used depending on actual pm2.5 value
    const lv_color_t airQualityColors[6] = {LV_COLOR_GREEN, LV_COLOR_GREEN, LV_COLOR_YELLOW,
                                            LV_COLOR_ORANGE, LV_COLOR_RED, LV_COLOR_RED};
};