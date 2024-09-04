#include "ui/widgets/Container.h"

Container::Container(lv_obj_t *par, const lv_obj_t *copy, lv_coord_t width,
                     lv_coord_t height, lv_coord_t x_position, lv_coord_t y_position)
{
    container = lv_cont_create(par, copy);
    lv_obj_set_size(container, width, height);
    lv_obj_set_pos(container, x_position, y_position);
}

Container::operator lv_obj_t *() const
{
    return container;
}
