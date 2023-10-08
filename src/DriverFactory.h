#pragma once
#include "Structs.h"

using namespace REngine;

Result* rengine_get_available_adapter(GraphicsBackend backend, void* messageEvent, Result* result, u32* length);
void rengine_create_driver(GraphicsDriverSettings* settings, SwapChainDesc* desc, Diligent::NativeWindow* nativeWnd, GraphicsDriverResult* result);
void rengine_create_swapchain(SwapChainCreationInfo* creationInfo, Result* result);