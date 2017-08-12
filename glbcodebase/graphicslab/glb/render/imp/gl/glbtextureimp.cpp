//-----------------------------------------------------------------------
// Declaration: Copyright (c), i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017/03/13
// Brief: Implement the texture with opengl
//-----------------------------------------------------------------------
#ifdef GLB_PLATFORM_OPENGL

#include "glbtextureimp.h"

#include <stdint.h>

#include <vector>

#include <GL\glew.h>

#include "render/glbtexture.h"

#include "util/glbmacro.h"
#include "util/glbtexturereader.h"
#include "util/glbtexturewriter.h"

namespace glb {

namespace render {

namespace texture {

//-----------------------------------------------------------------------------------
// CONSTANT VALUE
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// TYPE DECLARATION
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// CLASS DECLARATION
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// CLASS DEFINITION
//-----------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Texture::Imp DEFINITION
//--------------------------------------------------------------------------------------
Texture::Imp::Imp()
: m_Type(TEX_NONE)
, m_Format(FMT_R8G8B8A8)
, m_TexID(-1)
, m_Width(0)
, m_Height(0)
, m_Depth(0)
, m_BPP(0)
, m_TexObj(-1) {
    memset(m_TexName, 0, sizeof(m_TexName));
}

Texture::Imp::~Imp() {
    Destroy();
}

Texture::Imp* Texture::Imp::Create(const char* texture_name, int32_t type) {
    Texture::Imp* tex = NULL;

    if (texture_name != NULL) {
        int8_t* texture_data = NULL;
        int32_t texture_width = 0;
        int32_t texture_height = 0;
        int32_t texture_pixel_format = util::TPFT_UNKOWN;
        if (util::TextureReader::ReadTexture(texture_name, &texture_data, texture_width, texture_height, texture_pixel_format)) {
            GLuint tex_obj = 0;
            glGenTextures(1, &tex_obj);

            if (type == TEX_2D) {
                CreateGLTexture2D(tex_obj, texture_width, texture_height, texture_data, texture_pixel_format);
            } else if (type == TEX_3D) {
                glBindTexture(GL_TEXTURE_3D, tex_obj);
                glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA8, texture_width, texture_width, texture_width, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
                glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                glGenerateMipmap(GL_TEXTURE_3D);
                glBindTexture(GL_TEXTURE_3D, 0);
            }

            util::TextureReader::ReleaseData(&texture_data);

            tex = new Texture::Imp();
            tex->m_Type = type;
            tex->m_TexObj = tex_obj;
            memcpy(tex->m_TexName, texture_name, strlen(texture_name));
            tex->m_TexName[strlen(texture_name)] = 0;

            if (type == TEX_2D) {
                tex->m_Width = texture_width;
                tex->m_Height = texture_height;
                tex->m_Depth = 0;
                SetTexturePixelFormat(tex, texture_pixel_format);
            }
            else if (type == TEX_3D) {
                tex->m_Width = texture_width;
                tex->m_Height = texture_width;
                tex->m_Depth = texture_width;
            }
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return tex;
}

Texture::Imp* Texture::Imp::Create(int32_t width, int32_t height) {
    Texture::Imp* tex = NULL;

    if (width > 0 && height > 0) {
        int32_t tex_id = 0;
        glGenTextures(1, reinterpret_cast<GLuint*>(&tex_id));
        glBindTexture(GL_TEXTURE_2D, tex_id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glGenerateMipmap(GL_TEXTURE_2D);

        tex = new Texture::Imp();
        if (tex != NULL) {
            tex->m_Type = TEX_2D;
            static char default_name[] = "DefaultEmpty";
            memcpy(tex->m_TexName, default_name, sizeof(default_name));
            tex->m_TexObj = tex_id;
            tex->m_Width = width;
            tex->m_Height = height;
            tex->m_Format = FMT_R8G8B8A8;
            tex->m_BPP = 4;
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return tex;
}

Texture::Imp* Texture::Imp::CreateFloat32Texture(int32_t width, int32_t height) {
    Texture::Imp* tex = NULL;

    if (width > 0 && height > 0) {
        int32_t tex_id = 0;
        glGenTextures(1, reinterpret_cast<GLuint*>(&tex_id));
        glBindTexture(GL_TEXTURE_2D, tex_id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
        glGenerateMipmap(GL_TEXTURE_2D);

        tex = new Texture::Imp();
        if (tex != NULL) {
            tex->m_Type = TEX_2D;
            static char default_name[] = "DefaultEmpty";
            memcpy(tex->m_TexName, default_name, sizeof(default_name));
            tex->m_TexObj = tex_id;
            tex->m_Width = width;
            tex->m_Height = height;
            tex->m_Format = FMT_R32G32B32A32F;
            tex->m_BPP = 16;
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return tex;
}

Texture::Imp* Texture::Imp::CreateFloat32DepthTexture(int32_t width, int32_t height) {
    Texture::Imp* tex = NULL;

    if (width > 0 && height > 0) {
        int32_t tex_id = 0;
        glGenTextures(1, reinterpret_cast<GLuint*>(&tex_id));
        glBindTexture(GL_TEXTURE_2D, tex_id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

        tex = new Texture::Imp;
        if (tex != NULL) {
            tex->m_Type = TEX_2D;
            tex->m_Depth = 0;
            tex->m_Height = height;
            static char default_name[] = "DefaultEmpty";
            memcpy(tex->m_TexName, default_name, sizeof(default_name));
            tex->m_TexObj = tex_id;
            tex->m_Width = width;
            tex->m_Format = FMT_DEPTH32F;
            tex->m_BPP = 4;
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return tex;
}

Texture::Imp* Texture::Imp::CreateFloat32CubeTexture(int32_t width, int32_t height) {
    Texture::Imp* tex = NULL;

    // Warning: The cube map's 6 texture must be square and have the same size
    if (width > 0 && height > 0 && width == height) {
        int32_t tex_id = 0;
        glGenTextures(1, reinterpret_cast<GLuint*>(&tex_id));
        glBindTexture(GL_TEXTURE_CUBE_MAP, tex_id);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP);

        for(int32_t i = 0; i < 6; i++) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
        }

        tex = new Texture::Imp;
        if (tex != NULL) {
            tex->m_Type = TEX_CUBE;
            tex->m_Depth = 0;
            tex->m_Height = height;
            static char default_name[] = "DefaultEmpty";
            memcpy(tex->m_TexName, default_name, sizeof(default_name));
            tex->m_TexObj = tex_id;
            tex->m_Width = width;
            tex->m_Format = FMT_R32G32B32A32F;
            tex->m_BPP = 16;
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return tex;
}

void Texture::Imp::CreateGLTexture2D(int32_t tex_obj, int32_t width, int32_t height, int8_t* texture_data, int32_t texture_pixel_format) {
    struct {
        int32_t pixel_format;
        int32_t internel_format;
        int32_t data_format;
        int32_t data_type;
    } gl_pixel_format_tbl[] = {
        {util::TPFT_R8G8B8, GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE},
        {util::TPFT_A8R8G8B8, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE},
        {util::TPFT_R8G8B8A8, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE},
        {util::TPFT_R16G16, GL_RG16, GL_RG, GL_UNSIGNED_SHORT},
        {util::TPFT_G16R16, GL_RG16, GL_RG, GL_UNSIGNED_SHORT},
    };
    static_assert(GLB_ARRAY_SIZE(gl_pixel_format_tbl) == util::TPFT_UNKOWN, "");

    for (int32_t i = 0; i < GLB_ARRAY_SIZE(gl_pixel_format_tbl); i++) {
        if (gl_pixel_format_tbl[i].pixel_format == texture_pixel_format) {
            glBindTexture(GL_TEXTURE_2D, tex_obj);
            glTexImage2D(GL_TEXTURE_2D, 0, gl_pixel_format_tbl[i].internel_format, width, height, 0, gl_pixel_format_tbl[i].data_format, gl_pixel_format_tbl[i].data_type, texture_data);
            glGenerateMipmap(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, 0);
            break;
        }
    }
}

void Texture::Imp::SetTexturePixelFormat(Texture::Imp* texture, int32_t texture_pixel_format) {
    if (texture != NULL) {
        struct {
            int32_t pixel_format_from_file;
            int32_t pixel_format_from_lib;
            int32_t bytes_per_pixel;
        } pixel_format[] = {
            {util::TPFT_R8G8B8, FMT_R8G8B8, 3},
            {util::TPFT_A8R8G8B8, FMT_R8G8B8A8, 4},
            {util::TPFT_R8G8B8A8, FMT_R8G8B8A8, 4},
            {util::TPFT_R16G16, FMT_R16G16, 4},
            {util::TPFT_G16R16, FMT_R16G16, 4},
        };
        static_assert(GLB_ARRAY_SIZE(pixel_format) == util::TPFT_UNKOWN, "");

        for (int32_t i = 0; i < GLB_ARRAY_SIZE(pixel_format); i++) {
            if (pixel_format[i].pixel_format_from_file == texture_pixel_format) {
                texture->m_Format = pixel_format[i].pixel_format_from_lib;
                texture->m_BPP = pixel_format[i].bytes_per_pixel;
                break;
            }
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void Texture::Imp::Destroy() {
    if (m_TexID != -1) {
        glBindTexture(GL_TEXTURE_2D, 0);
        glDeleteTextures(1, reinterpret_cast<const GLuint*>(&m_TexObj));
        m_TexObj = 0;
        m_TexID = -1;
    }
}

void Texture::Imp::UpdateTextureData(const void* pixels, int32_t miplevel) {
    switch (m_Type) {
    case TEX_2D:
        switch (m_Format) {
        case FMT_R8G8B8A8:
            glBindTexture(GL_TEXTURE_2D, m_TexObj);
            glTexSubImage2D(GL_TEXTURE_2D, miplevel, 0, 0, m_Width, m_Height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
            glBindTexture(GL_TEXTURE_2D, 0);
            break;

        case FMT_R32G32B32A32F:
            glBindTexture(GL_TEXTURE_2D, m_TexObj);
            glTexSubImage2D(GL_TEXTURE_2D, miplevel, 0, 0, m_Width, m_Height, GL_RGBA, GL_FLOAT, pixels);
            glBindTexture(GL_TEXTURE_2D, 0);
            break;
        default:
            GLB_SAFE_ASSERT(false);  // Only support RGBA now
            break;
        }
        break;

    default:
        GLB_SAFE_ASSERT(false);  // Only support 2D texture now
        break;
    }
}

void Texture::Imp::GetTextureData(void* pixel, int32_t miplevel) {
    switch (m_Type) {
    case TEX_2D:
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, m_TexObj);
        if (m_Format == FMT_R8G8B8A8) {
            glGetTexImage(GL_TEXTURE_2D, miplevel, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
        } else if (m_Format == FMT_R32G32B32A32F) {
            glGetTexImage(GL_TEXTURE_2D, miplevel, GL_RGBA, GL_FLOAT, pixel);
        }
        break;

    default:
        GLB_SAFE_ASSERT(false);  // Only support 2D texture now
        break;
    }
}

void Texture::Imp::GenerateMipmap() {
    switch (m_Type) {
    case TEX_2D:
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, m_TexObj);
        glGenerateMipmap(GL_TEXTURE_2D);
        break;

    default:
        GLB_SAFE_ASSERT(false);  // Only support 2D texture now
        break;
    }
}

void Texture::Imp::Save(const char* file_name, util::TEXTURE_FILE_TYPE file_type) {
    if (m_Type == TEX_2D) {
        int8_t* pixel = new int8_t[m_Width * m_Height * m_BPP];

        util::TEXTURE_PIXEL_FORMAT_TYPE pixel_format = util::TPFT_UNKOWN;
        if (m_Format == FMT_R8G8B8A8) {
            pixel_format = util::TPFT_R8G8B8A8;
        } else {
            // TODO: do not support now
            GLB_SAFE_ASSERT(false);
        }

        GetTextureData(pixel, 0);

        util::TextureWriter::Write(file_name, pixel, m_Width, m_Height, pixel_format, m_BPP, file_type);
    } else {
        // TODO: Only save 2D texture now
        GLB_SAFE_ASSERT(false);
    }
}

int32_t Texture::Imp::GetID() {
    return m_TexID;
}

void Texture::Imp::SetID(int32_t tex_id) {
    m_TexID = tex_id;
}

int32_t Texture::Imp::GetWidth() {
    return m_Width;
}

int32_t Texture::Imp::GetHeight() {
    return m_Height;
}

int32_t Texture::Imp::GetType() {
    return m_Type;
}

int32_t Texture::Imp::GetBPP() {
    return m_BPP;
}

const char* Texture::Imp::GetName() {
    return m_TexName;
}

int32_t Texture::Imp::GetNativeTex() {
    return m_TexObj;
}

};  // namespace texture

};  // namespace render

};  // namespace glb

#endif  // GLB_PLATFORM_OPENGL