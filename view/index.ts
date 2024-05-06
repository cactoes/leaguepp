namespace colors {
    // same as the css
    const primaryColor = '#98add9';
    const accentColor = '#af7dbf';

    function interpolateColor(color1: string, color2: string, factor: number) {
        if (factor === 0) return color1;
        if (factor === 1) return color2;

        const c1 = parseColor(color1);
        const c2 = parseColor(color2);

        const r = Math.round(lerp(c1.r, c2.r, factor));
        const g = Math.round(lerp(c1.g, c2.g, factor));
        const b = Math.round(lerp(c1.b, c2.b, factor));

        return rgbToHex(r, g, b);
    }

    function parseColor(color: string) {
        const hex = color.substring(1);
        const r = parseInt(hex.substring(0, 2), 16);
        const g = parseInt(hex.substring(2, 4), 16);
        const b = parseInt(hex.substring(4, 6), 16);
        return { r, g, b };
    }

    function lerp(value1: number, value2: number, factor: number) {
        return value1 * (1 - factor) + value2 * factor;
    }

    function rgbToHex(r: number, g: number, b: number) {
        const componentToHex = (c: number) => {
            const hex = c.toString(16);
            return hex.length === 1 ? "0" + hex : hex;
        };
        return `#${componentToHex(r)}${componentToHex(g)}${componentToHex(b)}`;
    }

    export function generateGradient(steps: number) {
        const colors = [];
        for (let i = 0; i < steps; i++) {
            const factor = i / (steps - 1);
            const interpolatedColor = interpolateColor(primaryColor, accentColor, factor);
            colors.push(interpolatedColor);
        }
        return colors;
    }
}; // colors

const noTarget = (target: string) => console.error(`Error: Failed to find target "${target}"`);

namespace uiComponents {
    export function createLabel(label: string) {
        const p = document.createElement("p");
        p.className = "label"
        p.innerText = label;
        return p;
    }

    export function createShadowLabel(label: string) {
        const shadow = document.createElement("span");
        shadow.className = "shadow";
        shadow.innerText = label;
        return shadow;
    }

    export function createFullLabel(label: string) {
        return [ uiComponents.createLabel(label), uiComponents.createShadowLabel(label) ];
    }

    interface tag_props_t {
        className?: string
        innerText?: string
        id?: string
        onclick?: ((this: GlobalEventHandlers, ev: MouseEvent) => any) | null;
        [key: string]: any
    }

    export function createElement<K extends keyof HTMLElementTagNameMap>(tagName: K, props: tag_props_t, childNodes?: HTMLElement[]): HTMLElementTagNameMap[K] {
        const element = document.createElement(tagName);
        for (const prop of Object.keys(props))
            (element as any)[prop] = (props as any)[prop];

        if (childNodes)
            for (const child of childNodes)
                element.appendChild(child);
            
        return element;
    }
}; // namespace uiComponents

const debounce = (callback:any, wait:number) => {
    let timeoutId: any = null;
    return (...args: any) => {
        window.clearTimeout(timeoutId);
        timeoutId = window.setTimeout(() => {
            callback(...args);
        }, wait);
    };
}

function createInfoBox(info: string) {
    return uiComponents.createElement(
        "div", { className: "infoDiv", innerText: "?" },
        [ uiComponents.createElement("p", { innerText: info }) ]
    );
}

function uiSetActiveTab(id: string) {
    const tablist = document.getElementById("tablist")!.children;

    for (let child of tablist)
        child.className = child.id == id+"tabLabel" ? "active" : "";

    const tabcontent = document.getElementById("tabcontent")!.children;
    for (let child of tabcontent)
        child.className = child.id == id ? "tab active" : "tab";
}

function uiCreateTab(label: string, isActive: boolean, id: string) {
    const tablist = document.getElementById("tablist")!;
    tablist.appendChild(uiComponents.createElement(
        "p", {
            id: id + "tabLabel",
            innerText: label,
            className: isActive ? "active" : "",
            onclick: () => {
                uiSetActiveTab(id);
            }
        }
    ));

    const tabcontent = document.getElementById("tabcontent")!;
    tabcontent.appendChild(uiComponents.createElement("div", { id, className: isActive ? "tab active" : "tab" }));
}

function uiCreateFrame(name: string, id: string, layout: string, target: string): void {
    const element = document.getElementById(target);
    if (!element)
        return noTarget(target);

    const frame = uiComponents.createElement("div", { id, className: "frame " + layout });

    if (name.length != 0)
        frame.appendChild(uiComponents.createElement("span", { innerText: name }));

    element.appendChild(frame);
}

function uiCreateButton(label: string, id: string, onclick: string, target: string): void {
    const element = document.getElementById(target);
    if (!element)
        return noTarget(target);

    element.appendChild(uiComponents.createElement(
        "div", { id, className: "button element noselect" },
        [ uiComponents.createElement("button", { innerText: label, onclick: () => { invoke(onclick, []); } }) ]
    ));
}

function uiCreateLabel(text: string, pos: string, id: string, target: string): void {
    const element = document.getElementById(target);
    if (!element)
        return noTarget(target);

    element.appendChild(uiComponents.createElement(
        "div", { id, className: "label element " + pos },
        [ ...uiComponents.createFullLabel(text) ]
    ));
}

function uiUpdateLabel(text: string, color: string, id: string): void {
    const element = document.getElementById(id) as HTMLDivElement | null;
    if (!element)
        return noTarget(id);

    const labels = uiComponents.createFullLabel(text);
    
    if (color.length != 0)
        labels[0].style.color = `#${color}`;

    element.replaceChildren(...labels);
}

function uiCreateCheckBox(label: string, info: string, state: boolean, id: string, onclick: string, target: string): void {
    const element = document.getElementById(target);
    if (!element)
        return noTarget(target);

    const checkbox = uiComponents.createElement("div", { className: "checkbox element", id }, [ uiComponents.createLabel(label) ]);

    if (info != "")
        checkbox.appendChild(createInfoBox(info));

    const input = uiComponents.createElement("input", { type: "checkbox", checked: state, onclick: () => { invoke(onclick, [ input.checked ]) } });

    checkbox.appendChild(uiComponents.createElement(
        "label", {},
        [ input,  uiComponents.createElement("span", {}) ]
    ));

    element.appendChild(checkbox);
}

function uiCreateSlider(label: string, info: string, id: string, onchange: string, min: number, max: number, target: string): void {
    const element = document.getElementById(target);
    if (!element)
        return noTarget(target);

    const slider = uiComponents.createElement(
        "div", {
            id, className: "slider element"
        },
        [ ...uiComponents.createFullLabel(label) ]
    );

    if (info != "")
        slider.appendChild(createInfoBox(info));

    const mid = Math.round((min + max) / 2);
    const span = uiComponents.createElement(
        "span", {
            id: id+"sliderTick",
            className: "sliderTick",
            innerText: `${mid}`,
        }
    );
    span.style.left = `${(mid - min) / (max - min) * 100}%`

    const doSliderEvents = debounce((newValue: number) => { invoke(onchange, [ newValue ]) }, 500);
    const handleSliderUpdate = (newValue: number) => {
        doSliderEvents(newValue);
        
        sldr.value = `${newValue}`;

        span.innerText = `${newValue}`;
        span.style.left = `${(newValue - min) / (max - min) * 100}%`
    }

    const sldr = uiComponents.createElement("input", { type: "range", min: `${min}`, max: `${max}`, oninput: () => { handleSliderUpdate(parseInt(sldr.value)); } });

    slider.appendChild(uiComponents.createElement("div", {},
        [
            uiComponents.createElement(
                "span", {
                    innerText: "-",
                    className: "decrement noselect",
                    onclick: () => { handleSliderUpdate(parseInt(sldr.value) - 1); }
                }
            ),
            uiComponents.createElement(
                "span", {
                    innerText: "+",
                    className: "increment noselect",
                    onclick: () => { handleSliderUpdate(parseInt(sldr.value) + 1); }
                }
            ),
            sldr, span 
        ]
    ));

    element.appendChild(slider);
}

function uiCreateDropDown(label: string, info: string, id: string, isMulti: boolean, activeValues: string[], onchange: string, target: string, items: string[]): void {
    const element = document.getElementById(target);
    if (!element)
        return noTarget(target);

    // <div class="dropdown element" id=`${id}`>
    const dropdown = uiComponents.createElement(
        "div", {
            id, className: "dropdown element"
        },
        [ ...uiComponents.createFullLabel(label) ]
    );

    if (info != "")
        dropdown.appendChild(createInfoBox(info));

    const disabledInput = uiComponents.createElement("input", { id: id + "input", type: "text", disabled: true });
    const dropdownContent = uiComponents.createElement("div", { id: id + "content", className: "content noselect" });

    const updateInput = () => {
        const selectedItems: string[] = [];
        for (const element of dropdownContent.children as HTMLCollectionOf<HTMLParagraphElement>)
            if (element.className == "selected")
                selectedItems.push(element.innerText);

        disabledInput.value = selectedItems.join(", ");
        return selectedItems;
    }

    const gradient = colors.generateGradient(items.length);

    for (let i = 0; i < items.length; i++) {
        const item = items[i];
        const p = uiComponents.createElement(
            "p", {
                innerText: item,
                className: activeValues.includes(item) ? "selected" : "",
                onclick: () => {
                    if (!isMulti)
                        for (const element of dropdownContent.children as HTMLCollectionOf<HTMLParagraphElement>)
                            element.className = "";
        
                    p.className = p.className == "selected" ? "" : "selected";
                    const selectedItems = updateInput();
                    invoke(onchange, [ p.innerText, p.className == "selected", selectedItems ]);
                }
            }
        );
        p.style.color = gradient[i];
        dropdownContent.appendChild(p);
    }

    updateInput();

    dropdown.appendChild(uiComponents.createElement("div", { className: "wrapper" }, [ disabledInput, dropdownContent ]));
 
    element.appendChild(dropdown);
}

function uiCreateSelector(label: string, info: string, state: number, id: string, onchange: string, target: string, items: string[]): void {
    const element = document.getElementById(target);
    if (!element)
        return noTarget(target);

    const selector = uiComponents.createElement(
        "div", {
            id, className: "selector element"
        },
        [ ...uiComponents.createFullLabel(label) ]
    );

    if (info != "")
        selector.appendChild(createInfoBox(info));

    const selectorContent = uiComponents.createElement("div", { id: id + "content", className: "content noselect" });

    const gradient = colors.generateGradient(items.length);

    for (let i = 0; i < items.length; i++) {
        const item = items[i];
        const p = uiComponents.createElement(
            "p", {
                innerText: item,
                className: i == state ? "selected" : "",
                onclick: () => {
                    if (p.className == "selected")
                        return;
        
                    for (const element of document.getElementById(id + "content")!.children as HTMLCollectionOf<HTMLParagraphElement>)
                        element.className = "";
        
                    p.className = "selected";
                    
                    // state that gets returned is ignored
                    invoke(onchange, [ p.innerText ]);
                }
            }
        );
        p.style.backgroundColor = gradient[i];
        selectorContent.appendChild(p);
    }

    selector.appendChild(selectorContent);

    element.appendChild(selector);
}

function uiCreateInput(label: string, info: string, id: string, value: string, onchange: string, target: string): void {
    const element = document.getElementById(target);
    if (!element)
        return noTarget(target);

    const input = uiComponents.createElement(
        "div", {
            id, className: "input element"
        },
        [ ...uiComponents.createFullLabel(label) ]
    );

    if (info != "")
        input.appendChild(createInfoBox(info));

    const container = uiComponents.createElement("div", {});

    const inputField = uiComponents.createElement(
        "input", {
            type: "text",
            value: value,
            spellcheck: false,
            autocapitalize: "false",
            autocomplete: "off",
            onblur: debounce(async () => { inputField.value = await invoke(onchange, [ inputField.value ]); }, 250)
        }
    );

    container.appendChild(inputField);

    input.appendChild(container);

    element.appendChild(input);
}

function uiCreateList(label: string, info: string, id: string, activeValues: string[], validator: string, onchange: string, target: string): void {
    const element = document.getElementById(target);
    if (!element)
        return noTarget(target);

    const list = uiComponents.createElement(
        "div", {
            id, className: "list element"
        },
        [ ...uiComponents.createFullLabel(label) ]
    );

    if (info != "")
        list.appendChild(createInfoBox(info));

    const container = uiComponents.createElement("div", {});

    const listView = uiComponents.createElement("div", { id: id + "listView", className: "listView" });

    const RenderListViewItems = () => {
        listView.innerHTML = "";
        for (const value of activeValues) {
            const item = uiComponents.createElement(
                "p", {
                    innerText: value,
                    onclick: () => {
                        activeValues = activeValues.filter(activeValue => value != activeValue);
                        listView.removeChild(item);
                        invoke(onchange, [ activeValues ]);
                    }
                }
            );
            listView.appendChild(item);
        }
    }

    RenderListViewItems();

    container.appendChild(listView);

    const listInput = uiComponents.createElement("div", { className: "listInput" });
    listInput.onkeydown = async (ev: KeyboardEvent) => {
        if (ev.key == "Enter") {
            const value = inputField.value;
            const isValidItem = await invoke(validator, [ value ]);
            if (isValidItem) {
                const newValue = await invoke<string>(onchange, [ [ ...activeValues, value ] ]);
                activeValues.push(newValue);
                RenderListViewItems();
                inputField.value = "";
            }
        }
    }
    const inputField = uiComponents.createElement(
        "input", {
            id: id + "inputFieldId",
            type: "text",
            spellcheck: false,
            autocapitalize: "false",
            autocomplete: "off"
        }
    );
    listInput.appendChild(inputField);
    listInput.appendChild(uiComponents.createElement(
        "button", {
            id: id + "addListItem",
            innerText: "add",
            onclick: async () => {
                const value = inputField.value;
                const isValidItem = await invoke(validator, [ value ]);
                if (isValidItem) {
                    const newValue = await invoke<string>(onchange, [ [ ...activeValues, value ] ]);
                    activeValues.push(newValue);
                    RenderListViewItems();
                    inputField.value = "";
                }
            }
        }
    ));

    container.appendChild(listInput);
    list.appendChild(container);
    element.appendChild(list);
}

function uiUpdateList(id: string, activeValues: string[], validator: string, onchange: string): void {
    const element = document.getElementById(id) as HTMLDivElement | null;
    if (!element)
        return noTarget(id);

    const listView = document.getElementById(id + "listView") as HTMLDivElement;

    const RenderListViewItems = () => {
        listView.innerHTML = "";
        for (const value of activeValues) {
            const item = uiComponents.createElement(
                "p", {
                    innerText: value,
                    onclick: () => {
                        activeValues = activeValues.filter(activeValue => value != activeValue);
                        listView.removeChild(item);
                        invoke(onchange, [ activeValues ]);
                    }
                }
            );
            listView.appendChild(item);
        }
    }

    RenderListViewItems();

    const add = document.getElementById(id + "addListItem") as HTMLButtonElement;
    const inputField = document.getElementById(id + "inputFieldId") as HTMLInputElement;
    add.onclick = async () => {
        const value = inputField.value;
        const isValidItem = await invoke(validator, [ value ]);
        if (isValidItem) {
            const newValue = await invoke<string>(onchange, [ [ ...activeValues, value ] ]);
            activeValues.push(newValue);
            RenderListViewItems();
            inputField.value = "";
        }
    }
}

function uiCreateNotificationBox(title: string, description: string, typeId: number, parent: HTMLDivElement): HTMLDivElement {
    let isRemoving = false;

    const clearThisElement = () => {
        if (!isRemoving) {
            isRemoving = true;

            notification.classList.add("fade");
            setTimeout(() => parent.removeChild(notification), 300);
        }
    }

    let timeout = setTimeout(clearThisElement, 4700);

    const notification = uiComponents.createElement(
        "div", {
            className: "notification noselect",
            onclick: () => {
                clearTimeout(timeout);
                clearThisElement();
            }
        },
        [
            uiComponents.createElement(
                "div", {
                    className: "content"
                },
                [
                    uiComponents.createElement("p", { className: "contentTitle", innerText: title }),
                    uiComponents.createElement("p", { className: "contentDescription", innerText: description })
                ]
            ),
            uiComponents.createElement("div", { className: "statusbar" }, [ uiComponents.createElement("div", {}) ])
        ]
    );

    return notification;
}

function createNotification(title: string, description: string, typeId: number): void {
    const target = document.getElementById("notification_wrapper") as HTMLDivElement;
    const box = uiCreateNotificationBox(title, description, typeId, target);
    target.appendChild(box);
}

async function main() {
    register(uiCreateTab);
    register(uiSetActiveTab);
    register(uiCreateFrame);
    register(uiCreateButton);
    register(uiCreateLabel);
    register(uiUpdateLabel);
    register(uiCreateCheckBox);
    register(uiCreateSlider);
    register(uiCreateDropDown);
    register(uiCreateSelector);
    register(uiCreateInput);
    register(uiCreateList);
    register(uiUpdateList);

    register(createNotification);

    for (const button of (document.getElementsByClassName("button") as HTMLCollectionOf<HTMLDivElement>))
        button.addEventListener("click", () => invoke("HandleWindowEvent", [ button.id ]))

    await invoke("GetMainLayout", []);
}

main();