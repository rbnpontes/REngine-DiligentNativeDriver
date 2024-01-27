const wasmModule = require('librengine-module.js');

let _module = null;

async function init() {
    _module = await wasmModule();
    await _module.ready;
    return _module;
}

function getModule() {
    return _module;
}

module.exports = {
    init,
    getModule
};