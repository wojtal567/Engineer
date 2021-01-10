#include "lvgl.h"

/*******************************************************************************
 * Size: 16 px
 * Bpp: 2
 * Opts: 
 ******************************************************************************/

#ifndef MONTE16LOCK
#define MONTE16LOCK 1
#endif

#if MONTE16LOCK

/*-----------------
 *    BITMAPS
 *----------------*/



/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t gylph_bitmap[] = {
    /* U+F023 "" */
    0x0, 0x6f, 0x90, 0x0, 0x1f, 0xff, 0x40, 0x3,
    0xfa, 0xfc, 0x0, 0xbc, 0x3, 0xe0, 0xb, 0xc0,
    0x3e, 0x0, 0xfc, 0x3, 0xf0, 0xf, 0xc0, 0x3f,
    0xb, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xfb, 0xff, 0xff, 0xfe,

    /* U+F09C "" */
    0x0, 0x6f, 0x90, 0x0, 0x1f, 0xff, 0x40, 0x3,
    0xfa, 0xfc, 0x0, 0xbc, 0x3, 0xe0, 0xb, 0xc0,
    0x3e, 0x0, 0xfc, 0x2, 0xe0, 0xf, 0xc0, 0x0,
    0x0, 0xfc, 0x0, 0x0, 0xbf, 0xff, 0xff, 0xef,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xfb, 0xff, 0xff, 0xfe
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 224, .box_w = 14, .box_h = 16, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 56, .adv_w = 224, .box_w = 14, .box_h = 16, .ofs_x = 0, .ofs_y = -2}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint16_t unicode_list_0[] = {
    0x0, 0x79
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 61475, .range_length = 122, .glyph_id_start = 1,
        .unicode_list = unicode_list_0, .glyph_id_ofs_list = NULL, .list_length = 2, .type = LV_FONT_FMT_TXT_CMAP_SPARSE_TINY
    }
};



/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

/*Store all the custom data of the font*/
static lv_font_fmt_txt_dsc_t font_dsc = {
    .glyph_bitmap = gylph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 1,
    .bpp = 2,
    .kern_classes = 0,
    .bitmap_format = 0
};


/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
lv_font_t monte16lock = {
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 16,          /*The maximum line height required by the font*/
    .base_line = 2,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0)
    .underline_position = -6,
    .underline_thickness = 1,
#endif
    .dsc = &font_dsc           /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
};



#endif /*#if MONTE16LOCK*/
