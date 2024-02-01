const { getPtrSize, malloc, free, compileWriteInstructions } = require("./core");
const { getModule } = require("./native-module");
const { NativeObject } = require("./native-object");
const { PipelineState } = require("./pipeline-state");
const { TextureView } = require("./texture-view");

/**
 * 
 * @param {number} colorPtr 
 * @param {number[]} colors 
 */
function _fillColor(colorPtr, colors) {
    const ptrSize = getPtrSize();
    const heap = getModule().HEAPF32;
    for(let i =0; i < 4; ++i) {
        const ptr = colorPtr + (i * ptrSize);
        heap[(ptr >> 2)] = i < colors.length ? colors[i] : 0; 
    }
}
/**
 * 
 * @param {number} rtPtr 
 * @param {TextureView[]} rts 
 */
function _setRt(rtPtr, rts) {
    const ptrSize = getPtrSize();
    const heap = getModule().HEAPU32;
    rts.forEach(x => {
        heap[(rtPtr >> 2)] = x.handle;
        rtPtr += ptrSize;
    });
}

const $drawArgsPayload = Symbol('__payload');
class DrawArgs {
    constructor() {
        const payload = {
            numVertices : 0,
            numInstances : 1,
            startVertexLocation: 0,
            firstInstanceLocation: 0,
            dirty : true,
            disposed : false,
            ptr : malloc(4 * getPtrSize()),
            writeCall : ()=> void(0)
        };

        const instructions = [
            [()=> payload.numVertices, 'uint'],
            [()=> payload.numInstances, 'uint'],
            [()=> payload.startVertexLocation, 'uint'],
            [()=> payload.firstInstanceLocation, 'uint'],
        ];
        payload.writeCall = compileWriteInstructions(instructions, payload.ptr);
 
        this[$drawArgsPayload] = payload;
    }

    dispose() {
        const payload = this[$drawArgsPayload];
        if(payload.disposed)
            return;
        payload.disposed = true;
        free(payload.ptr);
    }

    toPtr() {
        const payload = this[$drawArgsPayload];
        if(payload.disposed)
            return 0;
        if(payload.dirty)
        {
            payload.dirty = false;
            payload.writeCall();
        }
        return payload.ptr;
    }

    get numVertices() { return this[$drawArgsPayload].numVertices; }
    set numVertices(value) {
        const payload = this[$drawArgsPayload];
        payload.numVertices = value;
        payload.dirty = true;
    }

    get numInstances() { return this[$drawArgsPayload].numInstances; }
    set numInstances(value) {
        const payload = this[$drawArgsPayload];
        payload.numInstances = value;
        payload.dirty = true;
    }

    get startVertexLocation() { return this[$drawArgsPayload].startVertexLocation; }
    set startVertexLocation(value) {
        const payload = this[$drawArgsPayload];
        payload.startVertexLocation = value;
        payload.dirty = true;
    }

    get firstInstanceLocation() { return this[$drawArgsPayload].firstInstanceLocation; }
    set firstInstanceLocation(value) {
        const payload = this[$drawArgsPayload];
        payload.firstInstanceLocation = value;
        payload.dirty = true;
    }
}
class CommandBuffer {
    /**
     * Create command buffer from command buffer ptr
     * @param {number} handle 
     */
    constructor(handle) {
        this.disposed = false;
        this.handle = handle;
        this._colorCopyBuffer = getModule()._malloc(4 * getPtrSize());
        this._rtCopyBuffer = getModule()._malloc(16 * getPtrSize());
    }

    dispose() {
        if(this.disposed)
            return;
        this.disposed = true;
        getModule()._free(this._colorCopyBuffer);
        getModule()._free(this._rtCopyBuffer);
        getModule()._rengine_object_releaseref(this.handle);
        
        this._colorCopyBuffer = this._rtCopyBuffer = 0;
    }
    /**
     * 
     * @param {TextureView} depthStencil 
     * @param {number} clearFlags 
     * @param {number} depth 
     * @param {number} stencil 
     * @returns {CommandBuffer} self instance
     */
    clearDepth(depthStencil, clearFlags, depth, stencil) {
        if(this.disposed || depthStencil.handle == 0)
            return this;
        
        getModule()._rengine_cmdbuffer_cleardepth(
            this.handle,
            depthStencil.handle,
            clearFlags,
            depth,
            stencil
        );

        return this;
    }
    /**
     * 
     * @param {TextureView} rt 
     * @param {number[]} colorArr 
     * @returns {CommandBuffer} self instance
     */
    clearRt(rt, colorArr) {
        if(this.disposed || rt.handle == 0)
            return this;
        _fillColor(this._colorCopyBuffer, colorArr);
        getModule()._rengine_cmdbuffer_clearrt(
            this.handle,
            rt.handle,
            this._colorCopyBuffer,
            0x0
        );

        return this;
    }

    commitBindings(srb) {
        throw new Error('not implemented');
    }
    copy(copyInfo) {
        throw new Error('not implemented');
    }
    /**
     * 
     * @param {DrawArgs} drawArgs 
     */
    draw(drawArgs) {
        if(this.disposed || !drawArgs)
            return this;
        getModule()._rengine_cmdbuffer_draw(this.handle, drawArgs.toPtr());
        return this;
    }
    drawIndexed(drawArgs) {
        throw new Error('not implemented');
    }
    map(buffer, mapType, mapFlags) {
        throw new Error('not implemented');
    }
    unmap(buffer, mapType) {
        throw new Error('not implemented');
    }
    updateBuffer(buffer, offset, data) {
        throw new Error('not implemented');
    }
    /**
     * 
     * @param {*} buffer 
     * @param {bool} reset 
     */
    setVertexBuffer(buffer, reset) {
        throw new Error('not implemented');
    }
    setVertexBuffers(startSlot, buffers, offsets, reset) {
        throw new Error('not implemented');
    }
    setIndexBuffer(buffer, byteOffset) {
        throw new Error('not implemented');
    }
    /**
     * 
     * @param {PipelineState} pipelineState 
     */
    setPipeline(pipelineState) {
        if(this.disposed || pipelineState.disposed)
            return this;
        getModule()._rengine_cmdbuffer_setpipeline(this.handle, pipelineState.handle);
        return this;
    }
    /**
     * 
     * @param {TextureView} rt 
     * @param {TextureView?} depthStencil 
     * @returns {CommandBuffer} self instance
     */
    setRt(rt, depthStencil) {
        if(this.disposed || rt.disposed)
            return this;
        _setRt(this._rtCopyBuffer, [rt]);
        getModule()._rengine_cmdbuffer_setrts(
            this.handle,
            this._rtCopyBuffer,
            1,
            depthStencil?.handle ?? 0,
            0x0);
        return this;
    }
    /**
     * 
     * @param {TextureView[]} rts render targets 
     * @param {TextureView?} depthStencil 
     */
    setRts(rts, depthStencil) {
        if(this.disposed || rts.length == 0)
            return this;
        if(rts[0].handle == 0)
            return this;
        _setRt(this._rtCopyBuffer, rts);
        getModule()._rengine_cmdbuffer_setrts(
            this.handle,
            this._rtCopyBuffer,
            rts.length,
            depthStencil?.handle ?? 0,
            0x0
        );
        return this;
    }
    setBlendFactors(r, g, b, a) {
        if(this.disposed)
            return this;
        getModule()._rengine_cmdbuffer_setblendfactors(
            this.handle,
            r / 255.0,
            g / 255.0,
            b / 255.0,
            a / 255.0
        );
        return this;
    } 
    setViewports(viewports, rtWidth, rtHeight) {
        throw new Error('not implemented');
    }
    setViewport(viewport, rtWidth, rtHeight) {
        throw new Error('not implemented');
    }
    setScissors(scissors, rtWidth, rtHeight) {
        throw new Error('not implemented');
    }
    setScissor(scissor, rtWidth, rtHeight) {
        throw new Error('not implemented');
    }
    compute(args) {
        throw new Error('not implemented');
    }
    setStencilRef(stencilRef) {
        if(this.disposed)
            return this;
        getModule()._rengine_cmdbuffer_set_stencil_ref(this.handle, stencilRef);
    }
    invalidateState() {
        return this;
    }
    nextSubpass() {
        return this;
    }
    generateMips(texView) {
        throw new Error('not implemented');
    }
    transitionResourceState(resourceBarriers) {
        throw new Error('not implemented');
    }
    resolveTextureSubresource(srcTexture, dstTexture, resolveDesc) {
        throw new Error('not implemented');
    }
    begin() {
        return this;
    }
    finishFrame() {
        return this;
    }
    finishCommandList() {
        return this;
    }
    executeCommandList(list) {
        return this;
    }
}

class NullCommandBuffer {
    constructor(){
        this.handle = 0;
        this.disposed = false;
        const instance = this;
        [
            'clearDepth',
            'clearRt',
            'commitBindings',
            'copy',
            'draw',
            'drawIndexed',
            'unmap',
            'updateBuffer',
            'setVertexBuffer',
            'setVertexBuffers',
            'setIndexBuffer',
            'setPipeline',
            'setRt',
            'setRts',
            'setBlendFactors',
            'setViewports',
            'setViewport',
            'setScissors',
            'setScissor',
            'compute',
            'setStencilRef',
            'invalidateState',
            'nextSubpass',
            'generateMips',
            'transitionResourceState',
            'resolveTextureSubresource',
            'begin',
            'finishFrame',
            'finishCommandList',
            'executeCommandList'
        ].forEach(x => {
            instance[x] = (...args)=> this;
        });
    }
    dispose() {}
    map(buffer, mapType, mapFlags) {
        return 0;
    }
}

const _nullCommandBuffer = new NullCommandBuffer();
function getNullCommandBuffer() { return _nullCommandBuffer; }

module.exports = {
    DrawArgs,
    CommandBuffer,
    getNullCommandBuffer
};