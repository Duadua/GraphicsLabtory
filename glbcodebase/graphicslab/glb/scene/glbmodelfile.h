//--------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016/06/19
// Brief: Define a interface to analysis the model file
// The format that supported by this interfact has showed below:
// .obj -- Exported by 3DMax2009
//--------------------------------------------------------------------
#ifndef GLB_GLBMODELFILE_H_
#define GLB_GLBMODELFILE_H_

#include <memory.h>
#include <stdint.h>
#include <stdio.h>

#include "math/glbvector.h"

#include "render/glbtexture.h"

namespace glb {

namespace scene {

class ModelEffectParam {
public:
    ModelEffectParam()
    : has_diffuse_tex(false)
    , has_alpha_tex(false)
    , has_normal_tex(false)
    , has_texcoord(false)
    , has_normal(false)
    , has_tanget(false)
    , has_binormal(false)
    , accept_light(false)
    , accept_shadow(false)
    , cast_shadow(false)
    , use_ao(false) {
    }

    virtual ~ModelEffectParam() {
    }

public:
    bool    has_diffuse_tex;
    bool    has_alpha_tex;
    bool    has_normal_tex;
    bool    has_texcoord;
    bool    has_normal;
    bool    has_tanget;
    bool    has_binormal;
    bool    accept_light;
    bool    accept_shadow;
    bool    cast_shadow;
    bool    use_ao;
};

class ModelMaterialParam {
public:
    ModelMaterialParam()
    : ambient(0.0f, 0.0f, 0.0)
    , diffuse(0.0f, 0.0f, 0.0f)
    , specular(0.0f, 0.0f, 0.0f)
    , emission(0.0f, 0.0f, 0.0f)
    , pow(0.0)
    , boundbox_min(0.0f, 0.0f, 0.0f)
    , boundbox_max(0.0f, 0.0f, 0.0f) {
        memset(diffuse_tex_name, 0, sizeof(diffuse_tex_name));
        memset(alpha_tex_name, 0, sizeof(alpha_tex_name));
        memset(normal_tex_name, 0, sizeof(normal_tex_name));
    }

    virtual ~ModelMaterialParam() {
    }

public:
    math::Vector ambient;
    math::Vector diffuse;
    math::Vector specular;
    math::Vector emission;
    float        pow;
    char         diffuse_tex_name[render::texture::kMaxTexNameLen];
    char         alpha_tex_name[render::texture::kMaxTexNameLen];
    char         normal_tex_name[render::texture::kMaxTexNameLen];
    math::Vector boundbox_min;
    math::Vector boundbox_max;
};

class ModelFile {
public:
    //----------------------------------------------------------------------------------------------
    // @brief: Extract the vertex and texture coordinate data from the model file
    // @param: model_file_name The name of the model's file
    // @param: effect_param The effect parameter to show if accept lignt, cast shadow, accept shadow and so on
    // @param: material_param The material parameter
    // @param: vertex_buf A pointer to recieve the vertex data. This can not be null.
    // @param: texcoord_buf A pointer to recieve the texture coordinate data. This is an optional parameter
    // @param: normal_buf A pointer to recieve the normal data. This is an optional parameter
    // @param: tangent_buf A pointer to recieve the tangent data. This is an optional parameter
    // @param: binormal_buf A pointter to recieve the binormal data. This is an optional parameter
    // @return: If successfully, return the number of triangles. Otherwise retur 0.
    //----------------------------------------------------------------------------------------------
    static int32_t ExtractModelData(
        const char* model_file_name, 
        ModelEffectParam& effect_param,
        ModelMaterialParam& material_param,
        float** vertex_buf,
        float** texcoord_buf = NULL,
        float** normal_buf = NULL,
        float** tangent_buf = NULL,
        float** binormal_buf = NULL
        );

    //----------------------------------------------------------------------------------------------
    // @brief: Release the buffer memory that returned by ExtractModelData method
    // @param: vertex_buf The buffer pointer hold the vertex data
    // @param: texcoord_buf The buffer pointer hold the texcoord data
    // @param: normal_buf The buffer pointer hold the normal data
    // @param: tangent_buf The buffer pointer hold the tangent data
    // @param: binormal_buf The buffer pointer hold the binormal data
    //----------------------------------------------------------------------------------------------
    static void RelaseBuf(float** vertex_buf, float** texcoord_buf = NULL, float** normal_buf = NULL
        , float** tangent_buf = NULL, float** binormal_buf = NULL);
};

};  // namespace scene

};  // namespace glb

#endif  // GLB_GLB_MODELFILE_H_
