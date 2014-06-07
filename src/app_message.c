#include <pebble.h>

#define NUM_MENU_SECTIONS 2
#define NUM_MENU_ICONS 3
#define NUM_FIRST_MENU_ITEMS 4
#define NUM_SECOND_MENU_ITEMS 3

static Window *window;

//Value Enum
enum {
	
  RED_VAL = 1,
  GREEN_VAL = 2,
  BLUE_VAL = 3,
  OFF_VAL = 4,
  NIGHT_VAL = 5,
  STROBE_VAL = 6,
  WHITE_VAL = 7 ,   
    
};

// Key values for AppMessage Dictionary
enum {
	STATUS_KEY = 0,	
	MESSAGE_KEY = 1
    
};


// Write message to buffer & send
void send_message(int message){
	DictionaryIterator *iter;
	
	app_message_outbox_begin(&iter);
	dict_write_uint8(iter, STATUS_KEY, 0x1);
	dict_write_uint8(iter, MESSAGE_KEY, message);

	dict_write_end(iter);
  	app_message_outbox_send();
}

// This is a menu layer
// You have more control than with a simple menu layer
static MenuLayer *menu_layer;

// Menu items can optionally have an icon drawn with them
//static GBitmap *menu_icons[NUM_MENU_ICONS];

static int current_icon = 0;

// You can draw arbitrary things in a menu item such as a background
//static GBitmap *menu_background;

// A callback is used to specify the amount of sections of menu items
// With this, you can dynamically add and remove sections
static uint16_t menu_get_num_sections_callback(MenuLayer *menu_layer, void *data) {
  return NUM_MENU_SECTIONS;
}

// Each section has a number of items;  we use a callback to specify this
// You can also dynamically add and remove items using this
static uint16_t menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  switch (section_index) {
    case 0:
      return NUM_FIRST_MENU_ITEMS;

    case 1:
      return NUM_SECOND_MENU_ITEMS;

    default:
      return 0;
  }
}

// A callback is used to specify the height of the section header
static int16_t menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  // This is a define provided in pebble.h that you may use for the default height
  return MENU_CELL_BASIC_HEADER_HEIGHT;
}

// Here we draw what each header is
static void menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data) {
  // Determine which section we're working with
  switch (section_index) {
    case 0:
      // Draw title text in the section header
      menu_cell_basic_header_draw(ctx, cell_layer, "Basic Colors");
      break;

    case 1:
      menu_cell_basic_header_draw(ctx, cell_layer, "Cool Stuff");
      break;
  }
}

// This is the menu item draw callback where you specify what each item should look like
static void menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
  // Determine which section we're going to draw in
  switch (cell_index->section) {
    case 0:
      // Use the row to specify which item we'll draw
      switch (cell_index->row) {
        case 0:
          // This is a basic menu item with a title and subtitle
          menu_cell_basic_draw(ctx, cell_layer, "Red", "Make lights red", NULL);
          break;

        case 1:
          // This is a basic menu icon with a cycling icon
           menu_cell_basic_draw(ctx, cell_layer, "Green", "Make lights Green", NULL);
          break;

        case 2:
          // Here we use the graphics context to draw something different
          // In this case, we show a strip of a watchface's background
                     menu_cell_basic_draw(ctx, cell_layer, "Blue", "Make lights Blue", NULL);
          break;
        
        case 3:
          // Here we use the graphics context to draw something different
          // In this case, we show a strip of a watchface's background
                     menu_cell_basic_draw(ctx, cell_layer, "White", "All of 'em ", NULL);
          break;
      }
      break;
  case 1: 
    switch (cell_index->row) {
        case 0:
          // This is a basic menu item with a title and subtitle
          menu_cell_basic_draw(ctx, cell_layer, "Off", "Make lights turn off", NULL);
          break;

        case 1:
          // This is a basic menu icon with a cycling icon
           menu_cell_basic_draw(ctx, cell_layer, "Night", "Dim Red", NULL);
          break;

        case 2:
          // Here we use the graphics context to draw something different
          // In this case, we show a strip of a watchface's background
                     menu_cell_basic_draw(ctx, cell_layer, "Stobe", "Party Time", NULL);
          break;
    }
  }
}

// Here we capture when a user selects a menu item
void menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
  // Use the row to specify which item will receive the select action
  switch (cell_index->section) { 
    case 0: 
      switch (cell_index->row) {
      // This is the menu item with the cycling icon
      case 0:
       send_message(RED_VAL);
        break;
      case 1: 
      send_message(GREEN_VAL);
      break; 
      case 2: 
      send_message(BLUE_VAL);
      break; 
      case 3: 
      send_message(WHITE_VAL);
      break; 
      }
    break;
  case 1: 
    switch (cell_index->row) {
      // This is the menu item with the cycling icon
      case 0:
       send_message(OFF_VAL);
        break;
      case 1: 
      send_message(NIGHT_VAL);
      break; 
      case 2: 
      send_message(STROBE_VAL);
      break; 
      }
    break; 
  }
  
  
  

}

// This initializes the menu upon window load
void window_load(Window *window) {
  // Here we load the bitmap assets
  // resource_init_current_app must be called before all asset loading
  //int num_menu_icons = 0;
  //menu_icons[num_menu_icons++] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_MENU_ICON_BIG_WATCH);
  //menu_icons[num_menu_icons++] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_MENU_ICON_SECTOR_WATCH);
  //menu_icons[num_menu_icons++] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_MENU_ICON_BINARY_WATCH);

  // And also load the background
  //menu_background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND_BRAINS);

  // Now we prepare to initialize the menu layer
  // We need the bounds to specify the menu layer's viewport size
  // In this case, it'll be the same as the window's
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);

  // Create the menu layer
  menu_layer = menu_layer_create(bounds);

  // Set all the callbacks for the menu layer
  menu_layer_set_callbacks(menu_layer, NULL, (MenuLayerCallbacks){
    .get_num_sections = menu_get_num_sections_callback,
    .get_num_rows = menu_get_num_rows_callback,
    .get_header_height = menu_get_header_height_callback,
    .draw_header = menu_draw_header_callback,
    .draw_row = menu_draw_row_callback,
    .select_click = menu_select_callback,
  });

  // Bind the menu layer's click config provider to the window for interactivity
  menu_layer_set_click_config_onto_window(menu_layer, window);

  // Add it to the window for display
  layer_add_child(window_layer, menu_layer_get_layer(menu_layer));
}

void window_unload(Window *window) {
  // Destroy the menu layer
  menu_layer_destroy(menu_layer);

  // Cleanup the menu icons
  //for (int i = 0; i < NUM_MENU_ICONS; i++) {
  //  gbitmap_destroy(menu_icons[i]);
 // }

  // And cleanup the background
 // gbitmap_destroy(menu_background);
}
	


// Called when a message is received from PebbleKitJS
static void in_received_handler(DictionaryIterator *received, void *context) {
	Tuple *tuple;
	
	tuple = dict_find(received, STATUS_KEY);
	if(tuple) {
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Received Status: %d", (int)tuple->value->uint32); 
	}
	
	tuple = dict_find(received, MESSAGE_KEY);
	if(tuple) {
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Received Message: %s", tuple->value->cstring);
	}}

// Called when an incoming message from PebbleKitJS is dropped
static void in_dropped_handler(AppMessageResult reason, void *context) {	
}

// Called when PebbleKitJS does not acknowledge receipt of a message
static void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {
}

void init(void) {
	window = window_create();
    // Setup the window handlers
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
	window_stack_push(window, true);
	
	// Register AppMessage handlers
	app_message_register_inbox_received(in_received_handler); 
	app_message_register_inbox_dropped(in_dropped_handler); 
	app_message_register_outbox_failed(out_failed_handler);
		
	app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
	
	//send_message(RED_VAL);
}

void deinit(void) {
	app_message_deregister_callbacks();
	window_destroy(window);
}

int main( void ) {
	init();
	app_event_loop();
	deinit();
}