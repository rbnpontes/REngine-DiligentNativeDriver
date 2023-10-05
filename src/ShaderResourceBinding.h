#pragma once

#include "./Structs.h"

RENGINE void rengine_srb_set(
	Diligent::IShaderResourceBinding* srb, 
	REngine::ShaderTypeFlags flags, 
	string* resourceName, 
	Diligent::IDeviceObject* resource
);