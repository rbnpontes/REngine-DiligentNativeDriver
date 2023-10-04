#include "Buffer.h"
#include <Buffer.h>

void rengine_buffer_fill(REngine::BufferDescDTO* desc, Diligent::BufferDesc* output)
{
	output->Name = desc->name;
	output->Size = desc->size;
	output->BindFlags = desc->bindFlags;
	output->CPUAccessFlags = desc->accessFlags;
	output->Mode = desc->mode;
	output->ElementByteStride = desc->elementByteStride;
}

RENGINE void rengine_buffer_getdesc(Diligent::IBuffer* buffer, REngine::BufferDescDTO* output)
{
	Diligent::BufferDesc desc = buffer->GetDesc();
	output->name = desc.Name;
	output->size = desc.Size;
	output->accessFlags = desc.CPUAccessFlags;
	output->mode = desc.Mode;
	output->elementByteStride = desc.ElementByteStride;
}
