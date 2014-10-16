#include <pebble.h>

static Window *s_main_window;

static TextLayer *s_time_layer;
static TextLayer *s_initial_time_layer;
static TextLayer *s_reminder_layer;

// static time_t *static_time;
static time_t init_time;

static void set_and_display_initial_time() {
  init_time = time(NULL); // store time when watchface was launched
  // static_time = &init_time;
  
  static char initial_time_text[] = "00:00";
  struct tm *initial_time = localtime(&init_time);
  
  strftime(initial_time_text, sizeof("00:00"), "%I:%M", initial_time);
  
  text_layer_set_text(s_initial_time_layer, initial_time_text);
}

static void show_reminder_text() {
  text_layer_set_text_color(s_reminder_layer, GColorBlack);
}

static void hide_reminder_text() {
  text_layer_set_text_color(s_reminder_layer, GColorClear);
}

static void update_time() {
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  
  // create long-lived buffer to store current time
  static char buffer[] = "00:00";
  
  // write time into buffer
  if(clock_is_24h_style() == true) {
    strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
    //    which string - what size - which format - loc of time struct
  } else {
    strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
  }
  
  // display buffer on TextLayer
  text_layer_set_text(s_time_layer, buffer);
  double seconds = difftime(temp, init_time); 
  
  if (seconds > 61) {
    set_and_display_initial_time();
    show_reminder_text();
    vibes_double_pulse();
  } else {
    hide_reminder_text();
  } 
}

static void main_window_load(Window *window) {
  // create and configure time layer
  
  s_time_layer = text_layer_create(GRect(0, 10, 144, 50)); // point the pointer to the location of the newly-created TextLayer - modify in place
  
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  
  // create and configure initial time layer
  
  s_initial_time_layer = text_layer_create(GRect(0, 55, 144, 50));
  
  text_layer_set_background_color(s_initial_time_layer, GColorClear);
  text_layer_set_text_color(s_initial_time_layer, GColorBlack);
  
  text_layer_set_font(s_initial_time_layer, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
  text_layer_set_text_alignment(s_initial_time_layer, GTextAlignmentCenter);  // add s_time_layer as a child layer of window's root layer
  
  // create and configure reminder layer
   
  s_reminder_layer = text_layer_create(GRect(0, 85, 144, 50));
  
  text_layer_set_background_color(s_reminder_layer, GColorClear);
  text_layer_set_text_color(s_reminder_layer, GColorClear);
  
  text_layer_set_font(s_reminder_layer, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
  text_layer_set_text_alignment(s_reminder_layer, GTextAlignmentCenter);  // add s_time_layer as a child layer of window's root layer
  
  text_layer_set_text(s_reminder_layer, "REMINDER");
  
  // add text layers to window
  
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_initial_time_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_reminder_layer));
  
  
  set_and_display_initial_time();
} 

static void main_window_unload(Window *window) { // destroy what you created in main_window_load
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_initial_time_layer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void init() {
  // point global pointer to newly-created window 
  s_main_window = window_create(); // presumably this returns the address of the Window
                                   // if that's how pointers work
  
  // subscribe to TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  
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