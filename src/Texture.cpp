#include "./Texture.h"

void rengine_texture_fill(REngine::TextureDescDTO* desc, Diligent::TextureDesc* output)
{
	output->Name = desc->name;
	output->Type = desc->dimension;
	output->Width = desc->width;
	output->Height = desc->height;
	output->ArraySize = desc->arraySizeOrDepth;
	output->Format = (Diligent::TEXTURE_FORMAT)desc->format;
	output->MipLevels = desc->mipLevels;
	output->SampleCount = desc->sampleCount;
	output->BindFlags = desc->bindFlags;
	output->Usage = desc->usage;
	output->CPUAccessFlags = desc->accessFlags;
	output->MiscFlags = desc->textureFlags;

	output->ClearValue.Color[0] = desc->clear_r;
	output->ClearValue.Color[1] = desc->clear_g;
	output->ClearValue.Color[2] = desc->clear_b;
	output->ClearValue.Color[3] = desc->clear_a;
	output->ClearValue.Format = (Diligent::TEXTURE_FORMAT)desc->clear_format;
	output->ClearValue.DepthStencil.Depth = desc->clear_depth;
	output->ClearValue.DepthStencil.Stencil = desc->clear_stencil;
}

void rengine_texture_fill(
	REngine::TextureDataDTO* data,
	u32 numTexData,
	std::vector<Diligent::TextureSubResData>& subresources,
	Diligent::TextureData* output
)
{
	subresources.resize(numTexData);
	output->NumSubresources = numTexData;
	output->pSubResources = subresources.data();
	
	for (u32 i = 0; i < numTexData; ++i) {
		subresources[i].pData = data[i].data;
		subresources[i].DepthStride = data[i].depthStride;
		subresources[i].pSrcBuffer = data[i].srcBuffer;
		subresources[i].SrcOffset = data[i].srcOffset;
		subresources[i].Stride = data[i].stride;
	}
}

RENGINE void rengine_texture_getdesc(
	Diligent::ITexture* texture,
	REngine::TextureDescDTO* output
)
{
	if (!texture || !output)
		return;
	Diligent::TextureDesc desc = texture->GetDesc();
	output->name = desc.Name;
	output->dimension = desc.Type;
	output->width = desc.Width;
	output->height = desc.Height;
	output->arraySizeOrDepth = desc.ArraySize;
	output->format = desc.Format;
	output->mipLevels = desc.MipLevels;
	output->sampleCount = desc.SampleCount;
	output->bindFlags = desc.BindFlags;
	output->usage = desc.Usage;
	output->accessFlags = desc.CPUAccessFlags;
	output->textureFlags = desc.MiscFlags;

	output->clear_r = desc.ClearValue.Color[0];
	output->clear_g = desc.ClearValue.Color[1];
	output->clear_b = desc.ClearValue.Color[2];
	output->clear_a = desc.ClearValue.Color[3];
	output->clear_format = desc.ClearValue.Format;
	output->clear_depth = desc.ClearValue.DepthStencil.Depth;
	output->clear_stencil = desc.ClearValue.DepthStencil.Stencil;
}

RENGINE void rengine_texture_getdefaultview(
	Diligent::ITexture* texture, 
	Diligent::TEXTURE_VIEW_TYPE viewType,
	REngine::Result* result)
{
#ifdef RENGINE_DEBUG
	if (!texture) {
		result->error = "Texture is required";
		return;
	}
	if (viewType == Diligent::TEXTURE_VIEW_UNDEFINED) {
		result->error = "View Type cannot be undefined";
		return;
	}
#endif
	result->value = texture->GetDefaultView(viewType);
}

RENGINE void rengine_textureview_getparent(Diligent::ITextureView* texView, REngine::Result* result)
{
#ifdef RENGINE_DEBUG
	if (!texView) {
		result->error = "Texture View is required";
		return;
	}
#endif

	Diligent::ITexture* texture = texView->GetTexture();
	result->value = texture;
#ifdef RENGINE_DEBUG
	if (!texture)
		result->error = "Error at get parent. Parent is null";
#endif
}

RENGINE void rengine_textureview_getdesc(Diligent::ITextureView* texView, REngine::TextureViewDesc* output)
{
#ifdef RENGINE_DEBUG
	if (!texView || !output)
		return;
#endif

	Diligent::TextureViewDesc desc = texView->GetDesc();
	output->viewType = desc.ViewType;
	output->dimension = desc.TextureDim;
	output->format = desc.Format;
	output->mostDetailedMip = desc.MostDetailedMip;
	output->mipLevels = desc.NumMipLevels;
	output->firstSlice = desc.FirstArrayOrDepthSlice();
	output->slicesCount = desc.NumArrayOrDepthSlices();
	output->accessFlags = desc.AccessFlags;
	output->allowMipMapGeneration = RENGINE_TEST_FLAG(desc.Flags, Diligent::TEXTURE_VIEW_FLAG_ALLOW_MIP_MAP_GENERATION);
}
