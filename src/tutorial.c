#include <pebble.h>

static Window *s_main_window;

static void main_window_load(Window *window) {
  
}

static void main_window_unload(Window *window) {
  
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