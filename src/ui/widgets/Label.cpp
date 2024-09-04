#include "ui/widgets/Label.h"

Label::Label(lv_obj_t *par, const lv_obj_t *copy, lv_coord_t x_position, lv_coord_t y_position,
             const char *text, lv_color_t color)
{
    label = lv_label_create(par, copy);
    lv_obj_set_pos(label, x_position, y_position);
    lv_label_set_text(label, text);
    lv_obj_set_style_local_text_color(label, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, color);
}

Label::operator lv_obj_t *() const
{
    return label;
}