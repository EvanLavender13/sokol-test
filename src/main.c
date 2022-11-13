// #include "math.h"
// #include "stdio.h"
// #include "stdlib.h"

#include "sokol_time.h"

#include "app.h"


static App app = { 0 };

void init() 
{
    app_init(&app);
}

void frame() 
{
    app_draw(&app);
}

void cleanup() 
{
    app_cleanup();
    sg_shutdown();
}

void event(const sapp_event *e) 
{
    float mouse_down_pos[2] = { 0 };
    int win_x = e->mouse_x;
    int win_y = sapp_height() - 1 - e->mouse_y;

    switch (e->type)
    {
        case SAPP_EVENTTYPE_MOUSE_DOWN:
        {
            mouse_down_pos[0] = win_x;
            mouse_down_pos[1] = win_y;
            parcc_grab_begin(app.camera, win_x, win_y, e->mouse_button);
            break;
        }
        case SAPP_EVENTTYPE_MOUSE_UP:
        {
            parcc_grab_end(app.camera);
            break;
        }
        case SAPP_EVENTTYPE_MOUSE_MOVE:
        {
            parcc_grab_update(app.camera, win_x, win_y);
            break;
        }
        default:
        {
            break;
        }
    }

    __cdbgui_event(e);
}

sapp_desc sokol_main(int argc, char *argv[]) 
{
    return (sapp_desc) 
    {
        .init_cb = init,
        .frame_cb = frame,
        .cleanup_cb = cleanup,
        .event_cb = event,
        .width = 800,
        .height = 600,
        .window_title = "sokol-test app"
    };
}