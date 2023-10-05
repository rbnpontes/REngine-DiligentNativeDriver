#pragma once
#include "./Structs.h"

RENGINE void rengine_device_create_buffer(
	Diligent::IRenderDevice* device,
	REngine::BufferDescDTO* desc,
	ulong size,
	void* data,
	REngine::Result* result
);
RENGINE void rengine_device_create_shader(
	Diligent::IRenderDevice* device,
	REngine::ShaderCreateInfoDTO* createInfo,
	REngine::Result* result
);
RENGINE void rengine_device_create_graphicspipeline(
	Diligent::IRenderDevice* device,
	REngine::GraphicsPipelineDescDTO* desc,
	byte isOpenGl,
	REngine::Result* result
);
RENGINE void rengine_device_create_computepipeline(
	Diligent::IRenderDevice* device,
	REngine::ComputePipelineDesc* desc,
	byte isOpenGl,
	REngine::Result* result
);
RENGINE void rengine_device_create_texture(
	Diligent::IRenderDevice* device,
	REngine::TextureDescDTO* desc,
	REngine::TextureDataDTO* data,
	uint numTexData,
	REngine::Result* result
);