#include <iostream>
#include <Windows.h>
#include <ShlObj.h>
#include <Shlwapi.h>

HRESULT CreateLink(LPCSTR lpszPathObj, LPCSTR lpszPathLink, LPCSTR lpszDesc) { 
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

#include <filesystem>

int main() {
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