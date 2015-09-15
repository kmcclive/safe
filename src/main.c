#include <pebble.h>
#include "location_window.h"
  
int main() {
  show_location_window();
  app_event_loop();
  return 0;
}
