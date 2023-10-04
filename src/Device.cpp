#include "./Device.h"
#include "./Buffer.h"
#include "./Shader.h"
#include "./PipelineState.h"
#include "./Texture.h"

RENGINE void rengine_device_create_buffer(
	Diligent::IRenderDevice* device,
	REngine::BufferDescDTO* desc,
	ulong size,
	void* data,
	REngine::Result* result
)
{
	if (!device)
	{
		result->error = "Device is required";
		return;
	}

	if (!desc) {
		result->error = "Buffer Description is required";
		return;
	}

	Diligent::BufferDesc bufferDesc;
	rengine_buffer_fill(desc, &bufferDesc);

	Diligent::BufferData bufferData;
	bufferData.DataSize = size;
	bufferData.pData = data;

	Diligent::IBuffer* buffer = null;
	device->CreateBuffer(bufferDesc, data != null ? &bufferData : null, &buffer);

	result->value = buffer;
	if (!buffer)
		result->error = "Could not possible to create IBuffer";
}

RENGINE void rengine_device_create_shader(
	Diligent::IRenderDevice* device,
	REngine::ShaderCreateInfo* createInfo,
	REngine::Result* result
)
{
	if (!device) 
	{
		result->error = "Device is required";
		return;
	}

	if (!createInfo) {
		result->error = "Buffer Description is required";
		return;
	}

	Diligent::ShaderCreateInfo ci;
	std::vector<Diligent::ShaderMacro> macros;
	rengine_shader_fill(createInfo, &ci, macros);

	Diligent::IShader* shader = null;
	device->CreateShader(ci, &shader);

	result->value = shader;
	if (!shader)
		result->error = "Could not possible to create IShader";
}

RENGINE void rengine_device_create_graphicspipeline(
	Diligent::IRenderDevice* device,
	REngine::GraphicsPipelineDesc* desc,
	byte isOpenGl,
	REngine::Result* result
)
{
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

	Diligent::IPipelineState* pipeline = null;
	Diligent::GraphicsPipelineStateCreateInfo ci;
	std::vector<Diligent::LayoutElement> layoutElements;
	std::vector<Diligent::ImmutableSamplerDesc> immutableSamplers;

	rengine_pipelinestate_fill(desc, isOpenGl, layoutElements, immutableSamplers, &ci);
	device->CreateGraphicsPipelineState(ci, &pipeline);

	result->value = pipeline;
	if (!pipeline)
		result->error = "Could not possible to create graphics IPipelineState";
}

RENGINE void rengine_device_create_computepipeline(
	Diligent::IRenderDevice* device,
	REngine::ComputePipelineDesc* desc,
	byte isOpenGl,
	REngine::Result* result
)
{
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

	Diligent::IPipelineState* pipeline = null;
	Diligent::ComputePipelineStateCreateInfo ci;
	std::vector<Diligent::ImmutableSamplerDesc> samplers;
	rengine_pipelinestate_fill(desc, isOpenGl, samplers, &ci);

	device->CreateComputePipelineState(ci, &pipeline);

	result->value = pipeline;
	if (!pipeline)
		result->error = "Could not possible to create compute IPipelineState";
}

RENGINE void rengine_device_create_texture(
	Diligent::IRenderDevice* device,
	REngine::TextureDesc* desc,
	REngine::TextureData* data,
	uint numTexData,
	REngine::Result* result
)
{
	if (!device) {
		result->error = "Device is required";
		return;
	}

	if (!desc) {
		result->error = "Texture Description is required";
		return; 
	}

	Diligent::ITexture* texture = null;
	Diligent::TextureDesc texDesc;
	Diligent::TextureData texData;
	std::vector<Diligent::TextureSubResData> subresources;

	rengine_texture_fill(desc, &texDesc);
	if(numTexData > 0)
		rengine_texture_fill(data, numTexData, subresources, &texData);

	device->CreateTexture(texDesc, numTexData > 0 ? &texData : null, &texture);

	result->value = texture;
	if (!texture)
		result->error = "Could not possible to create texture";
}