#pragma once
#include "./Structs.h"

RENGINE void rengine_device_create_buffer(
	Diligent::IRenderDevice* device,
	REngine::BufferDescDTO* desc,
	u64 size,
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
	u8 isOpenGl,
	REngine::Result* result
);
RENGINE void rengine_device_create_computepipeline(
	Diligent::IRenderDevice* device,
	REngine::ComputePipelineDesc* desc,
	u8 isOpenGl,
	REngine::Result* result
);
RENGINE void rengine_device_create_pscache(
	Diligent::IRenderDevice* device,
	void* data,
	u64 dataSize,
	REngine::Result* result
);
RENGINE void rengine_device_create_texture(
	Diligent::IRenderDevice* device,
	REngine::TextureDescDTO* desc,
	REngine::TextureDataDTO* data,
	u32 numTexData,
	REngine::Result* result
);