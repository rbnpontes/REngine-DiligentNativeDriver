const { getModule } = require('./native-module');

const NativeType = {
    int: 0,
    uint: 1,
    float: 2,
    bool: 3,
    ptr: 4,
    function: 5,
    string: 6
};

function getPtrSize() {
    return getModule().HEAP32.BYTES_PER_ELEMENT;
}

/**
 * get size of type
 * @param {FunctionConstructor} type 
 */
function sizeof(type) {
    if (type.sizeof)
        return type.sizeof();
    const instance = new type();
    return Object.keys(instance).length * getPtrSize();
}

function _readCall(memAddr, type, module) {
    const _type = NativeType[type];
    const _idx = memAddr >> 2;
    const _module = module ?? getModule();
    const calls = [
        () => _module.HEAP32[_idx],
        () => _module.HEAPU32[_idx],
        () => _module.HEAPF32[_idx],
        () => Boolean(_module.HEAPU32[_idx]),
        () => _module.HEAPU32[_idx],
        () => _module.HEAPU32[_idx],
        () => getString(memAddr)
    ];

    return _type < calls.length ? calls[_type] : () => void (0);
}

/**
 * read memory address
 * @param {number} memAddr 
 * @param {keyof NativeType} type 
 */
function read(memAddr, type) {
    return _readCall(memAddr, type)();
}


function _writeCall(memAddr, type, module) {
    const _type = NativeType[type];
    const _idx = memAddr >> 2;
    const _module = module ?? getModule();
    const calls = [
        (x) => _module.HEAP32[_idx] = x,
        (x) => _module.HEAPU32[_idx] = x,
        (x) => _module.HEAPF32[_idx] = x,
        (x) => _module.HEAPU32[_idx] = x ? 0x1 : 0x0,
        (x) => _module.HEAPU32[_idx] = x,
        () => { throw new Error('not supported function write. you must convert function to ptr and use ptr type instead.'); },
        () => { throw new Error('not supported string write. you must convert string to ptr and use ptr type instead.'); },
    ];

    const call = _type < calls.length ? calls[_type] : () => void (0);
    return call;
}
/**
 * write memory address
 * @param {number} memAddr 
 * @param {*} value 
 * @param {keyof NativeType} type 
 */
function write(memAddr, value, type) {
    const call = _writeCall(memAddr, type);
    call(value);
}

/**
 * helper method used to write a list of instructions
 * on a given memory address
 * @param {[Function, keyof NativeType][]} instructions 
 * @param {number} memAddr 
 */
function writeInstructions(instructions, memAddr) {
    if (!Array.isArray(instructions))
        return;
    instructions.forEach((pair) => {
        const [call, type] = pair;
        const value = call();
        write(memAddr, value, type);
        memAddr += getPtrSize();
    });
}

/**
 * Works on the same way of @type {writeInstructions}
 * But instead of resolve types at each call
 * This compile method resolves this types into a single
 * optimized call.
 * Note: you still pay the process of build, this is recommended
 * if you wan't call multiple times.
 * If you are running this code under V8, then V8 can optimize this
 * call in a low level.
 * @param {[Function, keyof NativeType][]} instructions 
 * @param {number} memAddr 
 */
function compileWriteInstructions(instructions, memAddr) {
    if(!Array.isArray(instructions))
        return ()=> void(0);
    const module = getModule();
    const calls = instructions.map((pair, idx)=> {
        const [call, type] = pair;
        const instWriteCall = _writeCall(memAddr + (idx * getPtrSize()), type, module);
        return ()=> instWriteCall(call());
    });
    return ()=> {
        let i = calls.length - 1;
        while(i >= 0){
            calls[i]();
            --i;
        }
    };
}

/**
 * helper method used to read a memory address from an list of instructions
 * @param {[Function, keyof NativeType][]} instructions 
 * @param {number} memAddr
 */
function readInstructions(instructions, memAddr) {
    if (!Array.isArray(instructions))
        return;
    instructions.forEach((pair)=> {
        const [call, type] = pair;
        const value = read(memAddr, type);
        call(value);
        memAddr += getPtrSize();
    });
}

/**
 * this method works in the same way of @type {readInstructions}
 * But instead of resolve types at each read call
 * This method resolves first all dependencies to make
 * an optimized call, ideal if you need to read a memory address
 * multiple times
 * @param {*} instructions 
 * @param {*} memAddr 
 * @returns 
 */
function compileReadInstructions(instructions, memAddr) {
    if(!Array.isArray(instructions))
        return ()=> void(0);
    const module = getModule();
    const calls = instructions.map((pair, idx)=> {
        const [call, type] = pair;
        const readCall = _readCall(memAddr + (idx * getPtrSize()), type, module);
        return ()=> call(readCall());
    });
    return ()=> {
        let i = calls.length - 1;
        while(i >= 0) {
            calls[i]();
            --i;
        }
    };
}

/**
 * read string from pointer
 * @param {number} strPtr 
 * @returns {string}
 */
function getString(strPtr) {
    const module = getModule();
    return module.UTF8ToString(module.HEAPU32[strPtr >> 2]);
}

/**
 * allocate string pointer
 * @param {string} str
 * @returns {number} string ptr 
 */
function allocString(str) {
    if(!str)
        return 0;
    return getModule().allocateUTF8(str);
}

/**
 * set a value on whole memory
 * @param {number} ptr memory address
 * @param {number} size memory size
 * @param {number} value memory value
 */ 
function memset(ptr, size, value) {
    const maxValue = ptr + size;
    const module = getModule();
    while(ptr < maxValue) {
        module.HEAP8[(ptr >> 0)] = value;
        ++ptr;
    }
}

/**
 * Allocate memory address
 * @param {number} size
 * @returns {number} allocated memory address 
 */
function malloc(size) {
    if(size == 0)
        return 0;
    return getModule()._malloc(size);
}
/**
 * Free pointer
 * @param {number} ptr 
 */
function free(ptr) {
    if(ptr != 0)
        getModule()._free(ptr);
}


const uint = Object.freeze({
    min : 0,
    max : Math.pow(2, 32) - 1
});
module.exports = {
    uint,
    sizeof,
    getPtrSize,
    read,
    write,
    writeInstructions,
    compileWriteInstructions,
    readInstructions,
    compileReadInstructions,
    getString,
    allocString,
    memset,
    malloc,
    free
};