const { getModule } = require('./native-module');
const { ObjectRegistry } = require('./object-registry');

function _handleRelease(ptr) {
    if(ptr == 0)
        return;
    ObjectRegistry.unlock(this);
    this._ptr = 0;
}

class NativeObject {
    constructor(handle) {
        const module = getModule();

        this.handle = handle;
        this.disposed = false;

        const func = _handleRelease.bind(this);
        this._funcPtr = module.addFunction(_handleRelease.bind(this), 'vi');
        this._func = func; // store function inside object, otherwise gc will collect
        ObjectRegistry.lock(this);
        getModule()._rengine_object_set_release_callback(this.handle, this._funcPtr);
    }

    get strongRefs() {
        if(!this.handle)
            return 0;
        return getModule()._rengine_object_strongref_count(this.handle);
    }
    get weakRefs() {
        if(!this.handle)
            return 0;
        return getModule()._rengine_object_weakref_count(this.handle);
    }

    dispose() {
        if(this.disposed)
            return;
        this.disposed = true;
        this.onDispose();

        let refs = Math.max(this.strongRefs, this.weakRefs);
        let handle = this.handle;
        this.handle = 0;

        if(refs > 0) {
            getModule()._rengine_object_set_release_callback(handle, 0);
            getModule()._rengine_object_releaseref(handle);
        }

        if(handle != 0)
            ObjectRegistry.unlock(handle);
    }

    onDispose() {}
}

module.exports = {
    NativeObject
};