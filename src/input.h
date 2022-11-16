#ifndef LAVS_INPUT_H_
#define LAVS_INPUT_H_

#include "sokol_app.h"

#include "camera.h"

void input_handle(parcc_context *camera, const sapp_event *event)
{
    camera_input(camera, event);

    
}

#endif