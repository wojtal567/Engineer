#pragma once

#include <lvgl.h>

class Styles
{
public:
    static void initStyles();

    static lv_style_t borderlessStyle;
    static lv_style_t containerStyle;
    static lv_style_t transparentBackgroundStyle;

    static lv_style_t lineStyle;

    // Different font sizes using lvgl styles
    static lv_style_t font12Style;
    static lv_style_t font20Style;
    static lv_style_t font22Style;
    static lv_style_t whiteFontStyle;
};