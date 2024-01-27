const { sizeof, readInstructions } = require("./core");
const { TextureViewType, ResourceDimension, TextureFormat, UavAccessFlag } = require("./enums");
const { getModule } = require("./native-module");
const { NativeObject } = require("./native-object");
const { SwapChainSize } = require("./swapchain-structs");

class TextureViewDesc {
    constructor() {
        this.viewType = TextureViewType.undef;
        this.dimension = ResourceDimension.undef;
        this.format = TextureFormat.unknown;
        this.mostDetailedMip = 0;
        this.mipLevels = 0;
        this.firstSlice = 0;
        this.slicesCount = 0;
        this.accessFlags = UavAccessFlag.unspecified;
        this.allowMipMapGeneration = false;
    }
}

function getTexViewDesc(texPtr) {
    const result = new TextureViewDesc();
    if(texPtr == 0)
        return result;

    const descPtr = getModule()._malloc(sizeof(TextureViewDesc));
    const instructions = [
        [(x)=> result.viewType = x, 'uint'],
        [(x)=> result.dimension = x, 'uint'],
        [(x)=> result.format = x, 'uint'],
        [(x)=> result.mostDetailedMip = x, 'uint'],
        [(x)=> result.mipLevels = x, 'uint'],
        [(x)=> result.firstSlice = x, 'uint'],
        [(x)=> result.slicesCount = x, 'uint'],
        [(x)=> result.accessFlags = x, 'uint'],
        [(x)=> result.allowMipMapGeneration = x, 'bool']
    ];

    getModule()._rengine_textureview_getdesc(texPtr, descPtr);
    readInstructions(instructions, descPtr);
    getModule()._free(descPtr);

    return result;
}

class TextureView extends NativeObject {

}
class InternalTextureView {
    constructor(handle, size) {
        const objName = getModule()._rengine_object_getname(handle);
        
        this.handle = handle;
        this.disposed = false;
        this.size = size;
        this.name = getModule().UTF8ToString(objName);
        this.desc = getTexViewDesc(handle);
    }

    dispose() {}
}

class NullTextureView {
    constructor(){
        this.handle = 0;
        this.disposed = false;
        this.size = new SwapChainSize(0, 0);
        this.name = "null";
        this.desc = new TextureViewDesc();
    }
}

const _nullTextureView = new NullTextureView();
function getNullTextureView() {
    return _nullTextureView;
}

module.exports = {
    TextureView,
    InternalTextureView,
    getNullTextureView,
};