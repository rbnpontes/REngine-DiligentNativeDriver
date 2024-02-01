const { CommandBuffer } = require("./command-buffer");
const { getPtrSize, write, writeInstructions, readInstructions, uint } = require("./core");
const { Device } = require("./device");
const { getModule } = require("./native-module");
const { ObjectRegistry } = require("./object-registry");
const { SwapChainDesc, getPtrFromSwapChainDesc, SwapChain } = require("./swapchain");

class EngineDriver {
    constructor(immediateCommand, device, factoryPtr, disposeCalls) {
        this.disposed = false;
        this.immediateCommand = immediateCommand;
        this.device = device;
        this.factoryPtr = factoryPtr;
        this._disposeCalls = disposeCalls;
    }

    dispose() {
        if (this.disposed)
            return;
        this.disposed = true;
        this.immediateCommand.dispose();
        this.device.dispose();
        ObjectRegistry.clearRegistry();

        this.immediateCommand = this.device = null;
        getModule()._rengine_object_releaseref(this.factoryPtr);

        this._disposeCalls.forEach(x => x());
    }
}

class GraphicsDriverSettings {
    constructor() {
        this.enableValidation = false;
        this.messageCallback = () => void (0);
    }
}
class DriverCreationDesc {
    constructor() {
        this.settings = new GraphicsDriverSettings();
        this.swapChainDesc = new SwapChainDesc();
        this.canvasSelector = '#canvas';
    }
}
class NativeWindow {
    constructor(selector) {
        this.selectorPtr = getModule().allocateUTF8(selector);
        this.handle = getModule()._malloc(getPtrSize());
        write(this.handle, this.selectorPtr, 'ptr');
    }

    dispose() {
        getModule()._free(this.handle);
        getModule()._free(this.selectorPtr);
    }
}

function _onMessageCallback(severity, messagePtr, messageFunc, messageFile, line) {
    const module = getModule();
    const args = [
        severity,
        module.UTF8ToString(messagePtr),
        module.UTF8ToString(messageFunc),
        module.UTF8ToString(messageFile),
        line
    ];

    if (!this.messageCallback) {
        console.log('[Driver Message]: ', args);
        return;
    }

    this.messageCallback.call(this, args);
}
/**
 * 
 * @param {GraphicsDriverSettings} settings 
 */
function createDriverSettingsPtr(settings) {
    const module = getModule();
    const maxUintValue = uint.max;
    const ptr = module._malloc(5 * getPtrSize());
    const messageCallbackPtr = module.addFunction(_onMessageCallback.bind(settings), 'viiiii');

    const instructions = [
        [() => settings.enableValidation, 'bool'],
        [() => 5, 'int'],
        [() => maxUintValue, 'uint'],
        [() => 0, 'uint'],
        [() => messageCallbackPtr, 'ptr']
    ];

    writeInstructions(instructions, ptr);
    return [ptr, messageCallbackPtr];
}
function allocDriverResult() {
    const ptr = getModule()._malloc(3 * getPtrSize());
    return ptr;
}
function readDriverResult(ptr) {
    const result = { driver: null, swapChain: null, error: '' };
    const instructions = [
        [(x) => result.driver = x, 'ptr'],
        [(x) => result.swapChain = x, 'ptr'],
        [(x) => result.error = x, 'string']
    ];
    readInstructions(instructions, ptr);
    getModule()._free(ptr);

    return result;
}
function readDriverData(ptr) {
    const result = { device: 0, context: 0, factory: 0 };
    const instructions = [
        [(x) => result.device = x, 'ptr'],
        [(x) => result.context = x, 'ptr'],
        [(x) => result.factory = x, 'ptr']
    ];
    readInstructions(instructions, ptr);
    return result;
}
/**
 * Create driver instance
 * @param {DriverCreationDesc} creationDesc 
 * @returns {[EngineDriver, SwapChain]} driver instance and SwapChain
 */
function createDriver(creationDesc) {
    const module = getModule();

    const { settings, swapChainDesc, canvasSelector } = creationDesc;

    const [settingsPtr, messageCallbackPtr] = createDriverSettingsPtr(settings);
    const swapChainDescPtr = getPtrFromSwapChainDesc(swapChainDesc);
    const nativeWindow = new NativeWindow(canvasSelector);
    const driverResultPtr = allocDriverResult();

    const releaseList = [
        () => module._free(swapChainDescPtr),
        () => nativeWindow.dispose(),
        () => module._free(driverResultPtr),
        () => module._free(settingsPtr)
    ];
    const freeResources = () => releaseList.forEach(x => x());

    module._rengine_create_driver(settingsPtr, swapChainDescPtr, nativeWindow.handle, driverResultPtr);
    const result = readDriverResult(driverResultPtr);

    if (result.error) {
        freeResources();
        throw new Error('[Driver]: ' + result.error);
    }

    const rawDriver = readDriverData(result.driver);
    const swapChain = new SwapChain(result.swapChain);

    const commandBuffer = new CommandBuffer(module.getValue(rawDriver.context, '*'));
    const device = new Device(rawDriver.device);

    return [
        new EngineDriver(commandBuffer, device, module.getValue(rawDriver.factory, '*'), [
            () => module._free(messageCallbackPtr)
        ]),
        swapChain
    ];
}

module.exports = {
    EngineDriver,
    GraphicsDriverSettings,
    DriverCreationDesc,
    createDriver
};