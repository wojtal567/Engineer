#pragma once
#include <lvgl.h>

extern "C"
{
    extern lv_font_t monte16lock;
    extern lv_font_t hugeSymbolsFont48;
}

namespace Symbols
{
    constexpr const char *LOCK_SYMBOL = "\xEF\x80\xA3";
    constexpr const char *UNLOCK_SYMBOL = "\xEF\x82\x9C";

    constexpr const char *INFO_SYMBOL = "\xEF\x81\x9A";
    constexpr const char *CLOCK_SYMBOL = "\xEF\x80\x97";
    constexpr const char *WIFI_SYMBOL = "\xEF\x87\xAB";
    constexpr const char *COGS_SYMBOL = "\xEF\x82\x85";
}