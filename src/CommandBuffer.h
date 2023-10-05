#pragma once
#include "./Structs.h"

RENGINE void rengine_cmdbuffer_cleardepth(
	Diligent::IDeviceContext* context,
	Diligent::ITextureView* depthStencil,
	Diligent::CLEAR_DEPTH_STENCIL_FLAGS clearFlags,
	float depth,
	byte stencil,
	byte isDeferred);
RENGINE void rengine_cmdbuffer_clearrt(
	Diligent::IDeviceContext* context,
	Diligent::ITextureView* rt,
	float* color,
	byte isDeferred
);
RENGINE void rengine_cmdbuffer_commitbindings(
	Diligent::IDeviceContext* context,
	Diligent::IShaderResourceBinding* shaderRes,
	byte isDeferred
);
RENGINE void rengine_cmdbuffer_copy_tex(
	Diligent::IDeviceContext* context,
	REngine::CopyTextureInfoDTO* copyInfo,
	byte isDeferred
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
	ulong byteOffset,
	byte isDeferred
);
RENGINE void rengine_cmdbuffer_setvbuffer(
	Diligent::IDeviceContext* context,
	uint startSlot,
	uint numBuffers,
	Diligent::IBuffer** buffers,
	ulong* offsets,
	byte isDeferred
);
RENGINE void rengine_cmdbuffer_setpipeline(
	Diligent::IDeviceContext* context,
	Diligent::IPipelineState* pipeline
);
RENGINE void rengine_cmdbuffer_setrts(
	Diligent::IDeviceContext* context,
	Diligent::ITextureView** rts,
	byte numRts,
	Diligent::ITextureView* depth,
	byte isDeferred
);
RENGINE void rengine_cmdbuffer_updtbuffer(
	Diligent::IDeviceContext* context,
	Diligent::IBuffer* buffer,
	ulong offset,
	ulong size,
	void* ptr,
	byte isDeferred
);