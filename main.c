#include "lvgl/lvgl.h"

#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "./ui/ui.h"

extern const lv_pinyin_dict_t my_lv_ime_pinyin_dict[];

static void lv_linux_disp_init(void)
{
    lv_display_t * disp = lv_linux_fbdev_create();
    lv_linux_fbdev_set_file(disp, "/dev/fb0");
}

static void apply_chinese_font(lv_obj_t * root, int font_size)
{
    static bool initialized = false;
    static lv_style_t style;
    static lv_font_t * font = NULL;

    if(initialized || root == NULL) {
        return;
    }

    font = lv_freetype_font_create("./simkai.ttf",
                                   LV_FREETYPE_FONT_RENDER_MODE_BITMAP,
                                   font_size,
                                   LV_FREETYPE_FONT_STYLE_NORMAL);
    if(font == NULL) {
        font = lv_freetype_font_create("/simkai.ttf",
                                       LV_FREETYPE_FONT_RENDER_MODE_BITMAP,
                                       font_size,
                                       LV_FREETYPE_FONT_STYLE_NORMAL);
    }

    if(font != NULL) {
        lv_style_init(&style);
        lv_style_set_text_font(&style, font);
        lv_obj_add_style(root, &style, 0);
    }

    initialized = true;
}

void init_pinyin(lv_obj_t * keyboard)
{
    lv_obj_t * pinyin_ime = lv_ime_pinyin_create(lv_screen_active());
    lv_ime_pinyin_set_dict(pinyin_ime, (lv_pinyin_dict_t *)my_lv_ime_pinyin_dict);
    lv_ime_pinyin_set_keyboard(pinyin_ime, keyboard);

    lv_obj_t * cand_panel = lv_ime_pinyin_get_cand_panel(pinyin_ime);
    lv_obj_set_size(cand_panel, LV_PCT(100), LV_PCT(10));
    lv_obj_align_to(cand_panel, keyboard, LV_ALIGN_OUT_TOP_MID, 0, 0);
}

int main(void)
{
    lv_init();
    lv_linux_disp_init();

    lv_indev_t * touch = lv_evdev_create(LV_INDEV_TYPE_POINTER, "/dev/input/event0");
    lv_evdev_set_calibration(touch, 0, 0, 800, 480);

    ui_init();
    apply_chinese_font(ui_Screen1, 24);

    while(1) {
        assistant_ui_poll();
        lv_timer_handler();
        usleep(5000);
    }

    return 0;
}
