#include "stdio.h"
#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include "sokol_app.h"
#include "app.h"

#define UNUSED(x) (void)(x)

static App app = { 0 };

void init() 
{
    app_init(&app);
}

void frame() 
{
    app_update(&app);
}

void cleanup() 
{
    app_cleanup(&app);
}

void event(const sapp_event *e) 
{
    app_event(&app, e);
}

sapp_desc sokol_main(int argc, char *argv[]) 
{
    UNUSED(argc);
    UNUSED(argv);
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