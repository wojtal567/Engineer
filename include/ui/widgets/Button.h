#include <lvgl.h>

class Button
{
public:
    Button(lv_obj_t *par, const lv_obj_t *copy, int width, int height,
           int x_position, int y_position, lv_event_cb_t event_cb);
    operator lv_obj_t *() const;

private:
    lv_obj_t *button;
};