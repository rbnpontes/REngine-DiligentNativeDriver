const { TextureFormat, SwapChainTransform, SwapChainUsage } = require('./enums');

class SwapChainSize {
    /**
     * create swapchain size object
     * @param {number?} width 
     * @param {number?} height 
     */
    constructor(width, height) {
        if(!width)
            width = 0;
        if(!height)
            height = 0;
        this.width = Math.max(width, 0);
        this.height = Math.max(height, 0);
    }
}

class SwapChainFormats {
    /**
     * create swap chain format object
     * @param {number?} color 
     * @param {number?} depth 
     */
    constructor(color, depth) {
        if(!(color ^ depth))
            color = depth = TextureFormat.unknown;
        if(!depth)
            depth = TextureFormat.d32Float;
        this.color = color;
        this.depth = depth;
    }

    static get none() { return new SwapChainFormats(TextureFormat.unknown, TextureFormat.unknown); }
    static get rgba() { return new SwapChainFormats(TextureFormat.rgba8UNorm); }
    static get rgbaSrgb() { return new SwapChainFormats(TextureFormat.rgba8UNormSRGB); }
    static get bgra() { return new SwapChainFormats(TextureFormat.bgra8UNorm); }
    static get bgraSrgb() { return new SwapChainFormats(TextureFormat.bgraSrgb); }
}

class SwapChainDesc {
    constructor(){
        this.size = new SwapChainSize();
        this.formats = SwapChainFormats.rgba;
        this.bufferCount = 2;
        this.usage = SwapChainUsage.renderTarget;
        this.transform = SwapChainTransform.optimal;
        this.defaultDepthValue = 1.0;
        this.defaultStencilValue = 0x0;
        this.isPrimary = true;
    }
}

module.exports = {
    SwapChainSize,
    SwapChainFormats,
    SwapChainDesc
};