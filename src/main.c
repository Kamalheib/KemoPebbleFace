#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_center_layer;

static void update_time() {
    time_t temp = time(NULL);
    struct tm *tick_time = localtime(&temp);
    
    static char s_buffer[8];
    strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ? "%H:%M" : "%I:%M", tick_time);
    
    text_layer_set_text(s_center_layer, s_buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
    update_time();
}

static void main_window_load(Window *window) {
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);
    
    s_center_layer = text_layer_create(GRect(0, 60, bounds.size.w, 45));
    text_layer_set_font(s_center_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
    text_layer_set_text_alignment(s_center_layer, GTextAlignmentCenter);
    layer_add_child(window_layer, text_layer_get_layer(s_center_layer));
}

static void main_window_unload(Window *window) {
    text_layer_destroy(s_center_layer);
}

static void init() {

    s_main_window = window_create();
    window_set_window_handlers(s_main_window, (WindowHandlers) {
        .load = main_window_load,
        .unload = main_window_unload
    });
    
    tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
    window_stack_push(s_main_window, true);
}

static void deinit(){
    window_destroy(s_main_window);
}

int main(void){
    init();
    app_event_loop();
    deinit();
}