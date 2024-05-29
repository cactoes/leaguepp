
// const noTarget = (target: string) => console.error(`Error: Failed to find target "${target}"`);

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
        (element as any)[prop] = (props as any)[prop];

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
            [ createElement("img", { src: name }) ]);

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

function createUIComponent(type: string, ...args: any) {
    console.log(`[creating (${type})]\n      name: "${args[0]}"\n        id: "${args[1]}"\n    target: "${args[2]}"`)

    createFunctions[type]?.apply(null, args);
}

function updateUIComponent(type: string, ...args: any) {
    updateFunction[type]?.apply(null, args);
}

async function main() {
    register(createUIComponent);
    register(updateUIComponent);
    
    for (const button of (document.getElementsByClassName("buttonWindowEvent") as HTMLCollectionOf<HTMLDivElement>)) {
        button.addEventListener("click", () => setTimeout(() => invoke("HandleWindowEvent", [ button.id ]), 100));
    }

    await invoke("RenderLayout", [])
}

main();