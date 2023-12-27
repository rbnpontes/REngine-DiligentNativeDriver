#pragma once

#include "./Structs.h"

void rengine_buffer_fill(REngine::BufferDescDTO* desc, Diligent::BufferDesc* output);

RENGINE void rengine_buffer_getdesc(Diligent::IBuffer* buffer, REngine::BufferDescDTO* output);
RENGINE void rengine_buffer_create_view(Diligent::IBuffer* buffer, REngine::BufferViewCreateDesc* createDesc, REngine::Result* result);
RENGINE Diligent::IBufferView* rengine_buffer_get_default_view(Diligent::IBuffer* buffer, Diligent::BUFFER_VIEW_TYPE viewType);
RENGINE void rengine_buffer_set_state(Diligent::IBuffer* buffer, Diligent::RESOURCE_STATE state);
RENGINE Diligent::RESOURCE_STATE rengine_buffer_get_state(const Diligent::IBuffer* buffer);