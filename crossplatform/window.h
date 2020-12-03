#ifndef __CROSSPLATFORM_WINDOW_H__
#define __CROSSPLATFORM_WINDOW_H__
#include "../core/fc_common.h"

typedef struct window window_t;

// --window related -------------
window_t *create_window(const char* title, unsigned int width, unsigned int height);


void destroy_window(window_t* window_ptr);
void window_draw_image(window_t *window_ptr, TgaImage* image_ptr);
void window_draw_buffer(window_t *window_ptr, FcFrameBuffer* fb_ptr );
// event related ---------------
void input_poll_events();
void input_key_pressed();



#endif
