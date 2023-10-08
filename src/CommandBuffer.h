#pragma once
#include "./Structs.h"

RENGINE void rengine_cmdbuffer_cleardepth(
	Diligent::IDeviceContext* context,
	Diligent::ITextureView* depthStencil,
	Diligent::CLEAR_DEPTH_STENCIL_FLAGS clearFlags,
	float depth,
	u8 stencil,
	u8 isDeferred);
RENGINE void rengine_cmdbuffer_clearrt(
	Diligent::IDeviceContext* context,
	Diligent::ITextureView* rt,
	float* color,
	u8 isDeferred
);
RENGINE void rengine_cmdbuffer_commitbindings(
	Diligent::IDeviceContext* context,
	Diligent::IShaderResourceBinding* shaderRes,
	u8 isDeferred
);
RENGINE void rengine_cmdbuffer_copy_tex(
	Diligent::IDeviceContext* context,
	REngine::CopyTextureInfoDTO* copyInfo,
	u8 isDeferred
);
RENGINE void rengine_cmdbuffer_draw(
	Diligent::IDeviceContext* context,
	Diligent::DrawAttribs* drawAttribs
);
RENGINE void rengine_cmdbuffer_drawindexed(
	Diligent::IDeviceContext* context,
	Diligent::DrawIndexedAttribs* drawAttribs
);
RENGINE void* rengine_cmdbuffer_map(
	Diligent::IDeviceContext* context,
	Diligent::IBuffer* buffer,
	Diligent::MAP_TYPE mapType,
	Diligent::MAP_FLAGS mapFlags
);
RENGINE void rengine_cmdbuffer_unmap(
	Diligent::IDeviceContext* context,
	Diligent::IBuffer* buffer,
	Diligent::MAP_TYPE mapType
);
RENGINE void rengine_cmdbuffer_setibuffer(
	Diligent::IDeviceContext* context,
	Diligent::IBuffer* buffer,
	u64 byteOffset,
	u8 isDeferred
);
RENGINE void rengine_cmdbuffer_setvbuffer(
	Diligent::IDeviceContext* context,
	u32 startSlot,
	u32 numBuffers,
	Diligent::IBuffer** buffers,
	u64* offsets,
	u8 reset,
	u8 isDeferred
);
RENGINE void rengine_cmdbuffer_setpipeline(
	Diligent::IDeviceContext* context,
	Diligent::IPipelineState* pipeline
);
RENGINE void rengine_cmdbuffer_setrts(
	Diligent::IDeviceContext* context,
	Diligent::ITextureView** rts,
	u8 numRts,
	Diligent::ITextureView* depth,
	u8 isDeferred
);
RENGINE void rengine_cmdbuffer_updtbuffer(
	Diligent::IDeviceContext* context,
	Diligent::IBuffer* buffer,
	u64 offset,
	u64 size,
	void* ptr,
	u8 isDeferred
);