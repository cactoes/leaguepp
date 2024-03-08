// Import Terser so we can use it
const { minify } = require('terser');

// Import fs so we can read/write files
const fs = require('fs');
const fse = require('fs-extra');
const path = require('path');

const viewRoot = path.join(__dirname, "view");

function GetSpacesString(count) {
    let out = "";

    for (let i = 0; i < count; i++)
        out += " ";

    return out;
}

function MinifyCSS(cssString) {
    // Remove comments
    cssString = cssString.replace(/\/\*[\s\S]*?\*\//g, '');

    // Remove whitespace before and after characters
    cssString = cssString.replace(/\s*([{}:;,])\s*/g, '$1');

    // Remove whitespace between selectors
    cssString = cssString.replace(/\s*([^\{\};]+)\s*\{\s*([^\{\}]+)\s*\}/g, '$1{$2}');

    // Remove whitespace between property-value pairs
    cssString = cssString.replace(/\s*([^:;]+)\s*:\s*([^;]+)\s*;/g, '$1:$2;');

    return cssString;
}

function CleanTarget(target) {
    fse.rmSync(path.join(__dirname, target, "view"), { recursive: true, force: true });
}

function CreateFolders(target) {
    if (!fs.existsSync(path.join(__dirname, target)))
        fs.mkdirSync(path.join(__dirname, target));

    if (!fs.existsSync(path.join(__dirname, target, "view")))
        fs.mkdirSync(path.join(__dirname, target, "view"));

    if (!fs.existsSync(path.join(__dirname, target, "view", "media")))
        fs.mkdirSync(path.join(__dirname, target, "view", "media"));
}

function CopyBasic(target, copy) {
    for (const type of copy) {
        if (type == ".js") {
            fse.copySync(path.join(__dirname, "build/ts"), path.join(__dirname, target, "view/js"), { overwrite: true });
            continue;  
        }

        fs.readdirSync(viewRoot).forEach(item => {
            if (type.startsWith(".") && item.endsWith(type)) {
                fs.copyFileSync(path.join(viewRoot, item), path.join(__dirname, target, "view", item));
                return;
            }

            if (item == type) {
                fse.copySync(path.join(__dirname, "view", type), path.join(__dirname, target, "view", type), { overwrite: true });
                return;
            }
        });
    }
}

function CompileJS(target) {
    // Define the config for how Terser should minify the code
    // This is set to how you currently have this web tool configured
    const config = {
        compress: {
            dead_code: true,
            drop_console: false,
            drop_debugger: true,
            keep_classnames: false,
            keep_fargs: true,
            keep_fnames: false,
            keep_infinity: false
        },
        mangle: {
            eval: false,
            keep_classnames: false,
            keep_fnames: false,
            toplevel: false,
            safari10: false
        },
            module: false,
            sourceMap: false,
        output: {
            comments: false
        }
    };
    
    const compiledJSRoot = path.join(__dirname, "build/ts");
    
    fs.readdirSync(compiledJSRoot).forEach(async item => {
        if (!item.endsWith(".js") || item.endsWith(".min.js"))
            return;
    
        // Load in your code to minify
        const code = fs.readFileSync(path.join(compiledJSRoot, item), 'utf8');
        
        // Minify the code with Terser
        const minified = await minify(code, config);
        
        // Save the code!
        fs.writeFileSync(path.join(compiledJSRoot, item.replace(".js", ".min.js")), minified.code);
    });
        
    let indexHTML = fs.readFileSync(path.join(__dirname, target, "view/index.html")).toString();
    
    let lines = indexHTML.split("\n");

    let deferredScripts = [];
    
    for (const line in lines) {
        if (!lines[line].includes("></script>"))
            continue;
    
        const match = lines[line].match(/"(.+)"/g);
        const hasDefer = lines[line].includes("defer");
        const spaceCount = lines[line].indexOf("<");
    
        // remove "js/ & .js" from the match to get just the file name
        const fileName = match[0].slice(4, -4);
    
        const targetFile = path.join(compiledJSRoot, `${fileName}.min.js`);
    
        if (fs.existsSync(targetFile)) {
            if (hasDefer) {
                deferredScripts.push(`${GetSpacesString(spaceCount)}<script>`+ fs.readFileSync(targetFile).toString() + "</script>");
                lines.splice(line, 1);
            } else {
                lines[line] = `${GetSpacesString(spaceCount)}<script>`+ fs.readFileSync(targetFile).toString() + "</script>";
            }
        } else {
            console.error(`File not found: ${fileName}`)
        }
    }

    for (const script of deferredScripts) {
        const closigBodyIndex = lines.indexOf("</body>");
        lines.splice(closigBodyIndex - 1, 0, script);
    }
    
    fs.writeFileSync(path.join(__dirname, target, "view/index.html"), lines.join("\n"));
}

function CompileCSS(target) {
    let indexHTML = fs.readFileSync(path.join(__dirname, target, "view/index.html")).toString();
    
    let lines = indexHTML.split("\n");
    
    for (const line in lines) {
        if (!lines[line].includes("<link rel=\"stylesheet\""))
            continue;
    
        const match = lines[line].match(/href="(.+)"/g);
        const spaceCount = lines[line].indexOf("<");
    
        // remove href=" & " from the match to get just the file name
        const fileName = match[0].slice(6, -1);

        const targetFile = path.join(viewRoot, fileName);
    
        if (fs.existsSync(targetFile)) {
            lines[line] = `${GetSpacesString(spaceCount)}<style>` + MinifyCSS(fs.readFileSync(targetFile).toString()) + "</style>";
        } else {
            console.error(`File not found: ${fileName}`)
        }
    }
    
    fs.writeFileSync(path.join(__dirname, target, "view/index.html"), lines.join("\n"));
}

function main(argc, argv) {
    let configType;

    const index = argv.indexOf("-c");
    if (index != -1)
        if (index + 1 < argc)
            configType = argv[index + 1];

    const allConfigs = JSON.parse(fs.readFileSync(path.join(__dirname, "vcconfig.json")).toString());

    let activeConfig = undefined;

    for (const config of allConfigs)
        if (config.type == configType)
            activeConfig = config;

    if (activeConfig == undefined) {
        console.error("[VC]: Invalid config");
        return 1;
    }

    if (activeConfig.clean) {
        console.log("[VC]: Cleaning dirs")   
        CleanTarget(activeConfig.target);
    }

    console.log("[VC]: Creating folders")
    CreateFolders(activeConfig.target);

    console.log("[VC]: Copying files")
    CopyBasic(activeConfig.target, activeConfig.copy);

    if (activeConfig.compile.js) {
        console.log("[VC]: Running js compiler");
        CompileJS(activeConfig.target);
    }

    if (activeConfig.compile.css) {
        console.log("[VC]: Running css compiler");
        CompileCSS(activeConfig.target);
    }

    return 0;
}

console.log("exited with code:", main(process.argv.length, process.argv));