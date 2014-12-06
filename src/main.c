#include <pebble.h>
 
#include "kirbies.h"
  
static Window *s_main_window;
static TextLayer *s_time_layer;
static GFont s_time_font;
static BitmapLayer *s_kirby_layer;
static GBitmap *s_kirby;

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
  
  // Time
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_EMULOGIC_24));
  
  s_time_layer = text_layer_create(GRect(-4, 138, 148, 30));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  
  text_layer_set_font(s_time_layer, s_time_font);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  
  // Kirby
  s_kirby = gbitmap_create_with_resource(RESOURCE_ID_KIRBY_NORMAL);
  s_kirby_layer = bitmap_layer_create(GRect(24, 10, 96, 120));
  bitmap_layer_set_bitmap(s_kirby_layer, s_kirby);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_kirby_layer));
}

static void main_window_unload(Window *window) {
  
  text_layer_destroy(s_time_layer);
  fonts_unload_custom_font(s_time_font);
  
  bitmap_layer_destroy(s_kirby_layer);
  gbitmap_destroy(s_kirby);
}

static void init() {
  
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  
  s_main_window = window_create();
  window_set_background_color(s_main_window, GColorWhite);
  
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