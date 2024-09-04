#include <lvgl.h>

class Label
{
public:
    Label(lv_obj_t *par, const lv_obj_t *copy, lv_coord_t x_position, lv_coord_t y_position,
          const char *text = "", lv_color_t color = LV_COLOR_WHITE);
    operator lv_obj_t *() const;

private:
    lv_obj_t *label;
};