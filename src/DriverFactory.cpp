#include "Api.h"
#include "./Structs.h"

#ifdef WIN32
#include <EngineFactoryD3D12.h>
#include <EngineFactoryD3D11.h>
#endif

#include <EngineFactoryVk.h>
#include <EngineFactoryOpenGL.h>

#include <vector>


#include "./SwapChain.h"
#include "./StringDatabase.h"

using namespace REngine;

void rengine_fill_create_info(GraphicsDriverSettings* settings, Diligent::EngineCreateInfo* ci) {
	ci->NumDeferredContexts = settings->numDeferredCtx;
	ci->AdapterId = settings->adapterId;
	ci->EnableValidation = settings->enableValidation;
}
#ifdef WIN32
void rengine_fill_d3d12_create_info(GraphicsDriverSettings* settings, Diligent::EngineD3D12CreateInfo* ci) {
	rengine_fill_create_info(settings, ci);

	memcpy(ci->CPUDescriptorHeapAllocationSize, settings->d3d12->cpuDescriptorHeapAllocationSize, sizeof(uint) * 4);
	memcpy(ci->GPUDescriptorHeapSize, settings->d3d12->gpuDescriptorHeapSize, sizeof(uint) * 2);
	memcpy(ci->GPUDescriptorHeapDynamicSize, settings->d3d12->gpuDescriptorHeapDynamicSize, sizeof(uint) * 2);
	memcpy(ci->DynamicDescriptorAllocationChunkSize, settings->d3d12->dynamicDescriptorAllocationChunkSize, sizeof(uint) * 2);
	memcpy(ci->QueryPoolSizes, settings->d3d12->queryPoolSize, sizeof(uint) * Diligent::QUERY_TYPE_NUM_TYPES);

	ci->DynamicHeapPageSize = settings->d3d12->dynamicHeapPageSize;
	ci->NumDynamicHeapPagesToReserve = settings->d3d12->numDynamicHeapPagesToReserve;
}
#endif
void rengine_fill_descriptor_pool_size(VulkanSettings::DescriptorPoolSize* desc, Diligent::VulkanDescriptorPoolSize* output) 
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
void rengine_fill_vk_create_info(GraphicsDriverSettings* settings, Diligent::EngineVkCreateInfo* ci) {
	rengine_fill_create_info(settings, ci);

	memcpy(ci->QueryPoolSizes, settings->d3d12->queryPoolSize, sizeof(uint) * Diligent::QUERY_TYPE_NUM_TYPES);
	
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
#endif
	return false;
}

RENGINE void rengine_get_available_adapter(GraphicsBackend backend, void* messageEvent, Result* result, uint* length) {
	using namespace Diligent;
	Diligent::Version graphicsVersion;

	rengine_stringdb_reset();

#ifdef WIN32
	if (backend == GraphicsBackend::D3D11 || backend == GraphicsBackend::D3D12)
		graphicsVersion = Diligent::Version{ 11, 0 };
#endif

	if (messageEvent == null) {
		result->error = "Message Event Callback is Required.";
		return;
	}

	IEngineFactory* factory = null;
	DebugMessageCallbackType callback = static_cast<DebugMessageCallbackType>(messageEvent);

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
		auto d3d12Factory = GetEngineFactoryD3D12();
		factory = d3d12Factory;
		factory->SetMessageCallback(callback);

		d3d12Factory->LoadD3D12();
	}
		break;
#endif
	case GraphicsBackend::Vulkan:
	{
		factory = GetEngineFactoryVk();
		factory->SetMessageCallback(callback);
	}
		break;
	case GraphicsBackend::OpenGL:
	{
		factory = GetEngineFactoryOpenGL();
		factory->SetMessageCallback(callback);
	}
		break;
	default:
		result->error = "Not supported this backend type.";
		break;
	}

	if (!factory)
		return;

	uint adaptersCount = 0;
	std::vector<GraphicsAdapterInfo> adapters;
	factory->EnumerateAdapters(graphicsVersion, adaptersCount, nullptr);
	adapters.resize(adaptersCount);
	factory->EnumerateAdapters(graphicsVersion, adaptersCount, adapters.data());

	factory->Release();

	GraphicsAdapter* finalAdapters = new GraphicsAdapter[adaptersCount];
	for (uint i = 0; i < adaptersCount; ++i) {
		finalAdapters[i].id = i;
		finalAdapters[i].deviceId = adapters[i].DeviceId;
		finalAdapters[i].adapterType = (AdapterType)adapters[i].Type;
		finalAdapters[i].vendorId = adapters[i].VendorId;
		finalAdapters[i].name = rengine_stringdb_store(adapters[i].Description);
	}

	result->value = finalAdapters;
	*length = adaptersCount;
}

RENGINE void rengine_create_driver(GraphicsDriverSettings* settings, SwapChainDesc* desc, Diligent::NativeWindow* nativeWnd, GraphicsDriverResult* result) 
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

	if (desc && nativeWnd == null) {
		result->error = "Native Window is required when SwapChainDesc is present.";
		return;
	}

	if (desc && nativeWnd) {
		if (!rengine_is_valid_window(nativeWnd)) {
			result->error = "Native Window is Required when SwapChainDesc is present.";
			return;
		}
	}

	IRenderDevice* renderDevice = null;
	IDeviceContext** deviceContexts = new IDeviceContext*[settings->numDeferredCtx + 1];

	IEngineFactory* factory = null;

	DebugMessageCallbackType callback = static_cast<DebugMessageCallbackType>(settings->messageCallback);

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

		IEngineFactoryD3D11* engineFactory = GetEngineFactoryD3D11();
		engineFactory->SetMessageCallback(callback);
		
		engineFactory->CreateDeviceAndContextsD3D11(ci, &renderDevice, deviceContexts);
		if (desc) {
			Diligent::SwapChainDesc swapChainDesc;
			rengine_swapchain_fill_desc(desc, &swapChainDesc);

			engineFactory->CreateSwapChainD3D11(
				renderDevice,
				deviceContexts[0],
				swapChainDesc,
				FullScreenModeDesc{},
				*nativeWnd,
				&result->swapChain
			);
		}

		factory = engineFactory;
	}
		break;
	case GraphicsBackend::D3D12:
	{
		EngineD3D12CreateInfo ci;
		rengine_fill_d3d12_create_info(settings, &ci);
		ci.GraphicsAPIVersion = { 11, 0 };

		IEngineFactoryD3D12* engineFactory = GetEngineFactoryD3D12();
		engineFactory->SetMessageCallback(callback);
		engineFactory->LoadD3D12();

		engineFactory->CreateDeviceAndContextsD3D12(ci, &renderDevice, deviceContexts);

		if (desc) {
			Diligent::SwapChainDesc swapChainDesc;
			rengine_swapchain_fill_desc(desc, &swapChainDesc);

			engineFactory->CreateSwapChainD3D12(
				renderDevice,
				deviceContexts[0],
				swapChainDesc,
				FullScreenModeDesc{},
				*nativeWnd,
				&result->swapChain
			);
		}

		factory = engineFactory;
	}
		break;
#endif
	case GraphicsBackend::Vulkan:
	{
		EngineVkCreateInfo ci;
		rengine_fill_vk_create_info(settings, &ci);

		const char* const ppIgnoreDebugMessages[] = //
			{
				// Validation Performance Warning: [ UNASSIGNED-CoreValidation-Shader-OutputNotConsumed ]
				// vertex shader writes to output location 1.0 which is not consumed by fragment shader
				"UNASSIGNED-CoreValidation-Shader-OutputNotConsumed" //
			};
		ci.ppIgnoreDebugMessageNames = ppIgnoreDebugMessages;
		ci.IgnoreDebugMessageCount   = _countof(ppIgnoreDebugMessages);

		IEngineFactoryVk* engineFactory = GetEngineFactoryVk();
		engineFactory->SetMessageCallback(callback);
	
		engineFactory->CreateDeviceAndContextsVk(ci, &renderDevice, deviceContexts);
		if (desc) {
			Diligent::SwapChainDesc swapChainDesc;
			rengine_swapchain_fill_desc(desc, &swapChainDesc);

			engineFactory->CreateSwapChainVk(
				renderDevice,
				deviceContexts[0],
				swapChainDesc,
				*nativeWnd,
				&result->swapChain
			);
		}
	}
		break;
	case GraphicsBackend::OpenGL:
	{
		if (!desc) {
			result->error = "SwapChain Description is required when Graphics Backend is OpenGL";
			delete[] deviceContexts;
			return;
		}

		Diligent::SwapChainDesc swapChainDesc;
		rengine_swapchain_fill_desc(desc, &swapChainDesc);

		EngineGLCreateInfo ci;
		rengine_fill_create_info(settings, &ci);
		ci.NumDeferredContexts = 0; // Deferred Context on OpenGL is not supported
		ci.Window = *nativeWnd;

		IDeviceContext* immediateCtx;
		IEngineFactoryOpenGL* engineFactory = GetEngineFactoryOpenGL();
		engineFactory->SetMessageCallback(callback);

		engineFactory->CreateDeviceAndSwapChainGL(
			ci, 
			&renderDevice, 
			&immediateCtx, swapChainDesc, &result->swapChain);

		deviceContexts[0] = immediateCtx;

		factory = engineFactory;
	}
		break;
	default:
	{
		result->error = "Not supported this backend type.";
		return;
	}
	}


	result->driver = new GraphicsDriver();
	result->driver->device = renderDevice;
	result->driver->factory = factory;
	result->driver->contexts = deviceContexts;
}

RENGINE void rengine_create_swapchain(SwapChainCreationInfo* creationInfo, Result* result)
{
	if (!creationInfo->device) {
		result->error = "Device is Required.";
		return;
	}

	if (!creationInfo->deviceContext) {
		result->error = "DeviceContext aka .NET(ICommandBuffer) is required.";
		return;
	}

	if (!creationInfo->factory) {
		result->error = "EngineFactory is required.";
	}

	if (!creationInfo->swapChainDesc) {
		result->error = "SwapChain Description is Required.";
		return;
	}

	if (!creationInfo->window) {
		result->error = "Window is required. How do you think you will create a SwapChain ?";
		return;
	}

	Diligent::ISwapChain* swapChain = null;
	Diligent::SwapChainDesc swapChainDesc;
	rengine_swapchain_fill_desc(creationInfo->swapChainDesc, &swapChainDesc);
	
	switch (creationInfo->backend)
	{
#ifdef WIN32
	case GraphicsBackend::D3D11:
	{
		static_cast<Diligent::IEngineFactoryD3D11*>(creationInfo->factory)->CreateSwapChainD3D11(
			creationInfo->device,
			creationInfo->deviceContext,
			swapChainDesc,
			Diligent::FullScreenModeDesc {},
			*creationInfo->window,
			&swapChain
		);
	}
		break;
	case GraphicsBackend::D3D12:
	{
		static_cast<Diligent::IEngineFactoryD3D12*>(creationInfo->factory)->CreateSwapChainD3D12(
			creationInfo->device,
			creationInfo->deviceContext,
			swapChainDesc,
			Diligent::FullScreenModeDesc {},
			*creationInfo->window,
			&swapChain
		);
	}
		break;
#endif
	case GraphicsBackend::Vulkan:
	{
		static_cast<Diligent::IEngineFactoryVk*>(creationInfo->factory)->CreateSwapChainVk(
			creationInfo->device,
			creationInfo->deviceContext,
			swapChainDesc,
			*creationInfo->window,
			&swapChain
		);
	}
		break;
	case GraphicsBackend::OpenGL:
		result->error = "Not supported SwapChain creation on OpenGL backend.";
		break;
	}

	result->value = swapChain;
}