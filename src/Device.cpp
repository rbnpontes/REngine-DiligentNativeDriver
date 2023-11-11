#include "./Device.h"
#include "./Buffer.h"
#include "./Shader.h"
#include "./PipelineState.h"
#include "./Texture.h"

RENGINE void rengine_device_create_buffer(
	Diligent::IRenderDevice* device,
	REngine::BufferDescDTO* desc,
	u64 size,
	void* data,
	REngine::Result* result
)
{
#ifdef RENGINE_DEBUG
	if (!device)
	{
		result->error = "Device is required";
		return;
	}

	if (!desc) {
		result->error = "Buffer Description is required";
		return;
	}
#endif

	Diligent::BufferDesc bufferDesc;
	rengine_buffer_fill(desc, &bufferDesc);

	Diligent::BufferData bufferData;
	bufferData.DataSize = size;
	bufferData.pData = data;

	Diligent::IBuffer* buffer = null;
	device->CreateBuffer(bufferDesc, data != null ? &bufferData : null, &buffer);

	result->value = buffer;
#ifdef RENGINE_DEBUG
	if (!buffer)
		result->error = "Could not possible to create IBuffer";
#endif
}

RENGINE void rengine_device_create_shader(
	Diligent::IRenderDevice* device,
	REngine::ShaderCreateInfoDTO* createInfo,
	REngine::Result* result
)
{
#ifdef RENGINE_DEBUG
	if (!device) 
	{
		result->error = "Device is required";
		return;
	}

	if (!createInfo) {
		result->error = "Buffer Description is required";
		return;
	}
#endif

	Diligent::ShaderCreateInfo ci;
	std::vector<Diligent::ShaderMacro> macros;
	rengine_shader_fill(createInfo, &ci, macros);

	Diligent::IShader* shader = null;
	device->CreateShader(ci, &shader);

	result->value = shader;
#ifdef RENGINE_DEBUG
	if (!shader)
		result->error = "Could not possible to create IShader";
#endif
}

RENGINE void rengine_device_create_graphicspipeline(
	Diligent::IRenderDevice* device,
	REngine::GraphicsPipelineDescDTO* desc,
	u8 isOpenGl,
	REngine::Result* result
)
{
#ifdef RENGINE_DEBUG
	if (!device) {
		result->error = "Device is required";
		return;
	}

	if (!desc) {
		result->error = "Graphics Pipeline Description is required";
		return; 
	}

	if (!desc->shader_vs || !desc->shader_ps)
	{
		result->error = "Pixel Shader and Vertex Shader is required";
		return;
	}
#endif

	Diligent::IPipelineState* pipeline = null;
	Diligent::GraphicsPipelineStateCreateInfo ci;
	std::vector<Diligent::LayoutElement> layoutElements;
	std::vector<Diligent::ImmutableSamplerDesc> immutableSamplers;

	rengine_pipelinestate_fill(desc, isOpenGl, layoutElements, immutableSamplers, &ci);
	device->CreateGraphicsPipelineState(ci, &pipeline);

	result->value = pipeline;
#ifdef RENGINE_DEBUG
	if (!pipeline)
		result->error = "Could not possible to create graphics IPipelineState";
#endif
}

RENGINE void rengine_device_create_computepipeline(
	Diligent::IRenderDevice* device,
	REngine::ComputePipelineDesc* desc,
	u8 isOpenGl,
	REngine::Result* result
)
{
#ifdef RENGINE_DEBUG
	if (!device) {
		result->error = "Device is required";
		return;
	}

	if (!desc) {
		result->error = "Compute Pipeline Description is required";
		return; 
	}

	if (!desc->shader)
	{
		result->error = "Compute Shader is required";
		return;
	}
#endif
	Diligent::IPipelineState* pipeline = null;
	Diligent::ComputePipelineStateCreateInfo ci;
	std::vector<Diligent::ImmutableSamplerDesc> samplers;
	rengine_pipelinestate_fill(desc, isOpenGl, samplers, &ci);

	device->CreateComputePipelineState(ci, &pipeline);

	result->value = pipeline;
#ifdef RENGINE_DEBUG
	if (!pipeline)
		result->error = "Could not possible to create compute IPipelineState";
#endif
}

RENGINE void rengine_device_create_pscache(Diligent::IRenderDevice* device, void* data, u64 dataSize, REngine::Result* result)
{
#ifdef RENGINE_DEBUG
	if (!device) {
		result->error = "Device is required";
		return;
	}
#endif

	Diligent::PipelineStateCacheCreateInfo ci;
	ci.CacheDataSize = dataSize;
	ci.pCacheData = data;
	ci.Desc.Mode = Diligent::PSO_CACHE_MODE_LOAD_STORE;
	ci.Desc.Name = "REngine Pipeline State Cache";
#ifdef RENGINE_DEBUG
	ci.Desc.Flags = Diligent::PSO_CACHE_FLAG_VERBOSE;
#endif

	Diligent::IPipelineStateCache* cache = null;
	device->CreatePipelineStateCache(ci, &cache);

	result->value = cache;
	if (cache == null)
		result->error = "Error has ocurred while is creating Pipeline State Cache";
}

RENGINE void rengine_device_create_texture(
	Diligent::IRenderDevice* device,
	REngine::TextureDescDTO* desc,
	REngine::TextureDataDTO* data,
	u32 numTexData,
	REngine::Result* result
)
{
#ifdef RENGINE_DEBUG
	if (!device) {
		result->error = "Device is required";
		return;
	}

	if (!desc) {
		result->error = "Texture Description is required";
		return; 
	}
#endif

	Diligent::ITexture* texture = null;
	Diligent::TextureDesc texDesc;
	Diligent::TextureData texData;
	std::vector<Diligent::TextureSubResData> subresources;

	rengine_texture_fill(desc, &texDesc);
	if(numTexData > 0)
		rengine_texture_fill(data, numTexData, subresources, &texData);

	device->CreateTexture(texDesc, numTexData > 0 ? &texData : null, &texture);

	result->value = texture;
#ifdef RENGINE_DEBUG
	if (!texture)
		result->error = "Could not possible to create texture";
#endif
}