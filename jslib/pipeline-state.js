const { uint, allocString, free, malloc, getPtrSize, writeInstructions, write } = require("./core");
const { 
    BlendMode, 
    FillMode, 
    CullMode, 
    CompareMode, 
    StencilOp, 
    TextureFormat, 
    ElementType, 
    PrimitiveType, 
    TextureFilterMode, 
    TextureAddressMode 
} = require("./enums");
const { getModule } = require("./native-module");
const { NativeObject } = require("./native-object");
const { getNullShader, Shader } = require('./shader');

class PipelineBlendState {
    constructor() {
        this.colorWriteEnabled = true;
        this.blendMode = BlendMode.replace;
        this.alphaToCoverageEnabled = false;
    }
}
class PipelineRasterizerState {
    constructor() {
        this.fillMode = FillMode.solid;
        this.cullMode = CullMode.both;
        this.constantDepthBias = 0;
        this.slopeScaledDepthBias = 0;
        this.scissorTestEnabled = false;
        this.lineAntiAlias = false;
    }
}
class PipelineDepthStencilState {
    constructor(){
        this.enableDepth = true;
        this.depthWriteEnabled = false;
        this.stencilTestEnabled = false;
        this.depthCompareFunction = CompareMode.always;
        this.stencilCompareFunction = CompareMode.always;
        this.stencilOpOnPassed = StencilOp.keep;
        this.stencilOpOnStencilFailed = StencilOp.keep;
        this.stencilOpOnDepthFailed = StencilOp.keep;
        this.stencilCompareMask = 0;
        this.stencilWriteMask = 0;
    }
}
class PipelineStateOutputDesc {
    constructor() {
        this.depthStencilFormat = TextureFormat.unknown;
        /**
         * @type {number[]}
         */
        this.renderTargetFormats = [];
        this.multiSample = 1;
    }
}
class InputLayoutElementDesc {
    constructor() {
        this.bufferIndex = 0;
        this.bufferStride = this.elementOffset = uint.max;
        this.instanceStepRate = 0;
        this.elementType = ElementType.int;
        this.isNormalized = false;
    }
}
class PipelineInputLayoutElementDesc {
    constructor(){
        this.inputIndex = 0;
        this.input = new InputLayoutElementDesc();
    }
}
class GraphicsPipelineShaders {
    constructor() {
        /**
         * Vertex Shader
         * @type {Shader}
         */
        this.vertexShader
        /**
         * Pixel Shader
         * @type {Shader}
         */ 
        = this.pixelShader 
        /**
         * Domain Shader
         * @type {Shader}
         */
        = this.domainShader 
        /**
         * Hull Shader
         * @type {Shader}
         */
        = this.hullShader
        /**
         * Geometry Shader
         * @type {Shader}
         */
        = this.geometryShader
        = getNullShader();
    }
}
class TextureAddressModes {
    constructor(mode = TextureAddressMode.wrap) {
        this.u = mode;
        this.v = mode;
        this.w = mode;
    }
}
class SamplerStateDesc {
    /**
     * 
     * @param {TextureFilterMode?} filterMode 
     * @param {TextureAddressMode} addressMode 
     * @param {*} anisotropy 
     */
    constructor(filterMode = TextureFilterMode.nearest, addressMode = TextureAddressMode.wrap, anisotropy = 4) {
        this.filterMode = filterMode;
        this.anisotropy = anisotropy;
        this.shadowCompare = false;
        this.addressMode = new TextureAddressModes(addressMode);
    }
}
class ImmutableSamplerDesc {
    constructor(){
        this.name = '';
        this.sampler = new SamplerStateDesc();
    }
}

class GraphicsPipelineDesc {
    constructor(){
        this.name = '';
        this.blendState = new PipelineBlendState();
        this.rasterizerState = new PipelineRasterizerState();
        this.depthStencilState = new PipelineDepthStencilState();
        this.primitiveType = PrimitiveType.triangleList;
        /**
         * @type {Array<PipelineInputLayoutElementDesc>}
         */
        this.inputLayout = [];
        /**
         * @type {Array<ImmutableSamplerDesc>}
         */
        this.immutableSamplers = [];
        this.output = new PipelineStateOutputDesc();
        this.shaders = new GraphicsPipelineShaders();
    }
}

/**
 * 
 * @param {PipelineInputLayoutElementDesc} elementDesc 
 * @returns {number}
 */
function _createInputLayoutElementPtr(elementDesc) {
    const ptr = malloc(7 * getPtrSize());
    const instructions = [
        [()=> elementDesc.inputIndex, 'uint'],
        [()=> elementDesc.input.bufferIndex, 'uint'],
        [()=> elementDesc.input.bufferStride, 'uint'],
        [()=> elementDesc.input.elementOffset, 'uint'],
        [()=> elementDesc.input.instanceStepRate, 'uint'],
        [()=> elementDesc.input.elementType, 'uint'],
        [()=> elementDesc.input.isNormalized, 'bool']
    ];
    writeInstructions(instructions, ptr);
}

/**
 * 
 * @param {ImmutableSamplerDesc} sampler 
 */
function _createImmutableSamplerPtr(sampler) {
    const namePtr = allocString(sampler.name);
    const instructions = [
        [()=> namePtr, 'ptr'],
        [()=> sampler.sampler.filterMode, 'uint'],
        [()=> sampler.sampler.anisotropy, 'uint'],
        [()=> sampler.sampler.shadowCompare, 'bool'],
        [()=> sampler.sampler.addressMode.u, 'uint'],
        [()=> sampler.sampler.addressMode.v, 'uint'],
        [()=> sampler.sampler.addressMode.w, 'uint'],
    ];
    const ptr = malloc(instructions.length * getPtrSize());
    writeInstructions(instructions, ptr);

    return [ptr, namePtr];
}

/**
 * 
 * @param {GraphicsPipelineDesc} desc
 * @returns {[number, Function]} pointer to desc and dispose call 
 */
function createGraphicsPipelineDescPtr(desc) {
    const module = getModule();
    
    const namePtr = desc.name ? allocString(desc.name) : 0;
    const ptrs2Dispose = [];
    
    const disposeCalls = [
        ()=> ptrs2Dispose.forEach(x => free(x)),
    ];
    
    // copy input layout pointers to input layout array
    const inputLayoutsPtr = malloc(desc.inputLayout.length * getPtrSize());
    {
        desc.inputLayout.forEach((element, idx) => {
            const elementPtr = _createInputLayoutElementPtr(element);
            module.HEAPU32[(inputLayoutsPtr >> 2) + (idx * getPtrSize())] = elementPtr;
            ptrs2Dispose.push(elementPtr);
        });
        ptrs2Dispose.push(inputLayoutsPtr);
    }
    const rtFormatsPtr = malloc(desc.output.renderTargetFormats.length * getPtrSize());
    {
        desc.output.renderTargetFormats.forEach((fmt, idx) => {
            module.HEAPU32[(rtFormatsPtr >> 2) + (idx * getPtrSize())] = fmt;
        });
        ptrs2Dispose.push(rtFormatsPtr);
    }
    const immutableSamplersPtr = malloc(desc.immutableSamplers.length * getPtrSize());
    {
        desc.immutableSamplers.forEach((smpl, idx) => {
            const [smplPtr, smplNamePtr] = _createImmutableSamplerPtr(smpl);
            module.HEAPU32[(immutableSamplersPtr >> 2) + (idx * getPtrSize())] = smplPtr;

            ptrs2Dispose.push(smplNamePtr);
            ptrs2Dispose.push(smplPtr);
        });
        ptrs2Dispose.push(immutableSamplersPtr);
    }

    const instructions = [
        [()=> namePtr, 'ptr'],
        [()=> desc.blendState.colorWriteEnabled, 'bool'],
        [()=> desc.blendState.blendMode, 'uint'],
        [()=> desc.blendState.alphaToCoverageEnabled, 'bool'],
        [()=> desc.rasterizerState.fillMode, 'uint'],
        [()=> desc.rasterizerState.cullMode, 'uint'],
        [()=> desc.rasterizerState.constantDepthBias, 'float'],
        [()=> desc.rasterizerState.slopeScaledDepthBias, 'float'],
        [()=> desc.rasterizerState.scissorTestEnabled, 'bool'],
        [()=> desc.rasterizerState.lineAntiAlias, 'uint'],
        [()=> desc.depthStencilState.enableDepth, 'bool'],
        [()=> desc.depthStencilState.depthWriteEnabled, 'bool'],
        [()=> desc.depthStencilState.stencilTestEnabled, 'bool'],
        [()=> desc.depthStencilState.depthCompareFunction, 'uint'],
        [()=> desc.depthStencilState.stencilCompareFunction, 'uint'],
        [()=> desc.depthStencilState.stencilOpOnPassed, 'uint'],
        [()=> desc.depthStencilState.stencilOpOnStencilFailed, 'uint'],
        [()=> desc.depthStencilState.stencilOpOnDepthFailed, 'uint'],
        [()=> desc.depthStencilState.stencilCompareMask, 'uint'],
        [()=> desc.depthStencilState.stencilWriteMask, 'uint'],
        [()=> desc.primitiveType, 'uint'],
        [()=> inputLayoutsPtr, 'ptr'],
        [()=> desc.inputLayout.length, 'uint'],
        [()=> desc.output.depthStencilFormat, 'uint'],
        [()=> rtFormatsPtr, 'ptr'],
        [()=> desc.output.renderTargetFormats.length, 'uint'],
        [()=> desc.output.multiSample, 'uint'],
        [()=> immutableSamplersPtr, 'ptr'],
        [()=> desc.immutableSamplers.length, 'uint'],
        [()=> desc.shaders.vertexShader.handle, 'ptr'],
        [()=> desc.shaders.pixelShader.handle, 'ptr'],
        [()=> desc.shaders.domainShader.handle, 'ptr'],
        [()=> desc.shaders.hullShader.handle, 'ptr'],
        [()=> desc.shaders.geometryShader.handle, 'ptr'],
        [()=> 0, 'ptr']
    ];

    const descPtr = malloc(instructions.length * getPtrSize());
    writeInstructions(instructions, descPtr);

    return [descPtr, ()=> {
        disposeCalls.forEach(x => x());
    }];
}

class PipelineState extends NativeObject {
    /**
     * @constructor
     * @param {number} handle 
     * @param {GraphicsPipelineDesc} desc 
     */
    constructor(handle, desc) {
        super(handle);
        this.desc = Object.freeze(desc);
    }

    get name() {
        return this.desc.name;
    }
}

class NullPipelineState {
    constructor() {
        this.handle = 0;
        this.disposed = false;
        this.desc = new GraphicsPipelineDesc();
    }
    dispose() {}
}

const _nullPipelineState = new NullPipelineState();

function getNullPipelineState() {
    return _nullPipelineState;
}
module.exports = {
    PipelineBlendState,
    PipelineRasterizerState,
    PipelineDepthStencilState,
    PipelineStateOutputDesc,
    InputLayoutElementDesc,
    PipelineInputLayoutElementDesc,
    GraphicsPipelineShaders,
    TextureAddressModes,
    SamplerStateDesc,
    ImmutableSamplerDesc,
    GraphicsPipelineDesc,
    createGraphicsPipelineDescPtr,
    PipelineState,
    getNullPipelineState
};