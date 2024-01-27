const TextureFormat = Object.seal({
    unknown                 : 0,
    rgba32Typeless          : 1,
    rgba32Float             : 2,
    rgba32UInt              : 3,
    rgba32SInt              : 4,
    rgb32Typeless           : 5,
    rgb32Float              : 6,
    rgb32UInt               : 7,
    rgb32SInt               : 8,
    rgba16Typeless          : 9,
    rgba16Float             : 10,
    rgba16UNorm             : 11,
    rgba16UInt              : 12,
    rgba16SNorm             : 13,
    rgba16SInt              : 14,
    rg32Typeless            : 15,
    rg32Float               : 16,
    rg32UInt                : 17,
    rg32SInt                : 18,
    r32G8X24Typeless        : 19,
    d32FloatS8X24UInt       : 20,
    r32FloatX8X24Typeless   : 21,
    x32TypelessG8X24UInt    : 22,
    rgb10A2Typeless         : 23,
    rgb10A2UNorm            : 24,
    rgb10A2UInt             : 25,
    r11G11B10Float          : 26,
    rgba8Typeless           : 27,
    rgba8UNorm              : 28,
    rgba8UNormSRGB          : 29,
    rgba8UInt               : 30,
    rgba8SNorm              : 31,
    rgba8SInt               : 32,
    rg16Typeless            : 33,
    rg16Float               : 34,
    rg16UNorm               : 35,
    rg16UInt                : 36,
    rg16SNorm               : 37,
    rg16SInt                : 38,
    r32Typeless             : 39,
    d32Float                : 40,
    r32Float                : 41,
    r32UInt                 : 42,
    r32SInt                 : 43,
    r24G8Typeless           : 44,
    d24UNormS8UInt          : 45,
    r24UNormX8Typeless      : 46,
    x24TypelessG8UInt       : 47,
    rg8Typeless             : 48,
    rg8UNorm                : 49,
    rg8UInt                 : 50,
    rg8SNorm                : 51,
    rg8SInt                 : 52,
    r16Typeless             : 53,
    r16Float                : 54,
    d16UNorm                : 55,
    r16UNorm                : 56,
    r16UInt                 : 57,
    r16SNorm                : 58,
    r16SInt                 : 59,
    r8Typeless              : 60,
    r8UNorm                 : 61,
    r8UInt                  : 62,
    r8SNorm                 : 63,
    r8SInt                  : 64,
    a8UNorm                 : 65,
    r1UNorm                 : 66,
    rgb9E5Sharedexr         : 67,
    rg8B8G8UNorm            : 68,
    g8R8G8B8UNorm           : 69,
    bc1Typeless             : 70,
    bc1UNorm                : 71,
    bc1UNormSRGB            : 72,
    bc2Typeless             : 73,
    bc2UNorm                : 74,
    bc2UNormSRGB            : 75,
    bc3Typeless             : 76,
    bc3UNorm                : 77,
    bc3UNormSRGB            : 78,
    bc4Typeless             : 79,
    bc4UNorm                : 80,
    bc4SNorm                : 81,
    bc5Typeless             : 82,
    bc5UNorm                : 83,
    bc5SNorm                : 84,
    b5G6R5UNorm             : 85,
    b5G5R5A1UNorm           : 86,
    bgra8UNorm              : 87,
    bgrx8UNorm              : 88,
    r10G10B10XrBiasA2UNorm  : 89,
    bgra8Typeless           : 90,
    bgra8UNormSRGB          : 91,
    bgrx8Typeless           : 92,
    bgrx8UNormSRGB          : 93,
    bc6HTypeless            : 94,
    bc6HUf16                : 95,
    bc6HSf16                : 96,
    bc7Typeless             : 97,
    bc7UNorm                : 98,
    bc7UNormSRGB            : 99,
});

const SwapChainUsage = Object.seal({
    none                : 0x0,
    renderTarget        : 0x1,
    shaderResource      : 0x2,
    inputAttachment     : 0x4,
    copySource          : 0x8
});

const SwapChainTransform = Object.seal({
    optimal                     : 0,
    identity                    : 1,
    rotate90                    : 2,
    rotate180                   : 3,
    rotate270                   : 4,
    horizontalMirror            : 5,
    horizontalMirrorRotate90    : 6,
    horizontalMirrorRotate180   : 7,
    horizontalMirrorRotate270   : 8
})

const TextureViewType = Object.seal({
    undef               : 0,
    shaderResource      : 1,
    renderTarget        : 2,
    depthStencil        : 3,
    readOnlyDepthStencil: 4,
    unorderedAccess     : 5,
    shadingRate         : 6
});

const ResourceDimension = Object.seal({
    undef           : 0,
    buffer          : 1,
    tex1D           : 2,
    tex1DArray      : 3,
    tex2D           : 4,
    tex2DArray      : 5,
    tex3D           : 6,
    texCube         : 7,
    texCubeArray    : 8
});

const UavAccessFlag = Object.seal({
    unspecified     : 0x0,
    read            : 0x01,
    write           : 0x02,
    readWrite       : 0x01 | 0x02
});

const BindFlags = Object.seal({
    none                : 0x0,
    VertexBuffer        : 1 << 0,
    IndexBuffer         : 1 << 1,
    UniformBuffer       : 1 << 2,
    ShaderResource      : 1 << 3,
    StreamOutput        : 1 << 4,
    RenderTarget        : 1 << 5,
    DepthStencil        : 1 << 6,
    UnorderedAccess     : 1 << 7,
    IndirectDrawArgs    : 1 << 8,
    InputAttachment     : 1 << 9,
    RayTracing          : 1 << 10,
    ShadingRate         : 1 << 11
});

const Usage = Object.seal({
    immutable   : 0,
    default     : 1,
    dynamic     : 2,
    staging     : 3,
    unified     : 4,
    sparse      : 5
});

const CpuAccessFlags = Object.seal({
    none : 0,
    read : 1 << 0,
    write : 1 << 1
});

const TextureFlags = Object.seal({
    none            : 0,
    generateMip     : 1 << 0,
    memoryLess      : 1 << 1,
    sparseAliasing  : 1 << 2,
    subSampled      : 1 << 3
});

const ClearDepthStencil = Object.seal({
    none : 0x0,
    depth : 0x1,
    stencil : 0x2 
});

module.exports = {
    TextureFormat,
    SwapChainUsage,
    SwapChainTransform,
    TextureViewType,
    ResourceDimension,
    UavAccessFlag,
    BindFlags,
    Usage,
    CpuAccessFlags,
    TextureFlags,
    ClearDepthStencil
};