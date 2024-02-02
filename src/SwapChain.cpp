#include "./SwapChain.h"

void rengine_swapchain_fill_desc(REngine::SwapChainDesc* desc, Diligent::SwapChainDesc* output)
{
	output->Width = desc->width;
	output->Height = desc->height;

	output->ColorBufferFormat = (Diligent::TEXTURE_FORMAT)desc->colorFormat;
	output->DepthBufferFormat = (Diligent::TEXTURE_FORMAT)desc->depthFormat;

	output->Usage = (Diligent::SWAP_CHAIN_USAGE_FLAGS)desc->usage;
	output->PreTransform = (Diligent::SURFACE_TRANSFORM)desc->transform;

	output->BufferCount = desc->bufferCount;
	output->DefaultDepthValue = desc->defaultDepthValue;
	output->DefaultStencilValue = desc->defaultStencilValue;
	output->IsPrimary = desc->isPrimary;
}

RENGINE void rengine_swapchain_get_desc(Diligent::ISwapChain* swapChain, REngine::SwapChainDesc* output) 
{
	Diligent::SwapChainDesc desc = swapChain->GetDesc();
	output->width = desc.Width;
	output->height = desc.Height;

	output->colorFormat = desc.ColorBufferFormat;
	output->depthFormat = desc.DepthBufferFormat;

	output->usage = desc.Usage;
	output->transform = desc.PreTransform;

	output->bufferCount = desc.BufferCount;
	output->defaultDepthValue = desc.DefaultDepthValue;
	output->defaultStencilValue = desc.DefaultStencilValue;
	output->isPrimary = desc.IsPrimary ? 1 : 0;
}
#ifndef __EMSCRIPTEN__
RENGINE void rengine_swapchain_present(Diligent::ISwapChain* swapChain, u32 sync) 
{
	if(swapChain)
		swapChain->Present(sync);
}
#endif
RENGINE void rengine_swapchain_resize(Diligent::ISwapChain* swapChain, u32 width, u32 height, u32 transform)
{
	if (swapChain)
		swapChain->Resize(width, height, (Diligent::SURFACE_TRANSFORM)transform);
}

RENGINE Diligent::ITextureView* rengine_swapchain_get_backbuffer(Diligent::ISwapChain* swapChain)
{
	if (swapChain)
		return swapChain->GetCurrentBackBufferRTV();
	return null;
}

RENGINE Diligent::ITextureView* rengine_swapchain_get_depthbuffer(Diligent::ISwapChain* swapChain) 
{
	if (swapChain)
		return swapChain->GetDepthBufferDSV();
	return null;
}