
function getTargetElement(id: string, nolog: boolean = false): HTMLDivElement {
    const element = document.getElementById(id);
    if (!element) {
        if (!nolog)
            console.error(`Error: Failed to find target "${id}"`);
        throw new Error(`Error: Failed to find target "${id}"`)
    }

    return element as HTMLDivElement;
}

interface tag_props_t {
    className?: string
    innerText?: string
    id?: string
    [key: string]: any
}

function createElement<K extends keyof HTMLElementTagNameMap>(tagName: K, props: tag_props_t, childNodes?: HTMLElement[]): HTMLElementTagNameMap[K] {
    const element = document.createElement(tagName);
    for (const prop of Object.keys(props))
        element[prop as keyof HTMLElementTagNameMap[K]] = props[prop];

    if (childNodes)
        for (const child of childNodes)
            element.appendChild(child);
        
    return element;
}

const createFunctions: ICreateFunctions = {
    frame: (name: string, id: string, target: string, hasBorder: boolean, layout: string, align: string) => {
        const element = getTargetElement(target);

        const frame = createElement("div", { id, className: "frame" })
        if (layout)
            frame.classList.add(layout);
        
        if (align)
            frame.classList.add(align);

        if (hasBorder || name.length > 0)
            frame.classList.add("border");

        if (name.length > 0)
            frame.appendChild(createElement("span", { innerText: name }))

        element.appendChild(frame);
    },
    label: (name: string, id: string, target: string) => {
        const element = getTargetElement(target);

        const label = createElement("div", { id, className: "label element" },
            [ createElement("p", { innerText: name }) ]);

        element.appendChild(label);
    },
    button: (name: string, id: string, target: string, callback: string) => {
        const element = getTargetElement(target);

        const button = createElement("div", { id, className: "button element" },
            [ createElement("button", { innerText: name, onclick: () => invoke(callback, []) }) ]);

        element.appendChild(button);
    },
    image: (name: string, id: string, target: string, width: number, height: number) => {
        const element = getTargetElement(target);

        const button = createElement("div", { id, className: "image element" },
            [ createElement("img", { src: `media/${name}` }) ]);

        button.style.width = `${width}px`;
        button.style.height = `${height}px`;

        element.appendChild(button);
    }
};

const updateFunction: IUpdateFunctions = {
    label: (name: string, id: string) => {
        const element = getTargetElement(id);
        element.replaceChildren(createElement("p", { innerText: name }));
    }
};

interface format_args {
    name: string,
    id: string,
    target: string,
    [key: string]: string
}

function parseArgs(args: any[]): format_args {
    const remainder = args.slice(3).map((item, index) => { return { [`a${index + 3}`]: item } });
    return Object.assign({
        name: args[0],
        id: args[1],
        target: args[2],
    }, ...remainder);
}

function formatArgs(base: number, args: format_args) {
    return Object.keys(args).map(item => {
        return `\n${Array.from({ length: base - item.length }).join(" ")}${item}: "${args[item]}"`;
    });
}

function createUIComponent(type: string, ...args: any) {
    console.log(`[creating (${type})]${formatArgs(10, parseArgs(args))}`);
    createFunctions[type]?.apply(null, args);
}

function updateUIComponent(type: string, ...args: any) {
    console.log(`[updating (${type})]${formatArgs(10, parseArgs(args))}`);
    updateFunction[type]?.apply(null, args);
}

interface bounds_check_correction {
    right: number,
    bottom: number
};

class ContextMenu {
    public isEnabled: boolean = true;
    public isOpened: boolean = false;

    constructor(private element: HTMLDivElement) {}

    public setMenuPos(x: number, y: number) {
        const correction = this.doBoundsCheck(x, y);

        this.element.style.left = `${x - correction.right}px`;
        this.element.style.top = `${y - correction.bottom}px`;
    }

    public setMenuState(state: "open" | "close") {
        if (state == "open") {
            this.element.className = "";
            this.isOpened = true;
        } else {
            this.element.className = "hidden";
            this.isOpened = false;
        }
    }

    public getClientRect(): DOMRect {
        return this.element.getBoundingClientRect();
    }

    public isInMenu(x: number, y: number): boolean {
        const rect = this.getClientRect();
    
        return x >= rect.left && x <= rect.right &&
               y >= rect.top && y <= rect.bottom;
    }

    public doBoundsCheck(x: number, y: number): bounds_check_correction {
        const rect = this.getClientRect();
        const bodyRect = document.body.getBoundingClientRect();

        return {
            right: Math.max(0, x + rect.width - bodyRect.right),
            bottom: Math.max(0, y + rect.height - bodyRect.bottom)
        };
    }

    public addGroup(name: string, args: string[]): void {
        const group = createElement("div", { className: "group" });

        for (const item of args)
            group.appendChild(createElement("p", { innerText: item, onclick: () => invoke("OnContextMenuItemClicked", [ name, item ]) }));

        this.element.children[0].appendChild(group);
    }
};

const LMB: number = 0;

async function main() {
    const contextMenu = new ContextMenu(document.getElementById("contextMenu") as HTMLDivElement);

    function setContextMenuEnabled(state: boolean) {
        contextMenu.isEnabled = state;
    }

    function createContextMenuGroup(name: string, ...args: any) {
        contextMenu.addGroup(name, args);
    }

    register(createUIComponent);
    register(updateUIComponent);
    register(setContextMenuEnabled);
    register(createContextMenuGroup);
    
    for (const button of (document.getElementsByClassName("buttonWindowEvent") as HTMLCollectionOf<HTMLDivElement>))
        button.addEventListener("click", () => setTimeout(() => invoke("HandleWindowEvent", [ button.id ]), 100));

    // document.addEventListener("mousemove", (event: MouseEvent) => {

    // });

    document.addEventListener("click", (event: MouseEvent) => {
        if (event.button != LMB || !contextMenu.isOpened)
            return;

        if (!contextMenu.isInMenu(event.clientX, event.clientY)) {
            contextMenu.setMenuState("close");
            return;
        }

        if (event.target && (event.target as HTMLElement).className != "group") {
            const target = event.target as HTMLParagraphElement;
            contextMenu.setMenuState("close");

            switch (target.id) {
                case "copy":
                    const selectedText = window.getSelection()?.toString();
                    if (selectedText)
                        navigator.clipboard.writeText(selectedText);
                    break;
                default:
                    break;
            }
        }
    });

    document.addEventListener("contextmenu", (event: MouseEvent) => {
        event.preventDefault();

        if (!contextMenu.isEnabled)
            return;

        if (!contextMenu.isOpened)
            contextMenu.setMenuState("open");

        contextMenu.setMenuPos(event.clientX + document.documentElement.scrollLeft, event.clientY + document.documentElement.scrollTop);
    });

    await invoke("RenderLayout", []);
    await invoke("SetContextMenuData", []);
}

main();