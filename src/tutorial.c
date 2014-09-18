#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_time_layer;

static void main_window_load(Window *window) {
  s_time_layer = text_layer_create(GRect(0, 55, 144, 50)); // point the pointer to the location of the newly-created TextLayer - modify in place
  
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  
  text_layer_set_text(s_time_layer, "00:00");
  
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  
  // add s_time_layer as a child layer of window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
}

static void main_window_unload(Window *window) { // destroy what you created in main_window_load
  text_layer_destroy(s_time_layer);
}

static void init() {
  // point global pointer to newly-created window 
  s_main_window = window_create(); // presumably this returns the address of the Window
                                   // if that's how pointers work
  
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load, 
    .unload = main_window_unload
  }); // set window handlers - helper functions to do keep all the window loading/unloading out of main
  
  // makes the new window the visible window, with animated=true, because obviously
  window_stack_push(s_main_window, true); 
} 



static void deinit() {
  // memory management best practice
  // match every _create() call with the equivalent _destroy() call
  
  window_destroy(s_main_window); 
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}