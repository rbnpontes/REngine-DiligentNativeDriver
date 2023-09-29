#pragma once

#include "Api.h"
#include "DeviceContext.h"
#include "RenderDevice.h"

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
		string* name;
		AdapterType adapterType;
	};

	struct GraphicsDriver {
		Diligent::IRenderDevice* device;
		Diligent::IDeviceContext* immediateCtx;
		Diligent::IDeviceContext** deferredCtx;
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

		byte colorFormat;
		byte depthFormat;

		byte usage;
		byte transform;

		uint bufferCount;
		float defaultDepthValue;
		byte defaultStencilValue;
		boolean isPrimary;
	};
}