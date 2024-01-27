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

/**
 * read memory address
 * @param {number} memAddr 
 * @param {keyof NativeType} type 
 */
function read(memAddr, type) {
    const _type = NativeType[type];
    const _idx = memAddr >> 2;
    const _module = getModule();
    const calls = [
        () => _module.HEAP32[_idx],
        () => _module.HEAPU32[_idx],
        () => _module.HEAPF32[_idx],
        () => Boolean(_module.HEAPU32[_idx]),
        () => _module.HEAPU32[_idx],
        () => _module.HEAPU32[_idx],
        () => _module.UTF8ToString(_module.HEAPU32[_idx])
    ];

    const call = _type < calls.length ? calls[_type] : () => void (0);
    return call();
}

/**
 * write memory address
 * @param {number} memAddr 
 * @param {*} value 
 * @param {keyof NativeType} type 
 */
function write(memAddr, value, type) {
    const _type = NativeType[type];
    const _idx = memAddr >> 2;
    const _module = getModule();
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

module.exports = {
    sizeof,
    getPtrSize,
    read,
    write,
    writeInstructions,
    readInstructions
};