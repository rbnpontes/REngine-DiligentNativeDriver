const { init } = require('./native-module');
const exportsModule = require('./exports');

async function _initEngineModule() {
    await init();
    return {...exportsModule};

}

window.rengine_init = _initEngineModule;