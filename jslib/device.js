const { getModule } = require("./native-module");
const { PipelineState, getNullPipelineState, createGraphicsPipelineDescPtr } = require("./pipeline-state");
const { Result } = require("./result");
const { ShaderCreateInfo, createShaderCiPtr, getNullShader, Shader } = require("./shader");

class Device {
    /**
     * 
     * @param {number} handle 
     */
    constructor(handle) {
        this.handle = handle;
        this.disposed = false;
    }

    dispose() {
        if(this.disposed)
            return;
        getModule()._rengine_object_releaseref(this.handle);
        this.disposed = true;
    }

    /**
     * 
     * @param {ShaderCreateInfo} createInfo 
     */
    createShader(createInfo) {
        if(this.disposed || !createInfo)
            return getNullShader();

        const [shaderCiPtr, disposableCall] = createShaderCiPtr(createInfo);
        const result = new Result();
        getModule()._rengine_device_create_shader(this.handle, shaderCiPtr, result.getResultPtr());
        
        result.load();
        result.dispose();
        disposableCall();

        if(result.error)
            throw new Error(result.error);
        if(result.value == 0)
            throw new Error('Could not possible to create shader. Value: null');
        return new Shader(result.value, createInfo);
    }

    /**
     * 
     * @param {GraphicsPipelineDesc} pipelineDesc 
     * @returns {PipelineState} return a pipeline state
     */
    createPipelineState(pipelineDesc) {
        if(this.disposed || !pipelineDesc)
            return getNullPipelineState();
        const [pipelineDescPtr, disposableCall] = createGraphicsPipelineDescPtr(pipelineDesc);
        const result = new Result();
        getModule()._rengine_device_create_graphicspipeline(
            this.handle,
            pipelineDescPtr,
            0x1,
            result.getResultPtr()
        );

        result.load();
        result.dispose();
        disposableCall();

        if(result.error)
            throw new Error(result.error);
        if(result.value == 0)
            throw new Error('Could not possible to create pipeline state. Value: null');
        return new PipelineState(result.value, pipelineDesc);
    }
}

module.exports = {
    Device
};