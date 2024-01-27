const { writeInstructions, readInstructions, getPtrSize } = require('./core');
const { getModule } = require('./native-module');
const { NativeObject } = require('./native-object');
const { SwapChainDesc, SwapChainSize, SwapChainFormats } = require('./swapchain-structs');
const { InternalTextureView, getNullTextureView } = require('./texture-view');

class SwapChainDescDTO {
    constructor() {
        this.width = 0;
        this.height = 0;
        this.colorFormat = 0;
        this.depthFormat = 0;
        this.usage = 0;
        this.transform = 0;
        this.bufferCount = 0;
        this.defaultDepthValue = 0.0;
        this.defaultStencilValue = 0;
        this.isPrimary = false;
    }

    static set(instance, mem) {
        const instructions = [
            [() => instance.width, 'int'],
            [() => instance.height, 'int'],
            [() => instance.colorFormat, 'int'],
            [() => instance.depthFormat, 'int'],
            [() => instance.usage, 'int'],
            [() => instance.transform, 'int'],
            [() => instance.bufferCount, 'int'],
            [() => instance.defaultDepthValue, 'float'],
            [() => instance.defaultStencilValue, 'int'],
            [() => instance.isPrimary, 'bool']
        ];
        writeInstructions(instructions, mem);
    }
    static get(instance, mem) {
        const instructions = [
            [(x)=> instance.width = x, 'uint'],
            [(x)=> instance.height = x, 'uint'],
            [(x)=> instance.colorFormat = x, 'uint'],
            [(x)=> instance.depthFormat = x, 'uint'],
            [(x)=> instance.usage = x, 'uint'],
            [(x)=> instance.transform = x, 'uint'],
            [(x)=> instance.bufferCount = x, 'uint'],
            [(x)=> instance.defaultDepthValue = x, 'float'],
            [(x)=> instance.defaultStencilValue = x, 'uint'],
            [(x)=> instance.isPrimary = x, 'bool']
        ];
        readInstructions(instructions, mem);
    }
    static sizeof() {
        return 10 * getPtrSize();
    }
}   

/**
 * get swap chain description from swapchain pointer
 * @param {number} swapChainPtr 
 * @returns {SwapChainDesc}
 */
function getSwapChainDesc(swapChainPtr) {
    const swapChainDescPtr = getModule()._malloc(SwapChainDescDTO.sizeof());
    getModule()._rengine_swapchain_get_desc(swapChainPtr, swapChainDescPtr);
    
    const desc = new SwapChainDescDTO();
    SwapChainDescDTO.set(desc, swapChainDescPtr);
    getModule()._free(swapChainDescPtr);
    
    const result = new SwapChainDesc();
    result.size = new SwapChainSize(desc.width, desc.height);
    result.formats = new SwapChainFormats(desc.colorFormat, desc.depthFormat);
    result.bufferCount = desc.bufferCount;
    result.usage = desc.usage;
    result.transform = desc.transform;
    result.defaultDepthValue = desc.defaultDepthValue;
    result.defaultStencilValue = desc.defaultStencilValue;
    result.isPrimary = desc.isPrimary;

    return result;
}
/**
 * create swapchain desc native ptr from an give swapchain desc
 * @param {SwapChainDesc} swapChainDesc 
 */
function getPtrFromSwapChainDesc(swapChainDesc) {
    const ptr = getModule()._malloc(SwapChainDescDTO.sizeof());
    const dto = new SwapChainDescDTO();
    dto.width = Math.max(swapChainDesc.size.width, 0);
    dto.height = Math.max(swapChainDesc.size.height, 0);
    dto.colorFormat = swapChainDesc.formats.color;
    dto.depthFormat = swapChainDesc.formats.depth;
    dto.usage = swapChainDesc.usage;
    dto.transform = swapChainDesc.transform;
    dto.bufferCount = swapChainDesc.bufferCount;
    dto.defaultDepthValue = swapChainDesc.defaultDepthValue;
    dto.defaultStencilValue = swapChainDesc.defaultStencilValue;
    dto.isPrimary = swapChainDesc.isPrimary;

    SwapChainDescDTO.set(dto, ptr);
    return ptr;
}


/**
 * 
 * @param {SwapChain} instance 
 */
function _collectBuffers(instance) {
    const colorBuffer = getModule()._rengine_swapchain_get_backbuffer(instance.handle);
    instance.colorBuffer.handle = colorBuffer;
    if(instance.depthBuffer)
        instance.depthBuffer.handle = getModule()._rengine_swapchain_get_depthbuffer(instance.handle);
}

class SwapChain extends NativeObject {
    constructor(handle) {
        super(handle);
        this.desc = getSwapChainDesc(handle);
        this.size = this.desc.size;
        
        const depthBuffer = getModule()._rengine_swapchain_get_depthbuffer(handle);
        const colorBuffer = getModule()._rengine_swapchain_get_backbuffer(handle);
        if(depthBuffer != 0)
            this.depthBuffer = new InternalTextureView(depthBuffer, this.size);
        this.colorBuffer = new InternalTextureView(colorBuffer, this.size);
        this.onResize = ()=> {};
    }

    /**
     * 
     * @param {boolean} vsync 
     */
    present(vsync) {
        getModule()._rengine_swapchain_present(this.handle, vsync ? 1 : 0);
        _collectBuffers(this);
    }

    /**
     * 
     * @param {number} width
     * @param {number} height
     * @param {number} transform 
     */
    resize(width, height, transform) {
        const currSize = this.size;

        width = Math.max(width, 1);
        height = Math.max(height, 1);
        
        if(currSize.width == width && currSize.height == height)
            return;
        
        getModule()._rengine_swapchain_resize(this.handle, width, height, transform);
        this.desc.transform = transform;
        if(this.onResize)
            this.onResize.call(this, new SwapChainSize(width, height), transform);
    }
}
class NullSwapChain {
    constructor() {
        this.handle = 0;
        this.disposed = false;
        this.size = new SwapChainSize(0, 0);
        
        this.depthBuffer = getNullTextureView();
        this.colorBuffer = getNullTextureView();
    }

    dispose() {}

    present(vsync){}

    resize(width, height, transform) {}
}


const _nullSwapChain = new NullSwapChain();
function getNullSwapChain() {
    return _nullSwapChain;
}

module.exports = {
    SwapChain,
    SwapChainDesc,
    SwapChainFormats,
    SwapChainSize,
    getPtrFromSwapChainDesc,
    getNullSwapChain
};