#include "ui/widgets/Button.h"
Button::Button(lv_obj_t *par, const lv_obj_t *copy, int width, int height, int x_position, int y_position,
               lv_event_cb_t callback) {
    button = lv_btn_create(par, copy);
    lv_obj_set_size(button, width, height);
    lv_obj_set_pos(button, x_position, y_position);
    lv_obj_set_event_cb(button, callback);
}

Button::operator lv_obj_t *() const { return button; }