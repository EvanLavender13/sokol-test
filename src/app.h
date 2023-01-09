#ifndef LAVS_APP_H_
#define LAVS_APP_H_

#include "par/par_camera_control.h"
#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_glue.h"
#include "cdbgui/cdbgui.h"
#include "cimgui/cimgui.h"
#include "sokol_imgui.h"
#include "sokol_time.h"

#include "flecs.h"
#include "HandmadeMath.h"
#include "camera_util.h"

#include "../res/test-shader.glsl.h"

#include "camera.h"
#include "components.h"
#include "input.h"
#include "shapes.h"

#define NUM_CUBES 10

typedef struct
{
    sg_pass_action pass_action;
    sg_pipeline pipeline;
    sg_bindings bindings;  
} GraphicsState;

typedef struct
{
    GraphicsState graphics;
    // parcc_context *camera;
    camera_t camera;

    ecs_world_t *world;
    ecs_entity_t render_system;
    hmm_vec3 cube_positions[NUM_CUBES]; 
} App;

typedef struct {
    float x, y, z;
    float r, g, b, a;
} Vertex;

void Move(ecs_iter_t *iter)
{
    Position *position = ecs_term(iter, Position, 1);
    // position->current = HMM_AddVec3(position->current, HMM_Vec3(1.0f * iter->delta_time, 0.0f, 0.0f));
}

void Render(ecs_iter_t *iter)
{
    Position *position = ecs_term(iter, Position, 1);
    hmm_mat4 *view_projection = iter->param;
    hmm_mat4 model = HMM_Translate(position->current);
    vs_params_t vs_params = 
    {
        .mvp = HMM_MultiplyMat4(*view_projection, model),
    };

    sg_apply_uniforms(SG_SHADERSTAGE_VS, SLOT_vs_params, &SG_RANGE(vs_params));
    sg_draw(0, 36, 1);    
}

void app_gui(App *app);

void app_init(App *app)
{
    sg_setup(&(sg_desc)
    {
        .context = sapp_sgcontext()
    });

    // simgui_setup(&(simgui_desc_t) { 0 });

    stm_setup();

    __cdbgui_setup(sapp_sample_count());

    // const parcc_properties camera_props =
    // {
    //     .mode = PARCC_ORBIT,
    //     .viewport_width = sapp_width(),
    //     .viewport_height = sapp_height(),
    //     .near_plane = 0.001f,
    //     .far_plane = 100.0f,
    //     .fov_orientation = PARCC_HORIZONTAL,
        
    //     .home_vector = { 0.0f, 0.0f, 5.0f },
    //     .orbit_zoom_speed = 0.5f,
    //     .orbit_strafe_speed = { 0.01f, 0.01f }
    // };

    // app->camera = parcc_create_context(&camera_props);
    // initialize camera helper
    cam_init(&app->camera, &(camera_desc_t)
    {
        .latitude = -10.0f,
        .longitude = 45.0f,
        .distance = 3.0f
    });    

    ecs_world_t *world = ecs_init();
    ECS_COMPONENT(world, Position);
    ECS_SYSTEM(world, Move, EcsOnUpdate, Position);
    // system cannot be run manually if ECS_SYSTEM macro is used outside scope
    ecs_entity_t render_system = ecs_system_init(world, &(ecs_system_desc_t)
    {
        .query.filter.terms =
        {
            { ecs_id(Position) }
        },
        .callback = Render
    });

    ecs_entity_t e = ecs_new_id(world);
    ecs_set(world, e, Position, { 0.0f, 0.0f, 0.0f });

    app->world = world;
    app->render_system = render_system;

    app->cube_positions[0] = HMM_Vec3( 0.0f,  0.0f,  0.0f);
    app->cube_positions[1] = HMM_Vec3( 2.0f,  5.0f, -15.0f);
    app->cube_positions[2] = HMM_Vec3(-1.5f, -2.2f, -2.5f);
    app->cube_positions[3] = HMM_Vec3(-3.8f, -2.0f, -12.3f);
    app->cube_positions[4] = HMM_Vec3( 2.4f, -0.4f, -3.5f);
    app->cube_positions[5] = HMM_Vec3(-1.7f,  3.0f, -7.5f);
    app->cube_positions[6] = HMM_Vec3( 1.3f, -2.0f, -2.5f);
    app->cube_positions[7] = HMM_Vec3( 1.5f,  2.0f, -2.5f);
    app->cube_positions[8] = HMM_Vec3( 1.5f,  0.2f, -1.5f);
    app->cube_positions[9] = HMM_Vec3(-1.3f,  1.0f, -1.5f);

    shapes_box(4);

    Vertex vertices[] =
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

void app_update(App *app)
{    
    ecs_progress(app->world, sapp_frame_duration());

    cam_update(&app->camera, sapp_width(), sapp_height());
    // hmm_mat4 view_projection = camera_view_projection(app->camera);
    hmm_mat4 view_projection = app->camera.view_proj;

    sg_begin_default_pass(&app->graphics.pass_action, sapp_width(), sapp_height());
    sg_apply_pipeline(app->graphics.pipeline);
    sg_apply_bindings(&app->graphics.bindings);
    
    ecs_run(app->world, app->render_system, sapp_frame_duration(), &view_projection);

    app_gui(app);

    sg_end_pass();
    sg_commit();    
}

void app_gui(App *app)
{
    simgui_new_frame(&(simgui_frame_desc_t)
    {
        .width = sapp_width(),
        .height = sapp_height(),
        .delta_time = sapp_frame_duration(),
        .dpi_scale = sapp_dpi_scale()
    });

    // camera_debug_gui(app->camera);
    __cdbgui_draw();
    // simgui_render();      
}

void app_event(App *app, const sapp_event *event)
{
    if (simgui_handle_event(event))
    {
        return;
    }

    input_handle(&app->camera, event);
    __cdbgui_event(event);
}

void app_cleanup(App *app)
{
    // simgui_shutdown();
    ecs_fini(app->world);
    __cdbgui_shutdown();
    sg_shutdown();
}

#endif