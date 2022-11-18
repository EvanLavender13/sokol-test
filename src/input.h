#ifndef LAVS_INPUT_H_
#define LAVS_INPUT_H_

#include "sokol_app.h"

#include "camera.h"

void input_handle(parcc_context *camera, const sapp_event *event)
{
    camera_input(camera, event);

    // [0] = forward/backward [1] = left/right
    float move_direction[2] = { 0 };
    if (event->type == SAPP_EVENTTYPE_KEY_DOWN)
    {
        switch (event->key_code)
        {
            case SAPP_KEYCODE_W:
            {
                move_direction[0] = 1.0f;
                break;
            }
            case SAPP_KEYCODE_S:
            {
                move_direction[0] = -1.0f;
                break;
            }
            case SAPP_KEYCODE_A:
            {
                move_direction[1] = -1.0f;
                break;
            }
            case SAPP_KEYCODE_D:
            {
                move_direction[1] = 1.0f;
                break;
            }
            default:
            {
                break;
            }
        }

        printf("move_direction=[%f, %f]\n", move_direction[0], move_direction[1]);
    }
}

#endif