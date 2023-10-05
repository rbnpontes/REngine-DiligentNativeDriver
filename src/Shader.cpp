#include "./Shader.h"
#include <vector>

static Diligent::SHADER_TYPE s_shaderTypes[] = {
	Diligent::SHADER_TYPE_VERTEX,
	Diligent::SHADER_TYPE_PIXEL,
	Diligent::SHADER_TYPE_COMPUTE,
	Diligent::SHADER_TYPE_GEOMETRY,
	Diligent::SHADER_TYPE_HULL,
	Diligent::SHADER_TYPE_DOMAIN,
};

void rengine_shader_fill(REngine::ShaderCreateInfoDTO* ci, Diligent::ShaderCreateInfo* output, std::vector<Diligent::ShaderMacro>& macros) {
	
	macros.resize(ci->numMacros);

	output->Desc.Name = ci->name;
	output->Desc.ShaderType = s_shaderTypes[(byte)ci->type];
	output->Desc.UseCombinedTextureSamplers = true;
	output->ByteCode = ci->byteCode;
	output->ByteCodeSize = ci->byteCodeLength;
	output->Source = ci->sourceCode;
	if (ci->sourceCode != nullptr)
		output->SourceLength = strlen(ci->sourceCode);
	output->SourceLanguage = Diligent::SHADER_SOURCE_LANGUAGE_HLSL;

	for (uint i = 0; i < ci->numMacros; ++i) {
		macros[i].Name = ci->macroKeys[i];
		macros[i].Definition = ci->macroValues[i];
	}
	output->Macros = Diligent::ShaderMacroArray{ macros.data(), ci->numMacros };
}

Diligent::SHADER_TYPE rengine_shader_convert_shader_flags(REngine::ShaderTypeFlags flags)
{
	Diligent::SHADER_TYPE result = Diligent::SHADER_TYPE_UNKNOWN;
	using namespace REngine;
	if (RENGINE_TEST_FLAG(flags, ShaderTypeFlags::Vertex))
		result = Diligent::SHADER_TYPE_VERTEX;
	if (RENGINE_TEST_FLAG(flags, ShaderTypeFlags::Pixel))
		result |= Diligent::SHADER_TYPE_PIXEL;
	if (RENGINE_TEST_FLAG(flags, ShaderTypeFlags::Compute))
		result |= Diligent::SHADER_TYPE_COMPUTE;
	if (RENGINE_TEST_FLAG(flags, ShaderTypeFlags::Geometry))
		result |= Diligent::SHADER_TYPE_GEOMETRY;
	if (RENGINE_TEST_FLAG(flags, ShaderTypeFlags::Hull))
		result |= Diligent::SHADER_TYPE_HULL;
	if (RENGINE_TEST_FLAG(flags, ShaderTypeFlags::Domain))
		result |= Diligent::SHADER_TYPE_DOMAIN;
	return result;
}

RENGINE REngine::ShaderType rengine_shader_gettype(Diligent::IShader* shader)
{
	using namespace Diligent;
	REngine::ShaderType type = REngine::ShaderType::Unknow;

	switch (shader->GetDesc().ShaderType)
	{
	case SHADER_TYPE_VERTEX:
		type = REngine::ShaderType::Vertex;
		break;
	case SHADER_TYPE_PIXEL:
		type = REngine::ShaderType::Pixel;
		break;
	case SHADER_TYPE_COMPUTE:
		type = REngine::ShaderType::Compute;
		break;
	case SHADER_TYPE_GEOMETRY:
		type = REngine::ShaderType::Geometry;
		break;
	case SHADER_TYPE_HULL:
		type = REngine::ShaderType::Hull;
		break;
	case SHADER_TYPE_DOMAIN:
		type = REngine::ShaderType::Domain;
		break;
	}

	return type;
}
