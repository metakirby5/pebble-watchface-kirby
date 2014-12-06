#include <pebble.h>
  
static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_name_layer;

static void update_time() {
  
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  
  static char buffer[] = "00:00";
  
  strftime(buffer, sizeof("00:00"), clock_is_24h_style() ? "%H:%M" : "%I:%M", tick_time);
  
  text_layer_set_text(s_time_layer, buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  
  update_time();
}

static void main_window_load(Window *window) {
  
  s_time_layer = text_layer_create(GRect(0, 55, 144, 50));
  s_name_layer = text_layer_create(GRect(0, 110, 144, 50));
  text_layer_set_background_color(s_time_layer, GColorBlack);
  text_layer_set_background_color(s_name_layer, GColorBlack);
  text_layer_set_text_color(s_time_layer, GColorWhite);
  text_layer_set_text_color(s_name_layer, GColorWhite);
  text_layer_set_text(s_name_layer, "u r a fgt");
  
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_font(s_name_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  text_layer_set_text_alignment(s_name_layer, GTextAlignmentCenter);
  
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_name_layer));
}

static void main_window_unload(Window *window) {
  
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_name_layer);
}

static void init() {
  
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  
  s_main_window = window_create();
  
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  
  window_stack_push(s_main_window, true);
  update_time();
}

static void deinit() {
  
  window_destroy(s_main_window);
}

int main() {
  init();
  app_event_loop();
  deinit();
}