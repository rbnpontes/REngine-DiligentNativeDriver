#include "./CommandBuffer.h"

Diligent::RESOURCE_STATE_TRANSITION_MODE rengine_cmdbuffer_get_state(byte isDeferred) {
	return isDeferred ? Diligent::RESOURCE_STATE_TRANSITION_MODE_VERIFY : Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION;
}

RENGINE void rengine_cmdbuffer_cleardepth(
	Diligent::IDeviceContext* context,
	Diligent::ITextureView* depthStencil,
	Diligent::CLEAR_DEPTH_STENCIL_FLAGS clearFlags,
	float depth,
	byte stencil,
	byte isDeferred)
{
	if (!context)
		return;
	context->ClearDepthStencil(
		depthStencil,
		clearFlags,
		depth,
		stencil,
		rengine_cmdbuffer_get_state(isDeferred)
	);
}

RENGINE void rengine_cmdbuffer_clearrt(
	Diligent::IDeviceContext* context,
	Diligent::ITextureView* rt,
	float* color,
	byte isDeferred
)
{
	if (!context)
		return;

	context->ClearRenderTarget(
		rt,
		color,
	rengine_cmdbuffer_get_state(isDeferred)
	);
}

RENGINE void rengine_cmdbuffer_commitbindings(
	Diligent::IDeviceContext* context,
	Diligent::IShaderResourceBinding* shaderRes,
	byte isDeferred
)
{
	if (!context)
		return;

	context->CommitShaderResources(shaderRes, rengine_cmdbuffer_get_state(isDeferred));
}

RENGINE void rengine_cmdbuffer_copy_tex(
	Diligent::IDeviceContext* context,
	REngine::CopyTextureInfo* copyInfo,
	byte isDeferred
)
{
	if (!context)
		return;

	Diligent::CopyTextureAttribs texAttribs;
	texAttribs.pSrcTexture = copyInfo->srcTexture;
	texAttribs.SrcMipLevel = copyInfo->srcMipLevel;
	texAttribs.SrcSlice = copyInfo->srcSlice;
	texAttribs.pSrcBox = copyInfo->srcBox;
	texAttribs.pDstTexture = copyInfo->dstTexture;
	texAttribs.DstMipLevel = copyInfo->dstMipLevel;
	texAttribs.DstSlice = copyInfo->dstSlice;
	texAttribs.DstX = copyInfo->dstX;
	texAttribs.DstY = copyInfo->dstY;
	texAttribs.DstZ = copyInfo->dstZ;
	texAttribs.DstTextureTransitionMode = texAttribs.SrcTextureTransitionMode
		= rengine_cmdbuffer_get_state(isDeferred);

	context->CopyTexture(texAttribs);
}

RENGINE void rengine_cmdbuffer_draw(
	Diligent::IDeviceContext* context,
	Diligent::DrawAttribs* drawAttribs
)
{
	if (!context)
		return;
	context->Draw(*drawAttribs);
}

RENGINE void rengine_cmdbuffer_drawindexed(
	Diligent::IDeviceContext* context,
	Diligent::DrawIndexedAttribs* drawAttribs
)
{
	if (!context)
		return;
	context->DrawIndexed(*drawAttribs);
}

RENGINE void* rengine_cmdbuffer_map(
	Diligent::IDeviceContext* context,
	Diligent::IBuffer* buffer,
	Diligent::MAP_TYPE mapType,
	Diligent::MAP_FLAGS mapFlags
)
{
	void* mappedData = null;
	if (!context)
		return mappedData;

	context->MapBuffer(
		buffer,
		mapType,
		mapFlags,
		mappedData
	);
	return mappedData;
}

RENGINE void rengine_cmdbuffer_unmap(
	Diligent::IDeviceContext* context,
	Diligent::IBuffer* buffer,
	Diligent::MAP_TYPE mapType
)
{
	if (!context)
		return;

	context->UnmapBuffer(buffer, mapType);
}

RENGINE void rengine_cmdbuffer_setibuffer(
	Diligent::IDeviceContext* context,
	Diligent::IBuffer* buffer,
	ulong byteOffset,
	byte isDeferred
)
{
	if (!context)
		return;
	context->SetIndexBuffer(buffer, byteOffset, rengine_cmdbuffer_get_state(isDeferred));
}

RENGINE void rengine_cmdbuffer_setvbuffer(
	Diligent::IDeviceContext* context,
	uint startSlot,
	uint numBuffers,
	Diligent::IBuffer** buffers,
	ulong* offsets,
	byte isDeferred
)
{
	if (!context)
		return;

	context->SetVertexBuffers(
		startSlot,
		numBuffers,
		buffers,
		offsets,
		rengine_cmdbuffer_get_state(isDeferred)
	);
}

RENGINE void rengine_cmdbuffer_setpipeline(
	Diligent::IDeviceContext* context,
	Diligent::IPipelineState* pipeline
)
{
	if (!context)
		return;

	context->SetPipelineState(pipeline);
}

RENGINE void rengine_cmdbuffer_setrts(
	Diligent::IDeviceContext* context,
	Diligent::ITextureView** rts,
	byte numRts,
	Diligent::ITextureView* depth,
	byte isDeferred
)
{
	if (!context)
		return;
	context->SetRenderTargets(
		numRts,
		rts,
		depth,
		rengine_cmdbuffer_get_state(isDeferred)
	);
}

RENGINE void rengine_cmdbuffer_updtbuffer(
	Diligent::IDeviceContext* context,
	Diligent::IBuffer* buffer,
	ulong offset,
	ulong size,
	void* ptr,
	byte isDeferred
) 
{
	if (!context)
		return;
	context->UpdateBuffer(
		buffer,
		offset,
		size,
		ptr,
		rengine_cmdbuffer_get_state(isDeferred)
	);
}