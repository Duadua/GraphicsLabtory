//-------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 09 / 26
// Brief: Scene hold all the object that need to be draw
//-------------------------------------------------------
#ifndef GLB_GLBSCENE_H_
#define GLB_GLBSCENE_H_

#include <stdint.h>

#include <vector>

#include "glblight.h"

namespace glb {

namespace camera {
    class CameraBase;
};

class Object;

namespace scene {

//-----------------------------------------------------------------------------------
// CONSTANT VALUE
//-----------------------------------------------------------------------------------
static const int32_t kMaxLight = 8;

//-----------------------------------------------------------------------------------
// TYPE DECLARATION
//-----------------------------------------------------------------------------------
enum {
    PRIMIAY_CAM = 0,
    SECONDARY_CAM,
    MAX_CAM,
};

//-----------------------------------------------------------------------------------
// CLASS DECLARATION
//----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// Scene DECLARATION
//----------------------------------------------------------------------------------
class Scene {
public:
    static void Initialize();
    static void Destroy();
    static void Update();

    // Space partioning
    static Vector GetSceneBoundBoxMax();
    static Vector GetSceneBoundBoxMin();

    // Object
    static int32_t AddObject(const char* object_file);
    static int32_t AddSkyObject(const char* object_file);
    static Object* GetObjectById(int32_t object_id);
    static Object* GetSkyObject();
    static void GetAllObjects(std::vector<Object*>& objs);

    // Light
    static void SetLight(light::Light light, int32_t id);
    static light::Light GetLight(int32_t id);

    // Camera
    static void SetCamera(int32_t type, camera::CameraBase* camera);
    static camera::CameraBase* GetCamera(int32_t type);
    static void SetCurCamera(int32_t type);
    static int32_t GetCurCamera();

    // Debug Draw
    static void AddBoundBox(Vector color);
};

};  // namespace scene

};  // namespace glb

#endif  // GLB_GLBSCENE_H_