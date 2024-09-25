#include "ui/Styles.h"

#include "ui/Symbols.h"

lv_style_t Styles::borderlessStyle;
lv_style_t Styles::containerStyle;
lv_style_t Styles::transparentBackgroundStyle;
lv_style_t Styles::lineStyle;
lv_style_t Styles::whiteButtonStyle;

lv_style_t Styles::font12Style;
lv_style_t Styles::font20Style;
lv_style_t Styles::font22Style;
lv_style_t Styles::whiteFontStyle;
lv_style_t Styles::hugeFontStyle;

void Styles::initStyles() {
    lv_style_init(&transparentBackgroundStyle);
    lv_style_set_bg_opa(&transparentBackgroundStyle, LV_STATE_DEFAULT, LV_OPA_0);

    lv_style_init(&borderlessStyle);
    lv_style_set_border_width(&borderlessStyle, LV_STATE_DEFAULT, 0);
    lv_style_set_outline_width(&borderlessStyle, LV_STATE_DEFAULT, 0);

    lv_style_init(&containerStyle);
    lv_style_set_bg_opa(&containerStyle, LV_STATE_DEFAULT, LV_OPA_0);
    lv_style_set_border_color(&containerStyle, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_radius(&containerStyle, LV_STATE_DEFAULT, 0);

    lv_style_init(&lineStyle);
    lv_style_set_line_width(&lineStyle, LV_STATE_DEFAULT, 2);
    lv_style_set_line_color(&lineStyle, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_line_rounded(&lineStyle, LV_STATE_DEFAULT, false);

    lv_style_init(&font12Style);
    lv_style_set_text_font(&font12Style, LV_STATE_DEFAULT, &lv_font_montserrat_12);

    lv_style_init(&font20Style);
    lv_style_set_text_font(&font20Style, LV_STATE_DEFAULT, &lv_font_montserrat_20);

    lv_style_init(&font22Style);
    lv_style_set_text_font(&font22Style, LV_STATE_DEFAULT, &lv_font_montserrat_22);

    lv_style_init(&whiteFontStyle);
    lv_style_set_text_color(&whiteFontStyle, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    lv_style_init(&hugeFontStyle);
    lv_style_set_text_font(&hugeFontStyle, LV_STATE_DEFAULT, &hugeSymbolsFont48);

    lv_style_init(&whiteButtonStyle);
    lv_style_set_bg_color(&whiteButtonStyle, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_radius(&whiteButtonStyle, LV_STATE_DEFAULT, 10);
    lv_style_set_border_width(&whiteButtonStyle, LV_STATE_DEFAULT, 0);
    lv_style_set_outline_width(&whiteButtonStyle, LV_STATE_DEFAULT, 0);
    lv_style_set_text_color(&whiteButtonStyle, LV_STATE_DEFAULT, LV_COLOR_BLACK);
}