#include "browser.hpp"

#include <nlohmann/json.hpp>
#include "helpers.hpp"

#define WM_HANDLE_BROWSER_EVENTS (WM_USER + 0x0001)

Browser::Browser(const Window& window, const BROWSER_CONFIG& config) : m_window(window) {
    SetWindowLongPtrA(m_window.GetWindowHandle(), GWLP_USERDATA, (LONG_PTR)this);
    
    RegisterFunction("MoveWindow", &Browser::MoveWindow, this);
    RegisterFunction("HandleWindowEvent", &Browser::HandleWindowEvent, this);

    ComPtr<CoreWebView2EnvironmentOptions> options = Make<CoreWebView2EnvironmentOptions>();
    options->put_AdditionalBrowserArguments(L"--disable-web-security");

    CreateCoreWebView2EnvironmentWithOptions(nullptr, nullptr, options.Get(),
        Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>([this, &config](HRESULT, ICoreWebView2Environment* environment) -> HRESULT {
            return environment->CreateCoreWebView2Controller(m_window.GetWindowHandle(),
            Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                [this, &config](HRESULT, ICoreWebView2Controller* controller) -> HRESULT {
                    if (controller != nullptr) {
                        m_webViewController = controller;
                        m_webViewController->get_CoreWebView2(&m_webView2);
                    }

                    wil::com_ptr<ICoreWebView2Settings> comptrSettings;
                    m_webView2->get_Settings(&comptrSettings);
                    comptrSettings->put_IsScriptEnabled(TRUE);
                    comptrSettings->put_IsWebMessageEnabled(TRUE);
                    comptrSettings->put_AreDevToolsEnabled(config.flags.Has<BROWSER_WINDOW_FLAG_DEV_TOOLS>());

                    wil::com_ptr<ICoreWebView2_13> comptrWebView2_13;
                    wil::com_ptr<ICoreWebView2Profile> comptrWebView2Profile;
                    wil::com_ptr<ICoreWebView2Profile2> comptrWebView2Profile2;

                    if (!FAILED(m_webView2->QueryInterface(IID_PPV_ARGS(&comptrWebView2_13)))) {
                        if (!FAILED(comptrWebView2_13->get_Profile(&comptrWebView2Profile))) {
                            if (!FAILED(comptrWebView2Profile->QueryInterface(IID_PPV_ARGS(&comptrWebView2Profile2)))) {
                                comptrWebView2Profile2->ClearBrowsingData(COREWEBVIEW2_BROWSING_DATA_KINDS_FILE_SYSTEMS |
                                    COREWEBVIEW2_BROWSING_DATA_KINDS_INDEXED_DB | COREWEBVIEW2_BROWSING_DATA_KINDS_WEB_SQL |
                                    COREWEBVIEW2_BROWSING_DATA_KINDS_CACHE_STORAGE | COREWEBVIEW2_BROWSING_DATA_KINDS_ALL_DOM_STORAGE |
                                    COREWEBVIEW2_BROWSING_DATA_KINDS_ALL_SITE | COREWEBVIEW2_BROWSING_DATA_KINDS_DISK_CACHE |
                                    COREWEBVIEW2_BROWSING_DATA_KINDS_DOWNLOAD_HISTORY | COREWEBVIEW2_BROWSING_DATA_KINDS_GENERAL_AUTOFILL |
                                    COREWEBVIEW2_BROWSING_DATA_KINDS_BROWSING_HISTORY | COREWEBVIEW2_BROWSING_DATA_KINDS_SETTINGS |
                                    COREWEBVIEW2_BROWSING_DATA_KINDS_ALL_PROFILE, nullptr);
                            }
                        }
                    }

                    if (!FAILED(m_webView2->QueryInterface(IID_PPV_ARGS(&m_webView2_2)))) {
                        m_webView2_2->add_DOMContentLoaded(
                            Callback<ICoreWebView2DOMContentLoadedEventHandler>(
                                [this](ICoreWebView2*, ICoreWebView2DOMContentLoadedEventArgs*) -> HRESULT {
                                    OnDomContentLoaded();
                                    m_isReady = true;
                                    m_webView2->ExecuteScript(StringToWideString("__core_set_title__(\"" + m_window.GetTitle() + "\")").c_str(), nullptr);
                                    return S_OK;
                                }
                            ).Get(), nullptr
                        );
                    }

                    // update screen
                    RECT bounds;
                    GetClientRect(m_window.GetWindowHandle(), &bounds);
                    m_webViewController->put_Bounds(bounds);

                    const auto coreFunctions = L"const __mouse_data__={pos:{x:0,y:0},isDragging:!1};document.addEventListener('mousedown',(e=>{1!=e.button&&'drag'!=getComputedStyle(e.target,null).getPropertyValue('--webkit-app-region')||(__mouse_data__.isDragging=!0,__mouse_data__.pos.x=e.clientX,__mouse_data__.pos.y=e.clientY,document.getElementsByTagName('body')[0].style.cursor='grabbing')})),document.addEventListener('mousemove',(e=>{__mouse_data__.isDragging&&invoke('MoveWindow',[e.clientX-__mouse_data__.pos.x,e.clientY-__mouse_data__.pos.y])})),document.addEventListener('mouseup',(()=>{__mouse_data__.isDragging&&(__mouse_data__.isDragging=!1,document.getElementsByTagName('body')[0].style.cursor='')}));const __function_data__={resultMap:new Map,registerdFunctions:{}};function __core_result_handler__(e,_){__function_data__.resultMap.get(e).resolve(_),__function_data__.resultMap.delete(e)}function __core_call_js__(e,..._){__function_data__.registerdFunctions[e]?__function_data__.registerdFunctions[e](..._):console.error(`Function ${e} not registered`)}async function invoke(e,_){return new Promise(((t,n)=>{if(__function_data__.resultMap.get(e))return __function_data__.resultMap.get(e);__function_data__.resultMap.set(e,{resolve:t,reject:n}),window.chrome.webview.postMessage({__name__:e,__args__:_})}))}function register(e){__function_data__.registerdFunctions[e.name]=e}function __core_set_title__(e){document.getElementById('title')&&(document.getElementById('title').innerText=e)}";
                    m_webView2->AddScriptToExecuteOnDocumentCreated(coreFunctions, nullptr);

                    m_webView2->add_WebMessageReceived(
                        Callback<ICoreWebView2WebMessageReceivedEventHandler>(
                            [this](ICoreWebView2* sender, ICoreWebView2WebMessageReceivedEventArgs* args) -> HRESULT {
                                wil::unique_cotaskmem_string comptrWebMessageRaw;
                                args->get_WebMessageAsJson(&comptrWebMessageRaw);

                                const nlohmann::json webMessageJson = nlohmann::json::parse(WideStringToString(comptrWebMessageRaw.get()));

                                const auto functionName = webMessageJson.at("__name__").get<std::string>();
                                const auto functionArguments = webMessageJson.at("__args__").get<nlohmann::json>();

                                JSArgs arguments = {};

                                for (const auto& arg : functionArguments.get<nlohmann::json::array_t>()) {
                                    switch (arg.type()) {
                                        case nlohmann::detail::value_t::string:
                                            arguments.push_back(JSValue(arg.get<std::string>()));
                                            break;
                                        case nlohmann::detail::value_t::number_unsigned:
                                        case nlohmann::detail::value_t::number_integer:
                                            arguments.push_back(JSValue(arg.get<int>()));
                                            break;
                                        case nlohmann::detail::value_t::boolean:
                                            arguments.push_back(JSValue(arg.get<bool>()));
                                            break;
                                        case nlohmann::detail::value_t::number_float:
                                            arguments.push_back(JSValue(arg.get<float>()));
                                            break;
                                        case nlohmann::detail::value_t::array: {
                                                std::vector<JSValue> out;

                                                for (const auto& sub_arg : arg.get<nlohmann::json::array_t>()) {
                                                    switch (sub_arg.type()) {
                                                        case nlohmann::detail::value_t::string:
                                                            out.push_back(JSValue(sub_arg.get<std::string>()));
                                                            break;
                                                        case nlohmann::detail::value_t::number_unsigned:
                                                        case nlohmann::detail::value_t::number_integer:
                                                            out.push_back(JSValue(sub_arg.get<int>()));
                                                            break;
                                                        case nlohmann::detail::value_t::boolean:
                                                            out.push_back(JSValue(sub_arg.get<bool>()));
                                                            break;
                                                        case nlohmann::detail::value_t::number_float:
                                                            out.push_back(JSValue(sub_arg.get<float>()));
                                                            break;
                                                        default:
                                                            break;
                                                    }
                                                }

                                                arguments.push_back(JSValue(out));
                                            }
                                            break;
                                        default:
                                            break;
                                    }
                                }

                                const auto& functionWrapper = m_methodMap.at(functionName);
                                const std::any functionResult = functionWrapper(this, arguments);

                                const std::wstring resultHandlerScript = StringToWideString(
                                    "__core_result_handler__(\"" + functionName +
                                    "\"," + AnyToMappedString(functionResult) + ");");

                                return sender->ExecuteScript(resultHandlerScript.c_str(), nullptr);
                            }
                        ).Get(), nullptr
                    );

                    // load the page
                    char target[MAX_PATH] = {};
                    GetFullPathNameA("view\\index.html", MAX_PATH, target, nullptr);

                    return m_webView2->Navigate(
                        StringToWideString(target).c_str()
                    );
                }
            ).Get());
        }).Get()
    );
}

void Browser::CallJSFunction(const std::string& name, const std::vector<std::any>& args) {
    EventManager::EVENT evt{};
    evt.args = args;
    evt.name = name;
    m_eventManager.Add(evt);

    PostMessageA(m_window.GetWindowHandle(), WM_HANDLE_BROWSER_EVENTS, 0, 0);
}

LRESULT Browser::OnWindowMessage(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    const auto handle = reinterpret_cast<Browser*>(GetWindowLongPtr(wnd, GWLP_USERDATA));

    switch (msg) {
        case WM_CLOSE:
            DestroyWindow(wnd);
            return FALSE;
        case WM_DESTROY:
            PostQuitMessage(0);
            return FALSE;
        case WM_SIZE: {
            RECT bounds;
            GetClientRect(wnd, &bounds);
            if (handle)
                handle->m_webViewController->put_Bounds(bounds);
            return FALSE;
        }
        case WM_HANDLE_BROWSER_EVENTS: {
            if (handle)
                handle->EmptyEventBus();
            return FALSE;
        }
        default:
            return DefWindowProcA(wnd, msg, wparam, lparam);
    }
}

void Browser::EmptyEventBus(void) {
    EventManager::EVENT evt{};
    while (m_eventManager.Get(evt)) {
        std::string script = "__core_call_js__(\"" + evt.name + "\",";

        for (const auto& arg : evt.args)
            script += AnyToMappedString(arg) + ",";

        script[script.size() - 1] = ')';

        m_webView2->ExecuteScript(StringToWideString(script).c_str(), nullptr);
    }
}

void Browser::MoveWindow(Browser*, JSArgs args) {
    RECT rect;
    GetWindowRect(m_window.GetWindowHandle(), &rect);

    int newLeft = rect.left + args.at(0).As<int>();
    int newTop = rect.top +args.at(1).As<int>();
    int newWidth = rect.right - rect.left;
    int newHeight = rect.bottom - rect.top;

    SetWindowPos(m_window.GetWindowHandle(), nullptr, newLeft, newTop, newWidth, newHeight,
        SWP_NOZORDER | SWP_NOACTIVATE);
}

bool Browser::HandleWindowEvent(Browser*, JSArgs args) {
    if (args.size() < 1)
        return false;

    const auto event = args.at(0).As<std::string>();

    if (event == "minimize")
        return ShowWindow(m_window.GetWindowHandle(), SW_MINIMIZE);

    if (event == "resize") {
        WINDOWPLACEMENT wp;
        wp.length = sizeof(WINDOWPLACEMENT);
        if (GetWindowPlacement(m_window.GetWindowHandle(), &wp))
            return ShowWindow(m_window.GetWindowHandle(), wp.showCmd != SW_SHOWMAXIMIZED ? SW_SHOWMAXIMIZED : SW_RESTORE);

        return false;
    }

    if (event == "close") {
        PostMessageA(m_window.GetWindowHandle(), WM_CLOSE, 0, 0);
        return true;
    }

    return false;
}

void Browser::OnDomContentLoaded() {
    if (m_eventMethodMap.contains("DOMContentLoaded"))
        for (const auto& pFunction : m_eventMethodMap.at("DOMContentLoaded"))
            pFunction(this);
}

void Browser::AddDOMContentLoadedHandler(std::function<void(Browser*)> func) {
    m_eventMethodMap["DOMContentLoaded"].emplace_back(func);
}

void Browser::RegisterFunction(const std::string& name, std::function<std::any(Browser*, JSArgs)> func) {
    m_methodMap[name] = func;
}