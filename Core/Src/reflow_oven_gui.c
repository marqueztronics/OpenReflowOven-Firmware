/*
 * reflow_oven_gui.c
 *
 *  Created on: Aug 5, 2024
 *      Author: Leonardo Marquez
 */

#include "reflow_oven_gui.h"
#include "stm32f4xx_hal.h"

extern enum states state;

lv_chart_series_t* ser1;
lv_obj_t* chart;
lv_obj_t* phase_label;
lv_obj_t* temp_label;
lv_obj_t* time_label;
lv_obj_t* profile_label;

LV_IMAGE_DECLARE(OpenReflowOven_Logo_Small);
LV_IMAGE_DECLARE(qrcode_url_hardware);
LV_FONT_DECLARE(lv_font_montserrat_20);

void reflow_oven_ui(void)
{
	/* Screens for the GUI */
	lv_obj_t* screen_splash = lv_obj_create(NULL);
    lv_obj_t* screen_home = lv_obj_create(NULL);
    lv_obj_t* screen_reflow = lv_obj_create(NULL);
    lv_obj_t* screen_about = lv_obj_create(NULL);


    /**** Screen Splash ****/

    // Logo
    lv_obj_t * logo = lv_img_create(screen_splash); /*Crate an image object*/
    lv_img_set_src(logo, &OpenReflowOven_Logo_Small);  /*Set the created file as image*/
    lv_obj_align(logo, LV_ALIGN_CENTER, 0, 0);
    lv_scr_load(screen_splash);

    // Hang for 3 seconds to show splash screen
    for(int i=0; i < 600; i++)
    {
    	lv_timer_handler();
    	lv_delay_ms(5);
    }

    /**** Screen Home ****/

    // Start Button
    lv_obj_t* start_label;

    lv_obj_t* start_btn = lv_button_create(screen_home);
    lv_obj_add_event_cb(start_btn, start_event_handler, LV_EVENT_ALL, screen_reflow);
    lv_obj_set_size(start_btn, 180, 40);
    lv_obj_align(start_btn, LV_ALIGN_CENTER, 0, -60);
    lv_obj_remove_flag(start_btn, LV_OBJ_FLAG_PRESS_LOCK);

    start_label = lv_label_create(start_btn);
    lv_label_set_text(start_label, "START");
    lv_obj_center(start_label);

    // Profiles Button
    lv_obj_t* profiles_label;

    lv_obj_t* profiles_btn = lv_button_create(screen_home);
    //lv_obj_add_event_cb(profiles_btn, start_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_set_size(profiles_btn, 180, 40);
    lv_obj_align(profiles_btn, LV_ALIGN_CENTER, 0, 0);
    lv_obj_remove_flag(profiles_btn, LV_OBJ_FLAG_PRESS_LOCK);

    profiles_label = lv_label_create(profiles_btn);
    lv_label_set_text(profiles_label, "PROFILES");
    lv_obj_center(profiles_label);

    // About Button
    lv_obj_t* about_label;

    lv_obj_t* about_btn = lv_button_create(screen_home);
    lv_obj_add_event_cb(about_btn, load_scr_event_handler, LV_EVENT_ALL, screen_about);
    lv_obj_set_size(about_btn, 180, 40);
    lv_obj_align(about_btn, LV_ALIGN_CENTER, 0, 60);
    lv_obj_remove_flag(about_btn, LV_OBJ_FLAG_PRESS_LOCK);

    about_label = lv_label_create(about_btn);
    lv_label_set_text(about_label, "ABOUT");
    lv_obj_center(about_label);

    lv_scr_load(screen_home);

    /**** Screen Reflow ****/

    /* Reflow Profile label */
    profile_label = lv_label_create(screen_reflow);
    lv_label_set_text(profile_label, "Reflow Profile Name");
    lv_obj_align(profile_label, LV_ALIGN_TOP_LEFT, 10, 10);

    /* Phase label */
    phase_label = lv_label_create(screen_reflow);
    lv_label_set_text(phase_label, "Phase: PREHEAT");
    lv_obj_align(phase_label, LV_ALIGN_TOP_LEFT, 10, 30);

    /* Temp label */
    temp_label = lv_label_create(screen_reflow);
    lv_label_set_text(temp_label, "-- C");
    lv_obj_align(temp_label, LV_ALIGN_TOP_RIGHT, -10, 10);

    /* Time label */
    time_label = lv_label_create(screen_reflow);
    lv_label_set_text(time_label, "-- s");
    lv_obj_align(time_label, LV_ALIGN_TOP_RIGHT, -10, 30);

    /*Create a chart*/
    chart = lv_chart_create(screen_reflow);
    lv_obj_set_size(chart, 205, 120);
    lv_obj_align(chart, LV_ALIGN_BOTTOM_LEFT, 35, -50);
    lv_chart_set_type(chart, LV_CHART_TYPE_SCATTER);

    // Reduce dot size in line chart
    lv_obj_set_style_size(chart, 0, 0, LV_PART_INDICATOR);

    lv_chart_set_point_count(chart, 480);
    lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_X, 0, 480);
    lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, 0, 300);

    /*horizontal scale*/
    lv_obj_t* scale_bottom = lv_scale_create(screen_reflow);
    lv_scale_set_mode(scale_bottom, LV_SCALE_MODE_HORIZONTAL_BOTTOM);
    lv_obj_set_size(scale_bottom, 180, 25);
    lv_obj_align_to(scale_bottom, chart, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    lv_scale_set_range(scale_bottom, 0, 480);
    lv_scale_set_total_tick_count(scale_bottom, 7);
    lv_scale_set_major_tick_every(scale_bottom, 2);
    lv_obj_set_style_pad_hor(scale_bottom, lv_chart_get_first_point_center_offset(chart), 0);

    /* Horizontal scale label (time)*/
    lv_obj_t* horizontal_label = lv_label_create(screen_reflow);
    lv_label_set_text(horizontal_label, "Time (s)");
    lv_obj_align_to(horizontal_label, scale_bottom, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

    /*Vertical scale*/
    lv_obj_t* scale_left = lv_scale_create(screen_reflow);
    lv_scale_set_mode(scale_left, LV_SCALE_MODE_VERTICAL_LEFT);
    lv_obj_set_size(scale_left, 25, 120);
    lv_obj_align_to(scale_left, chart, LV_ALIGN_OUT_LEFT_MID, 0, 0);
    lv_scale_set_range(scale_left, 0, 300);
    lv_scale_set_total_tick_count(scale_left, 7);
    lv_scale_set_major_tick_every(scale_left, 2);
    lv_obj_set_style_pad_ver(scale_left, lv_chart_get_first_point_center_offset(chart), 0);

    /* Vertical scale label (Temperature)*/
    lv_obj_t* vertical_label = lv_label_create(screen_reflow);
    lv_label_set_text(vertical_label, "Temp (C)");
    lv_obj_align_to(vertical_label, scale_left, LV_ALIGN_OUT_TOP_LEFT, -7, -5);

    /*Add two data series*/
    ser1 = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_GREEN), LV_CHART_AXIS_PRIMARY_Y);

    // Stop Button
    lv_obj_t* stop_label;

    lv_obj_t* stop_btn = lv_button_create(screen_reflow);
    lv_obj_add_event_cb(stop_btn, stop_event_handler, LV_EVENT_ALL, screen_home);
    lv_obj_align_to(stop_btn, chart, LV_ALIGN_OUT_RIGHT_MID, 10, -5);
    lv_obj_remove_flag(stop_btn, LV_OBJ_FLAG_PRESS_LOCK);

    stop_label = lv_label_create(stop_btn);
    lv_label_set_text(stop_label, "STOP");
    lv_obj_center(stop_label);


    /**** Screen About ****/

    /* Reflow Profile label */
    lv_obj_t* title_label = lv_label_create(screen_about);
    lv_obj_set_style_text_font(title_label, &lv_font_montserrat_20, LV_PART_MAIN);
    lv_label_set_text(title_label, "Open Reflow Oven");
    lv_obj_align(title_label, LV_ALIGN_TOP_MID, 0, 10);

    /* Version label */
    lv_obj_t* version_label = lv_label_create(screen_about);
    lv_label_set_text(version_label, "Version: 0.1.0");
    lv_obj_align(version_label, LV_ALIGN_TOP_MID, 0, 35);

    /* Open Source label */
    lv_obj_t* open_label = lv_label_create(screen_about);
    lv_label_set_text(open_label, "Source code:");
    lv_obj_align(open_label, LV_ALIGN_TOP_MID, 0, 55);

    // QR
    lv_obj_t * qr_hardware_url = lv_img_create(screen_about); /*Crate an image object*/
    lv_img_set_src(qr_hardware_url, &qrcode_url_hardware);  /*Set the created file as image*/
    lv_obj_align(qr_hardware_url, LV_ALIGN_TOP_MID, 0, 77);
    // Back Button
    lv_obj_t* back_label;

    lv_obj_t* back_btn = lv_button_create(screen_about);
    lv_obj_add_event_cb(back_btn, load_scr_event_handler, LV_EVENT_ALL, screen_home);
    lv_obj_align(back_btn, LV_ALIGN_BOTTOM_MID, 0, -15);
    lv_obj_remove_flag(back_btn, LV_OBJ_FLAG_PRESS_LOCK);

    back_label = lv_label_create(back_btn);
    lv_label_set_text(back_label, "BACK");
    lv_obj_center(back_label);


}

void start_event_handler(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t* usr_data_screen = lv_event_get_user_data(e);
    if (code == LV_EVENT_CLICKED) {
        lv_scr_load(usr_data_screen);
        state = PREHEAT;	// change state machine to pre-heat so reflow starts
    }
}

void stop_event_handler(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t* usr_data_screen = lv_event_get_user_data(e);
    if (code == LV_EVENT_CLICKED) {
        lv_scr_load(usr_data_screen);
        lv_chart_set_all_value(chart, ser1, LV_CHART_POINT_NONE);
        state = IDLE;	// change state machine to idle
    }
}

void load_scr_event_handler(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t* usr_data_screen = lv_event_get_user_data(e);
    if (code == LV_EVENT_CLICKED) {
        lv_scr_load(usr_data_screen);
    }
}

void update_chart(uint32_t x_value, uint32_t y_value)
{
    /*Set the next points on 'ser1'*/
    lv_chart_set_next_value2(chart, ser1,x_value, y_value);
}

