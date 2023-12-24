#include "Buffer.h"
#include <Buffer.h>

void rengine_buffer_fill(REngine::BufferDescDTO* desc, Diligent::BufferDesc* output)
{
	output->Name = desc->name;
	output->Size = desc->size;
	output->Usage = desc->usage;
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
	output->usage = desc.Usage;
}

RENGINE void rengine_buffer_create_view(Diligent::IBuffer* buffer, REngine::BufferViewCreateDesc* createDesc, REngine::Result* result)
{
	if(buffer == null)
	{
		result->error = "Buffer is null";
		return;
	}
	Diligent::BufferViewDesc desc;
	desc.Name = createDesc->name;
	desc.ViewType = createDesc->viewType;
	desc.Format.IsNormalized = createDesc->format_isNormalized;
	desc.Format.NumComponents = createDesc->format_numComponents;
	desc.Format.ValueType = createDesc->format_valueType;
	desc.ByteOffset = createDesc->byteOffset;
	desc.ByteWidth = createDesc->byteWidth;

	Diligent::IBufferView* view;
	buffer->CreateView(desc, &view);

	result->value = view;
	if (view == null)
		result->error = "Error has occurred while is creating Buffer View";
}

RENGINE Diligent::IBufferView* rengine_buffer_get_default_view(Diligent::IBuffer* buffer, Diligent::BUFFER_VIEW_TYPE viewType)
{
	if (buffer == null)
		return null;
	return buffer->GetDefaultView(viewType);
}