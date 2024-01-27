#include "Api.h"
#include "./Structs.h"

#ifdef WIN32
#include <EngineFactoryD3D12.h>
#include <EngineFactoryD3D11.h>
#endif

#ifndef __EMSCRIPTEN__
	#include <EngineFactoryVk.h>
#endif
#include <EngineFactoryOpenGL.h>

#include <vector>


#include "./SwapChain.h"
#include "./StringDatabase.h"

using namespace REngine;

void rengine_fill_create_info(const GraphicsDriverSettings* settings, Diligent::EngineCreateInfo* ci) {
	ci->NumDeferredContexts = settings->numDeferredCtx;
	ci->AdapterId = settings->adapterId;
	ci->EnableValidation = settings->enableValidation;
}
#ifdef WIN32
void rengine_fill_d3d12_create_info(const GraphicsDriverSettings* settings, Diligent::EngineD3D12CreateInfo* ci) {
	rengine_fill_create_info(settings, ci);

	memcpy(ci->CPUDescriptorHeapAllocationSize, settings->d3d12->cpuDescriptorHeapAllocationSize, sizeof(u32) * 4);
	memcpy(ci->GPUDescriptorHeapSize, settings->d3d12->gpuDescriptorHeapSize, sizeof(u32) * 2);
	memcpy(ci->GPUDescriptorHeapDynamicSize, settings->d3d12->gpuDescriptorHeapDynamicSize, sizeof(u32) * 2);
	memcpy(ci->DynamicDescriptorAllocationChunkSize, settings->d3d12->dynamicDescriptorAllocationChunkSize, sizeof(u32) * 2);
	memcpy(ci->QueryPoolSizes, settings->d3d12->queryPoolSize, sizeof(u32) * Diligent::QUERY_TYPE_NUM_TYPES);

	ci->DynamicHeapPageSize = settings->d3d12->dynamicHeapPageSize;
	ci->NumDynamicHeapPagesToReserve = settings->d3d12->numDynamicHeapPagesToReserve;
}
#endif
#ifndef __EMSCRIPTEN__
void rengine_fill_descriptor_pool_size(const VulkanSettings::DescriptorPoolSize* desc, Diligent::VulkanDescriptorPoolSize* output) 
{
	output->MaxDescriptorSets = desc->max;
	output->NumSeparateSamplerDescriptors = desc->sepSm;
	output->NumCombinedSamplerDescriptors = desc->cmbSm;
	output->NumSampledImageDescriptors = desc->smpImg;
	output->NumStorageImageDescriptors = desc->strImg;
	output->NumUniformBufferDescriptors = desc->ub;
	output->NumUniformTexelBufferDescriptors = desc->utxb;
	output->NumStorageTexelBufferDescriptors = desc->stTxB;
	output->NumInputAttachmentDescriptors = desc->inptAtt;
	output->NumAccelStructDescriptors = desc->accelSt;
}

void rengine_fill_vk_create_info(const GraphicsDriverSettings* settings, Diligent::EngineVkCreateInfo* ci) {
	rengine_fill_create_info(settings, ci);

	memcpy(ci->QueryPoolSizes, settings->vulkan->queryPoolSizes, sizeof(u32) * Diligent::QUERY_TYPE_NUM_TYPES);
	
	ci->InstanceLayerCount = settings->vulkan->instanceLayerNamesCount;
	ci->ppInstanceLayerNames = settings->vulkan->instanceLayerNames;

	ci->InstanceExtensionCount = settings->vulkan->instanceExtensionNamesCount;
	ci->ppInstanceExtensionNames = settings->vulkan->instanceExtensionNames;


	ci->DeviceExtensionCount = settings->vulkan->deviceExtensionNamesCount;
	ci->ppDeviceExtensionNames = settings->vulkan->deviceExtensionNames;
	
	ci->IgnoreDebugMessageCount = settings->vulkan->ignoreDebugMessageNamesCount;
	ci->ppIgnoreDebugMessageNames = settings->vulkan->ignoreDebugMessageNames;

	rengine_fill_descriptor_pool_size(settings->vulkan->mainDescriptorPoolSize, &ci->MainDescriptorPoolSize);
	rengine_fill_descriptor_pool_size(settings->vulkan->dynamicDescriptorPoolSize, &ci->DynamicDescriptorPoolSize);

	ci->DeviceLocalMemoryPageSize = settings->vulkan->deviceLocalMemoryPageSize;
	ci->HostVisibleMemoryPageSize = settings->vulkan->hostVisibleMemoryPageSize;
	ci->DeviceLocalMemoryReserveSize = settings->vulkan->deviceLocalMemoryReserveSize;
	ci->HostVisibleMemoryReserveSize = settings->vulkan->hostVisibleMemoryReserveSize;
	ci->UploadHeapPageSize = settings->vulkan->uploadHeapPageSize;
	ci->DynamicHeapPageSize = settings->vulkan->dynamicHeapPageSize;
	ci->DynamicHeapSize = settings->vulkan->dynamicHeapSize;
	ci->DynamicHeapPageSize = settings->vulkan->dynamicHeapPageSize;
}
#endif

bool rengine_is_valid_window(Diligent::NativeWindow* window) {
#ifdef PLATFORM_WIN32
	return window->hWnd != null;
#elif PLATFORM_UNIVERSAL_WINDOWS
	return window->pCoreWindow != null;
#elif PLATFORM_ANDROID
	return window->pAWindow != null;
#elif PLATFORM_LINUX
	return window->WindowId != 0 || window->pDisplay != null || window->pXCBConnection != null;
#elif PLATFORM_MACOS
	return window->pNSView != null;
#elif PLATFORM_EMSCRIPTEN
	return window->pCanvasId != null;
#else
	return false;
#endif
}

#ifndef __EMSCRIPTEN__
RENGINE void rengine_get_available_adapter(GraphicsBackend backend, void* messageEvent, Result* result, u32* length) {
	using namespace Diligent;
	Diligent::Version graphics_version;

	rengine_stringdb_reset();

#ifdef WIN32
	if (backend == GraphicsBackend::D3D11 || backend == GraphicsBackend::D3D12)
		graphics_version = Diligent::Version{ 11, 0 };
#endif

	if (messageEvent == null) {
		result->error = "Message Event Callback is Required.";
		return;
	}

	IEngineFactory* factory = null;
	const auto callback = reinterpret_cast<DebugMessageCallbackType>(messageEvent);

	switch (backend)
	{
#ifdef WIN32
	case GraphicsBackend::D3D11:
	{
		factory = GetEngineFactoryD3D11();
		factory->SetMessageCallback(callback);
	}
		break;
	case GraphicsBackend::D3D12:
	{
		const auto d3d12_factory = GetEngineFactoryD3D12();
		factory = d3d12_factory;
		factory->SetMessageCallback(callback);

		d3d12_factory->LoadD3D12();
	}
		break;
#endif
#ifndef __EMSCRIPTEN__
	case GraphicsBackend::Vulkan:
	{
		factory = GetEngineFactoryVk();
		factory->SetMessageCallback(callback);
	}
		break;
#endif
	case GraphicsBackend::OpenGL:
	{
		factory = GetEngineFactoryOpenGL();
		factory->SetMessageCallback(callback);
	}
		break;
	case GraphicsBackend::Unknow:
	case GraphicsBackend::Metal:
	case GraphicsBackend::Software:
		result->error = "Not supported this backend type.";
		break;
	}

	if (!factory)
		return;

	u32 adapters_count = 0;
	std::vector<GraphicsAdapterInfo> adapters;
	factory->EnumerateAdapters(graphics_version, adapters_count, nullptr);
	adapters.resize(adapters_count);
	factory->EnumerateAdapters(graphics_version, adapters_count, adapters.data());

	factory->Release();

	const auto final_adapters = new GraphicsAdapter[adapters_count];
	for (u32 i = 0; i < adapters_count; ++i) {
		GraphicsAdapter adapter;
		adapter.id = i;
		adapter.deviceId = adapters[i].DeviceId;
		adapter.adapterType = static_cast<AdapterType>(adapters[i].Type);
		adapter.vendorId = adapters[i].VendorId;
		adapter.name = rengine_stringdb_store(adapters[i].Description);
		adapter.localMemory = adapters[i].Memory.LocalMemory;
		adapter.hostVisibleMemory = adapters[i].Memory.HostVisibleMemory;
		adapter.unifiedMemory = adapters[i].Memory.UnifiedMemory;
		adapter.maxMemoryAlloc = adapters[i].Memory.MaxMemoryAllocation;
		adapter.unifiedMemoryCpuAccess = adapters[i].Memory.UnifiedMemoryCPUAccess;
		adapter.memoryLessTextureBindFlags = adapters[i].Memory.MemorylessTextureBindFlags;

		final_adapters[i] = adapter;
	}

	result->value = final_adapters;
	*length = adapters_count;
}
#endif

RENGINE void rengine_create_driver(GraphicsDriverSettings* settings, SwapChainDesc* desc, Diligent::NativeWindow* native_wnd, GraphicsDriverResult* result) 
{
	using namespace Diligent;

	if (!settings) {
		result->error = "Driver Settings is required";
		return;
	}

	if (!settings->messageCallback) {
		result->error = "Message Event Callback is required";
		return;
	}

	if (desc && native_wnd == null) {
		result->error = "Native Window is required when SwapChainDesc is present.";
		return;
	}

	if (desc && native_wnd) {
		if (!rengine_is_valid_window(native_wnd)) {
			result->error = "Native Window is Required when SwapChainDesc is present.";
			return;
		}
	}

	IRenderDevice* render_device = null;
	auto device_contexts = new IDeviceContext*[settings->numDeferredCtx + 1];

	IEngineFactory* factory = null;

	auto callback = reinterpret_cast<DebugMessageCallbackType>(settings->messageCallback);

	switch (settings->backend)
	{
#if WIN32
	case GraphicsBackend::D3D11:
	{
		EngineD3D11CreateInfo ci;
		rengine_fill_create_info(settings, &ci);
		ci.GraphicsAPIVersion = { 11, 0 };
		if (ci.EnableValidation)
			ci.SetValidationLevel(VALIDATION_LEVEL_2);

		IEngineFactoryD3D11* engine_factory = GetEngineFactoryD3D11();
		engine_factory->SetMessageCallback(callback);
		
		engine_factory->CreateDeviceAndContextsD3D11(ci, &render_device, device_contexts);
		if (desc) {
			Diligent::SwapChainDesc swapChainDesc;
			rengine_swapchain_fill_desc(desc, &swapChainDesc);

			engine_factory->CreateSwapChainD3D11(
				render_device,
				device_contexts[0],
				swapChainDesc,
				FullScreenModeDesc{},
				*native_wnd,
				&result->swapChain
			);
		}

		factory = engine_factory;
	}
		break;
	case GraphicsBackend::D3D12:
	{
		EngineD3D12CreateInfo ci;
		rengine_fill_d3d12_create_info(settings, &ci);
		ci.GraphicsAPIVersion = { 11, 0 };

		IEngineFactoryD3D12* engine_factory = GetEngineFactoryD3D12();
		engine_factory->SetMessageCallback(callback);
		engine_factory->LoadD3D12();

		engine_factory->CreateDeviceAndContextsD3D12(ci, &render_device, device_contexts);

		if (desc) {
			Diligent::SwapChainDesc swapchain_desc;
			rengine_swapchain_fill_desc(desc, &swapchain_desc);

			engine_factory->CreateSwapChainD3D12(
				render_device,
				device_contexts[0],
				swapchain_desc,
				FullScreenModeDesc{},
				*native_wnd,
				&result->swapChain
			);
		}

		factory = engine_factory;
	}
		break;
#endif
#ifndef __EMSCRIPTEN__
	case GraphicsBackend::Vulkan:
	{
		EngineVkCreateInfo ci;
		rengine_fill_vk_create_info(settings, &ci);

		// ReSharper disable once CppVariableCanBeMadeConstexpr
		const char* const ignore_debug_messages[] = //
			{
				// Validation Performance Warning: [ UNASSIGNED-CoreValidation-Shader-OutputNotConsumed ]
				// vertex shader writes to output location 1.0 which is not consumed by fragment shader
				"UNASSIGNED-CoreValidation-Shader-OutputNotConsumed" //
			};
		ci.ppIgnoreDebugMessageNames = ignore_debug_messages;
		ci.IgnoreDebugMessageCount   = _countof(ignore_debug_messages);

		IEngineFactoryVk* engine_factory = GetEngineFactoryVk();
		engine_factory->SetMessageCallback(callback);
	
		engine_factory->CreateDeviceAndContextsVk(ci, &render_device, device_contexts);
		if (desc) {
			Diligent::SwapChainDesc swapchain_desc;
			rengine_swapchain_fill_desc(desc, &swapchain_desc);

			engine_factory->CreateSwapChainVk(
				render_device,
				device_contexts[0],
				swapchain_desc,
				*native_wnd,
				&result->swapChain
			);
		}
	}
		break;
#endif
	case GraphicsBackend::OpenGL:
	{
		if (!desc) {
			result->error = "SwapChain Description is required when Graphics Backend is OpenGL";
			delete[] device_contexts;
			return;
		}

		Diligent::SwapChainDesc swapchain_desc;
		rengine_swapchain_fill_desc(desc, &swapchain_desc);

		EngineGLCreateInfo ci;
		rengine_fill_create_info(settings, &ci);
		ci.NumDeferredContexts = 0; // Deferred Context on OpenGL is not supported
		ci.Window = *native_wnd;

		IDeviceContext* immediateCtx;
		IEngineFactoryOpenGL* engine_factory = GetEngineFactoryOpenGL();
		engine_factory->SetMessageCallback(callback);

		engine_factory->CreateDeviceAndSwapChainGL(
			ci, 
			&render_device, 
			&immediateCtx, swapchain_desc, &result->swapChain);

		device_contexts[0] = immediateCtx;

		factory = engine_factory;
	}
		break;
	case GraphicsBackend::Unknow:
	case GraphicsBackend::Metal:
	case GraphicsBackend::Software:
	{
		result->error = "Not supported this backend type.";
		return;
	}
	}

	result->driver = new GraphicsDriver();
	result->driver->device = render_device;
	result->driver->factory = factory;
	result->driver->contexts = device_contexts;
}

#ifndef __EMSCRIPTEN__
RENGINE void rengine_create_swapchain(const SwapChainCreationInfo* creation_info, Result* result)
{
	if (!creation_info->device) {
		result->error = "Device is Required.";
		return;
	}

	if (!creation_info->deviceContext) {
		result->error = "DeviceContext aka .NET(ICommandBuffer) is required.";
		return;
	}

	if (!creation_info->factory) {
		result->error = "EngineFactory is required.";
	}

	if (!creation_info->swapChainDesc) {
		result->error = "SwapChain Description is Required.";
		return;
	}

	if (!creation_info->window) {
		result->error = "Window is required. How do you think you will create a SwapChain ?";
		return;
	}

	Diligent::ISwapChain* swapChain = null;
	Diligent::SwapChainDesc swapChainDesc;
	rengine_swapchain_fill_desc(creation_info->swapChainDesc, &swapChainDesc);
	
	switch (creation_info->backend)
	{
#ifdef WIN32
	case GraphicsBackend::D3D11:
	{
		dynamic_cast<Diligent::IEngineFactoryD3D11*>(creation_info->factory)->CreateSwapChainD3D11(
			creation_info->device,
			creation_info->deviceContext,
			swapChainDesc,
			Diligent::FullScreenModeDesc {},
			*creation_info->window,
			&swapChain
		);
	}
		break;
	case GraphicsBackend::D3D12:
	{
		dynamic_cast<Diligent::IEngineFactoryD3D12*>(creation_info->factory)->CreateSwapChainD3D12(
			creation_info->device,
			creation_info->deviceContext,
			swapChainDesc,
			Diligent::FullScreenModeDesc {},
			*creation_info->window,
			&swapChain
		);
	}
		break;
#endif
#ifndef __EMSCRIPTEN__
	case GraphicsBackend::Vulkan:
	{
		dynamic_cast<Diligent::IEngineFactoryVk*>(creation_info->factory)->CreateSwapChainVk(
			creation_info->device,
			creation_info->deviceContext,
			swapChainDesc,
			*creation_info->window,
			&swapChain
		);
	}
		break;
#endif
	case GraphicsBackend::OpenGL:
		result->error = "Not supported SwapChain creation on OpenGL backend.";
		break;
	case GraphicsBackend::Unknow:
	case GraphicsBackend::Metal:
	case GraphicsBackend::Software:
		result->error = "Not supported this Graphics backend.";
		break;
	}

	result->value = swapChain;
}
#endif

