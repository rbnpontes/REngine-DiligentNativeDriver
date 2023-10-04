#pragma once
#include "./Structs.h"
#include <vector>

void rengine_texture_fill(
	REngine::TextureDesc* desc,
	Diligent::TextureDesc* output
);

void rengine_texture_fill(
	REngine::TextureData* data,
	uint numTexData,
	std::vector<Diligent::TextureSubResData> subresources,
	Diligent::TextureData* output
);

RENGINE void rengine_texture_getdesc(
	Diligent::ITexture* texture,
	REngine::TextureDesc* output
);
RENGINE void rengine_texture_getdefaultview(
	Diligent::ITexture* texture,
	Diligent::TEXTURE_VIEW_TYPE viewType,
	REngine::Result* result
);

RENGINE void rengine_textureview_getparent(
	Diligent::ITextureView* texView,
	REngine::Result* result
);
RENGINE void rengine_textureview_getdesc(
	Diligent::ITextureView* texView,
	REngine::TextureViewDesc* output
);