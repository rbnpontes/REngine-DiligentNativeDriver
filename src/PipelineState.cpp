#include "./PipelineState.h"
#include "./Definitions.h"
static Diligent::PRIMITIVE_TOPOLOGY s_primitiveTopologies[] = {
	Diligent::PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
	Diligent::PRIMITIVE_TOPOLOGY_LINE_LIST,
	Diligent::PRIMITIVE_TOPOLOGY_POINT_LIST,
	Diligent::PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP,
	Diligent::PRIMITIVE_TOPOLOGY_LINE_STRIP
};
static Diligent::COMPARISON_FUNCTION s_comparisonFunctions[] = {
	Diligent::COMPARISON_FUNC_ALWAYS,
	Diligent::COMPARISON_FUNC_EQUAL,
	Diligent::COMPARISON_FUNC_NOT_EQUAL,
	Diligent::COMPARISON_FUNC_LESS,
	Diligent::COMPARISON_FUNC_LESS_EQUAL,
	Diligent::COMPARISON_FUNC_GREATER,
	Diligent::COMPARISON_FUNC_GREATER_EQUAL
};
static bool s_isBlendEnabled[] = {
	false,
	true,
	true,
	true,
	true,
	true,
	true,
	true,
	true,
	true,
};
static Diligent::BLEND_FACTOR s_sourceBlends[] = {
	Diligent::BLEND_FACTOR_ONE,
	Diligent::BLEND_FACTOR_ONE,
	Diligent::BLEND_FACTOR_DEST_COLOR,
	Diligent::BLEND_FACTOR_SRC_ALPHA,
	Diligent::BLEND_FACTOR_SRC_ALPHA,
	Diligent::BLEND_FACTOR_ONE,
	Diligent::BLEND_FACTOR_INV_DEST_ALPHA,
	Diligent::BLEND_FACTOR_ONE,
	Diligent::BLEND_FACTOR_SRC_ALPHA,
	Diligent::BLEND_FACTOR_SRC_ALPHA,
};
static Diligent::BLEND_FACTOR s_destBlends[] = {
	Diligent::BLEND_FACTOR_ZERO,
	Diligent::BLEND_FACTOR_ONE,
	Diligent::BLEND_FACTOR_ZERO,
	Diligent::BLEND_FACTOR_INV_SRC_ALPHA,
	Diligent::BLEND_FACTOR_ONE,
	Diligent::BLEND_FACTOR_INV_SRC_ALPHA,
	Diligent::BLEND_FACTOR_DEST_ALPHA,
	Diligent::BLEND_FACTOR_ONE,
	Diligent::BLEND_FACTOR_ONE,
	Diligent::BLEND_FACTOR_INV_SRC_ALPHA,
};
static Diligent::BLEND_FACTOR s_sourceAlphaBlends[] = {
	Diligent::BLEND_FACTOR_ONE,
	Diligent::BLEND_FACTOR_ONE,
	Diligent::BLEND_FACTOR_DEST_COLOR,
	Diligent::BLEND_FACTOR_SRC_ALPHA,
	Diligent::BLEND_FACTOR_SRC_ALPHA,
	Diligent::BLEND_FACTOR_ONE,
	Diligent::BLEND_FACTOR_INV_DEST_ALPHA,
	Diligent::BLEND_FACTOR_ONE,
	Diligent::BLEND_FACTOR_SRC_ALPHA,
	Diligent::BLEND_FACTOR_ZERO,
};
static Diligent::BLEND_FACTOR s_destAlphaBlends[] = {
	Diligent::BLEND_FACTOR_ZERO,
	Diligent::BLEND_FACTOR_ONE,
	Diligent::BLEND_FACTOR_ZERO,
	Diligent::BLEND_FACTOR_INV_SRC_ALPHA,
	Diligent::BLEND_FACTOR_ONE,
	Diligent::BLEND_FACTOR_INV_SRC_ALPHA,
	Diligent::BLEND_FACTOR_DEST_ALPHA,
	Diligent::BLEND_FACTOR_ONE,
	Diligent::BLEND_FACTOR_ONE,
	Diligent::BLEND_FACTOR_INV_SRC_ALPHA,
};
static Diligent::BLEND_OPERATION s_blendOperations[] = {
	Diligent::BLEND_OPERATION_ADD,
	Diligent::BLEND_OPERATION_ADD,
	Diligent::BLEND_OPERATION_ADD,
	Diligent::BLEND_OPERATION_ADD,
	Diligent::BLEND_OPERATION_ADD,
	Diligent::BLEND_OPERATION_ADD,
	Diligent::BLEND_OPERATION_ADD,
	Diligent::BLEND_OPERATION_REV_SUBTRACT,
	Diligent::BLEND_OPERATION_REV_SUBTRACT,
	Diligent::BLEND_OPERATION_ADD,
};
static Diligent::STENCIL_OP s_stencilOperations[] = {
	Diligent::STENCIL_OP_KEEP,
	Diligent::STENCIL_OP_ZERO,
	Diligent::STENCIL_OP_REPLACE,
	Diligent::STENCIL_OP_INCR_WRAP,
	Diligent::STENCIL_OP_DECR_WRAP,
};
static Diligent::CULL_MODE s_cullMode[] = {
	Diligent::CULL_MODE_NONE,
	Diligent::CULL_MODE_BACK,
	Diligent::CULL_MODE_FRONT
};
static Diligent::FILL_MODE s_fillMode[] = {
	Diligent::FILL_MODE_SOLID,
	Diligent::FILL_MODE_WIREFRAME,
};
static u32 s_numComponents[] = {
	1,
	1,
	2,
	3,
	4,
	4,
	4
};
static Diligent::VALUE_TYPE s_valueTypes[] = {
	Diligent::VT_INT32,
	Diligent::VT_FLOAT32,
	Diligent::VT_FLOAT32,
	Diligent::VT_FLOAT32,
	Diligent::VT_FLOAT32,
	Diligent::VT_UINT8,
	Diligent::VT_UINT8,
};
static bool s_isNormalized[] = {
	false,
	false,
	false,
	false,
	false,
	false,
	true
};
static Diligent::FILTER_TYPE s_minMagFilters[][2] = {
	{ Diligent::FILTER_TYPE_POINT, Diligent::FILTER_TYPE_COMPARISON_POINT },
	{ Diligent::FILTER_TYPE_LINEAR, Diligent::FILTER_TYPE_COMPARISON_LINEAR },
	{ Diligent::FILTER_TYPE_LINEAR, Diligent::FILTER_TYPE_COMPARISON_LINEAR },
	{ Diligent::FILTER_TYPE_ANISOTROPIC, Diligent::FILTER_TYPE_COMPARISON_ANISOTROPIC },
	{ Diligent::FILTER_TYPE_POINT, Diligent::FILTER_TYPE_COMPARISON_POINT },
};
static Diligent::FILTER_TYPE s_mipFilters[][2] = {
	{ Diligent::FILTER_TYPE_POINT, Diligent::FILTER_TYPE_COMPARISON_POINT },
	{ Diligent::FILTER_TYPE_POINT, Diligent::FILTER_TYPE_COMPARISON_POINT },
	{ Diligent::FILTER_TYPE_LINEAR, Diligent::FILTER_TYPE_COMPARISON_LINEAR },
	{ Diligent::FILTER_TYPE_ANISOTROPIC, Diligent::FILTER_TYPE_COMPARISON_ANISOTROPIC },
	{ Diligent::FILTER_TYPE_LINEAR, Diligent::FILTER_TYPE_LINEAR },
};
static Diligent::TEXTURE_ADDRESS_MODE s_addressModes[] = {
	Diligent::TEXTURE_ADDRESS_WRAP,
	Diligent::TEXTURE_ADDRESS_MIRROR,
	Diligent::TEXTURE_ADDRESS_CLAMP,
};

void rengine_pipelinestate_fill(
	REngine::GraphicsPipelineDescDTO* desc, 
	bool isOpenGl, 
	std::vector<Diligent::LayoutElement>& layoutElements,
	std::vector<Diligent::ImmutableSamplerDesc>& immutableSamplers, 
	Diligent::GraphicsPipelineStateCreateInfo* output
)
{
	output->PSODesc.Name = desc->name;

	output->pVS = desc->shader_vs;
	output->pPS = desc->shader_ps;
	output->pDS = desc->shader_ds;
	output->pHS = desc->shader_hs;
	output->pGS = desc->shader_gs;

	rengine_pipelinestate_fill(desc->inputLayouts, desc->numInputLayouts, layoutElements);
	rengine_pipelinestate_fill(desc->immutableSamplers, desc->numImmutableSamplers, immutableSamplers);

	output->GraphicsPipeline.InputLayout.LayoutElements = layoutElements.data();
	output->GraphicsPipeline.InputLayout.NumElements = layoutElements.size();

	output->PSODesc.ResourceLayout.ImmutableSamplers = immutableSamplers.data();
	output->PSODesc.ResourceLayout.NumImmutableSamplers = immutableSamplers.size();

	output->GraphicsPipeline.PrimitiveTopology = s_primitiveTopologies[desc->primitiveType];
	output->GraphicsPipeline.NumRenderTargets = desc->output_numRtFormats;
	for (u8 i = 0; i < desc->output_numRtFormats; ++i)
		output->GraphicsPipeline.RTVFormats[i] = (Diligent::TEXTURE_FORMAT)desc->output_rtFormats[i];
	output->GraphicsPipeline.DSVFormat = (Diligent::TEXTURE_FORMAT)desc->output_depthStencilFormat;
	output->GraphicsPipeline.SmplDesc.Count = desc->output_multiSample;

	output->GraphicsPipeline.BlendDesc.AlphaToCoverageEnable = desc->blendState_alphaToCoverage;
	output->GraphicsPipeline.BlendDesc.IndependentBlendEnable = false;

	if (desc->output_numRtFormats > 0) {
		u8 blendModeIdx = desc->blendState_blendMode;
		output->GraphicsPipeline.BlendDesc.RenderTargets[0].BlendEnable = s_isBlendEnabled[blendModeIdx];
		output->GraphicsPipeline.BlendDesc.RenderTargets[0].SrcBlend = s_sourceBlends[blendModeIdx];
		output->GraphicsPipeline.BlendDesc.RenderTargets[0].DestBlend = s_destBlends[blendModeIdx];
		output->GraphicsPipeline.BlendDesc.RenderTargets[0].BlendOp = s_blendOperations[blendModeIdx];
		output->GraphicsPipeline.BlendDesc.RenderTargets[0].SrcBlendAlpha = s_sourceAlphaBlends[blendModeIdx];
		output->GraphicsPipeline.BlendDesc.RenderTargets[0].DestBlendAlpha = s_destAlphaBlends[blendModeIdx];
		output->GraphicsPipeline.BlendDesc.RenderTargets[0].BlendOpAlpha = s_blendOperations[blendModeIdx];
		output->GraphicsPipeline.BlendDesc.RenderTargets[0].RenderTargetWriteMask = desc->blendState_colorWriteEnabled
			? Diligent::COLOR_MASK_ALL
			: Diligent::COLOR_MASK_NONE;
	}

	output->GraphicsPipeline.DepthStencilDesc.DepthEnable = desc->depthStencilState_enableDepth;
	output->GraphicsPipeline.DepthStencilDesc.DepthWriteEnable = desc->depthStencilState_depthWriteEnabled;
	output->GraphicsPipeline.DepthStencilDesc.DepthFunc = s_comparisonFunctions[desc->depthStencilState_depthCmpFunc];
	output->GraphicsPipeline.DepthStencilDesc.StencilEnable = desc->depthStencilState_stencilTestEnabled;
	output->GraphicsPipeline.DepthStencilDesc.StencilReadMask = desc->depthStencilState_stencilCmpMask;
	output->GraphicsPipeline.DepthStencilDesc.StencilWriteMask = desc->depthStencilState_stencilWriteMask;

	Diligent::StencilOpDesc& frontFace = output->GraphicsPipeline.DepthStencilDesc.FrontFace;
	Diligent::StencilOpDesc& backFace = output->GraphicsPipeline.DepthStencilDesc.BackFace;
	
	frontFace.StencilFailOp = s_stencilOperations[desc->depthStencilState_stencilOpOnStencilFailed];
	frontFace.StencilDepthFailOp = s_stencilOperations[desc->depthStencilState_stencilOpOnDepthFailed];
	frontFace.StencilPassOp = s_stencilOperations[desc->depthStencilState_stencilOpOnPassed];
	frontFace.StencilFunc = s_comparisonFunctions[desc->depthStencilState_stencilCmpFunc];

	backFace.StencilFailOp = s_stencilOperations[desc->depthStencilState_stencilOpOnStencilFailed];
	backFace.StencilDepthFailOp = s_stencilOperations[desc->depthStencilState_stencilOpOnDepthFailed];
	backFace.StencilPassOp = s_stencilOperations[desc->depthStencilState_stencilOpOnPassed];
	backFace.StencilFunc = s_comparisonFunctions[desc->depthStencilState_stencilCmpFunc];

	u32 depthBits = 24;
	if (desc->output_depthStencilFormat == Diligent::TEX_FORMAT_D16_UNORM)
		depthBits = 16;

	int scaledDepthBias = isOpenGl ? 0 : (int)(desc->rasterizerState_constantDepthBias * (1 << depthBits));

	output->GraphicsPipeline.RasterizerDesc.FillMode = s_fillMode[desc->rasterizerState_fillMode];
	output->GraphicsPipeline.RasterizerDesc.CullMode = s_cullMode[desc->rasterizerState_cullMode];
	output->GraphicsPipeline.RasterizerDesc.FrontCounterClockwise = false;
	output->GraphicsPipeline.RasterizerDesc.DepthBias = scaledDepthBias;
	output->GraphicsPipeline.RasterizerDesc.SlopeScaledDepthBias = desc->rasterizerState_slopeScaledDepthBias;
	output->GraphicsPipeline.RasterizerDesc.DepthClipEnable = true;
	output->GraphicsPipeline.RasterizerDesc.ScissorEnable = desc->rasterizerState_scissorTestEnabled;
	output->GraphicsPipeline.RasterizerDesc.AntialiasedLineEnable = !isOpenGl && desc->rasterizerState_lineAntiAlias;
	
	output->PSODesc.ResourceLayout.DefaultVariableType = Diligent::SHADER_RESOURCE_VARIABLE_TYPE_MUTABLE;
}

void rengine_pipelinestate_fill(
	REngine::ComputePipelineDesc* desc,
	bool isOpenGl,
	std::vector<Diligent::ImmutableSamplerDesc>& samplers,
	Diligent::ComputePipelineStateCreateInfo* output
)
{
	output->PSODesc.Name = desc->name;
	output->pCS = desc->shader;
	
	output->PSODesc.ResourceLayout.DefaultVariableType = Diligent::SHADER_RESOURCE_VARIABLE_TYPE_MUTABLE;

	rengine_pipelinestate_fill(desc->samplers, desc->numSamplers, samplers);
	output->PSODesc.ResourceLayout.ImmutableSamplers = samplers.data();
	output->PSODesc.ResourceLayout.NumImmutableSamplers = desc->numSamplers;
}

void rengine_pipelinestate_fill(
	REngine::PipelineInputLayoutElementDesc* inputLayout,
	u8 numInputLayout,
	std::vector<Diligent::LayoutElement>& layoutElements
)
{
	layoutElements.resize(numInputLayout);

	for (u8 i = 0; i < numInputLayout; ++i) 
	{
		layoutElements[i].InputIndex = inputLayout[i].inputIndex;
		layoutElements[i].RelativeOffset = inputLayout[i].elementOffset;
		layoutElements[i].NumComponents = s_numComponents[(u8)inputLayout[i].elementType];
		layoutElements[i].ValueType = s_valueTypes[(u8)inputLayout[i].elementType];
		layoutElements[i].IsNormalized = s_isNormalized[(u8)inputLayout[i].elementType];
		layoutElements[i].BufferSlot = inputLayout[i].bufferIndex;
		layoutElements[i].Stride = inputLayout[i].bufferStride;
		layoutElements[i].Frequency = inputLayout[i].instanceStepRate != 0
			? Diligent::INPUT_ELEMENT_FREQUENCY_PER_INSTANCE
			: Diligent::INPUT_ELEMENT_FREQUENCY_PER_VERTEX;
		layoutElements[i].InstanceDataStepRate = inputLayout[i].instanceStepRate;
	}
}

void rengine_pipelinestate_fill(
	REngine::ImmutableSamplerDesc* samplers,
	u8 numSamplers,
	std::vector<Diligent::ImmutableSamplerDesc>& immutableSamplers,
	Diligent::SHADER_TYPE shaderTypes
)
{
	immutableSamplers.resize(numSamplers);

	for (u8 i = 0; i < numSamplers; ++i) {
		u8 filterModeIdx = (u8)(samplers[i].sampler_filterMode == REngine::TextureFilterMode::Default
			? SAMPLER_DEFAULT_FILTER_MODE
			: samplers[i].sampler_filterMode);
		u8 shadowCmp = samplers[i].sampler_shadowCmp ? 1 : 0;

		immutableSamplers[i].ShaderStages = shaderTypes;
		immutableSamplers[i].SamplerOrTextureName = samplers[i].name;
		immutableSamplers[i].Desc.MinFilter = s_minMagFilters[filterModeIdx][shadowCmp];
		immutableSamplers[i].Desc.MagFilter = s_minMagFilters[filterModeIdx][shadowCmp];
		immutableSamplers[i].Desc.MipFilter = s_mipFilters[filterModeIdx][shadowCmp];
		immutableSamplers[i].Desc.AddressU = s_addressModes[(u8)samplers[i].sampler_addressMode_u];
		immutableSamplers[i].Desc.AddressV = s_addressModes[(u8)samplers[i].sampler_addressMode_v];
		immutableSamplers[i].Desc.AddressW = s_addressModes[(u8)samplers[i].sampler_addressMode_w];
		immutableSamplers[i].Desc.MaxAnisotropy = samplers[i].sampler_anisotropy == 0
			? SAMPLER_DEFAULT_TEX_ANISOTROPY
			: samplers[i].sampler_anisotropy;
		immutableSamplers[i].Desc.ComparisonFunc = Diligent::COMPARISON_FUNC_EQUAL;
	}
}

RENGINE void rengine_pipelinestate_createresourcebinding(Diligent::IPipelineState* pipeline, REngine::Result* result)
{
	if (!pipeline) {
		result->error = "Pipeline is required";
		return;
	}

	Diligent::IShaderResourceBinding* binding = null;
	pipeline->CreateShaderResourceBinding(&binding, false);

	result->value = binding;
	if (!binding)
		result->error = "Could not possible to create Shader Resource Binding";
}
