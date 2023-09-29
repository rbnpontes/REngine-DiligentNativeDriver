#include "./SwapChain.h"

void rengine_fill_swapchain_desc(REngine::SwapChainDesc* desc, Diligent::SwapChainDesc* output)
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
