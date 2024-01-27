
let _trackingObjects = {};

function _lock(obj, ptr) {
    if(ptr == 0)
        throw new Error('Can´t lock object with null address');
    let target = _trackingObjects[ptr];
    if(target) {
        target = new WeakRef(obj);
        _trackingObjects[ptr] = target;
        return;
    }

    _trackingObjects[ptr] = new WeakRef(obj);
}

class ObjectRegistry {
    static lock(obj, handle = 0) {
        if(handle == 0)
            handle = obj.handle;
        _lock(obj, handle);
    }
    static unlock(handle) {
        _trackingObjects[handle] = null;
    }
    static acquire(handle) {
        let ref = _trackingObjects[handle];
        if(!ref)
            return null;
        const target = ref.deref();
        return target ? target : null;
    }
    static clearRegistry() {
        _trackingObjects = {};
    }
}

module.exports = {
    ObjectRegistry
};