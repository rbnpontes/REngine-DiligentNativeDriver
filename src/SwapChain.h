#pragma once

#include "Structs.h"

void rengine_swapchain_fill_desc(REngine::SwapChainDesc* desc, Diligent::SwapChainDesc* output);
RENGINE void rengine_swapchain_get_desc(Diligent::ISwapChain* swapChain, REngine::SwapChainDesc* output);
RENGINE void rengine_swapchain_present(Diligent::ISwapChain* swapChain, u32 sync);
RENGINE void rengine_swapchain_resize(Diligent::ISwapChain* swapChain, u32 width, u32 height, u32 transform);
RENGINE Diligent::ITextureView* rengine_swapchain_get_backbuffer(Diligent::ISwapChain* swapChain);
RENGINE Diligent::ITextureView* rengine_swapchain_get_depthbuffer(Diligent::ISwapChain* swapChain);