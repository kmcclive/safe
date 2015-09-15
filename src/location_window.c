#include <pebble.h>
#include "location_window.h"
#include "loading_window.h"

typedef enum {
  KEY_STATUS = 0,
  KEY_CITY = 1,
  KEY_STREET = 2
} key_type;

typedef enum {
  STATUS_OFF = 2,
  STATUS_HOME = 4,
  STATUS_AWAY = 5
} status_type;

typedef enum {
  ICON_MENU,
  ICON_OFF,
  ICON_HOME,
  ICON_AWAY
} icon_type;

static bool is_loaded = false;
static icon_type s_top_button_icon = ICON_HOME;
static icon_type s_middle_button_icon = ICON_MENU;
static icon_type s_bottom_button_icon = ICON_AWAY;
static status_type s_status = STATUS_OFF;
static GBitmap *s_res_image_off;

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GBitmap *s_res_image_home;
static GBitmap *s_res_image_menu;
static GBitmap *s_res_image_away;
static GFont s_res_gothic_18_bold;
static GFont s_res_bitham_30_black;
static GFont s_res_gothic_14;
static ActionBarLayer *s_actionbarlayer;
static TextLayer *s_textlayer_city;
static TextLayer *s_textlayer_status;
static TextLayer *s_textlayer_street;

static void initialise_ui(void) {
  s_window = window_create();
  #ifndef PBL_SDK_3
    window_set_fullscreen(s_window, true);
  #endif
  
  s_res_image_home = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_HOME);
  s_res_image_menu = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_MENU);
  s_res_image_away = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_AWAY);
  s_res_gothic_18_bold = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);
  s_res_bitham_30_black = fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK);
  s_res_gothic_14 = fonts_get_system_font(FONT_KEY_GOTHIC_14);
  // s_actionbarlayer
  s_actionbarlayer = action_bar_layer_create();
  action_bar_layer_add_to_window(s_actionbarlayer, s_window);
  action_bar_layer_set_background_color(s_actionbarlayer, GColorBlack);
  action_bar_layer_set_icon(s_actionbarlayer, BUTTON_ID_UP, s_res_image_home);
  action_bar_layer_set_icon(s_actionbarlayer, BUTTON_ID_SELECT, s_res_image_menu);
  action_bar_layer_set_icon(s_actionbarlayer, BUTTON_ID_DOWN, s_res_image_away);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_actionbarlayer);
  
  // s_textlayer_city
  s_textlayer_city = text_layer_create(GRect(0, 0, 120, 20));
  text_layer_set_background_color(s_textlayer_city, GColorBlack);
  text_layer_set_text_color(s_textlayer_city, GColorWhite);
  text_layer_set_text(s_textlayer_city, "City");
  text_layer_set_text_alignment(s_textlayer_city, GTextAlignmentCenter);
  text_layer_set_font(s_textlayer_city, s_res_gothic_18_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_city);
  
  // s_textlayer_status
  s_textlayer_status = text_layer_create(GRect(0, 65, 120, 35));
  text_layer_set_text(s_textlayer_status, "Off");
  text_layer_set_text_alignment(s_textlayer_status, GTextAlignmentCenter);
  text_layer_set_font(s_textlayer_status, s_res_bitham_30_black);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_status);
  
  // s_textlayer_street
  s_textlayer_street = text_layer_create(GRect(0, 152, 120, 16));
  text_layer_set_background_color(s_textlayer_street, GColorBlack);
  text_layer_set_text_color(s_textlayer_street, GColorWhite);
  text_layer_set_text(s_textlayer_street, "12345 Easy St.");
  text_layer_set_text_alignment(s_textlayer_street, GTextAlignmentCenter);
  text_layer_set_font(s_textlayer_street, s_res_gothic_14);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_street);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  action_bar_layer_destroy(s_actionbarlayer);
  text_layer_destroy(s_textlayer_city);
  text_layer_destroy(s_textlayer_status);
  text_layer_destroy(s_textlayer_street);
  gbitmap_destroy(s_res_image_home);
  gbitmap_destroy(s_res_image_menu);
  gbitmap_destroy(s_res_image_away);
}
// END AUTO-GENERATED UI CODE

status_type get_next_status(status_type status) {
  switch (status) {
    case STATUS_OFF:
      return STATUS_HOME;
    case STATUS_HOME:
      return STATUS_AWAY;
    case STATUS_AWAY:
    default:
      return STATUS_OFF;
  }
}

status_type get_status_for_icon(icon_type icon) {
  switch (icon) {
    case ICON_OFF:
      return STATUS_OFF;
    case ICON_HOME:
      return STATUS_HOME;
    case ICON_AWAY:
      return STATUS_AWAY;
    default:
      return STATUS_OFF;
  }
}

icon_type get_icon_for_status(status_type status) {
  switch (status) {
    case STATUS_OFF:
      return ICON_OFF;
    case STATUS_HOME:
      return ICON_HOME;
    case STATUS_AWAY:
      return ICON_AWAY;
    default:
      return ICON_MENU;
  }
}

icon_type get_icon(int button_id) {
  switch (button_id) {
    case BUTTON_ID_UP:
      return s_top_button_icon;
    case BUTTON_ID_SELECT:
      return s_middle_button_icon;
    case BUTTON_ID_DOWN:
      return s_bottom_button_icon;
    default:
      return ICON_MENU;
  }
}

void set_icon(int button_id, icon_type icon){
  GBitmap *image;
  
  switch(button_id) {
    case BUTTON_ID_UP:
      s_top_button_icon = icon;
      break;
    case BUTTON_ID_SELECT:
      s_middle_button_icon = icon;
      break;
    case BUTTON_ID_DOWN:
      s_bottom_button_icon = icon;
      break;
  }
  
  switch (icon) {
    case ICON_OFF:
      image = s_res_image_off;
      break;
    case ICON_HOME:
      image = s_res_image_home;
      break;
    case ICON_AWAY:
      image = s_res_image_away;
      break;
    case ICON_MENU:
    default:
      image = s_res_image_menu;
      break;
  }
  
  action_bar_layer_set_icon(s_actionbarlayer, button_id, image);
}

const char * get_city() {
  return text_layer_get_text(s_textlayer_city);
}

void set_city(const char * city) {
  text_layer_set_text(s_textlayer_city, city);
}

const char * get_street() {
  return text_layer_get_text(s_textlayer_street);
}

void set_street(const char * street) {
  text_layer_set_text(s_textlayer_street, street);
}

status_type get_status() {
  return s_status;
}

void set_status(status_type status) {
  s_status = status;
  
  switch (status) {
    case STATUS_OFF:
      text_layer_set_text(s_textlayer_status, "Off");
      break;
    case STATUS_HOME:
      text_layer_set_text(s_textlayer_status, "Home");
      break;
    case STATUS_AWAY:
      text_layer_set_text(s_textlayer_status, "Away");
      break;
  }
  
  status_type next_status = get_next_status(status);
  set_icon(BUTTON_ID_UP, get_icon_for_status(next_status));
  
  next_status = get_next_status(next_status);
  set_icon(BUTTON_ID_DOWN, get_icon_for_status(next_status));
}

void send_status(status_type status) {
  // Begin dictionary
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);

  // Add a key-value pair
  dict_write_uint8(iter, KEY_STATUS, (int)status);

  // Send the message!
  app_message_outbox_send();
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  icon_type icon = get_icon(BUTTON_ID_UP);
  status_type status = get_status_for_icon(icon);
  send_status(status);
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {

}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  icon_type icon = get_icon(BUTTON_ID_DOWN);
  status_type status = get_status_for_icon(icon);
  send_status(status);
}

static void click_config_provider(void *context) {
  // Register the ClickHandlers
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  // Read first item
  Tuple *t = dict_read_first(iterator);

  // For all items
  while(t != NULL) {
    // Which key was received?
    switch(t->key) {
    case KEY_STATUS:
      set_status((int)t->value->int32);
      break;
    case KEY_CITY:
      set_city(t->value->cstring);
      break;
    case KEY_STREET:
      set_street(t->value->cstring);
      break;
    default:
      APP_LOG(APP_LOG_LEVEL_ERROR, "Key %d not recognized!", (int)t->key);
      break;
    }

    // Look for next item
    t = dict_read_next(iterator);
  }
  
  if (!is_loaded) {
    is_loaded = true;
    hide_loading_window();
    window_stack_push(s_window, true);
  }
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

static void handle_window_unload(Window* window) {
  destroy_ui();
  
  gbitmap_destroy(s_res_image_off);
}

void show_location_window(void) {
  show_loading_window();
  
  initialise_ui();
  
  s_res_image_off = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_OFF);
  
  window_set_click_config_provider(s_window, click_config_provider);
  
  // Register callbacks
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  
  // Open AppMessage
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
  
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
}

void hide_location_window(void) {
  window_stack_remove(s_window, true);
}
