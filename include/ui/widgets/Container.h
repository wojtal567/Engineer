#include <lvgl.h>

class Container
{
public:
    Container(lv_obj_t *par, const lv_obj_t *copy, lv_coord_t width, lv_coord_t height,
              lv_coord_t x_position, lv_coord_t y_position);
    operator lv_obj_t *() const;

private:
    lv_obj_t *container;
};