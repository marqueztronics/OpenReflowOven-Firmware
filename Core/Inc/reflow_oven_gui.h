/*
 * reflow_oven_gui.h
 *
 *  Created on: Aug 5, 2024
 *      Author: leona
 */

#ifndef INC_REFLOW_OVEN_GUI_H_
#define INC_REFLOW_OVEN_GUI_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "../../Drivers/lvgl/lvgl.h"

void reflow_oven_ui(void);
void start_event_handler(lv_event_t* e);
void stop_event_handler(lv_event_t* e);
void load_scr_event_handler(lv_event_t* e);
void update_chart(uint32_t x_value, uint32_t y_value);
void previous_profile_event_handler(lv_event_t* e);
void next_profile_event_handler(lv_event_t* e);
void select_profile_event_handler(lv_event_t* e);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /* INC_REFLOW_OVEN_GUI_H_ */
