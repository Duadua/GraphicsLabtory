//------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2018. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2018 / 06 / 11
// Brief: Core routine for model editor
//------------------------------------------------------------------
#include "glb_modeleditor_core.h"

#include <fstream>

#include "GL/glew.h"

#include "Importer.hpp"
#include "scene.h"
#include "postprocess.h"

using namespace glb;

ApplicationCore* ApplicationCore::s_Instance = NULL;

ApplicationCore::ApplicationCore()
: m_SceneMesh(-1) {
    s_Instance = this;
    memset(m_SceneMeshName, 0, sizeof(m_SceneMeshName));
}

ApplicationCore::~ApplicationCore() {
    s_Instance = NULL;
    Destroy();
}

app::ApplicationBase* ApplicationCore::Create() {
    return new ApplicationCore();
}

ApplicationCore* ApplicationCore::GetInstance() {
    return s_Instance;
}

bool ApplicationCore::Initialize() {
    // Camera
    scene::ModelCamera* cam = scene::ModelCamera::Create(math::Vector(10.0f, 10.0f, 0.0), math::Vector(0.0f, 0.0f, 0.0));
    scene::Scene::SetCamera(glb::scene::PRIMIAY_CAM, cam);

    // Light
    scene::Light ambient(scene::AMBIENT_LIGHT);
    ambient.color = math::Vector(0.1f, 0.1f, 0.1f);
    scene::Scene::SetLight(ambient, 0);

    scene::Light light(scene::PARALLEL_LIGHT);
    light.color = math::Vector(1.2f, 1.2f, 1.2f);
    light.dir = math::Vector(-1.0f, -1.0f, 1.0f);
    light.dir.Normalize();
    scene::Scene::SetLight(light, 1);

    // Perspective
    render::Render::SetPerspective(glb::render::Render::PRIMARY_PERS, 69.0f, 800 * 1.0f / 600, 0.1f, 500.0f);

    // HDR
    glb::render::Render::SetHighLightBase(1.5f);

    return true;
}

void ApplicationCore::Update(float dt) {
    util::ProfileTime time;
    time.BeginProfile();

    Input::Update();

    UpdateCamera();

    // Update Scene
    scene::Scene::Update();
    render::Render::Draw();

    GLB_CHECK_GL_ERROR;

    time.EndProfile();
    // printf("%f\n", time.GetProfileTimeInMs());
}

void ApplicationCore::Destroy() {
}

bool ApplicationCore::AddModel(const char* name) {
    Assimp::Importer importer;
    GLB_SAFE_ASSERT(importer.ValidateFlags(aiProcess_CalcTangentSpace | aiProcess_Triangulate) == true);
    const aiScene* scene = importer.ReadFile(name, aiProcess_CalcTangentSpace | aiProcess_Triangulate);

    bool result = false;
    if (scene && scene->HasMeshes())
    {
        // Only deal with mesh 0 now
        aiMesh* mesh = scene->mMeshes[0];
        int32_t trianglesNum = scene->mMeshes[0]->mNumFaces;
        float* vertexBuf = new float[trianglesNum * 3 * 3];
        float* texcoordBuf = mesh->HasTextureCoords(0) ? new float[trianglesNum * 3 * 2] : NULL;
        float* lightMapTexCoordBuf = mesh->HasTextureCoords(1) ? new float[trianglesNum * 3 * 2] : NULL;
        float* normalBuf = mesh->HasNormals() ? new float[trianglesNum * 3 * 3] : NULL;
        float* tangentBuf = mesh->HasTangentsAndBitangents() ? new float[trianglesNum * 3 * 3] : NULL;
        float* binormalBuf = mesh->HasTangentsAndBitangents() ? new float[trianglesNum * 3 * 3] : NULL;

        for (int32_t i = 0; i < trianglesNum; i++) {
            aiFace face = scene->mMeshes[0]->mFaces[i];
            if (face.mNumIndices == 3) {
                // Vertex Position
                for (int32_t j = 0; j < 3; j++) {
                    vertexBuf[i * 3 * 3 + j * 3 + 0] = mesh->mVertices[face.mIndices[j]].x;
                    vertexBuf[i * 3 * 3 + j * 3 + 1] = mesh->mVertices[face.mIndices[j]].y;
                    vertexBuf[i * 3 * 3 + j * 3 + 2] = mesh->mVertices[face.mIndices[j]].z;
                }

                // Vertex UV
                if (mesh->HasTextureCoords(0)) {
                    for (int32_t j = 0; j < 3; j++) {
                        texcoordBuf[i * 3 * 2 + j * 2 + 0] = mesh->mTextureCoords[0][face.mIndices[j]].x;
                        texcoordBuf[i * 3 * 2 + j * 2 + 1] = mesh->mTextureCoords[0][face.mIndices[j]].y;
                    }
                }

                // Vertex Second UV
                if (mesh->HasTextureCoords(1)) {
                    for (int32_t j = 0; j < 3; j++) {
                        lightMapTexCoordBuf[i * 3 * 2 + j * 2 + 0] = mesh->mTextureCoords[1][face.mIndices[j]].x;
                        lightMapTexCoordBuf[i * 3 * 2 + j * 2 + 1] = mesh->mTextureCoords[1][face.mIndices[j]].y;
                    }
                }

                // Vertex Normal
                if (mesh->HasNormals()) {
                    for (int32_t j = 0; j < 3; j++) {
                        normalBuf[i * 3 * 3 + j * 3 + 0] = mesh->mNormals[face.mIndices[j]].x;
                        normalBuf[i * 3 * 3 + j * 3 + 1] = mesh->mNormals[face.mIndices[j]].y;
                        normalBuf[i * 3 * 3 + j * 3 + 2] = mesh->mNormals[face.mIndices[j]].z;
                    }
                }

                // Vertex Tangent and Binormal
                if (mesh->HasTangentsAndBitangents()) {
                    for (int32_t j = 0; j < 3; j++) {
                        tangentBuf[i * 3 * 3 + j * 3 + 0] = mesh->mTangents[face.mIndices[j]].x;
                        tangentBuf[i * 3 * 3 + j * 3 + 1] = mesh->mTangents[face.mIndices[j]].y;
                        tangentBuf[i * 3 * 3 + j * 3 + 2] = mesh->mTangents[face.mIndices[j]].z;
                        binormalBuf[i * 3 * 3 + j * 3 + 0] = mesh->mBitangents[face.mIndices[j]].x;
                        binormalBuf[i * 3 * 3 + j * 3 + 1] = mesh->mBitangents[face.mIndices[j]].y;
                        binormalBuf[i * 3 * 3 + j * 3 + 2] = mesh->mBitangents[face.mIndices[j]].z;
                    }
                }
            } else {
                GLB_SAFE_ASSERT(false);  // Must be triangle
            }
        }

        render::mesh::MeshBase* model = render::mesh::TriangleMesh::Create(trianglesNum, vertexBuf, texcoordBuf, lightMapTexCoordBuf, normalBuf, tangentBuf, binormalBuf);
        render::mesh::Mgr::AddMesh(model);

        render::material::Material* material = render::material::Material::Create("res/default.mat");
        render::material::Mgr::AddMaterial(material);

        int32_t modelId = scene::Scene::AddObject(model, material);
        if (modelId != -1) {
            scene::Scene::GetObjectById(modelId)->SetCullFaceEnable(true);
            scene::Scene::GetObjectById(modelId)->SetCullFaceMode(glb::render::CULL_BACK);
            scene::Scene::GetObjectById(modelId)->SetDepthTestEnable(true);
            scene::Scene::GetObjectById(modelId)->SetWorldMatrix(math::Matrix::CreateIdentityMatrix());
            m_SceneMesh = modelId;
            memcpy(m_SceneMeshName, name, strlen(name));
            m_SceneMeshName[strlen(name)] = '\0';
            result = true;
        } else {
            GLB_SAFE_ASSERT(false);  // Something wrong
        }

        delete[] vertexBuf;
        vertexBuf = NULL;
        if (texcoordBuf) {
            delete[] texcoordBuf;
            texcoordBuf = NULL;
        }
        if (normalBuf) {
            delete[] normalBuf;
            normalBuf = NULL;
        }
        if (tangentBuf) {
            delete[] tangentBuf;
            tangentBuf = NULL;
        }
        if (binormalBuf) {
            delete[] binormalBuf;
            binormalBuf = NULL;
        }
    }

    return result;
}

bool ApplicationCore::SaveModel(const char* filePath) {
    Assimp::Importer importer;
    GLB_SAFE_ASSERT(importer.ValidateFlags(aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_FixInfacingNormals) == true);
    const aiScene* scene = importer.ReadFile(m_SceneMeshName, aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_FixInfacingNormals);

    bool result = false;
    if (scene && scene->HasMeshes())
    {
        std::ofstream output;
        output.open(filePath);
        if (!output.fail())
        {
            // Only deal with mesh 0 now
            aiMesh* mesh = scene->mMeshes[0];
            int32_t trianglesNum = scene->mMeshes[0]->mNumFaces;

            for (int32_t i = 0; i < trianglesNum; i++) {
                aiFace face = scene->mMeshes[0]->mFaces[i];
                if (face.mNumIndices == 3) {
                    std::string faceStr[3];

                    // Vertex Position
                    for (int32_t j = 0; j < 3; j++) {
                        output << "v ";
                        output << mesh->mVertices[face.mIndices[j]].x << " ";
                        output << mesh->mVertices[face.mIndices[j]].y << " ";
                        output << mesh->mVertices[face.mIndices[j]].z << "\n";

                        char temp[64];
                        sprintf_s(temp, "%d", i * 3 + j + 1);
                        faceStr[j] = faceStr[j] + temp;
                    }

                    // Vertex UV
                    if (mesh->HasTextureCoords(0)) {
                        for (int32_t j = 0; j < 3; j++) {
                            output << "vt ";
                            output << mesh->mTextureCoords[0][face.mIndices[j]].x << " ";
                            output << mesh->mTextureCoords[0][face.mIndices[j]].y << "\n";

                            char temp[64];
                            sprintf_s(temp, "%d", i * 3 + j + 1);
                            faceStr[j] = faceStr[j] + "/" + temp;
                        }
                    }

                    // Vertex Second UV
                    if (mesh->HasTextureCoords(1)) {
                        for (int32_t j = 0; j < 3; j++) {
                            output << "vlightmapt ";
                            output << mesh->mTextureCoords[1][face.mIndices[j]].x << " ";
                            output << mesh->mTextureCoords[1][face.mIndices[j]].y << "\n";

                            char temp[64];
                            sprintf_s(temp, "%d", i * 3 + j + 1);
                            faceStr[j] = faceStr[j] + "/" + temp;
                        }
                    }

                    // Vertex Normal
                    if (mesh->HasNormals()) {
                        for (int32_t j = 0; j < 3; j++) {
                            output << "vn ";
                            output << mesh->mNormals[face.mIndices[j]].x << " ";
                            output << mesh->mNormals[face.mIndices[j]].y << " ";
                            output << mesh->mNormals[face.mIndices[j]].z << "\n";

                            char temp[64];
                            sprintf_s(temp, "%d", i * 3 + j + 1);
                            faceStr[j] = faceStr[j] + "/" + temp;
                        }
                    }

                    // Vertex Tangent and Binormal
                    if (mesh->HasTangentsAndBitangents()) {
                        for (int32_t j = 0; j < 3; j++) {
                            output << "vtan ";
                            output << mesh->mTangents[face.mIndices[j]].x << " ";
                            output << mesh->mTangents[face.mIndices[j]].y << " ";
                            output << mesh->mTangents[face.mIndices[j]].z << "\n";

                            char temp[64];
                            sprintf_s(temp, "%d", i * 3 + j + 1);
                            faceStr[j] = faceStr[j] + "/" + temp;

                            output << "vbi ";
                            output << mesh->mBitangents[face.mIndices[j]].x << " ";
                            output << mesh->mBitangents[face.mIndices[j]].y << " ";
                            output << mesh->mBitangents[face.mIndices[j]].z << "\n";

                            sprintf_s(temp, "%d", i * 3 + j + 1);
                            faceStr[j] = faceStr[j] + "/" + temp;
                        }
                    }

                    output << "f " << faceStr[0] << " " << faceStr[1] << " " << faceStr[2] << "\n";
                } else {
                    GLB_SAFE_ASSERT(false);  // Must be triangle
                }
            }

            output.close();
            result = true;
        }
    }

    return result;
}

bool ApplicationCore::Preview(const char* name) {
    m_SceneMesh = scene::Scene::AddObject(name, "res/default.mat");
    if (m_SceneMesh != -1) {
        scene::Scene::GetObjectById(m_SceneMesh)->SetDepthTestEnable(true);
        scene::Scene::GetObjectById(m_SceneMesh)->SetWorldMatrix(math::Matrix::CreateIdentityMatrix());

        scene::Scene::GetObjectById(m_SceneMesh)->SetCullFaceEnable(true);
        scene::Scene::GetObjectById(m_SceneMesh)->SetCullFaceMode(glb::render::CULL_BACK);
    }

    return m_SceneMesh != -1;
}

void ApplicationCore::UpdateCamera() {
    if (Input::IsKeyboardButtonPressed(BK_LCONTROL) && Input::IsMouseButtonPressed(BM_LEFT)) {
        static float sRot = 0.0f;
        sRot = sRot + Input::GetMouseMoveX() * 0.1f;
        static float sDist = 0.0f;
        sDist = sDist + Input::GetMouseMoveY() * 0.05f;
        math::Vector pos = math::Vector(10.0f, 10.0f, 0.0) + math::Vector(1.0f, 1.0f, 0.0f) * sDist;
        math::Matrix rotY;
        rotY.MakeRotateYMatrix(sRot);
        pos = rotY * pos;
        scene::Scene::GetCurCamera()->SetPos(pos);
        scene::Scene::GetCurCamera()->Update(1.0f);
    }
}