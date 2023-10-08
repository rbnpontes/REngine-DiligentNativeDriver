#pragma once
#include "./Structs.h"
#include <vector>

void rengine_pipelinestate_fill(
	REngine::GraphicsPipelineDescDTO* desc, 
	bool isOpenGl, 
	std::vector<Diligent::LayoutElement>& layoutElements,
	std::vector<Diligent::ImmutableSamplerDesc>& immutableSamplers, 
	Diligent::GraphicsPipelineStateCreateInfo* output
);
void rengine_pipelinestate_fill(
	REngine::ComputePipelineDesc* desc,
	bool isOpenGl,
	std::vector<Diligent::ImmutableSamplerDesc>& samplers,
	Diligent::ComputePipelineStateCreateInfo* output
);
void rengine_pipelinestate_fill(
	REngine::PipelineInputLayoutElementDesc* inputLayout,
	u8 numInputLayout,
	std::vector<Diligent::LayoutElement>& layoutElements
);
void rengine_pipelinestate_fill(
	REngine::ImmutableSamplerDesc* samplers,
	u8 numSamplers,
	std::vector<Diligent::ImmutableSamplerDesc>& immutableSamplers,
	Diligent::SHADER_TYPE shaderTypes = Diligent::SHADER_TYPE_ALL_GRAPHICS
);

RENGINE void rengine_pipelinestate_createresourcebinding(
	Diligent::IPipelineState* pipeline,
	REngine::Result* result
);