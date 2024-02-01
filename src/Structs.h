#pragma once

#include "Api.h"
#include "DeviceContext.h"
#include "RenderDevice.h"
#include "EngineFactory.h"

namespace REngine
{
	enum class GraphicsBackend : u8 {
		Unknow =0,
		D3D11,
		D3D12,
		Metal,
		Vulkan,
		OpenGL,
		Software
	};
	enum class AdapterType : u8 {
		Unknow = 0,
		Software,
		Integrated,
		Dedicated,
		Discrete
	};

#ifdef WIN32
	struct D3D12Settings {
		u32* cpuDescriptorHeapAllocationSize;
		u32* gpuDescriptorHeapSize;
		u32* gpuDescriptorHeapDynamicSize;
		u32* dynamicDescriptorAllocationChunkSize;
		u32* queryPoolSize;

		u32 dynamicHeapPageSize;
		u32 numDynamicHeapPagesToReserve;
	};

#endif
#ifndef __EMSCRIPTEN__
	struct VulkanSettings {
		struct DescriptorPoolSize {
			u32 max;
			u32 sepSm;
			u32 cmbSm;
			u32 smpImg;
			u32 strImg;
			u32 ub;
			u32 sb;
			u32 utxb;
			u32 stTxB;
			u32 inptAtt;
			u32 accelSt;
		};

		string** instanceLayerNames;
		string** instanceExtensionNames;
		string** deviceExtensionNames;
		string** ignoreDebugMessageNames;

		u32 instanceLayerNamesCount;
		u32 instanceExtensionNamesCount;
		u32 deviceExtensionNamesCount;
		u32 ignoreDebugMessageNamesCount;

		DescriptorPoolSize* mainDescriptorPoolSize;
		DescriptorPoolSize* dynamicDescriptorPoolSize;

		u32 deviceLocalMemoryPageSize;
		u32 hostVisibleMemoryPageSize;
		u32 deviceLocalMemoryReserveSize;
		u32 hostVisibleMemoryReserveSize;
		u32 uploadHeapPageSize;
		u32 dynamicHeapSize;
		u32 dynamicHeapPageSize;

		u32* queryPoolSizes;
	};
#endif
	struct GraphicsDriverSettings {
#ifdef WIN32
		D3D12Settings* d3d12;
#endif
#ifndef __EMSCRIPTEN__
		VulkanSettings* vulkan;
#endif
		RENGINE_ALIGN
		boolean enableValidation;
		RENGINE_ALIGN
		GraphicsBackend backend;

		u32 adapterId;
		u32 numDeferredCtx;
		void* messageCallback;
	};

	struct GraphicsAdapter {
		u32 id;
		u32 vendorId;
		u32 deviceId;
		string* name;
		AdapterType adapterType;
		u64 localMemory;
		u64 hostVisibleMemory;
		u64 unifiedMemory;
		u64 maxMemoryAlloc;
		u8 unifiedMemoryCpuAccess;
		u32 memoryLessTextureBindFlags;
	};

	struct GraphicsDriver {
		Diligent::IRenderDevice* device;
		Diligent::IDeviceContext** contexts;
		Diligent::IEngineFactory* factory;
	};

	struct GraphicsDriverResult {
		GraphicsDriver* driver;
		Diligent::ISwapChain* swapChain;
		string* error;
	};

	struct Result {
		void* value;
		string* error;
	};

	struct SwapChainDesc {
		u32 width;
		u32 height;
		RENGINE_ALIGN
		u16 colorFormat;
		RENGINE_ALIGN
		u16 depthFormat;

		u32 usage;
		u32 transform;

		u32 bufferCount;
		float defaultDepthValue;
		RENGINE_ALIGN
		u8 defaultStencilValue;
		RENGINE_ALIGN
		boolean isPrimary;
	};

	struct SwapChainCreationInfo {
		GraphicsBackend backend;
		Diligent::IEngineFactory* factory;
		Diligent::IRenderDevice* device;
		Diligent::IDeviceContext* deviceContext;
		SwapChainDesc* swapChainDesc;
		Diligent::NativeWindow* window;
	};

	struct CopyTextureInfoDTO {
		Diligent::ITexture* srcTexture;
		u32 srcMipLevel;
		u32 srcSlice;
		Diligent::Box* srcBox;
		Diligent::ITexture* dstTexture;
		u32 dstMipLevel;
		u32 dstSlice;
		u32 dstX;
		u32 dstY;
		u32 dstZ;
	};

	struct BufferDescDTO {
		string* name;
		u64 size;
		Diligent::BIND_FLAGS bindFlags;
		Diligent::USAGE usage;
		Diligent::CPU_ACCESS_FLAGS accessFlags;
		Diligent::BUFFER_MODE mode;
		u32 elementByteStride;
	};

	enum class ShaderType : u8 {
		Vertex =0,
		Pixel,
		Compute,
		Geometry,
		Hull,
		Domain,
		Unknow
	};

	enum class ShaderTypeFlags {
		None =0,
		Vertex = 1 << 0,
		Pixel = 1 << 1,
		Compute = 1 << 2,
		Geometry = 1 << 3,
		Hull = 1 << 4,
		Domain = 1 << 5
	};

	struct ShaderCreateInfoDTO {
		string* name;
		RENGINE_ALIGN
		ShaderType type;
		string* sourceCode;
		u8* byteCode;
		u32 byteCodeLength;
		string** macroKeys;
		string** macroValues;
		u32 numMacros;
	};

	enum class ElementType : u8 {
		Int,
		Float,
		Vector2,
		Vector3,
		Vector4,
		UByte4
	};

	struct PipelineInputLayoutElementDesc {
		u32 inputIndex;
		u32 bufferIndex;
		u32 bufferStride;
		u32 elementOffset;
		u32 instanceStepRate;
		RENGINE_ALIGN
		ElementType elementType;
		RENGINE_ALIGN
		u8 normalized;
	};

	enum class TextureAddressMode : u8 {
		Wrap = 0,
		Mirror,
		Clamp
	};
	enum class TextureFilterMode : u8 {
		Nearest,
		Bilinear,
		Trilinear,
		Anisotropic,
		NearestAnisotropic,
		Default
	};

	struct ImmutableSamplerDesc {
		string* name;
		RENGINE_ALIGN
		TextureFilterMode sampler_filterMode;
		RENGINE_ALIGN
		u8 sampler_anisotropy;
		RENGINE_ALIGN
		u8 sampler_shadowCmp;
		RENGINE_ALIGN
		TextureAddressMode sampler_addressMode_u;
		RENGINE_ALIGN
		TextureAddressMode sampler_addressMode_v;
		RENGINE_ALIGN
		TextureAddressMode sampler_addressMode_w;
	};

	struct GraphicsPipelineDescDTO {
		string* name;

		RENGINE_ALIGN
		u8 blendState_colorWriteEnabled;
		RENGINE_ALIGN
		u8 blendState_blendMode;
		RENGINE_ALIGN
		u8 blendState_alphaToCoverage;
		
		RENGINE_ALIGN
		u8 rasterizerState_fillMode;
		RENGINE_ALIGN
		u8 rasterizerState_cullMode;
		float rasterizerState_constantDepthBias;
		float rasterizerState_slopeScaledDepthBias;
		RENGINE_ALIGN
		u8 rasterizerState_scissorTestEnabled;
		RENGINE_ALIGN
		u8 rasterizerState_lineAntiAlias;

		RENGINE_ALIGN
		u8 depthStencilState_enableDepth;
		RENGINE_ALIGN
		u8 depthStencilState_depthWriteEnabled;
		RENGINE_ALIGN
		u8 depthStencilState_stencilTestEnabled;
		RENGINE_ALIGN
		u8 depthStencilState_depthCmpFunc;
		RENGINE_ALIGN
		u8 depthStencilState_stencilCmpFunc;
		RENGINE_ALIGN
		u8 depthStencilState_stencilOpOnPassed;
		RENGINE_ALIGN
		u8 depthStencilState_stencilOpOnStencilFailed;
		RENGINE_ALIGN
		u8 depthStencilState_stencilOpOnDepthFailed;
		RENGINE_ALIGN
		u8 depthStencilState_stencilCmpMask;
		RENGINE_ALIGN
		u8 depthStencilState_stencilWriteMask;

		RENGINE_ALIGN
		u8 primitiveType;

		PipelineInputLayoutElementDesc* inputLayouts;
		RENGINE_ALIGN
		u8 numInputLayouts;

		RENGINE_ALIGN
		u16 output_depthStencilFormat;
#if defined(__EMSCRIPTEN__)
		u32* output_rtFormats;
#else
		u16* output_rtFormats;
#endif
		RENGINE_ALIGN
		u8 output_numRtFormats;
		RENGINE_ALIGN
		u8 output_multiSample;

		ImmutableSamplerDesc* immutableSamplers;
		RENGINE_ALIGN
		u8 numImmutableSamplers;

		Diligent::IShader* shader_vs;
		Diligent::IShader* shader_ps;
		Diligent::IShader* shader_ds;
		Diligent::IShader* shader_hs;
		Diligent::IShader* shader_gs;

		Diligent::IPipelineStateCache* pscache;
	};

	struct ComputePipelineDesc {
		string* name;

		ImmutableSamplerDesc* samplers;
		u8 numSamplers;

		Diligent::IShader* shader;

		Diligent::IPipelineStateCache* pscache;
	};

	struct TextureDescDTO {
		string* name;
		Diligent::RESOURCE_DIMENSION dimension;
		u32 width;
		u32 height;
		u32 arraySizeOrDepth;
		u16 format;
		u32 mipLevels;
		u32 sampleCount;
		Diligent::BIND_FLAGS bindFlags;
		Diligent::USAGE usage;
		Diligent::CPU_ACCESS_FLAGS accessFlags;
		Diligent::MISC_TEXTURE_FLAGS textureFlags;

		u16 clear_format;
		float clear_r;
		float clear_g;
		float clear_b;
		float clear_a;

		float clear_depth;
		u8 clear_stencil;
	};

	struct TextureViewDesc {
		RENGINE_ALIGN
		Diligent::TEXTURE_VIEW_TYPE viewType;
		RENGINE_ALIGN
		Diligent::RESOURCE_DIMENSION dimension;
		RENGINE_ALIGN
		Diligent::TEXTURE_FORMAT format;
		u32 mostDetailedMip;
		u32 mipLevels;
		u32 firstSlice;
		u32 slicesCount;
		RENGINE_ALIGN
		Diligent::UAV_ACCESS_FLAG accessFlags;
		RENGINE_ALIGN
		u8 allowMipMapGeneration;
	};

	struct TextureDataDTO {
		void* data;
		Diligent::IBuffer* srcBuffer;
		u64 srcOffset;
		u64 stride;
		u64 depthStride;
	};

	struct BufferViewCreateDesc
	{
		string* name;
		Diligent::BUFFER_VIEW_TYPE viewType;
		Diligent::VALUE_TYPE format_valueType;
		u8 format_numComponents;
		u8 format_isNormalized;
		u64 byteOffset;
		u64 byteWidth;
	};
#if defined(__EMSCRIPTEN__)
	struct WebDrawArgs
	{
		u32 numVertices;
		u32 numInstances;
		u32 startVertexLocation;
		u32 firstInstanceLocation;
	};
#endif
}