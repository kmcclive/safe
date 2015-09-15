#include <pebble.h>
#include "loading_window.h"

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GFont s_res_gothic_28_bold;
static TextLayer *s_textlayer_loading;

static void initialise_ui(void) {
  s_window = window_create();
  #ifndef PBL_SDK_3
    window_set_fullscreen(s_window, 0);
  #endif
  
  s_res_gothic_28_bold = fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD);
  // s_textlayer_loading
  s_textlayer_loading = text_layer_create(GRect(22, 60, 100, 32));
  text_layer_set_text(s_textlayer_loading, "Loading...");
  text_layer_set_text_alignment(s_textlayer_loading, GTextAlignmentCenter);
  text_layer_set_font(s_textlayer_loading, s_res_gothic_28_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_loading);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  text_layer_destroy(s_textlayer_loading);
}
// END AUTO-GENERATED UI CODE

static void handle_window_unload(Window* window) {
  destroy_ui();
}

void show_loading_window(void) {
  initialise_ui();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  window_stack_push(s_window, true);
}

void hide_loading_window(void) {
  window_stack_remove(s_window, true);
}
