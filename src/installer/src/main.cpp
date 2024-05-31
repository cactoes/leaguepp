#include <iostream>
#include <Windows.h>
#include <ShlObj.h>
#include <Shlwapi.h>

#include <uiframework.hpp>

// HRESULT CreateLink(LPCSTR lpszPathObj, LPCSTR lpszPathLink, LPCSTR lpszDesc) { 
//     HRESULT hres; 
//     IShellLink* psl; 

//     hres = CoInitialize(nullptr);
//     if (!SUCCEEDED(hres))
//         return hres;
 
//     // Get a pointer to the IShellLink interface. It is assumed that CoInitialize
//     // has already been called.
//     hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&psl); 
//     if (SUCCEEDED(hres)) { 
//         IPersistFile* ppf; 
 
//         // Set the path to the shortcut target and add the description. 
//         psl->SetPath(lpszPathObj); 
//         psl->SetDescription(lpszDesc); 
 
//         // Query IShellLink for the IPersistFile interface, used for saving the 
//         // shortcut in persistent storage. 
//         hres = psl->QueryInterface(IID_IPersistFile, (LPVOID*)&ppf); 
 
//         if (SUCCEEDED(hres)) { 
//             WCHAR wsz[MAX_PATH]; 
 
//             // Ensure that the string is Unicode. 
//             MultiByteToWideChar(CP_ACP, 0, lpszPathLink, -1, wsz, MAX_PATH); 
            
//             // Add code here to check return value from MultiByteWideChar 
//             // for success.
 
//             // Save the link by calling IPersistFile::Save. 
//             hres = ppf->Save(wsz, TRUE); 
//             ppf->Release(); 
//         } 
//         psl->Release(); 
//     } 

//     CoUninitialize();

//     return hres;
// }

#include <shellapi.h>
#include <filesystem>
#include <fstream>
#include <iostream>

// bool ExtractResource(LPCSTR resourceName, LPCSTR outputPath) {
//     // Find the resource
//     HRSRC hResource = FindResourceA(NULL, resourceName, "ZIPFILE");
//     if (!hResource) return false;

//     // Load the resource
//     HGLOBAL hLoadedResource = LoadResource(NULL, hResource);
//     if (!hLoadedResource) return false;

//     // Lock the resource to get a pointer to the data
//     LPVOID pLockedResource = LockResource(hLoadedResource);
//     if (!pLockedResource) return false;

//     // Get the size of the resource
//     DWORD resourceSize = SizeofResource(NULL, hResource);
//     if (!resourceSize) return false;

//     std::cout << resourceSize << "\n";

//     // Write the resource to the output file
//     std::ofstream outFile(outputPath, std::ios::binary);
//     outFile.write((const char*)pLockedResource, resourceSize);
//     outFile.close();

//     return true;
// }

// std::filesystem::path GetTempFolderPath() {
//     char tempPath[MAX_PATH];
//     DWORD result = GetTempPathA(MAX_PATH, tempPath);

//     return std::filesystem::path(tempPath) / "league++.cache";
// }

#include <windows.h>
#include <string>
#include <shlobj.h>
#include <iostream>
#include <sstream>

// https://stackoverflow.com/questions/12034943/win32-select-directory-dialog-from-c-c

static int CALLBACK BrowseCallbackProc(HWND hwnd,UINT uMsg, LPARAM lParam, LPARAM lpData) {

    if(uMsg == BFFM_INITIALIZED) {
        // std::string tmp = (const char *) lpData;
        // std::cout << "path: " << tmp << std::endl;
        SendMessageA(hwnd, BFFM_SETSELECTION, TRUE, lpData);
    }

    return 0;
}

std::string BrowseFolder(std::string saved_path) {
    char  path[MAX_PATH];
    const char* path_param = saved_path.c_str();

    BROWSEINFOA bi = { 0 };
    bi.lpszTitle = "Select install location";
    bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
    bi.lpfn = BrowseCallbackProc;
    bi.lParam = (LPARAM)path_param;

    LPITEMIDLIST pidl = SHBrowseForFolderA(&bi);

    if (pidl != 0) {
        // get the name of the folder and put it in path
        SHGetPathFromIDList ( pidl, path );

        // free memory used
        IMalloc* imalloc = 0;
        if (SUCCEEDED(SHGetMalloc(&imalloc))) {
            imalloc->Free(pidl);
            imalloc->Release();
        }

        return path;
    }

    return "";
}

int main(int, char** argv) {
    std::string path = BrowseFolder(argv[0]);
    std::cout << path << std::endl;
    // std::filesystem::path tempFolder = GetTempFolderPath();

    // CreateDirectoryA(tempFolder.string().c_str(), nullptr);

    // std::string zipFile = (tempFolder / "league++.zip").string();

    // bool r = ExtractResource("LEAGUEPP_ZIP", zipFile.c_str());

    // // Path to the zip file
    // const char* zipFilePath = zipFile.c_str();
    // // Path to the directory where you want to extract the files
    // const char* extractPath = "PATH_TO_OUT";

    // // Construct the full PowerShell command
    // char fullCommand[MAX_PATH];
    // sprintf(fullCommand, "cmd.exe /c powershell.exe -nologo -noprofile -command Expand-Archive -Path \"%s\" -DestinationPath \"%s\"", zipFilePath, extractPath);

    // // SHELLEXECUTEINFOA sei{};
    // // sei.lpVerb = "open";
    // // sei.lpFile = "cmd.exe";
    // // sei.lpParameters = fullCommand;
    // // sei.nShow = SW_SHOW;

    // // if (!ShellExecuteExA(&sei)) {
    // //     std::cout << "failed\n";
    // //     std::cout << GetLastError() << "\n";
    // //     ERROR_ACCESS_DENIED;
    // //     return 1;
    // // }

    // PROCESS_INFORMATION pi;
    // ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

    // // Startup information
    // STARTUPINFO si;
    // ZeroMemory(&si, sizeof(STARTUPINFO));
    // si.cb = sizeof(STARTUPINFO);
    // si.dwFlags = STARTF_USESHOWWINDOW;
    // si.wShowWindow = SW_HIDE; // Hide the window

    // // Create the process
    // if (!CreateProcess(NULL, (LPSTR)fullCommand, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
    //     std::cerr << "Failed to create process. Error code: " << GetLastError() << std::endl;
    //     return 1;
    // }

    // WaitForSingleObject(pi.hProcess, INFINITE);
    // CloseHandle(pi.hProcess);
    // CloseHandle(pi.hThread);

    // std::cout << tempFolder << "\n";

    // WINDOW_CONFIG config{};
    // config.flags = { WINDOW_FLAG_SHOW, WINDOW_FLAG_HIDE_TITLE_BAR };
    // config.size = { 500, 400 };
    // config.name = "league++ uninstaller";
    // config.iconId = 1;
    // config.iconName = "installer_icon.png";

    // BROWSER_CONFIG bwConfig{};
    // bwConfig.flags = { BROWSER_FLAG_DEV_TOOLS };

    // auto systemWindow = CreateSystemWindow(config, bwConfig, component::LAYOUT::VERTICAL);
    // systemWindow->EnableContextMenu(false);
    
    // auto frameMain = systemWindow->GetWindowFrame()->AddFrame("", false, component::LAYOUT::VERTICAL, component::ALIGN::CENTER);

    // frameMain->AddLabel("are you sure you want to uninstall league++?");
    // auto container = frameMain->AddFrame("", false, component::LAYOUT::HORIZONTAL_AUTO, component::ALIGN::CENTER);
    // container->AddButton("yes", [](){});
    // container->AddButton("no", [](){});

    // SystemPollWindowEvents();

    // char path[MAX_PATH];
    // SHGetFolderPathA(nullptr, CSIDL_COMMON_PROGRAMS, nullptr, 0, path);
    // PathAppendA(path, "\\league++.lnk");

    // char lpppath[MAX_PATH] = {};
    // GetFullPathNameA("league++.exe", MAX_PATH, lpppath, nullptr);

    // auto hr = CreateLink(lpppath, path, "testlink");
    // std::cout << std::hex << hr << "\n";

    // PathAppendA(lpppath, "..");

    // for (const auto& entry : std::filesystem::directory_iterator(lpppath)) {
    //     if (!entry.is_directory()) {
    //         DeleteFileA(entry.path().string().c_str());
    //     }
    // }

    // RemoveDirectoryA(lpppath);

    // std::cout << path << "\n";
    // std::cout << lpppath << "\n";

    // DeleteFileA(path);
}