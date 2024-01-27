const { getPtrSize, writeInstructions, readInstructions } = require("./core");
const { ResourceDimension, TextureFormat, BindFlags, Usage, CpuAccessFlags, TextureFlags } = require("./enums");
const { getModule } = require("./native-module");

class TextureSize {
    constructor(width, height) {
        this.width = width ?? 0;
        this.height = height ?? 0;
    }
}

class TextureClearValue {
    constructor() {
        this.format = TextureFormat.unknown;
        this.r = this.g = this.b = this.a = 0;
        this.depth = 1;
        this.stencil = 0;
    }
}
class TextureDesc {
    constructor() {
        this.name = '';
        this.dimension = ResourceDimension.tex2D;
        this.size = new TextureSize();
        this.arraySizeOrDepth = 1;
        this.format = TextureFormat.unknown;
        this.mipLevels = 1;
        this.sampleCount = 1;
        this.bindFlags = BindFlags.none;
        this.usage = Usage.immutable;
        this.accessFlags = CpuAccessFlags.none;
        this.flags = TextureFlags.none;
        this.clearValue = new TextureClearValue();
    }
}


function getTextureDescSize() {
    return (10 + 2 + 7) * getPtrSize();
}

/**
 * 
 * @param {TextureDesc} instance 
 * @param {number} memPtr 
 */
function createTextureDescPtr(instance, memPtr) {
    const ptr = getModule()._malloc(getTextureDescSize());
    const namePtr = getModule().allocateUTF8(instance.name ?? '');

    const instructions = [
        [()=> namePtr, 'ptr'],
        [()=> instance.dimension, 'uint'],
        [()=> instance.size.width, 'uint'],
        [()=> instance.size.height, 'uint'],
        [()=> instance.arraySizeOrDepth, 'uint'],
        [()=> instance.format, 'uint'],
        [()=> instance.mipLevels, 'uint'],
        [()=> instance.sampleCount, 'uint'],
        [()=> instance.bindFlags, 'uint'],
        [()=> instance.usage, 'uint'],
        [()=> instance.accessFlags, 'uint'],
        [()=> instance.flags, 'uint'],
        [()=> instance.clearValue.format, 'uint'],
        [()=> instance.clearValue.r, 'uint'],
        [()=> instance.clearValue.g, 'uint'],
        [()=> instance.clearValue.b, 'uint'],
        [()=> instance.clearValue.a, 'uint'],
        [()=> instance.clearValue.depth, 'float'],
        [()=> instance.clearValue.stencil, 'uint']
    ];

    writeInstructions(instructions, ptr);
    return [ptr, namePtr];
}
function getTextureDescFromTexture(texPtr) {
    const ptr = getModule()._malloc(getTextureDescSize()); 
    getModule()._rengine_texture_getdesc(texPtr, ptr);

    const result = new TextureDesc();
    const instructions = [
        [(x)=> result.name = x, 'string'],
        [(x)=> result.dimension = x, 'uint'],
        [(x)=> result.size.width = x, 'uint'],
        [(x)=> result.size.height = x, 'uint'],
        [(x)=> result.arraySizeOrDepth = x, 'uint'],
        [(x)=> result.format = x, 'uint'],
        [(x)=> result.mipLevels = x, 'uint'],
        [(x)=> result.sampleCount = x, 'uint'],
        [(x)=> result.bindFlags = x, 'uint'],
        [(x)=> result.usage = x, 'uint'],
        [(x)=> result.accessFlags = x, 'uint'],
        [(x)=> result.flags = x, 'uint'],
        [(x)=> result.clearValue.format = x, 'uint'],
        [(x)=> result.clearValue.r = x, 'uint'],
        [(x)=> result.clearValue.g = x, 'uint'],
        [(x)=> result.clearValue.b = x, 'uint'],
        [(x)=> result.clearValue.a = x, 'uint'],
        [(x)=> result.clearValue.depth = x, 'float'],
        [(x)=> result.clearValue.stencil = x, 'uint']
    ];

    readInstructions(instructions, ptr);
    getModule()._free(ptr);

    return result;
}

class Texture {

}

class InternalTexture {
    constructor(handle) {
        this.disposed = false;
        this.handle = handle;
        this.desc = getTextureDescFromTexture(handle);
    }

    dispose() {
        if(this.disposed)
            return;
        this.disposed = true;
        this.handle = 0;
    }
}

module.exports = {
    Texture,
    InternalTexture,
    TextureClearValue,
    TextureDesc,
    TextureSize
};