#pragma once
#include "Structs.h"

using namespace REngine;

Result* rengine_get_available_adapter(GraphicsBackend backend, void* messageEvent, Result* result, uint* length);
GraphicsDriverResult* rengine_create_driver(GraphicsDriverSettings* settings, SwapChainDesc* desc, Diligent::NativeWindow* nativeWnd);