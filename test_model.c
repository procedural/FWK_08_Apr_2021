// model + skybox + gizmo demo
// - rlyeh, public domain.

#define FWK_C
#include "fwk.h"

int main() {
    window_create(75, WINDOW_MSAA8);
    window_title(__FILE__);

    for(const char **list = file_list("fx**.fs"); *list; list++) {
        fx_load(*list);
    }

    camera_t cam = camera();
    model_t q1 = model("kgirls01.fbx", 0);
    skybox_t sky = skybox("cubemaps/stardust", 0);

    while( window_swap() ) {
        if(input(KEY_F5)) app_reload();
        if(input(KEY_ESC)) break;

        // fps camera
        bool active = ui_active() || gizmo_active() ? false : input(MOUSE_L) || input(MOUSE_M) || input(MOUSE_R);
        window_cursor( !active );

        if( active ) cam.speed = clampf(cam.speed + input_diff(MOUSE_W) / 10, 0.05f, 5.0f);
        vec2 mouse = scale2(vec2(input_diff(MOUSE_X), -input_diff(MOUSE_Y)), 0.2f * active);
        vec3 wasdec = scale3(vec3(input(KEY_D)-input(KEY_A),input(KEY_E)-input(KEY_C),input(KEY_W)-input(KEY_S)), cam.speed);
        camera_move(&cam, wasdec.x,wasdec.y,wasdec.z);
        camera_fps(&cam, mouse.x,mouse.y);

        // rendering
        ddraw_ground(0);
        ddraw_flush();

        fx_begin();


            profile(skeletal update) {
                float delta = window_delta() * 30; // 30hz anim
                q1.curframe = model_animate(q1, q1.curframe + delta);
            }
            profile(skeletal render) {
                static vec3 p = {0,0,0}, r = {0,0,0}, s = {2,2,2};
                gizmo(&p, &r, &s);

                mat44 M; rotationq44(M, eulerq(r)); scale44(M, s.x,s.y,s.z); relocate44(M, p.x,p.y,p.z);
                model_render(q1, cam.proj, cam.view, M);
            }
            profile(skybox) {
                skybox_push_state(&sky, cam.proj, cam.view);
                glEnable(GL_DEPTH_TEST);
                mesh_render(&sky.geometry);
                skybox_pop_state(&sky);
            }

        fx_end();

        // UI
        if( ui_begin("FX", 0) ) {
            for( int i = 0; i < 64; ++i ) {
                char *name = fx_name(i); if( !name ) break;
                bool b = fx_enabled(i);
                if( ui_bool(name, &b) ) fx_enable(i, fx_enabled(i) ^ 1);
            }
            ui_end();
        }
    }
}
