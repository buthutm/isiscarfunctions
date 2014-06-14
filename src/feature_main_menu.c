#include "pebble.h"

//ISIS Pebble App
//Michael Buthut

static AppSync sync;
static uint8_t sync_buffer[32];

enum ISIS_KEY {
  ISIS_CAR_FUNCTIONS = 0x0,         
  ISIS_NOTIFICATIONS = 0x1, 
};


static char *functions[] = {
	"WindowUp",
	"WindowDown",
	"WindowClose",
	"WindowOpen",
	"Flash"
};

static int current_function = 0;

static char *notifications[] = {
	"SmartAccessory"
};

static int current_notification = 0;

//Settings Screen *********************************************************************************
#define NUM_SETTINGS_MENU_SECTIONS 2
#define NUM_SETTINGS_MENU_ITEMS 3
	
Window* settings;

static SimpleMenuLayer *simple_menu_layer_settings;

static SimpleMenuSection settings_menu_sections[NUM_SETTINGS_MENU_SECTIONS];

static SimpleMenuItem settings_menu_items[NUM_SETTINGS_MENU_ITEMS];

//************************************************************************************************

static void sync_error_callback(DictionaryResult dict_error, AppMessageResult app_message_error, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message Sync Error: %d", app_message_error);
}

static void sync_tuple_changed_callback(const uint32_t key, const Tuple* new_tuple, const Tuple* old_tuple, void* context) {
 // switch (key) {
//    case ISIS_CAR_FUNCTIONS:
//	    APP_LOG(APP_LOG_LEVEL_DEBUG, "ISIS_CAR_FUNCTIONS");
 	
//		settings_menu_items[0].subtitle = (new_tuple->value->cstring);

//  		layer_mark_dirty(simple_menu_layer_get_layer(simple_menu_layer_settings));

 //     break;

//    case ISIS_NOTIFICATIONS:
//   	    APP_LOG(APP_LOG_LEVEL_DEBUG, "ISIS_NOTIFICATIONS");

   	    //Vibration and Show Text Message for a specific time
	  	
//		settings_menu_items[1].subtitle = (new_tuple->value->cstring);

//  		layer_mark_dirty(simple_menu_layer_get_layer(simple_menu_layer_settings));
//break
//      ;
//   }
	
}

static void setDictionaryWithClick(char* function, char* notification){
   Tuplet new_tuples[] = {
   TupletCString(ISIS_CAR_FUNCTIONS, function),
   TupletCString(ISIS_NOTIFICATIONS, notification),
 };
	
 APP_LOG(APP_LOG_LEVEL_DEBUG, "ISIS_CAR_FUNCTIONS: %s",function);
 APP_LOG(APP_LOG_LEVEL_DEBUG, "ISIS_NOTIFICATIONS: %s",notification);
	
 app_sync_set(&sync, new_tuples, ARRAY_LENGTH(new_tuples));
}


static void menu_select_callback_menu_item_3 (){
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Window Close clicked.");

  setDictionaryWithClick(functions[2],"");
 
}

static void menu_select_callback_menu_item_4 (){
  APP_LOG(APP_LOG_LEVEL_DEBUG, "WindowOpen clicked.");	
	
  setDictionaryWithClick(functions[3],"");

}

static void menu_select_callback_menu_item_5 (){
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Flash clicked.");

  setDictionaryWithClick(functions[4],"");

}

void settings_unload()
{
	app_sync_deinit(&sync);
	simple_menu_layer_destroy(simple_menu_layer_settings);
}


static void settings_load() {
	
	int num_a_items = 0;

  // This is an example of how you'd set a simple menu item
  //settings_menu_items[num_a_items++] = (SimpleMenuItem){
    // You should give each menu item a title and callback
  //  .title = "Window Up",
  //    .subtitle = "Click here",
  //    .callback = menu_select_callback_menu_item_1,
  //};
  // The menu items appear in the order saved in the menu items array
 // settings_menu_items[num_a_items++] = (SimpleMenuItem){
  //  .title = "Window Down",
   //   .subtitle = "Click here",
   //   .callback = menu_select_callback_menu_item_2,
 // };
  settings_menu_items[num_a_items++] = (SimpleMenuItem){
    .title = "Window Close",
      .subtitle = "Click here",
      .callback = menu_select_callback_menu_item_3,
  };
  settings_menu_items[num_a_items++] = (SimpleMenuItem){
    .title = "Window Open",
      .subtitle = "Click here",
      .callback = menu_select_callback_menu_item_4,
  };
  settings_menu_items[num_a_items++] = (SimpleMenuItem){
    .title = "Flash",
      .subtitle = "Click here",
      .callback = menu_select_callback_menu_item_5,
  };

  // Bind the menu items to the corresponding menu sections
  settings_menu_sections[0] = (SimpleMenuSection){
    .title = "ISIS Car Functions",
      .num_items = NUM_SETTINGS_MENU_ITEMS,
      .items = settings_menu_items,
  };

  Layer *window_layer = window_get_root_layer(settings);
  GRect bounds = layer_get_frame(window_layer);
	
  Tuplet initial_values[] = {
   TupletCString(ISIS_CAR_FUNCTIONS, ""),
   TupletCString(ISIS_NOTIFICATIONS, "")
 };
	
  app_sync_init(&sync, sync_buffer, sizeof(sync_buffer), initial_values, ARRAY_LENGTH(initial_values),
      sync_tuple_changed_callback, sync_error_callback, NULL);

  simple_menu_layer_settings = simple_menu_layer_create(bounds, settings, settings_menu_sections, NUM_SETTINGS_MENU_SECTIONS, NULL);

	
  layer_add_child(window_layer, simple_menu_layer_get_layer(simple_menu_layer_settings));
	
 // window_stack_push(settings, true); // The back button will dismiss the current window, not close the app.  So just press back to go back to the master view.

}

	
static void init() {
  settings = window_create();

  window_set_window_handlers(settings, (WindowHandlers) {
    .load = settings_load,
    .unload = settings_unload
  });

  const int inbound_size = 64;
  const int outbound_size = 64;
  app_message_open(inbound_size, outbound_size);

  const bool animated = true;
  window_stack_push(settings, animated);
}

static void deinit() {
  window_destroy(settings);
}


int main(void) {
  init();
	
  app_event_loop();
	
  deinit();
}

