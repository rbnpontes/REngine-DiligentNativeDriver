#include "./ShaderResourceBinding.h"
#include "./Shader.h"

RENGINE void rengine_srb_set(Diligent::IShaderResourceBinding* srb, REngine::ShaderTypeFlags flags, string* resourceName, Diligent::IDeviceObject* resource)
{
	if (!srb)
		return;
	Diligent::IShaderResourceVariable* vary = srb->GetVariableByName(rengine_shader_convert_shader_flags(flags), resourceName);
	if (vary)
		vary->Set(resource, Diligent::SET_SHADER_RESOURCE_FLAG_ALLOW_OVERWRITE);
}
