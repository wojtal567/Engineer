#include "globals/Peripherals.h"

// RTC, PMS5003, and SHT30 objects declaration

// TFT display using TFT_eSPI and LVGL library
lv_disp_buf_t disp_buf;
lv_color_t buf[LV_HOR_RES_MAX * 10];

// SD Card and SQLite database objects declaration

Config config = {"", "", 60000, 3600000, 30000, 5, 0, 30000};

namespace Peripherals {
TFT_eSPI tft = TFT_eSPI();
SHT3X sht30(0x44);  // TODO move address to config file
RtcDS1307<TwoWire> Rtc(Wire);
MySD mySDCard(PinConfig::sdCardPin);
SQLiteDb sampleDB("/sd/database.db", "/database.db", "samples");
PMS5003 *pmsSensor = new PMS5003(&Serial2, &Serial);
}  // namespace Peripherals
