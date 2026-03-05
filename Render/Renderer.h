#pragma once
/*
MIT License

Copyright (c) 2026 Patoke

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "4JLibs/4J_Render.h"
#include "Profiler.h"
#include <cstdint>
#include <mutex>
#include <unordered_map>
#include <vector>

#define MATRIX_MODE_MODELVIEW            0
#define MATRIX_MODE_MODELVIEW_PROJECTION 1
#define MATRIX_MODE_MODELVIEW_TEXTURE    2
#define MATRIX_MODE_MODELVIEW_CBUFF      3
#define MATRIX_MODE_MODELVIEW_MAX        4

#define STACK_TYPES    4
#define STACK_SIZE     16
#define MAX_MIP_LEVELS 5
#define MAX_TEXTURE_LAYERS 4
#define MAX_TEXTURES 512

#define NUM_COMMAND_HANDLES 0x800000
#define MAX_COMMAND_BUFFERS 16000

class Renderer
{
public:
    struct Context;
    struct CommandBuffer;

    void UpdateGamma(unsigned short usGamma);
    void MatrixMode(int type);
    void MatrixSetIdentity();
    void MatrixTranslate(float x, float y, float z);
    void MatrixRotate(float angle, float x, float y, float z);
    void MatrixScale(float x, float y, float z);
    void MatrixPerspective(float fovy, float aspect, float zNear, float zFar);
    void MatrixOrthogonal(float left, float right, float bottom, float top, float zNear, float zFar);
    void MatrixPop();
    void MatrixPush();
    void MatrixMult(float *mat);
    const float *MatrixGet(int type);
    void Set_matrixDirty();
	void Initialise(VkDevice *pDevice, VkSwapchainKHR *pSwapChain);
    VkQueue *InitialiseContext(bool fromPresent);
    void StartFrame();
    void DoScreenGrabOnNextPresent();
    void Present();
    void Clear(int flags, VkRect2D *pRect);
    void SetClearColour(const float colourRGBA[4]);
    bool IsWidescreen();
    bool IsHiDef();
    void CaptureThumbnail(ImageFileBuffer *pngOut);
    void CaptureScreen(ImageFileBuffer *jpgOut, XSOCIAL_PREVIEWIMAGE *previewOut);
    void BeginConditionalSurvey(int identifier);
    void EndConditionalSurvey();
    void BeginConditionalRendering(int identifier);
    void EndConditionalRendering();
    void DrawVertices(C4JRender::ePrimitiveType PrimitiveType, int count, void *dataIn, C4JRender::eVertexType vType,
                      C4JRender::ePixelShaderType psType);
    void DrawVertexBuffer(C4JRender::ePrimitiveType PrimitiveType, int count, VkBuffer *buffer, C4JRender::eVertexType vType,
                          C4JRender::ePixelShaderType psType);
    void CBuffLockStaticCreations();
    int CBuffCreate(int count);
    void CBuffDelete(int first, int count);
    void CBuffStart(int index, bool full);
    void CBuffClear(int index);
    int CBuffSize(int index);
    void CBuffEnd();
    bool CBuffCall(int index, bool full);
    void CBuffTick();
    void CBuffDeferredModeStart();
    void CBuffDeferredModeEnd();
    int TextureCreate();
    void TextureFree(int idx);
    void TextureBind(int idx);
    void TextureBind(int layer, int idx);
    void TextureBindVertex(int idx);
    void TextureSetTextureLevels(int levels);
    int TextureGetTextureLevels();
    void TextureSetParam(int param, int value);
    void TextureDynamicUpdateStart();
    void TextureDynamicUpdateEnd();
    void TextureData(int width, int height, void *data, int level, C4JRender::eTextureFormat format);
    void TextureDataUpdate(int xoffset, int yoffset, int width, int height, void *data, int level);
	std::error_code LoadTextureData(const char *szFilename,D3DXIMAGE_INFO *pSrcInfo, int **ppDataOut);
	std::error_code LoadTextureData(uint8_t *pbData, uint32_t dwBytes,D3DXIMAGE_INFO *pSrcInfo, int **ppDataOut);
	std::error_code SaveTextureData(const char *szFilename, D3DXIMAGE_INFO *pSrcInfo, int *ppDataOut);
	std::error_code SaveTextureDataToMemory(void *pOutput, int outputCapacity, int *outputLength, int width, int height, int *ppDataIn);
    void TextureGetStats();
	VkImageView *TextureGetTexture(int idx);
    void StateSetColour(float r, float g, float b, float a);
    void StateSetDepthMask(bool enable);
    void StateSetBlendEnable(bool enable);
    void StateSetBlendFunc(int src, int dst);
    void StateSetBlendFactor(unsigned int colour);
    void StateSetAlphaFunc(int func, float param);
    void StateSetDepthFunc(int func);
    void StateSetFaceCull(bool enable);
    void StateSetFaceCullCW(bool enable);
    void StateSetLineWidth(float width);
    void StateSetWriteEnable(bool red, bool green, bool blue, bool alpha);
    void StateSetDepthTestEnable(bool enable);
    void StateSetAlphaTestEnable(bool enable);
    void StateSetDepthSlopeAndBias(float slope, float bias);
    void StateSetFogEnable(bool enable);
    void StateSetFogMode(int mode);
    void StateSetFogNearDistance(float dist);
    void StateSetFogFarDistance(float dist);
    void StateSetFogDensity(float density);
    void StateSetFogColour(float red, float green, float blue);
    void StateSetLightingEnable(bool enable);
    void StateSetVertexTextureUV(float u, float v);
    void StateSetLightColour(int light, float red, float green, float blue);
    void StateSetLightAmbientColour(float red, float green, float blue);
    void StateSetLightDirection(int light, float x, float y, float z);
    void StateSetLightEnable(int light, bool enable);
    void StateSetViewport(C4JRender::eViewportType viewportType);
    void StateSetEnableViewportClipPlanes(bool enable);
    void StateSetTexGenCol(int col, float x, float y, float z, float w, bool eyeSpace);
    void StateSetStencil(VkCompareOp function, uint8_t stencil_ref, uint8_t stencil_func_mask, uint8_t stencil_write_mask);
    void StateSetForceLOD(int LOD);
    void BeginEvent(const char* eventName);
    void EndEvent();
    void Suspend();
    bool Suspended();
    void Resume();
    void StateUpdate();
private:
    void SetupShaders();
    void ConvertLinearToPng(ImageFileBuffer *pngOut, unsigned char *linearData, unsigned int width, unsigned int height);
    void DrawVertexSetup(C4JRender::eVertexType vType, C4JRender::ePixelShaderType psType, C4JRender::ePrimitiveType primitiveType, int *count,
                         bool *drawIndexed);
    void UpdateTexGenState();
    void UpdateLightingState();
    void UpdateViewportState();
    void UpdateFogState();
    void UpdateTextureState(int layer, bool vertexSampler);
    void MultWithStack(glm::mat4 matrix);

	//TODO: implement alternative for D3D11 pipeline states
	struct DepthStencilState {};
	struct BlendState {};
	struct RasterizerState {};
	struct SamplerState {};

    DepthStencilState *GetManagedDepthStencilState();
    BlendState *GetManagedBlendState();
    RasterizerState *GetManagedRasterizerState();
    SamplerState *GetManagedSamplerState(int layer);
    C4JRender::ePixelShaderType ResolvePixelShaderType(C4JRender::ePixelShaderType psType);
    void DeleteInternalBuffer(int index);
    Renderer::Context &getContext();
public:
    struct Texture
    {
        bool allocated;
        VkImage *texture;
        VkImageView *view;
        uint32_t VkImage;
        uint32_t mipLevels;
        uint32_t textureFormat;
        uint32_t samplerParams;
    };

    struct TexgenCBuffer
    {
		glm::mat4 unk0;
		glm::mat4 unk1;
    };

    enum eCommandType
    {
        COMMAND_ADD_MATRIX,
        COMMAND_ADD_VERTICES,
        COMMAND_BIND_TEXTURE,
        COMMAND_SET_COLOR,
        COMMAND_SET_DEPTH_FUNC,
        COMMAND_SET_DEPTH_MASK,
        COMMAND_SET_DEPTH_TEST,
        COMMAND_SET_LIGHTING_ENABLE,
        COMMAND_SET_LIGHT_ENABLE,
        COMMAND_SET_LIGHT_DIRECTION,
        COMMAND_SET_LIGHT_COLOUR,
        COMMAND_SET_LIGHT_AMBIENT_COLOUR,
        COMMAND_SET_BLEND_ENABLE,
        COMMAND_SET_BLEND_FUNC,
        COMMAND_SET_BLEND_FACTOR,
        COMMAND_SET_FACE_CULL,
    };

    struct CommandBuffer
    {
        CommandBuffer(bool full);
        ~CommandBuffer();
        void StartRecording();
        void EndRecording(VkDevice *device);
        std::uint64_t GetAllocated();
        bool IsBusy();
        void AddMatrix(const float *matrix);
        void AddVertices(unsigned int stride, unsigned int count, void *dataIn, Renderer::Context &c);
        void BindTexture(int layer, int idx);
        void SetColor(float r, float g, float b, float a);
        void SetDepthFunc(int func);
        void SetDepthMask(bool enable);
        void SetDepthTestEnable(bool enable);
        void SetLightingEnable(bool enable);
        void SetLightEnable(int light, bool enable);
        void SetLightDirection(int light, float x, float y, float z);
        void SetLightColour(int light, float r, float g, float b);
        void SetLightAmbientColour(float r, float g, float b);
        void SetBlendEnable(bool enable);
        void SetBlendFunc(int src, int dst);
        void SetBlendFactor(unsigned int factor);
        void SetFaceCull(bool enable);
        void Render(C4JRender::eVertexType vType, Renderer::Context &c, int primitiveType);

        struct Command
        {
            Renderer::eCommandType m_command_type;
            uint8_t commandPadding[12];

            union
            {
                uint8_t data[64];

                struct
                {
                    float m_matrix[16];
                    // glm::mat4 m_matrix;
                } add_matrix;

                struct
                {
                    unsigned int m_vertex_index_start;
                    unsigned int m_vertex_count;
                } add_vertices;

                struct
                {
                    unsigned int m_texture_layer;
                    unsigned int m_texture_index;
                } bind_texture;

                struct
                {
                    float m_color[4];
                } set_color;

                struct
                {
                    int m_depth_func;
                } set_depth_func;

                struct
                {
                    bool m_enable;
                } set_depth_mask;

                struct
                {
                    bool m_enable;
                } set_depth_test;

                struct
                {
                    bool m_enable;
                } set_lighting_enable;

                struct
                {
                    int m_light_index;
                    bool m_enable;
                } set_light_enable;

                struct
                {
                    int m_light_index;
                    float padding[3];
                    float m_direction[4];
                } set_light_direction;

                struct
                {
                    int m_light_index;
                    float m_color[3];
                } set_light_colour;

                struct
                {
                    uint8_t padding;
                    float m_color[3];
                } set_light_ambient_colour;

                struct
                {
                    bool m_enable;
                } set_blend_enable;

                struct
                {
                    int m_src;
                    int m_dst;
                } set_blend_func;

                struct
                {
                    unsigned int m_blend_factor;
                } set_blend_factor;

                struct
                {
                    bool m_enable;
                } set_face_cull;
            };
        };
        VkBuffer *m_vertexBuffer;
        void *m_vertexData;
        std::uint64_t m_vertexDataLength;
        std::vector<Command> m_commands;
        std::uint64_t m_allocated;
        uint8_t isActive;
    };

    struct DeferredCBuff
    {
        Renderer::CommandBuffer *m_command_buf;
        int m_vertex_index;
        int m_vertex_type;
        int m_primitive_type;
		glm::mat4 m_matrix;
    };

    struct Context
    {
        static const unsigned int VERTEX_BUFFER_SIZE = 0x100000;

        Context(VkDevice *device, VkQueue *deviceContext);

        VkQueue *m_pDeviceContext;
        //ID3DUserDefinedAnnotation *userAnnotation; TODO: Implement alternative for ID3DUserDefinedAnnotation
        uint32_t annotateDepth;
    	glm::mat4 matrixStacks[MATRIX_MODE_MODELVIEW_MAX][STACK_SIZE];
        bool matrixDirty[MATRIX_MODE_MODELVIEW_MAX];
        uint32_t stackPos[MATRIX_MODE_MODELVIEW_MAX];
        uint32_t stackType;
        uint32_t boundTextureIndex[MAX_TEXTURE_LAYERS];
        bool faceCullEnabled;
        bool depthTestEnabled;
        bool alphaTestEnabled;
        float alphaReference;
        bool depthWriteEnabled;
        bool fogEnabled;
        float fogNearDistance;
        float fogFarDistance;
        float fogDensity;
        float fogColourRed;
        float fogColourBlue;
        float fogColourGreen;
        uint32_t fogMode;
        bool lightingEnabled;
        bool lightEnabled[2];
        bool lightingDirty;
        uint32_t forcedLOD;
        uint8_t paddingAfterForceLOD[4];
		glm::vec4 lightDirection[2];
        glm::vec4 lightColour[2];
        glm::vec4 lightAmbientColour;
        VkBuffer *m_modelViewMatrix;
        VkBuffer *m_localTransformMatrix;
        VkBuffer *m_projectionMatrix;
        VkBuffer *m_textureMatrix;
        VkBuffer *m_vertexTexcoordBuffer;
        VkBuffer *m_fogParamsBuffer;
        VkBuffer *m_lightingStateBuffer;
        VkBuffer *m_texGenMatricesBuffer;
        VkBuffer *m_compressedTranslationBuffer;
        VkBuffer *m_thumbnailBoundsBuffer;
        VkBuffer *m_tintColorBuffer;
        VkBuffer *m_fogColourBuffer;
        VkBuffer *m_unkColorBuffer;
        VkBuffer *m_alphaTestBuffer;
        VkBuffer *m_clearColorBuffer;
        VkBuffer *m_forcedLODBuffer;
        uint64_t dynamicVertexBase;
        uint32_t dynamicVertexOffset;
        VkBuffer *dynamicVertexBuffer;
        glm::mat4 texGenMatrices[2];
        Renderer::CommandBuffer *commandBuffer;
        uint32_t recordingBufferIndex;
        uint32_t recordingVertexType;
        uint32_t recordingPrimitiveType;
        bool deferredModeEnabled;
        std::vector<DeferredCBuff> deferredBuffers;
        VkPipelineColorBlendStateCreateInfo blendDesc;
        VkPipelineDepthStencilStateCreateInfo depthStencilDesc;
        VkPipelineRasterizationStateCreateInfo rasterizerDesc;
        float blendFactor[4];
    };

    static uint32_t tlsIdx;
    static std::mutex totalAllocCS;
    static uint32_t s_auiWidths[];
    static uint32_t s_auiHeights[];
    static VkFormat textureFormats[];
    static VkPrimitiveTopology g_topologies[];
    static int totalAlloc;

    float m_fClearColor[4];
    VkDevice *m_pDevice;
    VkCommandBuffer *m_pDeviceContext;
    VkSwapchainKHR *m_pSwapChain;
    VkImageView *renderTargetView;
    VkImageView *renderTargetViews[4];
    VkImageView *renderTargetShaderResourceView;
    VkImageView *renderTargetShaderResourceViews[4];
    VkImage *renderTargetTextures[4];
    VkImageView *depthStencilView;
    VkShaderModule **vertexShaderTable;
    VkShaderModule *screenSpaceVertexShader;
    VkShaderModule *screenClearVertexShader;
    VkShaderModule **pixelShaderTable;
    VkShaderModule *screenSpacePixelShader;
    VkShaderModule *screenClearPixelShader;
    unsigned int *vertexStrideTable;
    //ID3D11InputLayout **inputLayoutTable; TODO: find way to implement in vulkan
    VkBuffer *quadIndexBuffer;
    VkBuffer *fanIndexBuffer;
    uint32_t defaultTextureIndex;
    uint16_t reservedRendererWord0;
    uint8_t paddingAfterRendererWord0[2];
    uint32_t presentCount;
    uint8_t rendererFlag0;
    uint8_t paddingAfterRendererFlag0[3];
	std::mutex m_commandBufferCS;
    uint32_t activeVertexType;
    uint32_t activePixelType;
    C4JRender::eViewportType m_ViewportType;
    uint8_t reservedRendererByte0;
    uint8_t paddingAfterViewportType[3];
    Renderer::Texture m_textures[512];
    uint32_t backBufferWidth;
    uint32_t backBufferHeight;
    uint8_t reservedRendererByte1;
    uint8_t paddingAfterRendererByte1[3];
    uint32_t reservedRendererDword1;
    int16_t *m_commandHandleToIndex;
    CommandBuffer **m_commandBuffers;
    uint8_t *m_commandPrimitiveTypes;
    glm::mat4 *m_commandMatrices;
    int *m_commandIndexToHandle;
    uint8_t *m_commandVertexTypes;
    uint32_t reservedRendererDword2;
    uint32_t reservedRendererDword3;
    std::unordered_map<int, BlendState *> managedBlendStates;
    std::unordered_map<int, DepthStencilState *> managedDepthStencilStates;
    std::unordered_map<int, SamplerState *> managedSamplerStates;
    std::unordered_map<int, RasterizerState *> managedRasterizerStates;
    bool m_bShouldScreenGrabNextFrame;
    bool m_bSuspended;
    uint8_t paddingAfterSuspendState[2];
};

// Singleton
extern Renderer InternalRenderManager;
