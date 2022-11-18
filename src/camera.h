#ifndef LAVS_CAMERA_H_
#define LAVS_CAMERA_H_

#include "par/par_camera_control.h"
#include "sokol_app.h"
#include "cimgui/cimgui.h"
#include "sokol_imgui.h"

void camera_input(parcc_context *camera, const sapp_event *event)
{
    static float mouse_down_pos[2] = { 0 };
    int win_x = event->mouse_x;
    int win_y = sapp_height() - 1 - event->mouse_y;

    switch (event->type)
    {
        case SAPP_EVENTTYPE_RESIZED:
        {
            parcc_properties camera_props;
            parcc_get_properties(camera, &camera_props);
            camera_props.viewport_width = sapp_width();
            camera_props.viewport_height = sapp_height();
            parcc_set_properties(camera, &camera_props);
            break;
        }
        case SAPP_EVENTTYPE_MOUSE_DOWN:
        {
            mouse_down_pos[0] = win_x;
            mouse_down_pos[1] = win_y;
            parcc_grab_begin(camera, win_x, win_y, event->mouse_button);
            break;
        }
        case SAPP_EVENTTYPE_MOUSE_UP:
        {
            parcc_grab_end(camera);
            if (win_x == mouse_down_pos[0] && win_y == mouse_down_pos[1])
            {
                printf("Clicked [%d, %d]\n", win_x, win_y);
            }
            break;
        }
        case SAPP_EVENTTYPE_MOUSE_MOVE:
        {
            parcc_grab_update(camera, win_x, win_y);
            break;
        }
        case SAPP_EVENTTYPE_MOUSE_SCROLL:
        {
            parcc_zoom(camera, win_x, win_y, event->scroll_y);
            break;
        }
        default:
        {
            break;
        }
    }        
}

void camera_debug_gui(parcc_context *camera)
{
    // camera controls
    parcc_properties camera_props;
    parcc_get_properties(camera, &camera_props);

    igSetNextWindowSize((ImVec2) { 450, 320 }, ImGuiCond_FirstUseEver);
    igBegin("Camera Controls", NULL, 0);

    igText("Camera Properties");
    char *fov_elems[2] = { "vertical", "horizontal" };
    igSliderInt("fov_orientation", (int*) &camera_props.fov_orientation, 0, 1, fov_elems[camera_props.fov_orientation], ImGuiSliderFlags_None);
    float fov_radians = HMM_ToRadians(camera_props.fov_degrees);
    igSliderAngle("fov_degrees", &fov_radians, 0.0f, 360.0f, "%.1f", ImGuiScrollFlags_None);
    igSliderFloat3("home_vector", camera_props.home_vector, -15.0f, 15.0f, "%.1f", ImGuiScrollFlags_None);
    igSliderFloat2("orbit_speed", camera_props.orbit_speed, 0.0f, 0.025f, "%.3f", ImGuiSliderFlags_None);
    igSliderFloat("orbit_zoom_speed", &camera_props.orbit_zoom_speed, 0.0f, 1.0f, "%.3f", ImGuiSliderFlags_None);
    igSliderFloat2("orbit_strafe_speed", camera_props.orbit_strafe_speed, 0.0f, 0.025f, "%.3f", ImGuiSliderFlags_None);

    camera_props.fov_degrees = fov_radians * (180.0f / HMM_PI32);
    parcc_set_properties(camera, &camera_props);

    igText("Camera Frame Properties");
    parcc_frame camera_frame = parcc_get_current_frame(camera);
    igSliderFloat("frame.phi", &camera_frame.phi, 0.0f, HMM_PI32 / 2.0f, "%.3f", ImGuiSliderFlags_None);
    igSliderFloat("frame.theta", &camera_frame.theta, -HMM_PI32, HMM_PI32, "%.3f", ImGuiSliderFlags_None);
    igSliderFloat("frame.pivot_distance", &camera_frame.pivot_distance, 0.1f, 100.0f, "%.1f", ImGuiSliderFlags_None);
    igSliderFloat3("frame.pivot", camera_frame.pivot, -15.0f, 15.0f, "%.1f", ImGuiScrollFlags_None);

    parcc_goto_frame(camera, camera_frame);
    if (igButton("Home", (ImVec2) { 0.0f, 0.0f }))
    {
        parcc_goto_frame(camera, parcc_get_home_frame(camera));
    }

    igEnd();
}

#endif