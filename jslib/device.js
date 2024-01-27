const { getModule } = require("./native-module");
const { NativeObject } = require("./native-object");

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
}

module.exports = {
    Device
};