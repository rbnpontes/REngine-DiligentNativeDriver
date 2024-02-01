#include "./CommandBuffer.h"

Diligent::RESOURCE_STATE_TRANSITION_MODE rengine_cmdbuffer_get_state(u8 isDeferred) {
	return isDeferred ? Diligent::RESOURCE_STATE_TRANSITION_MODE_VERIFY : Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION;
}

RENGINE void rengine_cmdbuffer_cleardepth(
	Diligent::IDeviceContext* context,
	Diligent::ITextureView* depthStencil,
	Diligent::CLEAR_DEPTH_STENCIL_FLAGS clearFlags,
	float depth,
	u8 stencil,
	u8 isDeferred)
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
	u8 isDeferred
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
	u8 isDeferred
)
{
	if (!context)
		return;

	context->CommitShaderResources(shaderRes, rengine_cmdbuffer_get_state(isDeferred));
}

RENGINE void rengine_cmdbuffer_copy_tex(
	Diligent::IDeviceContext* context,
	REngine::CopyTextureInfoDTO* copyInfo,
	u8 isDeferred
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

#if defined(__EMSCRIPTEN__)
RENGINE void rengine_cmdbuffer_draw(
	Diligent::IDeviceContext* context,
	REngine::WebDrawArgs* drawArgs
)
{
	if (!context)
		return;
	Diligent::DrawAttribs attribs;
	attribs.Flags = Diligent::DRAW_FLAG_VERIFY_ALL;
	attribs.NumVertices = drawArgs->numVertices;
	attribs.NumInstances = drawArgs->numInstances;
	attribs.StartVertexLocation = drawArgs->startVertexLocation;
	attribs.FirstInstanceLocation = drawArgs->firstInstanceLocation;
	context->Draw(attribs);
}
#else
RENGINE void rengine_cmdbuffer_draw(
	Diligent::IDeviceContext* context,
	Diligent::DrawAttribs* drawAttribs
)
{
	if (!context)
		return;
	context->Draw(*drawAttribs);
}
#endif

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
	u64 byteOffset,
	u8 isDeferred
)
{
	if (!context)
		return;
	context->SetIndexBuffer(buffer, byteOffset, rengine_cmdbuffer_get_state(isDeferred));
}

RENGINE void rengine_cmdbuffer_setvbuffer(
	Diligent::IDeviceContext* context,
	u32 startSlot,
	u32 numBuffers,
	Diligent::IBuffer** buffers,
	u64* offsets,
	u8 reset,
	u8 isDeferred
)
{
	if (!context)
		return;

	context->SetVertexBuffers(
		startSlot,
		numBuffers,
		buffers,
		offsets,
		rengine_cmdbuffer_get_state(isDeferred),
		reset ? Diligent::SET_VERTEX_BUFFERS_FLAG_RESET : Diligent::SET_VERTEX_BUFFERS_FLAG_NONE
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
	u8 numRts,
	Diligent::ITextureView* depth,
	u8 isDeferred
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
	u64 offset,
	u64 size,
	void* ptr,
	u8 isDeferred
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

RENGINE void rengine_cmdbuffer_setblendfactors(Diligent::IDeviceContext* context, float r, float g, float b, float a)
{
	if (!context)
		return;

	float colors[] = { r, g, b, a };
	context->SetBlendFactors(colors);
}

RENGINE void rengine_cmdbuffer_setviewports(Diligent::IDeviceContext* context, Diligent::Viewport* viewports, u8 numViewports, u32 rtWidth, u32 rtHeight)
{
	if (!context)
		return;

	context->SetViewports(
		numViewports,
		viewports,
		rtWidth,
		rtHeight
	);
}

RENGINE void rengine_cmdbuffer_setscissors(Diligent::IDeviceContext* context, Diligent::Rect* scissors, u8 numScissors, u32 rtWidth, u32 rtHeight)
{
	if (!context)
		return;

	context->SetScissorRects(
		numScissors,
		scissors,
		rtWidth,
		rtHeight
	);
}

RENGINE void rengine_cmdbuffer_compute(Diligent::IDeviceContext* context, Diligent::DispatchComputeAttribs* attribs)
{
	if (!context)
		return;

	context->DispatchCompute(*attribs);
}

RENGINE void rengine_cmdbuffer_begin_dbg_grp(Diligent::IDeviceContext* context, string* name, float* color)
{
	if (!context)
		return;
	context->BeginDebugGroup(name, color);
}

RENGINE void rengine_cmdbuffer_end_dbg_grp(Diligent::IDeviceContext* context)
{
	if (!context)
		return;
	context->EndDebugGroup();
}

RENGINE void rengine_cmdbuffer_insert_dbg_label(Diligent::IDeviceContext* context, string* label, float* color)
{
	if (!context)
		return;
	context->InsertDebugLabel(label, color);
}

RENGINE void rengine_cmdbuffer_begin(
	Diligent::IDeviceContext* context,
	u32 immediateCtxId
)
{
	if (!context)
		return;
	context->Begin(immediateCtxId);
}

RENGINE void rengine_cmdbuffer_finish_frame(
	Diligent::IDeviceContext* context
)
{
	if (context)
		context->FinishFrame();
}

RENGINE Diligent::ICommandList* rengine_cmdbuffer_finish_command_list(
	Diligent::IDeviceContext* context
)
{
	if (!context)
		return null;
	Diligent::ICommandList* commandList;
	context->FinishCommandList(&commandList);
	return commandList;
}

RENGINE void rengine_cmdbuffer_transition_shader_resources(
	Diligent::IDeviceContext* context,
	Diligent::IPipelineState* pipelineState,
	Diligent::IShaderResourceBinding* shaderResourceBinding
)
{
	if (!context)
		return;
	context->TransitionShaderResources(
		pipelineState,
		shaderResourceBinding
	);
}

RENGINE void rengine_cmdbuffer_set_stencil_ref(
	Diligent::IDeviceContext* context,
	u32 stencilRef
)
{
	if (!context)
		return;
	context->SetStencilRef(stencilRef);
}

RENGINE void rengine_cmdbuffer_invalidate_state(
	Diligent::IDeviceContext* context
)
{
	if (context)
		context->InvalidateState();
}

RENGINE void rengine_cmdbuffer_next_subpass(
	Diligent::IDeviceContext* context
)
{
	if (context)
		context->NextSubpass();
}

RENGINE void rengine_cmdbuffer_generate_mips(
	Diligent::IDeviceContext* context,
	Diligent::ITextureView* textureView
)
{
	if (context)
		context->GenerateMips(textureView);
}

RENGINE void rengine_cmdbuffer_transition_resource_states(
	Diligent::IDeviceContext* context,
	u32 barrierCount,
	Diligent::StateTransitionDesc* resourceBarriers
)
{
	if (context)
		context->TransitionResourceStates(barrierCount, resourceBarriers);
}

RENGINE void rengine_cmdbuffer_resolve_texture_subresource(
	Diligent::IDeviceContext* context,
	Diligent::ITexture* srcTexture,
	Diligent::ITexture* dstTexture,
	Diligent::ResolveTextureSubresourceAttribs* resolveAttribs
)
{
	if (context)
		context->ResolveTextureSubresource(srcTexture, dstTexture, *resolveAttribs);
}

RENGINE void rengine_cmdbuffer_exec_command_list(
	Diligent::IDeviceContext* context,
	u32 numCommandLists,
	Diligent::ICommandList** list
)
{
	if (!context || numCommandLists ==0 || list == null)
		return;
	context->ExecuteCommandLists(numCommandLists, list);
}