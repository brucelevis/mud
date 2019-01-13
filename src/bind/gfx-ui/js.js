
// Bindings utilities
function WrapperObject() {
}
// Object
function Object() { throw "cannot construct a Object, no constructor in IDL" }
Object.prototype = Object.create(WrapperObject.prototype);
Object.prototype.constructor = Object;
Object.prototype.__class__ = Object;
Object.__cache__ = {};
Module['Object'] = Object;
Object.prototype["__destroy__"] = Object.prototype.__destroy__ = function() {
    var self = this.ptr;
    _mud_Object__destroy(self);
};
// ViewerController
function ViewerController() { throw "cannot construct a ViewerController, no constructor in IDL" }
ViewerController.prototype = Object.create(WrapperObject.prototype);
ViewerController.prototype.constructor = ViewerController;
ViewerController.prototype.__class__ = ViewerController;
ViewerController.__cache__ = {};
Module['ViewerController'] = ViewerController;
ViewerController.prototype["__destroy__"] = ViewerController.prototype.__destroy__ = function() {
    var self = this.ptr;
    _mud_ViewerController__destroy(self);
};
// OrbitController
function OrbitController() { throw "cannot construct a OrbitController, no constructor in IDL" }
OrbitController.prototype = Object.create(WrapperObject.prototype);
OrbitController.prototype.constructor = OrbitController;
OrbitController.prototype.__class__ = OrbitController;
OrbitController.__cache__ = {};
Module['OrbitController'] = OrbitController;
OrbitController.prototype["set_eye"] = OrbitController.prototype.set_eye = function(rotation) {
    /* rotation <quat> [] */
    rotation = rotation.ptr;
    _mud_OrbitController_set_eye_1(rotation);
};
OrbitController.prototype["set_target"] = OrbitController.prototype.set_target = function(position) {
    /* position <vec3> [] */
    position = position.ptr;
    _mud_OrbitController_set_target_1(position);
};
Object.defineProperty(OrbitController.prototype, "position", {
    get: function() {
        var self = this.ptr;
        return wrapPointer(_mud_OrbitController__get_position(self), mud::vec3);
    },
    set: function(position) {
        var self = this.ptr;
        /* position <vec3> [] */
        position = position.ptr;
        _mud_OrbitController__set_position(self, position);
    }
});
Object.defineProperty(OrbitController.prototype, "yaw", {
    get: function() {
        var self = this.ptr;
        return _mud_OrbitController__get_yaw(self);
    },
    set: function(yaw) {
        var self = this.ptr;
        /* yaw <float> [] */
        _mud_OrbitController__set_yaw(self, yaw);
    }
});
Object.defineProperty(OrbitController.prototype, "pitch", {
    get: function() {
        var self = this.ptr;
        return _mud_OrbitController__get_pitch(self);
    },
    set: function(pitch) {
        var self = this.ptr;
        /* pitch <float> [] */
        _mud_OrbitController__set_pitch(self, pitch);
    }
});
Object.defineProperty(OrbitController.prototype, "distance", {
    get: function() {
        var self = this.ptr;
        return _mud_OrbitController__get_distance(self);
    },
    set: function(distance) {
        var self = this.ptr;
        /* distance <float> [] */
        _mud_OrbitController__set_distance(self, distance);
    }
});
OrbitController.prototype["__destroy__"] = OrbitController.prototype.__destroy__ = function() {
    var self = this.ptr;
    _mud_OrbitController__destroy(self);
};
// FreeOrbitController
function FreeOrbitController() { throw "cannot construct a FreeOrbitController, no constructor in IDL" }
FreeOrbitController.prototype = Object.create(WrapperObject.prototype);
FreeOrbitController.prototype.constructor = FreeOrbitController;
FreeOrbitController.prototype.__class__ = FreeOrbitController;
FreeOrbitController.__cache__ = {};
Module['FreeOrbitController'] = FreeOrbitController;
FreeOrbitController.prototype["__destroy__"] = FreeOrbitController.prototype.__destroy__ = function() {
    var self = this.ptr;
    _mud_FreeOrbitController__destroy(self);
};
// SpaceSheet
function SpaceSheet() { throw "cannot construct a SpaceSheet, no constructor in IDL" }
SpaceSheet.prototype = Object.create(WrapperObject.prototype);
SpaceSheet.prototype.constructor = SpaceSheet;
SpaceSheet.prototype.__class__ = SpaceSheet;
SpaceSheet.__cache__ = {};
Module['SpaceSheet'] = SpaceSheet;
SpaceSheet.prototype["__destroy__"] = SpaceSheet.prototype.__destroy__ = function() {
    var self = this.ptr;
    _mud_SpaceSheet__destroy(self);
};
// Viewer
function Viewer() { throw "cannot construct a Viewer, no constructor in IDL" }
Viewer.prototype = Object.create(WrapperObject.prototype);
Viewer.prototype.constructor = Viewer;
Viewer.prototype.__class__ = Viewer;
Viewer.__cache__ = {};
Module['Viewer'] = Viewer;
Object.defineProperty(Viewer.prototype, "scene", {
    get: function() {
        var self = this.ptr;
        return wrapPointer(_mud_Viewer__get_scene(self), mud::Scene);
    },
    set: function(scene) {
        var self = this.ptr;
        /* scene <Scene> [] */
        scene = scene.ptr;
        _mud_Viewer__set_scene(self, scene);
    }
});
Object.defineProperty(Viewer.prototype, "viewport", {
    get: function() {
        var self = this.ptr;
        return wrapPointer(_mud_Viewer__get_viewport(self), mud::Viewport);
    }});
Object.defineProperty(Viewer.prototype, "position", {
    get: function() {
        var self = this.ptr;
        return wrapPointer(_mud_Viewer__get_position(self), mud::vec2);
    },
    set: function(position) {
        var self = this.ptr;
        /* position <vec2> [] */
        position = position.ptr;
        _mud_Viewer__set_position(self, position);
    }
});
Object.defineProperty(Viewer.prototype, "size", {
    get: function() {
        var self = this.ptr;
        return wrapPointer(_mud_Viewer__get_size(self), mud::vec2);
    },
    set: function(size) {
        var self = this.ptr;
        /* size <vec2> [] */
        size = size.ptr;
        _mud_Viewer__set_size(self, size);
    }
});
Viewer.prototype["__destroy__"] = Viewer.prototype.__destroy__ = function() {
    var self = this.ptr;
    _mud_Viewer__destroy(self);
};
// SceneViewer
function SceneViewer() { throw "cannot construct a SceneViewer, no constructor in IDL" }
SceneViewer.prototype = Object.create(WrapperObject.prototype);
SceneViewer.prototype.constructor = SceneViewer;
SceneViewer.prototype.__class__ = SceneViewer;
SceneViewer.__cache__ = {};
Module['SceneViewer'] = SceneViewer;
SceneViewer.prototype["__destroy__"] = SceneViewer.prototype.__destroy__ = function() {
    var self = this.ptr;
    _mud_SceneViewer__destroy(self);
};

(function() {
    function setupEnums() {
        // OrbitMode
        Module['OrbitMode']['ThirdPerson'] = _mud_ui_OrbitMode_ThirdPerson();
        Module['OrbitMode']['Isometric'] = _mud_ui_OrbitMode_Isometric();
        Module['OrbitMode']['PseudoIsometric'] = _mud_ui_OrbitMode_PseudoIsometric();
    }
    if (Module['calledRun']) setupEnums();
    else addOnPreMain(setupEnums);
})();