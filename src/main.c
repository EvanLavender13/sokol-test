#include "math.h"

#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_glue.h"
#include "sokol_time.h"
#define HANDMADE_MATH_IMPLEMENTATION
#define HANDMADE_MATH_NO_SSE
#include "HandmadeMath.h"
#include "cdbgui/cdbgui.h"

#include "../res/test-shader.glsl.h"

#define TEX3D_DIM (32)

static struct 
{
    sg_pass_action pass_action;
    sg_pipeline pipeline;
    sg_bindings bindings;
    float rx, ry, t;
} state;

static uint32_t xorshift32() {
    static uint32_t x = 0x12345678;
    x ^= x<<13;
    x ^= x>>17;
    x ^= x<<5;
    return x;
}

void init() 
{
    sg_setup(&(sg_desc)
    {
        .context = sapp_sgcontext()
    });

    stm_setup();

    __cdbgui_setup(sapp_sample_count());

    state.pass_action = (sg_pass_action) 
    {
        .colors[0] = 
        { 
            .action = SG_ACTION_CLEAR,
            .value={ 0.2f, 0.3f, 0.3f, 1.0f } 
        }
    };    

    sg_shader shader = sg_make_shader(test_shader_desc(sg_query_backend()));

    float vertices[] = 
    {
        -1.0, -1.0, -1.0,    1.0, -1.0, -1.0,    1.0,  1.0, -1.0,   -1.0,  1.0, -1.0,
        -1.0, -1.0,  1.0,    1.0, -1.0,  1.0,    1.0,  1.0,  1.0,   -1.0,  1.0,  1.0,
        -1.0, -1.0, -1.0,   -1.0,  1.0, -1.0,   -1.0,  1.0,  1.0,   -1.0, -1.0,  1.0,
         1.0, -1.0, -1.0,    1.0,  1.0, -1.0,    1.0,  1.0,  1.0,    1.0, -1.0,  1.0,
        -1.0, -1.0, -1.0,   -1.0, -1.0,  1.0,    1.0, -1.0,  1.0,    1.0, -1.0, -1.0,
        -1.0,  1.0, -1.0,   -1.0,  1.0,  1.0,    1.0,  1.0,  1.0,    1.0,  1.0, -1.0,
    };

    state.bindings.vertex_buffers[0] = sg_make_buffer(&(sg_buffer_desc)
    {
        .type = SG_BUFFERTYPE_VERTEXBUFFER,
        .data = SG_RANGE(vertices),
        .label = "cube-vertices"
    });

    uint16_t indices[] = { 
        0, 1, 2,     0, 2, 3,
        6, 5, 4,     7, 6, 4,
        8, 9, 10,    8, 10, 11,
        14, 13, 12,  15, 14, 12,
        16, 17, 18,  16, 18, 19,
        22, 21, 20,  23, 22, 20        
    };
    state.bindings.index_buffer = sg_make_buffer(&(sg_buffer_desc)
    {
        .type = SG_BUFFERTYPE_INDEXBUFFER,
        .data = SG_RANGE(indices),
        .label = "cube-indices"
    });

    state.pipeline = sg_make_pipeline(&(sg_pipeline_desc) 
    {
        .shader = shader,
        .index_type = SG_INDEXTYPE_UINT16,
        .cull_mode = SG_CULLMODE_BACK,
        .depth = 
        {
            .compare = SG_COMPAREFUNC_LESS_EQUAL,
            .write_enabled = true,
        },
        .layout = 
        {
            .attrs = 
            {
                [ATTR_vs_position].format = SG_VERTEXFORMAT_FLOAT3
            }
        },
        .label = "cube-pipeline"
    });

    static uint32_t pixels[TEX3D_DIM][TEX3D_DIM][TEX3D_DIM];
    for (int x = 0; x < TEX3D_DIM; x++)
    {
        for (int y = 0; y < TEX3D_DIM; y++)
        {
            for (int z = 0; z < TEX3D_DIM; z++)
            {
                pixels[x][y][z] = xorshift32();
            }
        }
    }

    state.bindings.fs_images[SLOT_tex] = sg_make_image(&(sg_image_desc)
    {
        .type = SG_IMAGETYPE_3D,
        .width = TEX3D_DIM,
        .height = TEX3D_DIM,
        .num_slices = TEX3D_DIM,
        .num_mipmaps = 1,
        .pixel_format = SG_PIXELFORMAT_RGBA8,
        .min_filter = SG_FILTER_LINEAR,
        .mag_filter = SG_FILTER_LINEAR,
        .label = "random texture",
        .data.subimage[0][0] = SG_RANGE(pixels)
    });
}

void frame() 
{
    // compute vertex-shader params (mvp and texcoord-scale)
    const float t = (float)(sapp_frame_duration() * 60.0);
    state.rx += 1.0f * t;
    state.ry += 2.0f * t;
    state.t  += 0.03f * t;
    hmm_mat4 proj = HMM_Perspective(60.0f, sapp_widthf()/sapp_heightf(), 0.01f, 10.0f);
    hmm_mat4 view = HMM_LookAt(HMM_Vec3(0.0f, 1.5f, 6.0f), HMM_Vec3(0.0f, 0.0f, 0.0f), HMM_Vec3(0.0f, 1.0f, 0.0f));
    hmm_mat4 view_proj = HMM_MultiplyMat4(proj, view);
    hmm_mat4 rxm = HMM_Rotate(state.rx, HMM_Vec3(1.0f, 0.0f, 0.0f));
    hmm_mat4 rym = HMM_Rotate(state.ry, HMM_Vec3(0.0f, 1.0f, 0.0f));
    hmm_mat4 model = HMM_MultiplyMat4(rxm, rym);
    vs_params_t vs_params = 
    {
        .mvp = HMM_MultiplyMat4(view_proj, model),
    };

    sg_begin_default_pass(&state.pass_action, sapp_width(), sapp_height());
    sg_apply_pipeline(state.pipeline);
    sg_apply_bindings(&state.bindings);
    sg_apply_uniforms(SG_SHADERSTAGE_VS, SLOT_vs_params, &SG_RANGE(vs_params));
    sg_draw(0, 36, 1);
    __cdbgui_draw();
    sg_end_pass();
    sg_commit();
}

void cleanup() 
{
    __cdbgui_shutdown();
    sg_shutdown();
}

void event(const sapp_event *e) 
{
    if (e->type == SAPP_EVENTTYPE_KEY_DOWN) 
    {
        if (e->key_code == SAPP_KEYCODE_ESCAPE) 
        {
            sapp_request_quit();
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