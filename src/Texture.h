#pragma once
#include "./Structs.h"
#include <vector>

void rengine_texture_fill(
	REngine::TextureDescDTO* desc,
	Diligent::TextureDesc* output
);

void rengine_texture_fill(
	REngine::TextureDataDTO* data,
	u32 numTexData,
	std::vector<Diligent::TextureSubResData>& subresources,
	Diligent::TextureData* output
);

RENGINE void rengine_texture_getdesc(
	Diligent::ITexture* texture,
	REngine::TextureDescDTO* output
);
RENGINE void rengine_texture_getdefaultview(
	Diligent::ITexture* texture,
	Diligent::TEXTURE_VIEW_TYPE viewType,
	REngine::Result* result
);
RENGINE void rengine_texture_set_state(
	Diligent::ITexture* texture,
	Diligent::RESOURCE_STATE state
);
RENGINE Diligent::RESOURCE_STATE rengine_texture_get_state(
	Diligent::ITexture* texture
);
RENGINE u64 rengine_texture_get_gpuhandle(
	Diligent::ITexture* texture
);

RENGINE void rengine_textureview_getparent(
	Diligent::ITextureView* texView,
	REngine::Result* result
);
RENGINE void rengine_textureview_getdesc(
	Diligent::ITextureView* texView,
	REngine::TextureViewDesc* output
);