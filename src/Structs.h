#pragma once

#include "Api.h"
#include "DeviceContext.h"
#include "RenderDevice.h"
#include "EngineFactory.h"

namespace REngine
{
	enum class GraphicsBackend : byte {
		Unknow =0,
		D3D11,
		D3D12,
		Metal,
		Vulkan,
		OpenGL,
		Software
	};
	enum class AdapterType : byte {
		Unknow = 0,
		Software,
		Integrated,
		Dedicated,
		Discrete
	};

#ifdef WIN32
	struct D3D12Settings {
		uint* cpuDescriptorHeapAllocationSize;
		uint* gpuDescriptorHeapSize;
		uint* gpuDescriptorHeapDynamicSize;
		uint* dynamicDescriptorAllocationChunkSize;
		uint* queryPoolSize;

		uint dynamicHeapPageSize;
		uint numDynamicHeapPagesToReserve;
	};

#endif

	struct VulkanSettings {
		struct DescriptorPoolSize {
			uint max;
			uint sepSm;
			uint cmbSm;
			uint smpImg;
			uint strImg;
			uint ub;
			uint sb;
			uint utxb;
			uint stTxB;
			uint inptAtt;
			uint accelSt;
		};

		string** instanceLayerNames;
		string** instanceExtensionNames;
		string** deviceExtensionNames;
		string** ignoreDebugMessageNames;

		uint instanceLayerNamesCount;
		uint instanceExtensionNamesCount;
		uint deviceExtensionNamesCount;
		uint ignoreDebugMessageNamesCount;

		DescriptorPoolSize* mainDescriptorPoolSize;
		DescriptorPoolSize* dynamicDescriptorPoolSize;

		uint deviceLocalMemoryPageSize;
		uint hostVisibleMemoryPageSize;
		uint deviceLocalMemoryReserveSize;
		uint hostVisibleMemoryReserveSize;
		uint uploadHeapPageSize;
		uint dynamicHeapSize;
		uint dynamicHeapPageSize;

		uint* queryPoolSizes;
	};

	struct GraphicsDriverSettings {
#ifdef WIN32
		D3D12Settings* d3d12;
#endif
		VulkanSettings* vulkan;
		boolean enableValidation;
		GraphicsBackend backend;

		uint adapterId;
		uint numDeferredCtx;
		void* messageCallback;
	};

	struct GraphicsAdapter {
		uint id;
		uint vendorId;
		uint deviceId;
		string* name;
		AdapterType adapterType;
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
		uint width;
		uint height;

		ushort colorFormat;
		ushort depthFormat;

		uint usage;
		uint transform;

		uint bufferCount;
		float defaultDepthValue;
		byte defaultStencilValue;
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

	struct CopyTextureInfo {
		Diligent::ITexture* srcTexture;
		uint srcMipLevel;
		uint srcSlice;
		Diligent::Box* srcBox;
		Diligent::ITexture* dstTexture;
		uint dstMipLevel;
		uint dstSlice;
		uint dstX;
		uint dstY;
		uint dstZ;
	};

	struct BufferDescDTO {
		string* name;
		ulong size;
		Diligent::BIND_FLAGS bindFlags;
		Diligent::USAGE usage;
		Diligent::CPU_ACCESS_FLAGS accessFlags;
		Diligent::BUFFER_MODE mode;
		uint elementByteStride;
	};

	enum class ShaderType : byte {
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

	struct ShaderCreateInfo {
		string* name;
		ShaderType type;
		string* sourceCode;
		byte* byteCode;
		uint byteCodeLength;
		string** macroKeys;
		string** macroValues;
		uint numMacros;
	};

	enum class ElementType : byte {
		Int,
		Float,
		Vector2,
		Vector3,
		Vector4,
		UByte4,
		UByte4Norm
	};

	struct PipelineInputLayoutElementDesc {
		uint inputIndex;
		uint bufferIndex;
		uint bufferStride;
		uint elementOffset;
		uint instanceStepRate;
		ElementType elementType;
	};

	enum class TextureAddressMode : byte {
		Wrap = 0,
		Mirror,
		Clamp
	};
	enum class TextureFilterMode : byte {
		Nearest,
		Bilinear,
		Trilinear,
		Anisotropic,
		NearestAnisotropic,
		Default
	};

	struct ImmutableSamplerDesc {
		string* name;
		TextureFilterMode sampler_filterMode;
		byte sampler_anisotropy;
		byte sampler_shadowCmp;
		TextureAddressMode sampler_addressMode_u;
		TextureAddressMode sampler_addressMode_v;
		TextureAddressMode sampler_addressMode_w;
	};

	struct GraphicsPipelineDesc {
		string* name;

		byte blendState_colorWriteEnabled;
		byte blendState_blendMode;
		byte blendState_alphaToCoverage;

		byte rasterizerState_fillMode;
		byte rasterizerState_cullMode;
		float rasterizerState_constantDepthBias;
		float rasterizerState_slopeScaledDepthBias;
		byte rasterizerState_scissorTestEnabled;
		byte rasterizerState_lineAntiAlias;

		byte depthStencilState_enableDepth;
		byte depthStencilState_depthWriteEnabled;
		byte depthStencilState_stencilTestEnabled;
		byte depthStencilState_depthCmpFunc;
		byte depthStencilState_stencilCmpFunc;
		byte depthStencilState_stencilOpOnPassed;
		byte depthStencilState_stencilOpOnStencilFailed;
		byte depthStencilState_stencilOpOnDepthFailed;
		byte depthStencilState_stencilCmpMask;
		byte depthStencilState_stencilWriteMask;

		byte primitiveType;

		PipelineInputLayoutElementDesc* inputLayouts;
		byte numInputLayouts;

		ushort output_depthStencilFormat;
		ushort* output_rtFormats;
		byte output_numRtFormats;
		byte output_multiSample;

		ImmutableSamplerDesc* immutableSamplers;
		byte numImmutableSamplers;

		Diligent::IShader* shader_vs;
		Diligent::IShader* shader_ps;
		Diligent::IShader* shader_ds;
		Diligent::IShader* shader_hs;
		Diligent::IShader* shader_gs;
	};

	struct ComputePipelineDesc {
		string* name;

		ImmutableSamplerDesc* samplers;
		byte numSamplers;

		Diligent::IShader* shader;
	};

	struct TextureDesc {
		string* name;
		Diligent::RESOURCE_DIMENSION dimension;
		uint width;
		uint height;
		uint arraySizeOrDepth;
		ushort format;
		uint mipLevels;
		uint sampleCount;
		Diligent::BIND_FLAGS bindFlags;
		Diligent::USAGE usage;
		Diligent::CPU_ACCESS_FLAGS accessFlags;
		Diligent::MISC_TEXTURE_FLAGS textureFlags;

		ushort clear_format;
		float clear_r;
		float clear_g;
		float clear_b;
		float clear_a;

		float clear_depth;
		byte clear_stencil;
	};

	struct TextureViewDesc {
		Diligent::TEXTURE_VIEW_TYPE viewType;
		Diligent::RESOURCE_DIMENSION dimension;
		Diligent::TEXTURE_FORMAT format;
		uint mostDetailedMip;
		uint mipLevels;
		uint firstSlice;
		uint slicesCount;
		Diligent::UAV_ACCESS_FLAG accessFlags;
		byte allowMipMapGeneration;
	};

	struct TextureData {
		void* data;
		Diligent::IBuffer* srcBuffer;
		ulong srcOffset;
		ulong stride;
		ulong depthStride;
	};
}