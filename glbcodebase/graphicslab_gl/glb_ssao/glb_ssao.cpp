//----------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 02 / 16
// Brief: Demostrate the screen space ambient occlusion
//----------------------------------------------------------------------
#include "glb_ssao.h"

static const int32_t g_WindowWidth = 800;
static const int32_t g_WindowHeight = 600;
static int32_t g_CurButton = 0;

void glb_display();
void glb_timer(int);
void glb_mouse(int button, int state, int x, int y);
void glb_motion(int x, int y);
void glb_setup();

int main() {
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(g_WindowWidth, g_WindowHeight);
    glutCreateWindow("glb_ssao");
    glutDisplayFunc(glb_display);
    glutTimerFunc(1000.0f / 60.0f, glb_timer, 0);
    glutMouseFunc(glb_mouse);
    glutMotionFunc(glb_motion);

    glb_setup();
    
    glutMainLoop();

    return 0;
}

void glb_timer(int) {
    glutPostRedisplay();
    glutTimerFunc(1000.0f / 60.0f, glb_timer, 0);
}

void glb_mouse(int button, int state, int x, int y) {
    static int s_mouse_pos_x = 0, s_mouse_pos_y = 0;
    if (state == GLUT_DOWN) {
        g_CurButton = button;

        int32_t diff_x = 0;
        int32_t diff_y = 0;

        if (s_mouse_pos_x != 0 || s_mouse_pos_y != 0) {
            diff_x = x - s_mouse_pos_x;
            diff_y = y - s_mouse_pos_y;
        }

        s_mouse_pos_x = x;
        s_mouse_pos_y = y;

        int32_t type = glb::scene::Scene::GetCurCamera();
        camera::FreeCamera* cam = static_cast<camera::FreeCamera*>(glb::scene::Scene::GetCamera(type));
        if (button == GLUT_LEFT_BUTTON) {
            cam->Move(static_cast<float>(diff_x) * 0.5f, static_cast<float>(diff_y) * 0.5f, 0.0f);
        } else if (button == GLUT_MIDDLE_BUTTON) {
            cam->Move(0.0f, 0.0f, static_cast<float>(diff_y) * 0.5f);
        } else if (button == GLUT_RIGHT_BUTTON) {
            cam->Rotate(-static_cast<float>(diff_y) * 0.05f, -static_cast<float>(diff_x) * 0.05f);
        }
    } else {
        s_mouse_pos_x = 0;
        s_mouse_pos_y = 0;
        g_CurButton = 0;
    }
}

void glb_motion(int x, int y) {
    glb_mouse(g_CurButton, GLUT_DOWN, x, y);
}

void glb_display() {
    glb::profile::ProfileTime time;
    time.BeginProfile();

    // Update scene
    glb::light::Light lit = glb::scene::Scene::GetLight(0);
    float rotY = 1.0f;
    Matrix rot;
    rot.MakeRotateYMatrix(rotY);
    lit.dir = rot * lit.dir;
    glb::scene::Scene::Update();

    // Draw scene
    glb::render::Render::Draw();

    time.EndProfile();
    printf("%f\n", time.GetProfileTimeInMs());
}

void glb_setup() {
    glewInit();
    glb::log::Initialize();
    glb::scene::Scene::Initialize();
    glb::shader::Mgr::Initialize();
    glb::texture::Mgr::Initialize();
    glb::material::Mgr::Initialize();
    glb::mesh::Mgr::Initialize();
    glb::ModelMgr::Initialize();
    glb::render::Device::Initialize();
    glb::render::Render::Initialize(g_WindowWidth, g_WindowHeight);
    glb::EditorComm::Initialize();

    glb::EditorComm::PostData();

    // Camera
    camera::FreeCamera* cam = camera::FreeCamera::Create(glb::Vector(0.0f, 75.0f, 115.0f), glb::Vector(0.0f, 0.0f, 0.0f));
    glb::scene::Scene::SetCamera(glb::scene::PRIMIAY_CAM, cam);

    // Light
    light::Light light(light::PARALLEL_LIGHT);
    light.ambient = glb::Vector(0.5f, 0.5f, 0.5f);
    light.diffuse = glb::Vector(1.0f, 1.0f, 1.0f);
    light.specular = glb::Vector(2.0f, 2.0f, 2.0f);
    light.dir = glb::Vector(-1.0f, -1.0f, 1.0f);
    light.dir.Normalize();
    light.pow = 128.0f;
    glb::scene::Scene::SetLight(light, 0);

    // Perspective
    glb::render::Render::SetPerspective(glb::render::Render::PRIMARY_PERS, 69.0f, g_WindowWidth * 1.0f / g_WindowHeight, 0.1f, 500.0f);

    int32_t city = glb::scene::Scene::AddObject("city.obj");
    glb::scene::Scene::GetObjectById(city)->SetCullFaceEnable(true);
    glb::scene::Scene::GetObjectById(city)->SetCullFaceMode(glb::render::CULL_BACK);
    glb::scene::Scene::GetObjectById(city)->SetDepthTestEnable(true);
    glb::scene::Scene::GetObjectById(city)->SetPos(Vector(0.0, 0.0, 0.0));
}