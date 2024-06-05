#include <filesystem>
#include <fstream>
#include <iostream>
#include <windows.h>
#include <string>
#include <ShlObj.h>
#include <Shlwapi.h>
#include <format>
#include <numeric>
#include <uiframework.hpp>
#include <shared/win32.hpp>
#include <shared/general.hpp>

static const char* g_registryPath = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\" APP_GUID;

// from official docs
HRESULT CreateLink(LPCSTR lpszPathObj, LPCSTR lpszPathLink, LPCSTR lpszDesc, LPCSTR lpszWorkingDir) { 
    HRESULT hres; 
    IShellLink* psl; 

    hres = CoInitialize(nullptr);
    if (!SUCCEEDED(hres))
        return hres;
 
    // Get a pointer to the IShellLink interface. It is assumed that CoInitialize
    // has already been called.
    hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&psl); 
    if (SUCCEEDED(hres)) { 
        IPersistFile* ppf; 
 
        // Set the path to the shortcut target and add the description. 
        psl->SetPath(lpszPathObj); 
        psl->SetDescription(lpszDesc); 
        psl->SetWorkingDirectory(lpszWorkingDir);
 
        // Query IShellLink for the IPersistFile interface, used for saving the 
        // shortcut in persistent storage. 
        hres = psl->QueryInterface(IID_IPersistFile, (LPVOID*)&ppf); 
 
        if (SUCCEEDED(hres)) { 
            WCHAR wsz[MAX_PATH]; 
 
            // Ensure that the string is Unicode. 
            MultiByteToWideChar(CP_ACP, 0, lpszPathLink, -1, wsz, MAX_PATH); 
            
            // Add code here to check return value from MultiByteWideChar 
            // for success.
 
            // Save the link by calling IPersistFile::Save. 
            hres = ppf->Save(wsz, TRUE); 
            ppf->Release(); 
        } 
        psl->Release(); 
    } 

    CoUninitialize();

    return hres;
}

// extracts a resource from the exe to the target
bool ExtractResource(const char* resourceType, const char* resourceName, const std::string& outputPath) {
    HMODULE hModule = GetModuleHandleA(nullptr);
    if (!hModule) return false;

    HRSRC hRes = FindResourceA(hModule, resourceName, resourceType);
    if (!hRes) return false;

    HGLOBAL hResLoad = LoadResource(hModule, hRes);
    if (!hResLoad) return false;

    auto pResLock = (char*)LockResource(hResLoad);
    if (!pResLock) return false;

    DWORD resSize = SizeofResource(hModule, hRes);
    if (resSize == 0) return false;

    std::ofstream outFile(outputPath, std::ios::out | std::ios::binary);
    outFile.write(pResLock, resSize);

    return true;
}

std::filesystem::path GetTempFolderPath() {
    char tempPath[MAX_PATH];
    GetTempPathA(MAX_PATH, tempPath);
    return std::filesystem::path(tempPath) / "league++.cache";
}

bool InitInstallDir(const std::string& clientPath) {
    if (!CreateDirectoryA(clientPath.c_str(), nullptr)) {
        DWORD err = GetLastError();
        switch (err) {
            case ERROR_ALREADY_EXISTS:
                break;
            case ERROR_PATH_NOT_FOUND:
            default:
                CreateMessageBox("error", 1, "installer_icon.png", "failed to create install dir", message_box_type::MB_ERROR);
                return false;
        }
    }

    return true;
}

// creates a shortcut in the start menu ui
bool CreateStartMenuShortcut(const std::string& clientPath) {
    auto path = win32::GetFolderPath(CSIDL_COMMON_PROGRAMS) / "league++.lnk";
    return SUCCEEDED(CreateLink((clientPath + "\\league++.exe").c_str(), path.string().c_str(), "league++ shortcut", clientPath.c_str()));
}

// creates a desktop shortcut
bool CreateDesktopShortcut(const std::string& clientPath) {
    auto path = win32::GetFolderPath(CSIDL_DESKTOP) / "league++.lnk";
    return SUCCEEDED(CreateLink((clientPath + "\\league++.exe").c_str(), path.string().c_str(), "league++ shortcut", clientPath.c_str()));
}

// installs the actual files from the install to the target
bool InstallClient(const std::string& target) {
    std::filesystem::path tempFolder = GetTempFolderPath();
    
    // create a temp folder for out data
    if (!CreateDirectoryA(tempFolder.string().c_str(), nullptr)) {
        DWORD err = GetLastError();
        switch (err) {
            case ERROR_ALREADY_EXISTS:
                break;
            case ERROR_PATH_NOT_FOUND:
            default:
                CreateMessageBox("error", 1, "installer_icon.png", "failed to create temp dir", message_box_type::MB_ERROR);
                return false;
        }
    }

    std::string tempTarget = (tempFolder / "league++.zip").string();

    if (!ExtractResource("ZIPFILE", "LEAGUEPP_ZIP", tempTarget)) {
        CreateMessageBox("error", 1, "installer_icon.png", "failed to extract the client", message_box_type::MB_ERROR);
        return false;
    }

    STARTUPINFO startupInfo{};
    startupInfo.cb = sizeof(STARTUPINFO);
    startupInfo.dwFlags = STARTF_USESHOWWINDOW;
    startupInfo.wShowWindow = SW_HIDE;

    std::string command = std::format("cmd.exe /c powershell.exe -nologo -noprofile -command Expand-Archive -Path \"{}\" -DestinationPath \"{}\"", tempTarget.c_str(), target.c_str());

    PROCESS_INFORMATION processInfo{};

    if (!CreateProcessA(nullptr, (char*)command.c_str(), nullptr, nullptr, true, 0, nullptr, nullptr, &startupInfo, &processInfo)) {
        CreateMessageBox("error", 1, "installer_icon.png", "failed to create sub process", message_box_type::MB_ERROR);
        return false;
    }

    WaitForSingleObject(processInfo.hProcess, INFINITE);
    CloseHandle(processInfo.hProcess);
    CloseHandle(processInfo.hThread);
    return true;
}

uintmax_t CalculateFolderSize(const std::string& path) {
    auto iterator = std::filesystem::recursive_directory_iterator(path);
    return std::accumulate(
        std::filesystem::begin(iterator), std::filesystem::end(iterator), 0ull,
        [](std::uintmax_t total, const std::filesystem::directory_entry& entry) {
            return total + (std::filesystem::is_regular_file(entry) ? std::filesystem::file_size(entry) : 0);
        });
}

// initializes windows registery values
bool SetupRegistry(const std::string& clientPath) {
    auto entry = win32::RegistryEntry(g_registryPath);
    if (!entry.Init(true))
        return false;

    entry.SetRegValue<REG_EXPAND_SZ>("DisplayIcon",      clientPath + "\\league++.exe");
    entry.SetRegValue<REG_SZ>(       "DisplayName",      "league++ client");
    entry.SetRegValue<REG_SZ>(       "DisplayVersion",   (char*)BUILD_STRING);
    entry.SetRegValue<REG_EXPAND_SZ>("UninstallString",  clientPath + "\\uninstall.exe");
    entry.SetRegValue<REG_EXPAND_SZ>("InstallLocation",  clientPath);
    entry.SetRegValue<REG_SZ>(       "Publisher",        "NERV");
    entry.SetRegValue<REG_DWORD>(    "EstimatedSize",    (DWORD)((CalculateFolderSize(clientPath) + 1023) / 1024));
    return true;
}

int WinMain(HINSTANCE, HINSTANCE, LPSTR lpCmdLine, int) {
    bool checkBuildVersion = !IsDebug();
    std::vector<std::string> argv = win32::CommandLineToArgvExA(lpCmdLine);

    for (const auto& arg : argv) {
        if (arg == "-nobuildchk")
            checkBuildVersion = false;
    }

    auto entry = win32::RegistryEntry(g_registryPath);

    std::string installLocation {};
    if (entry.Init()) {
        entry.GetRegValue("InstallLocation", installLocation);
        if (!installLocation.empty())
            installLocation = (std::filesystem::path(installLocation) / "..").string();

        if (checkBuildVersion) {
            std::string targetVersion {};
            entry.GetRegValue("DisplayVersion", targetVersion);

            if (targetVersion == BUILD_STRING) {
                CreateMessageBox("info", 1, "installer_icon.png", "the latest league++ version has already been installed", message_box_type::MB_INFO);
                SystemPollWindowEvents();
                return 0;
            }
        }
    }

    WINDOW_CONFIG config{};
    config.flags = { WINDOW_FLAG_SHOW, WINDOW_FLAG_HIDE_TITLE_BAR };
    config.size = { 450, 200 };
    config.name = "league++ installer";
    config.iconId = 1;
    config.iconName = "installer_icon.png";

    BROWSER_CONFIG bwConfig{};
    // for debugging
    // bwConfig.flags = { BROWSER_FLAG_DEV_TOOLS };

    auto systemWindow = CreateSystemWindow(config, bwConfig, component::LAYOUT::VERTICAL);
    systemWindow->EnableContextMenu(false);

    auto frameMain = systemWindow->GetWindowFrame()->AddFrame("", false, component::LAYOUT::VERTICAL, component::ALIGN::NONE);

    auto contentFrame = frameMain->AddFrame("", false, component::LAYOUT::HORIZONTAL, component::ALIGN::NONE);

    contentFrame
        ->AddFrame("", false, component::LAYOUT::VERTICAL_AUTO, component::ALIGN::NONE)
        ->AddImage("disc_icon_2.png", { 100, 100 });

    auto container = contentFrame->AddFrame("", false, component::LAYOUT::VERTICAL);

    container
        ->AddFrame("", false, component::LAYOUT::HORIZONTAL_AUTO)
        ->AddLabel("to install league++ you will need to select a folder to install to");

    auto startMenuShortcutCheckbox = container
                ->AddFrame("", false, component::LAYOUT::HORIZONTAL_AUTO)
                ->AddCheckbox("start menu shortcut", true);

    auto desktopShortcutCheckbox = container
                ->AddFrame("", false, component::LAYOUT::HORIZONTAL_AUTO)
                ->AddCheckbox("dekstop shortcut", true);

    auto selector = container
                        ->AddFrame("", false, component::LAYOUT::HORIZONTAL_AUTO)
                        ->AddFolderSelector("location", win32::GetFolderPath(CSIDL_PROGRAM_FILES).string());

    auto controlsFrame = frameMain->AddFrame("", false, component::LAYOUT::HORIZONTAL_AUTO, component::ALIGN::NONE);
    controlsFrame->AddButton("install", [&]() {
            auto path = selector->GetPath() + "\\league++";
            if (path.empty()) {
                CreateMessageBox("error", 1, "installer_icon.png", "path cannot be empty", message_box_type::MB_ERROR);
                return;
            }

            // TODO: update ui to display that we are installing

            if (!InitInstallDir(path))
                return;

            if (!SetupRegistry(path))
                return;

            if (!InstallClient(path))
                return;
            
            if (startMenuShortcutCheckbox->GetState() && !CreateStartMenuShortcut(path))
                return;

            if (desktopShortcutCheckbox->GetState() && !CreateDesktopShortcut(path))
                return;
                
            CreateMessageBox("sucess", 1, "installer_icon.png", "league++ has been installed", message_box_type::MB_INFO);
        });

    controlsFrame->AddButton("exit", [&]() { systemWindow->CloseWindow(); });

    SystemPollWindowEvents();
    
    return 0;
}