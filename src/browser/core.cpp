#include "browser.hpp"

// only so we have access to the json library ww
#include <connector.hpp>

#include "../window/window.hpp"
#include "typehelpers.hpp"

static std::wstring GetRelativePath(const char* relPath) {
    WCHAR lpExePath[MAX_PATH];
    std::ignore = GetModuleFileNameW(nullptr, lpExePath, MAX_PATH);

    const std::filesystem::path fsExePath(lpExePath);
    const std::filesystem::path path = fsExePath.parent_path() / relPath / "index.html";

    wil::com_ptr<IUri> uri;
    CreateUri(path.c_str(), Uri_CREATE_ALLOW_IMPLICIT_FILE_SCHEME, 0, &uri);

    wil::unique_bstr uriBstr;
    uri->GetAbsoluteUri(&uriBstr);
    return std::wstring(uriBstr.get());
};

std::shared_ptr<browser::Browser> browser::CreateBrowser(const browser_config_t& browserConfig, const window_config_t& windowConfig) {
    if (!HAS_FLAG(browserConfig.flags, browser_flags::BF_ENABLE_DEBUG_CONSOLE))
        FreeConsole();

    auto _browser = std::make_shared<Browser>();

    _browser->m_windowHandle = window::Create(
        HAS_FLAG(windowConfig.flags, WF_RESIZEABLE),
        HAS_FLAG(windowConfig.flags, WF_HIDE_TITLE_BAR),
        HAS_FLAG(windowConfig.flags, WF_SHOW),
        windowConfig.width, windowConfig.height,
        windowConfig.startX, windowConfig.startY,
        windowConfig.name,
        windowConfig.icon,
        _browser.get()
    );

    ComPtr<CoreWebView2EnvironmentOptions> options = Make<CoreWebView2EnvironmentOptions>();
    options->put_AdditionalBrowserArguments(L"--disable-web-security");

    (void)CreateCoreWebView2EnvironmentWithOptions(nullptr, nullptr, options.Get(),
        Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>([_browser, browserConfig](HRESULT, ICoreWebView2Environment* environment) -> HRESULT {
            return environment->CreateCoreWebView2Controller(_browser->m_windowHandle,
            Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                [_browser, browserConfig](HRESULT, ICoreWebView2Controller* controller) -> HRESULT {
                    if (controller != nullptr) {
                        _browser->m_webViewController = controller;
                        _browser->m_webViewController->get_CoreWebView2(&_browser->m_webView2);
                    }

                    wil::com_ptr<ICoreWebView2Settings> comptrSettings;
                    _browser->m_webView2->get_Settings(&comptrSettings);
                    comptrSettings->put_IsScriptEnabled(TRUE);
                    comptrSettings->put_IsWebMessageEnabled(TRUE);
                    comptrSettings->put_AreDevToolsEnabled(HAS_FLAG(browserConfig.flags, BF_ALLOW_DEV_TOOLS));

                    wil::com_ptr<ICoreWebView2_13> comptrWebView2_13;
                    wil::com_ptr<ICoreWebView2Profile> comptrWebView2Profile;
                    wil::com_ptr<ICoreWebView2Profile2> comptrWebView2Profile2;

                    if (!FAILED(_browser->m_webView2->QueryInterface(IID_PPV_ARGS(&comptrWebView2_13)))) {
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

                    if (!FAILED(_browser->m_webView2->QueryInterface(IID_PPV_ARGS(&_browser->m_webView2_2)))) {
                        _browser->m_webView2_2->add_DOMContentLoaded(
                            Callback<ICoreWebView2DOMContentLoadedEventHandler>(
                                [_browser](ICoreWebView2*, ICoreWebView2DOMContentLoadedEventArgs*) -> HRESULT {
                                    _browser->OnDomContentLoaded();
                                    _browser->m_isReady = true;
                                    return S_OK;
                                }
                            ).Get(), nullptr
                        );
                    }

                    // update screen
                    RECT bounds;
                    GetClientRect(_browser->m_windowHandle, &bounds);
                    _browser->m_webViewController->put_Bounds(bounds);

                    const auto coreFunctions = L"const __mouse_data__={pos:{x:0,y:0},isDragging:!1};document.addEventListener(\"mousedown\",(_=>{1!=_.button&&\"drag\"!=getComputedStyle(_.target,null).getPropertyValue(\"--webkit-app-region\")||(__mouse_data__.isDragging=!0,__mouse_data__.pos.x=_.clientX,__mouse_data__.pos.y=_.clientY,document.getElementsByTagName(\"body\")[0].style.cursor=\"grabbing\")})),document.addEventListener(\"mousemove\",(_=>{__mouse_data__.isDragging&&invoke(\"MoveWindow\",[_.clientX-__mouse_data__.pos.x,_.clientY-__mouse_data__.pos.y])})),document.addEventListener(\"mouseup\",(()=>{__mouse_data__.isDragging&&(__mouse_data__.isDragging=!1,document.getElementsByTagName(\"body\")[0].style.cursor=\"\")}));const __function_data__={resultMap:new Map,registerdFunctions:{}};function __core_result_handler__(_,e){__function_data__.resultMap.get(_).resolve(e),__function_data__.resultMap.delete(_)}function __core_call_js__(_,...e){__function_data__.registerdFunctions[_]?__function_data__.registerdFunctions[_](...e):console.error(`Function ${_} not registered`)}async function invoke(_,e){return new Promise(((t,n)=>{if(__function_data__.resultMap.get(_))return __function_data__.resultMap.get(_);__function_data__.resultMap.set(_,{resolve:t,reject:n}),window.chrome.webview.postMessage({__name__:_,__args__:e})}))}function register(_){__function_data__.registerdFunctions[_.name]=_}";
                    _browser->m_webView2->AddScriptToExecuteOnDocumentCreated(coreFunctions, nullptr);

                    _browser->RegisterFunction("MoveWindow", &Browser::MoveWindow, _browser.get());

                    _browser->m_webView2->add_WebMessageReceived(
                        Callback<ICoreWebView2WebMessageReceivedEventHandler>(
                            [_browser](ICoreWebView2* sender, ICoreWebView2WebMessageReceivedEventArgs* args) -> HRESULT {
                                wil::unique_cotaskmem_string comptrWebMessageRaw;
                                args->get_WebMessageAsJson(&comptrWebMessageRaw);

                                const nlohmann::json webMessageJson = nlohmann::json::parse(WideStringToString(comptrWebMessageRaw.get()));

                                const auto functionName = webMessageJson.at("__name__").get<std::string>();
                                const auto functionArguments = webMessageJson.at("__args__").get<nlohmann::json>();

                                js_args_t arguments = {};

                                for (const auto& arg : functionArguments.get<nlohmann::json::array_t>()) {
                                    switch (arg.type()) {
                                        case nlohmann::detail::value_t::string:
                                            arguments.Push(js_value(arg.get<std::string>()));
                                            break;
                                        case nlohmann::detail::value_t::number_unsigned:
                                        case nlohmann::detail::value_t::number_integer:
                                            arguments.Push(js_value(arg.get<int>()));
                                            break;
                                        case nlohmann::detail::value_t::boolean:
                                            arguments.Push(js_value(arg.get<bool>()));
                                            break;
                                        case nlohmann::detail::value_t::number_float:
                                            arguments.Push(js_value(arg.get<float>()));
                                            break;
                                        case nlohmann::detail::value_t::array: {
                                                std::vector<js_value> out;

                                                for (const auto& sub_arg : arg.get<nlohmann::json::array_t>()) {
                                                    switch (sub_arg.type()) {
                                                        case nlohmann::detail::value_t::string:
                                                            out.push_back(js_value(sub_arg.get<std::string>()));
                                                            break;
                                                        case nlohmann::detail::value_t::number_unsigned:
                                                        case nlohmann::detail::value_t::number_integer:
                                                            out.push_back(js_value(sub_arg.get<int>()));
                                                            break;
                                                        case nlohmann::detail::value_t::boolean:
                                                            out.push_back(js_value(sub_arg.get<bool>()));
                                                            break;
                                                        case nlohmann::detail::value_t::number_float:
                                                            out.push_back(js_value(sub_arg.get<float>()));
                                                            break;
                                                        default:
                                                            break;
                                                    }
                                                }

                                                arguments.Push(js_value(out));
                                            }
                                            break;
                                        default:
                                            break;
                                    }
                                }

                                const auto& functionWrapper = _browser->m_methodMap.at(functionName);
                                const std::any functionResult = functionWrapper(_browser.get(), arguments);

                                const std::wstring resultHandlerScript = StringToWideString(
                                    "__core_result_handler__(\"" + functionName +
                                    "\"," + AnyToMappedString(functionResult) + ");");

                                return sender->ExecuteScript(resultHandlerScript.c_str(), nullptr);
                            }
                        ).Get(), nullptr
                    );

                    // load the page
                    return _browser->m_webView2->Navigate(GetRelativePath(browserConfig.relViewPath).c_str());
                }
            ).Get());
        }).Get()
    );

    return _browser;
}