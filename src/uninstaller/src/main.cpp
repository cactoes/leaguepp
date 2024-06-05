#include <filesystem>
#include <ShlObj.h>
#include <uiframework.hpp>
#include <shared/win32.hpp>
#include <shared/general.hpp>
#include <service.hpp>

static const char* g_registryPath = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\" APP_GUID;

bool RemoveLeagueClient(const std::string& installLocation) {
    // just to be sure
    if (installLocation.empty() || !installLocation.ends_with("league++"))
        return false;

    // remove all files
    try {
        for (const auto& entry : std::filesystem::directory_iterator(installLocation)) {
            if (entry.path().filename() != "uninstaller.exe")
                std::filesystem::remove_all(entry.path());
        }
    } catch (std::filesystem::filesystem_error error) {/* ignore? */}

    // remove start menu shortcut
    char path[MAX_PATH];
    SHGetFolderPathA(nullptr, CSIDL_COMMON_PROGRAMS, nullptr, 0, path);
    auto startMenuShortCut = (std::filesystem::path(path) / "league++.lnk").string();
    DeleteFileA(startMenuShortCut.c_str());

    // remove uninstaller (on reboot)
    char tempPath[MAX_PATH];
    GetTempPathA(MAX_PATH, tempPath);
    MoveFileExA((installLocation + "\\unsintaller.exe").c_str(), tempPath, MOVEFILE_DELAY_UNTIL_REBOOT);

    // remove registery key
    RegDeleteKeyExA(HKEY_LOCAL_MACHINE, g_registryPath, KEY_WOW64_64KEY, 0);

    // remove service
    if (win32::sc_handle svcManager = OpenSCManagerA(nullptr, SERVICES_ACTIVE_DATABASEA, SC_MANAGER_ALL_ACCESS); svcManager.IsValid()) {
        if (win32::sc_handle svc = svcManager.Call(&OpenServiceA, SERVICE_NAME, SERVICE_ALL_ACCESS); svc.IsValid()) {
            svc.Call(&DeleteService);
            svc.Call(&CloseServiceHandle);
        }
    }
    return true;
}

int WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    auto entry = win32::RegistryEntry(g_registryPath);

    if (!entry.Init()) {
        MessageBoxA(nullptr, "league++ has not been installed on this computer", "error", MB_OK | MB_ICONINFORMATION);
        return 0;
    }

    WINDOW_CONFIG config{};
    config.flags = { WINDOW_FLAG_SHOW, WINDOW_FLAG_HIDE_TITLE_BAR };
    config.size = { 350, 170 };
    config.name = "league++ uninstaller";
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
        ->AddFrame("", false, component::LAYOUT::VERTICAL_AUTO, component::ALIGN::VERTICAL)
        ->AddImage("disc_icon_2.png", { 100, 100 });

    auto container = contentFrame->AddFrame("", false, component::LAYOUT::VERTICAL, component::ALIGN::VERTICAL);

    container
        ->AddFrame("", false, component::LAYOUT::HORIZONTAL_AUTO)
        ->AddLabel("are you sure you want to uninstall league++?");

    auto frame = container->AddFrame("", false, component::LAYOUT::HORIZONTAL_AUTO);

    frame->AddButton("confirm", [&]() {
        std::string installLocation {};
        if (!entry.GetRegValue("InstallLocation", installLocation)) {
            CreateMessageBox("error", 1, "installer_icon.png", "failed to find league++ installation", message_box_type::MB_ERROR);
            return;
        }

        if (!RemoveLeagueClient(installLocation)) {
            MessageBoxA(nullptr, "failed to remove league++", "error", MB_OK | MB_ICONERROR);
            return;
        }

        MessageBoxA(nullptr, "league++ has been removed from this computer", "success", MB_OK | MB_ICONINFORMATION);
    });

    frame->AddButton("cancel", [&]() {
        systemWindow->CloseWindow();
    });


    SystemPollWindowEvents();

    return 0;
}