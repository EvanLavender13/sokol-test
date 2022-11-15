#include "stdio.h"

#include "sokol_app.h"
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
}

void event(const sapp_event *e) 
{
    app_event(&app, e);
}

sapp_desc sokol_main(int argc, char *argv[]) 
{
    return (sapp_desc) 
    {
        .init_cb = init,
        .frame_cb = frame,
        .cleanup_cb = cleanup,
        .event_cb = event,
        .width = 1200,
        .height = 800,
        .window_title = "sokol-test app"
    };
}