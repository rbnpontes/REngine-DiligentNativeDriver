const Module = require('./REngine-DiligentNativeDriver.js');

function bootstrap(module){
    const sizeof = {
        bool    : module.HEAP8.BYTES_PER_ELEMENT,
        int8    : module.HEAP8.BYTES_PER_ELEMENT,
        int16   : module.HEAP16.BYTES_PER_ELEMENT,
        int32   : module.HEAP32.BYTES_PER_ELEMENT,
        int64   : module.HEAP32.BYTES_PER_ELEMENT,
        uint8   : module.HEAPU8.BYTES_PER_ELEMENT,
        uint16  : module.HEAPU16.BYTES_PER_ELEMENT,
        uint32  : module.HEAPU32.BYTES_PER_ELEMENT,
        uint64  : module.HEAPU32.BYTES_PER_ELEMENT,
        ptr     : module.HEAP32.BYTES_PER_ELEMENT,
        function: module.HEAP32.BYTES_PER_ELEMENT,
        string  : module.HEAP32.BYTES_PER_ELEMENT,
        float   : module.HEAPF32.BYTES_PER_ELEMENT,
        double  : module.HEAPF64.BYTES_PER_ELEMENT
    };
    const valueTypes = {
        ptr: '*',
        bool: 'i8',
        int8 : 'i8',
        int16 : 'i16',
        int32: 'i32',
        int64 : 'i64',
        uint8 : 'i8',
        uint16: 'i16',
        uint32: 'i32',
        uint64: 'i64',
        function: 'i32',
        string: 'i32',
        float: 'float',
        double: 'double'
    };
    const uint = {
        min : 0,
        max : Math.pow(2, 32) - 1,
    };


    function _writeInstructions(instructions, mem){
        if(!Array.isArray(instructions))
            instructions = Array.from(instructions);

        let offset = 0;
        instructions.forEach((x) => {
            const [call, sizeType] = x;
            const currSize = sizeof[sizeType];
            const ptr = module.getValue(mem + offset, '*');
            module.setValue(ptr, call(), valueTypes[sizeType]);
            offset += currSize;
        });
    }
    function _readInstructions(instructions, mem){
        if(!Array.isArray(instructions))
            instructions = Array.from(instructions);
        let offset = 0;
        instructions.forEach((x)=> {
            const [call, sizeType] = x;
            const currSize = sizeof[sizeType];
            const ptr = module.getValue(mem + offset, '*');
            const value = module.getValue(ptr, valueTypes[sizeType]);
            call(value);
            offset += currSize;
        });
    }
    function _allocString(str) {
        // string must end with null terminator
        const ptr = module._malloc(str.length + 1);
        module.stringToUTF8(str, ptr, str.length);
        return ptr;
    }

    class GraphicsDriverSettings {
        constructor() {
            this.enableValidation = false;
            this.messageCallback = ()=> {};
        }

        static onMessageCallback(severity, messagePtr, messageFunc, messageFile, line)
        {
            this.messageCallback.call(
                null,
                severity,
                module.UTF8ToString(messagePtr),
                module.UTF8ToString(messageFunc),
                module.UTF8ToString(messageFile),
                line
            );
        }

        static set(instance, mem) {
            const instructions = [
                [()=> instance.enableValidation ? 0xFF : 0x00, 'bool'],
                [()=> 5, 'int8'],
                [()=> uint.max, 'uint32'],
                [()=> 0, 'uint32'],
                [()=> module.addFunction(GraphicsDriverSettings.onMessageCallback.bind(this), 'viiiii'), 'function']
            ];
            _writeInstructions(instructions, mem);
        }

        static offsets() {
            return [
                sizeof.int8, // enableValidation
                sizeof.int8, // backend
                sizeof.int32, // adapterId
                sizeof.int32, // numDeferredCtx
                sizeof.function // messageCallback
            ];
        }
        static sizeof() {
            return this.offsets().reduce((prev, curr)=> prev + curr);
        }
    }
    class SwapChainDesc {
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
                [()=> instance.width, 'int32'],
                [()=> instance.height, 'int32'],
                [()=> instance.colorFormat, 'int16'],
                [()=> instance.depthFormat, 'int16'],
                [()=> instance.usage, 'int32'],
                [()=> instance.transform, 'int32'],
                [()=> instance.bufferCount, 'int32'],
                [()=> instance.defaultDepthValue, 'float'],
                [()=> instance.defaultStencilValue, 'int8'],
                [()=> instance.isPrimary, 'bool']
            ];
            _writeInstructions(instructions, mem);
        }
        static offsets() {
            return [
                sizeof.uint32, // width
                sizeof.uint32, // height
                sizeof.uint16, // colorFormat
                sizeof.uint16, // depthFormat
                sizeof.uint32, // usage
                sizeof.uint32, // transform
                sizeof.uint32, // bufferCount,
                sizeof.float, // defaultDepthValue
                sizeof.uint8, // defaultStencilValue
                sizeof.bool // isPrimary
            ];
        }
        static sizeof() {
            return this.offsets().reduce((prev, curr)=> prev + curr);
        }
    }
    class GraphicsDriverResult {
        constructor() {
            this.driverPtr = null;
            this.swapChain = null;
            this.error = '';
        }

        static from(mem){
            const driver = new GraphicsDriverResult();
            const instructions = [
                [(x)=> driver.driverPtr = x, 'ptr'],
                [(x)=> driver.swapChain = x, 'ptr'],
                [(x)=> {
                    if(x != 0)
                        driver.error = module.UTF8ToString(x);
                }, 'ptr']
            ];
            _readInstructions(instructions, mem);
            return driver;
        }

        static offsets() {
            return [
                sizeof.int32,
                sizeof.int32,
                sizeof.string
            ];
        }
        static sizeof() {
            return this.offsets().reduce((prev, curr)=> prev + curr);
        }
    }

    function createDriver(creationDesc){
        const ci = creationDesc ?? {
            driverSettings : new GraphicsDriverSettings(),
            swapChainDesc : new SwapChainDesc(),
            canvasSelector: '#canvas'
        };

        const settings = ci.driverSettings ?? new GraphicsDriverSettings();
        const swapChainDesc = ci.swapChainDesc ??  new SwapChainDesc();
        const canvasSelector = ci.canvasSelector ?? "#canvas";

        const settingsPtr = module._malloc(GraphicsDriverSettings.sizeof());
        const swapChainPtr = module._malloc(SwapChainDesc.sizeof());
        const nativeWindowPtr = _allocString(canvasSelector);
        const resultPtr = module._malloc(GraphicsDriverResult.sizeof());

        GraphicsDriverSettings.set(settings, settingsPtr);
        SwapChainDesc.set(swapChainDesc, swapChainPtr);

        module._rengine_create_driver(settingsPtr, swapChainPtr, nativeWindowPtr, resultPtr);

        const result = GraphicsDriverResult.from(resultPtr);
        
        module._free(settingsPtr);
        module._free(swapChainPtr);
        module._free(nativeWindowPtr);
        module._free(resultPtr);

        if(result.error != '')
            throw new Error(result.error);

        console.log('REngine: Created Graphics Driver');
        return result;
    }


    return {
        SwapChainDesc,
        GraphicsDriverSettings,
        createDriver,
    };
}

async function init() {
    const module = await Module();
    await module.ready;
    return bootstrap(module);

}

window.rengine_init = init;