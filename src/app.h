#ifndef LAVS_APP_H_
#define LAVS_APP_H_

#include "HandmadeMath.h"
#include "par/par_camera_control.h"
#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_glue.h"
#include "sokol_time.h"

#include "cdbgui/cdbgui.h"

#include "../res/test-shader.glsl.h"

#define NUM_CUBES 10

typedef struct
{
    sg_pass_action pass_action;
    sg_pipeline pipeline;
    sg_bindings bindings;

    hmm_vec3 cube_positions[NUM_CUBES];    
} GraphicsState;

typedef struct app
{
    GraphicsState graphics;
    parcc_context *camera;

    uint64_t last_time;
    uint64_t delta_time;      
} App;

typedef struct {
    float x, y, z;
    float r, g, b, a;
} vertex_t;

void app_init(App *app)
{
    sg_setup(&(sg_desc)
    {
        .context = sapp_sgcontext()
    });

    stm_setup();

    __cdbgui_setup(sapp_sample_count());

    const parcc_properties camera_props =
    {
        .mode = PARCC_ORBIT,
        .viewport_width = sapp_width(),
        .viewport_height = sapp_height(),
        .near_plane = 0.001f,
        .far_plane = 100.0f,
        .fov_orientation = PARCC_HORIZONTAL,
        
        .home_vector = { 0.0f, 0.0f, 5.0f }
    };

    app->camera = parcc_create_context(&camera_props);

    app->graphics.cube_positions[0] = HMM_Vec3( 0.0f,  0.0f,  0.0f);
    app->graphics.cube_positions[1] = HMM_Vec3( 2.0f,  5.0f, -15.0f);
    app->graphics.cube_positions[2] = HMM_Vec3(-1.5f, -2.2f, -2.5f);
    app->graphics.cube_positions[3] = HMM_Vec3(-3.8f, -2.0f, -12.3f);
    app->graphics.cube_positions[4] = HMM_Vec3( 2.4f, -0.4f, -3.5f);
    app->graphics.cube_positions[5] = HMM_Vec3(-1.7f,  3.0f, -7.5f);
    app->graphics.cube_positions[6] = HMM_Vec3( 1.3f, -2.0f, -2.5f);
    app->graphics.cube_positions[7] = HMM_Vec3( 1.5f,  2.0f, -2.5f);
    app->graphics.cube_positions[8] = HMM_Vec3( 1.5f,  0.2f, -1.5f);
    app->graphics.cube_positions[9] = HMM_Vec3(-1.3f,  1.0f, -1.5f);

    vertex_t vertices[] =
    {
        /* pos                  color                   */
        { -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 1.0f },
        {  0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 1.0f },
        {  0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 1.0f },
        { -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 1.0f },

        { -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f },
        {  0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f },
        {  0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f },
        { -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f },

        { -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 1.0f },
        { -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 1.0f },
        { -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 1.0f },
        { -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 1.0f },

        {  0.5f, -0.5f, -0.5f,  1.0f, 0.5f, 0.0f, 1.0f },
        {  0.5f,  0.5f, -0.5f,  1.0f, 0.5f, 0.0f, 1.0f },
        {  0.5f,  0.5f,  0.5f,  1.0f, 0.5f, 0.0f, 1.0f },
        {  0.5f, -0.5f,  0.5f,  1.0f, 0.5f, 0.0f, 1.0f },

        { -0.5f, -0.5f, -0.5f,  0.0f, 0.5f, 1.0f, 1.0f },
        { -0.5f, -0.5f,  0.5f,  0.0f, 0.5f, 1.0f, 1.0f },
        {  0.5f, -0.5f,  0.5f,  0.0f, 0.5f, 1.0f, 1.0f },
        {  0.5f, -0.5f, -0.5f,  0.0f, 0.5f, 1.0f, 1.0f },

        { -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.5f, 1.0f },
        { -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f, 1.0f },
        {  0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f, 1.0f },
        {  0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.5f, 1.0f },
    };

    app->graphics.bindings.vertex_buffers[0] = sg_make_buffer(&(sg_buffer_desc)
    {
        .type = SG_BUFFERTYPE_VERTEXBUFFER,
        .data = SG_RANGE(vertices),
        .label = "cube-vertices"
    });

    uint16_t indices[] = { 
        0, 1, 2,  0, 2, 3,
        6, 5, 4,  7, 6, 4,
        8, 9, 10,  8, 10, 11,
        14, 13, 12,  15, 14, 12,
        16, 17, 18,  16, 18, 19,
        22, 21, 20,  23, 22, 20   
    };

    app->graphics.bindings.index_buffer = sg_make_buffer(&(sg_buffer_desc)
    {
        .type = SG_BUFFERTYPE_INDEXBUFFER,
        .data = SG_RANGE(indices),
        .label = "cube-indices"
    });

    app->graphics.pipeline = sg_make_pipeline(&(sg_pipeline_desc) 
    {
        .layout = {
            .attrs = {
                [ATTR_vs_position].format = SG_VERTEXFORMAT_FLOAT3,
                [ATTR_vs_color0].format   = SG_VERTEXFORMAT_FLOAT4
            }
        },
        .shader = sg_make_shader(test_shader_desc(sg_query_backend())),
        .index_type = SG_INDEXTYPE_UINT16,
        .cull_mode = SG_CULLMODE_BACK,
        .depth = {
            .compare = SG_COMPAREFUNC_LESS_EQUAL,
            .write_enabled = true
        },
        .label = "cube-pipeline"
    });

    app->graphics.pass_action = (sg_pass_action) 
    {
        .colors[0] = 
        { 
            .action = SG_ACTION_CLEAR,
            .value={ 0.1f, 0.1f, 0.1f, 1.0f } 
        }
    };
}

void app_draw(App *app)
{
    app->delta_time = stm_laptime(&app->last_time);

    // hmm_mat4 view = HMM_LookAt(
    //     app->graphics.camera_pos, 
    //     HMM_AddVec3(app->graphics.camera_pos, app->graphics.camera_front), 
    //     app->graphics.camera_up);
    hmm_mat4 view;
    hmm_mat4 projection;
    parcc_get_matrices(app->camera, (float*) &projection, (float*) &view);

    // hmm_mat4 projection = HMM_Perspective(45.0f, (float) sapp_width() / (float) sapp_height(), 0.1f, 100.0f);
    hmm_mat4 view_projection = HMM_MultiplyMat4(projection, view);

    sg_begin_default_pass(&app->graphics.pass_action, sapp_width(), sapp_height());
    sg_apply_pipeline(app->graphics.pipeline);
    sg_apply_bindings(&app->graphics.bindings);

    for (int i = 0; i < NUM_CUBES; i++)
    {
        hmm_mat4 model = HMM_Translate(app->graphics.cube_positions[i]);
        float angle = 20.0f * i;
        model = HMM_MultiplyMat4(model, HMM_Rotate(angle, HMM_Vec3(1.0f, 0.3f, 0.5f)));
        vs_params_t vs_params = 
        {
            .mvp = HMM_MultiplyMat4(view_projection, model),
        };

        sg_apply_uniforms(SG_SHADERSTAGE_VS, SLOT_vs_params, &SG_RANGE(vs_params));
        sg_draw(0, 36, 1);
    }

    __cdbgui_draw();
    sg_end_pass();
    sg_commit();    
}

void app_cleanup()
{
    __cdbgui_shutdown();
}

#endif