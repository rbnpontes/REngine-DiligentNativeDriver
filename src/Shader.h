#pragma once
#include "./Structs.h"
#include <vector>

void rengine_shader_fill(REngine::ShaderCreateInfo* ci, Diligent::ShaderCreateInfo* output, std::vector<Diligent::ShaderMacro>& macros);

Diligent::SHADER_TYPE rengine_shader_convert_shader_flags(REngine::ShaderTypeFlags flags);

RENGINE REngine::ShaderType rengine_shader_gettype(Diligent::IShader* shader);