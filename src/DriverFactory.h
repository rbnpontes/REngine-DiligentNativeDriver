#pragma once
#include "Structs.h"

using namespace REngine;

Result* rengine_get_available_adapter(GraphicsBackend backend, void* message_event, Result* result, u32* length);
void rengine_create_driver(GraphicsDriverSettings* settings, SwapChainDesc* desc, Diligent::NativeWindow* native_wnd, GraphicsDriverResult* result);
void rengine_create_swapchain(const SwapChainCreationInfo* creation_info, Result* result);