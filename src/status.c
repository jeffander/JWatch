// Standard includes
#include "pebble.h"


// App-specific data
//Window *window; // All apps must have at least one window
//TextLayer *time_layer; 
TextLayer *battery_layer;
TextLayer *connection_layer;

static void updateBatteryStatus(BatteryChargeState charge_state) {
  static char battery_text[] = "100% charged";

  if (charge_state.is_charging) {
    snprintf(battery_text, sizeof(battery_text), "charging");
  } else {
    snprintf(battery_text, sizeof(battery_text), "%d%% charged", charge_state.charge_percent);
  }
  text_layer_set_text(battery_layer, battery_text);
}

static void updateBluetoothStatus(bool connected) {
  text_layer_set_text(connection_layer, connected ? "connected" : "disconnected");
}

/*
// Called once per second
static void handle_second_tick(struct tm* tick_time, TimeUnits units_changed) {
  static char time_text[] = "00:00:00"; // Needs to be static because it's used by the system later.

  strftime(time_text, sizeof(time_text), "%T", tick_time);
  text_layer_set_text(time_layer, time_text);

  handle_battery(battery_state_service_peek());
}
*/

static void initBatteryStatus(Layer *root_layer) {
  //Layer *root_layer = window_get_root_layer(window);   
  GRect frame = layer_get_frame(root_layer);  
  
  battery_layer = text_layer_create(GRect(0, 120, /* width */ frame.size.w, 34 /* height */));
  text_layer_set_text_color(battery_layer, GColorWhite);
  text_layer_set_background_color(battery_layer, GColorClear);
  text_layer_set_font(battery_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  text_layer_set_text_alignment(battery_layer, GTextAlignmentCenter);
  text_layer_set_text(battery_layer, "100% charged");
  
  battery_state_service_subscribe(&updateBatteryStatus); 
  layer_add_child(root_layer, text_layer_get_layer(battery_layer));  
}

static void initBluetoothStatus(Layer *root_layer) {
  //Layer *root_layer = window_get_root_layer(window);   
  GRect frame = layer_get_frame(root_layer);  
  
  connection_layer = text_layer_create(GRect(0, 90, /* width */ frame.size.w, 34 /* height */));
  text_layer_set_text_color(connection_layer, GColorWhite);
  text_layer_set_background_color(connection_layer, GColorClear);
  text_layer_set_font(connection_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  text_layer_set_text_alignment(connection_layer, GTextAlignmentCenter);
  updateBluetoothStatus(bluetooth_connection_service_peek());
  
  bluetooth_connection_service_subscribe(&updateBluetoothStatus); 
  layer_add_child(root_layer, text_layer_get_layer(connection_layer)); 
}

static void deinitBatteryStatus(void) {
  battery_state_service_unsubscribe();
  text_layer_destroy(battery_layer);  
}

static void deinitBluetoothStatus(void) {
  bluetooth_connection_service_unsubscribe();
  text_layer_destroy(connection_layer);
}
