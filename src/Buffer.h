#pragma once

#include "./Structs.h"

void rengine_buffer_fill(REngine::BufferDescDTO* desc, Diligent::BufferDesc* output);

RENGINE void rengine_buffer_getdesc(Diligent::IBuffer* buffer, REngine::BufferDescDTO* output);