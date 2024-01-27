const { getPtrSize, readInstructions, memset } = require("./core");
const { getModule } = require("./native-module");

const $ptr = Symbol('__ptr');
class Result {
    constructor() {
        this.disposed = false;
        const ptr = getModule()._malloc(2 * getPtrSize());
        memset(ptr, 2 * getPtrSize(), 0x0);

        this.value = 0;
        this.error = '';
        this[$ptr] = ptr;
    }

    dispose() {
        if(this.disposed)
            return;
        this.disposed = true;
        getModule()._free(this[$ptr]);
    }

    load() {
        const instructions = [
            [x => this.value = x, 'ptr'],
            [x => this.error = x, 'string']
        ];
        readInstructions(instructions, this[$ptr]);
    }

    getResultPtr() {
        return this[$ptr];
    }
}

module.exports = { Result };