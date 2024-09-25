#pragma once
#include <RtcDS1307.h>
#include <TFT_eSPI.h>
#include <WEMOS_SHT3X.h>
#include <lvgl.h>

#include "../configs/pin_conf.h"
#include "MySD.hpp"
#include "PMS5003.hpp"

namespace Peripherals {
extern RtcDS1307<TwoWire> Rtc;
extern PMS5003* pmsSensor;
extern SHT3X sht30;

// TFT display using TFT_eSPI and LVGL library
extern TFT_eSPI tft;
extern lv_disp_buf_t disp_buf;
extern lv_color_t buf[LV_HOR_RES_MAX * 10];

// SD Card and SQLite database objects declaration
extern MySD mySDCard;
extern SQLiteDb sampleDB;
}  // namespace Peripherals