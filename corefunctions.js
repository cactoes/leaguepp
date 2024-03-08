// ====================
// SECTION: window move / drag & cursor hooks
// ====================
const __mouse_data__ = {
    pos: {
        x: 0,
        y: 0,
    },
    isDragging: false,
};

document.addEventListener("mousedown", (mouseEvent) => {
    if (1 == mouseEvent.button || getComputedStyle(mouseEvent.target, null).getPropertyValue("--webkit-app-region") == "drag") {
        __mouse_data__.isDragging = true;
        __mouse_data__.pos.x = mouseEvent.clientX
        __mouse_data__.pos.y = mouseEvent.clientY;
        document.getElementsByTagName("body")[0].style.cursor = "grabbing";
    }
});

document.addEventListener("mousemove", (mouseEvent) => {
    if (__mouse_data__.isDragging) {
        invoke("MoveWindow", [
            mouseEvent.clientX - __mouse_data__.pos.x,
            mouseEvent.clientY - __mouse_data__.pos.y
        ]);
    }
});

document.addEventListener("mouseup", () => {
    if (__mouse_data__.isDragging) {
        __mouse_data__.isDragging = false;
        document.getElementsByTagName("body")[0].style.cursor = "";
    }
});

// ====================
// SECTION: cpp invokers & js invokers
// ====================
const __function_data__ = {
    resultMap: new Map(),
    registerdFunctions: {}
};

function __core_result_handler__(name, result) {
    __function_data__.resultMap.get(name).resolve(result);
    __function_data__.resultMap.delete(name);
}

function __core_call_js__(name, ...args) {
    if (__function_data__.registerdFunctions[name])
        __function_data__.registerdFunctions[name](...args);
    else
        console.error(`Function ${name} not registered`);
}

async function invoke(name, args) {
    return new Promise((reslove, reject) => {
        if (__function_data__.resultMap.get(name))
            return  __function_data__.resultMap.get(name);

        __function_data__.resultMap.set(name, { resolve: reslove, reject: reject });
        window.chrome.webview.postMessage({ __name__: name, __args__: args });
    });
}

function register(func) {
    __function_data__.registerdFunctions[func.name] = func;
}

// old
// const auto jsCppInvoker = L"let __result_map__=new Map;window.__result__handler__=function(_,e){let l=__result_map__.get(_);l&&(l.resolve(e),__result_map__.delete(_))};async function invoke(e,_){return new Promise(((r,s)=>{__result_map__.get(e)||(__result_map__.set(e,{resolve:r,reject:s}),window.chrome.webview.postMessage({__name__:e,__args__:_}))}))};function register(_){window.__invokables__[_.name]=_}window.__invokables__={};window.__call__=(_,...n)=>{null!=window.__invokables__[_]?window.__invokables__[_](...n):console.error(`Function ${_} not registered`)};";
// const auto jsDragScript = L"let __MX__,__MY__,__dragging__=!1;document.addEventListener(\"mousedown\",(_=>{(2==_.which||\"drag\"==getComputedStyle(_.target,null).getPropertyValue(\"--webkit-app-region\"))&&(__dragging__=!0,__MX__=_.clientX,__MY__=_.clientY);})),document.addEventListener(\"mousemove\",(_=>{if(__dragging__){const e=_.clientX-__MX__,n=_.clientY-__MY__;invoke(\"MoveWindow\",[e,n])}})),document.addEventListener(\"mouseup\",(()=>{__dragging__=!1}));";
