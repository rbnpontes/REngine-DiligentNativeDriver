const { getPtrSize, writeInstructions } = require("./core");
const { ShaderType } = require("./enums");
const { getModule } = require("./native-module");
const { NativeObject } = require("./native-object");

class ShaderCreateInfo {
    constructor() {
        this.name = '';
        this.sourceCode = '';
        this.type = ShaderType.unknow;
        this.macros = {};
    }
}

/**
 * 
 * @param {ShaderCreateInfo} ci 
 * @returns {[number, Function]} return pointer and disposable call
 */
function createShaderCiPtr(ci) {
    const module = getModule();
    const ptrSize = getPtrSize();
    const ptr = module._malloc(8 * ptrSize);

    const namePtr = ci.name ? module.allocateUTF8(ci.name) : 0;
    const srcCodePtr = ci.name ? module.allocateUTF8(ci.sourceCode) : 0;
    const stringPointers = [namePtr, srcCodePtr];

    const macroEntries = Object.entries(ci.macros ?? {});
    const macroKeysPtr = macroEntries.length == 0 ? 0 : module._malloc(ptrSize * macroEntries.length);
    const macroValuesPtr = macroEntries.length == 0 ? 0 : module._malloc(ptrSize * macroEntries.length);

    const macroKeyInstructions = [];
    const macroValueInstructions = [];
    macroEntries.forEach(entry => {
        const [key, value] = entry;
        const keyPtr = module.allocateUTF8(key);
        const valuePtr = module.allocateUTF8(value);

        stringPointers.push(keyPtr);
        stringPointers.push(valuePtr);
        macroKeyInstructions.push([
            () => keyPtr, 'ptr'
        ]);
        macroValueInstructions.push([
            () => valuePtr, 'ptr'
        ]);
    });

    writeInstructions(macroKeyInstructions, macroKeysPtr);
    writeInstructions(macroValueInstructions, macroValuesPtr);

    const descInstructions = [
        [() => namePtr, 'ptr'],
        [() => ci.type, 'uint'],
        [() => srcCodePtr, 'ptr'],
        [() => 0, 'ptr'],
        [() => 0, 'uint'],
        [() => macroKeysPtr, 'ptr'],
        [() => macroValuesPtr, 'ptr'],
        [() => macroEntries.length, 'uint']
    ];

    writeInstructions(descInstructions, ptr);

    return [ptr, () => {
        stringPointers.forEach(x => module._free(x));
    }];
}

class Shader extends NativeObject {
    /**
     * 
     * @param {number} handle 
     * @param {ShaderCreateInfo} desc 
     */
    constructor(handle, desc) {
        super(handle);
        this.desc = { ...desc };
    }

    get type() { return this.desc.type; }
    get name() { return this.desc.name; }
}

class NullShader {
    constructor() {
        this.handle = 0;
        this.desc = new ShaderCreateInfo();
        this.disposed = false;
    }

    get type() { return ShaderType.unknow; }
    get name() { return "Null Shader"; }
    dispose() {}
}

const _nullShader = new NullShader();
const getNullShader = ()=> _nullShader;

module.exports = {
    ShaderCreateInfo,
    createShaderCiPtr,
    Shader,
    getNullShader
};