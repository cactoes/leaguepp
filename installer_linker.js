const fs = require('fs');
const fse = require('fs-extra');
const path = require('path');
const AdmZip = require('adm-zip');
const zip = new AdmZip();
const { execSync } = require('child_process');

const adminUAC = `
    <?xml version="1.0" encoding="utf-8"?>
    <assembly xmlns="urn:schemas-microsoft-com:asm.v1" manifestVersion="1.0">
        <trustInfo xmlns="urn:schemas-microsoft-com:asm.v3">
            <security>
                <requestedPrivileges>
                    <requestedExecutionLevel level="requireAdministrator" uiAccess="false" />
                </requestedPrivileges>
            </security>
        </trustInfo>
    </assembly>`

function main(argc, argv) {
    let configType;

    const index = argv.indexOf("-c");
    if (index != -1)
        if (index + 1 < argc)
            configType = argv[index + 1];

    const allConfigs = JSON.parse(fs.readFileSync(path.join(__dirname, "ilconfig.json")).toString());

    let activeConfig = undefined;

    for (const config of allConfigs)
        if (config.type == configType)
            activeConfig = config;

    if (activeConfig == undefined) {
        console.error("[IP]: Invalid config");
        return -1;
    }

    const installerLinkerTempDir = path.join(__dirname, "build/installer_linker");

    if (activeConfig.clean) {
        fse.rmSync(installerLinkerTempDir, { recursive: true, force: true });
    }
    
    if (!fs.existsSync(installerLinkerTempDir))
        fs.mkdirSync(installerLinkerTempDir);

    fs.writeFileSync(path.join(installerLinkerTempDir, "uac.manifest"), adminUAC);

    for (const file of activeConfig.files) {
        const filePath = path.join(__dirname, "build", activeConfig.type, file);

        const info = fs.statSync(filePath);
        if (info.isFile()) {
            zip.addLocalFile(filePath);
        } else if (info.isDirectory()) {
            zip.addLocalFolder(filePath, "view");
        }
    }

    zip.writeZip(path.join(installerLinkerTempDir, 'league++.zip'));

    fs.writeFileSync(path.join(installerLinkerTempDir, "resource.rc"), `LEAGUEPP_ZIP ZIPFILE "league++.zip"`)

    const RC_CMD = "rc /fo resource.res resource.rc";
    // const LINK_CMD = `link "..\\src\\installer\\installer.dir\\Debug\\main.obj" resource.res ..\\Debug\\uiframework.lib "..\\src\\installer\\installer.dir\\Debug\\resources.installer.res" /out:"..\\Debug\\installer.exe"`;
    // const MT_CMD = `mt -manifest uac.manifest -outputresource:"..\\Debug\\installer.exe"`;

    try {
        execSync(`cd ${installerLinkerTempDir.toString()} && "C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\\Auxiliary\\Build\\vcvars64.bat" && ${RC_CMD}`); //  && ${LINK_CMD} && ${MT_CMD}
    } catch (error) {
        console.log(error.message);
        return error.status;
    }

    return 0;
}

console.log("exited with code:", main(process.argv.length, process.argv));