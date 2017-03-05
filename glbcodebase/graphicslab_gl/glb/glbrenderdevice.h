//--------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 10 / 26
// Brief: Render device is used to render primitives
//--------------------------------------------------------------------
#ifndef GLB_GLBRENDERDEVICE_H_
#define GLB_GLBRENDERDEVICE_H_

#include <stdint.h>

#include "glbmatrix.h"
#include "glbstructs.h"
#include "glbvector.h"

namespace glb {

namespace render {

enum RenderState {
    RS_DEPTH_TEST = 0,
    RS_ALPHA_TEST,
    RS_CULL_FACE,
};

enum CullMode {
    CULL_NONE = 0,
    CULL_FRONT,
    CULL_BACK
};

enum PrimitiveType {
    PT_TRIANGLES = 0,
    PT_LINES,
};

enum ClearFlag {
    CLEAR_COLOR = 0x00000001,
    CLEAR_DEPTH = 0x00000002,
};

enum TextureSlot {
    TS_DIFFUSE = 0,
    TS_ALPHA,
    TS_NORMAL,
    TS_SHADOW,
    TS_HDRSCENE,
    TS_LOG_LUM,
    TS_HDR_BLOOM,
    TS_DEPTH,
    TS_RANDOM_ROTATE,
    TS_AO_MAP,
    TS_BI_BLUR_MAP,
    TS_MAX,
};

enum AlphaBlendFactor {
    FACTOR_SRC = 0,
    FACTOR_DST,
};

enum AlphaBlendFunc {
    FUNC_ZERO = 0,
    FUNC_ONE,
    FUNC_SRC_ALPHA,
    FUNC_DST_ALPHA,
    FUNC_ONE_MINUS_SRC_ALPHA,
    FUNC_ONE_MINUS_DST_ALPHA,
};

enum DrawColorBuffer {
    COLORBUF_NONE = 0,
    COLORBUF_FRONT,
    COLORBUF_BACK,
    COLORBUF_COLOR_ATTACHMENT0,
    COLORBUF_COLOR_ATTACHMENT1,
    COLORBUF_COLOR_ATTACHMENT2,
    COLORBUF_COLOR_ATTACHMENT3,
    COLORBUF_COLOR_ATTACHMENT4,
    COLORBUF_COLOR_ATTACHMENT5,
    COLORBUF_COLOR_ATTACHMENT6,
    COLORBUF_COLOR_ATTACHMENT7,
};

class Device {
public:
    static void Initialize();
    static void Destroy();

    // Vertex Buffer
    static void SetVertexArray(int32_t va);
    static void SetVertexBuffer(int32_t vb);
    static void SetVertexLayout(VertexLayout vlayout);

    // Texture
    static void SetTexture(int32_t slot, int32_t tex_obj, int32_t tex_unit);
    static void ClearTexture();

    // Shader
    static void SetShader(int32_t shader);
    static void SetShaderLayout(ShaderLayout layout);
    static void SetUniform1i(int32_t location, int32_t v);
    static void SetUniform1f(int32_t location, float v);
    static void SetUniform3f(int32_t location, const Vector& v);
    static void SetUniformMatrix(int32_t location, const Matrix& m);
    static void SetUniformSampler(int32_t location, int32_t slot);

    // Render State
    static void SetDepthTestEnable(bool enable);

    static void SetAlphaBlendEnable(bool enable);
    static void SetAlphaBlendFunc(AlphaBlendFactor factor, AlphaBlendFunc func);

    static void SetCullFaceEnable(bool enable);
    static void SetCullFaceMode(CullMode mode);

    // Render Target
    static void SetRenderTarget(int32_t rt);

    // Viewport
    static void SetViewport(int32_t x, int32_t y, int32_t width, int32_t height);

    // Draw
    static void SetDrawColorBuffer(DrawColorBuffer buffer);
    static void SetClearColor(float r, float g, float b);
    static void SetClearDepth(float depth);
    static void Clear(int32_t clear_flag);
    static void Draw(PrimitiveType type, int32_t first, int32_t size);
    static void SwapBuffer();
};

};  // namespace render

};  // namespace glb

#endif  // GLB_GLBRENDERDEVICE_H_