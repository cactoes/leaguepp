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
    const infoDiv = document.createElement("div");
    infoDiv.className = "infoDiv"
    infoDiv.innerText = "?"

    const infoBox = document.createElement("p");
    infoBox.innerText = info;
    infoDiv.appendChild(infoBox);

    return infoDiv;
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
    const tablistlabel = document.createElement("p");
    tablistlabel.id = id+"tabLabel";
    tablistlabel.innerText = label;
    if (isActive)
        tablistlabel.className = "active";
    tablistlabel.onclick = () => {
        uiSetActiveTab(id);
    }
    tablist.appendChild(tablistlabel)

    const tabcontent = document.getElementById("tabcontent")!;
    const tab = document.createElement("div");
    tab.id = id;
    tab.className = "tab";
    if (isActive)
        tab.className += " active";
    tabcontent.appendChild(tab);
}

function uiCreateFrame(name: string, id: string, layout: string, target: string): void {
    const element = document.getElementById(target);
    if (!element)
        return noTarget(target);

    const frame = document.createElement("div");
    frame.id = id;
    frame.className = "frame " + layout;

    if (name.length != 0) {
        const span = document.createElement("span");
        span.innerText = name;
        frame.appendChild(span);
    }

    element.appendChild(frame);
}

function uiCreateButton(label: string, id: string, onclick: string, target: string): void {
    const element = document.getElementById(target);
    if (!element)
        return noTarget(target);

    const button = document.createElement("div");
    button.className = "button element noselect";
    button.id = id;

    const btn = document.createElement("button");
    btn.innerText = label;
    btn.onclick = () => { invoke(onclick, []); }
    button.appendChild(btn);

    element.appendChild(button);
}

function uiCreateLabel(text: string, pos: string, id: string, target: string): void {
    const element = document.getElementById(target);
    if (!element)
        return noTarget(target);

    const label = document.createElement("div");
    label.className = "label element " + pos;
    label.id = id;

    const p = document.createElement("p");
    p.className = "label";
    p.innerText = text;
    label.appendChild(p);
    
    const shadow = document.createElement("span");
    shadow.className = "shadow";
    shadow.innerText = text;
    label.appendChild(shadow);

    element.appendChild(label);
}

function uiUpdateLabel(text: string, id: string): void {
    const element = document.getElementById(id) as HTMLDivElement | null;
    if (!element)
        return noTarget(id);

    const p = document.createElement("p");
    p.className = "label";
    p.innerText = text;

    const shadow = document.createElement("span");
    shadow.className = "shadow";
    shadow.innerText = text;
    
    element.replaceChildren(p, shadow);
}

function uiCreateCheckBox(label: string, info: string, state: boolean, id: string, onclick: string, target: string): void {
    const element = document.getElementById(target);
    if (!element)
        return noTarget(target);

    const checkbox = document.createElement("div");
    checkbox.className = "checkbox element";
    checkbox.id = id;

    const p = document.createElement("p");
    p.className = "label"
    p.innerText = label;
    checkbox.appendChild(p);

    if (info != "")
        checkbox.appendChild(createInfoBox(info));

    const lbl = document.createElement("label");
    const input = document.createElement("input");
    input.type = "checkbox";
    input.checked = state;
    input.onclick = () => {
        invoke(onclick, [ input.checked ])
    }
    lbl.appendChild(input);
    const span = document.createElement("span");
    lbl.appendChild(span);
    checkbox.appendChild(lbl);

    element.appendChild(checkbox);
}

function uiCreateSlider(label: string, info: string, id: string, onchange: string, min: number, max: number, target: string): void {
    const element = document.getElementById(target);
    if (!element)
        return noTarget(target);

    const slider = document.createElement("div");
    slider.className = "slider element";
    slider.id = id;

    //     <p class="label">${label}</p>
    const p = document.createElement("p");
    p.className = "label";
    p.innerText = label;
    slider.appendChild(p);

    //     <span class="shadow">${label}</span>
    const shadow = document.createElement("span");
    shadow.className = "shadow";
    shadow.innerText = label;
    slider.appendChild(shadow);

    if (info != "")
        slider.appendChild(createInfoBox(info));

    const div = document.createElement("div");

    const sldr = document.createElement("input");
    sldr.type = "range"
    sldr.min = `${min}`;
    sldr.max = `${max}`;

    const doSliderEvents = debounce((newValue: number) => {
        invoke(onchange, [ newValue ])
    }, 500);

    const doSliderAnimation = (newValue: number) => {
        const span = document.getElementById(id+"sliderTick") as HTMLSpanElement;
        span.innerText = `${newValue}`;
        span.style.left = `${(newValue - min) / (max - min) * 100}%`
    }

    sldr.oninput = () => {
        const newValue = parseInt(sldr.value);
        doSliderEvents(newValue);
        doSliderAnimation(newValue);
    }

    div.appendChild(sldr);

    //         <span class="decrement">-</span>
    const decrement = document.createElement("span");
    decrement.className = "decrement noselect";
    decrement.innerText = "-";
    decrement.onclick = () => {
        const newValue = parseInt(sldr.value) - 1;
        sldr.value = `${newValue}`;
        doSliderEvents(parseInt(sldr.value));
        doSliderAnimation(parseInt(sldr.value));
    }
    div.appendChild(decrement);

    //         <span class="increment">+</span>
    const increment = document.createElement("span");
    increment.className = "increment noselect";
    increment.innerText = "+";
    increment.onclick = () => {
        const newValue = parseInt(sldr.value) + 1;
        sldr.value = `${newValue}`;
        doSliderEvents(parseInt(sldr.value));
        doSliderAnimation(parseInt(sldr.value));
    }
    div.appendChild(increment);

    //         <span id=id+"sliderTick" class="sliderTick">${mid}</span>
    const span = document.createElement("span");
    span.id = id+"sliderTick";
    span.className = "sliderTick";
    const mid = Math.round((min + max) / 2);
    span.innerText = `${mid}`;
    span.style.left = `${(mid - min) / (max - min) * 100}%`
    div.appendChild(span);

    slider.appendChild(div);

    element.appendChild(slider);
}

function uiCreateDropDown(label: string, info: string, id: string, isMulti: boolean, activeValues: string[], onchange: string, target: string, items: string[]): void {
    const element = document.getElementById(target);
    if (!element)
        return noTarget(target);

    // <div class="dropdown element" id=`${id}`>
    const dropdown = document.createElement("div");
    dropdown.className = "dropdown element"
    dropdown.id = id;

    //     <p class="label">${label}</p>
    const p = document.createElement("p");
    p.className = "label";
    p.innerText = label;
    dropdown.appendChild(p);

    //     <span class="shadow">${label}</span>
    const shadow = document.createElement("span");
    shadow.className = "shadow";
    shadow.innerText = label;
    dropdown.appendChild(shadow);

    if (info != "")
        dropdown.appendChild(createInfoBox(info));

    const dropDownDiv = document.createElement("div");
    dropDownDiv.className = "wrapper"

    const disabledInput = document.createElement("input");
    disabledInput.type = "text";
    disabledInput.id = id + "input";
    disabledInput.disabled = true;
    dropDownDiv.appendChild(disabledInput);

    const dropdownContent = document.createElement("div");
    dropdownContent.className = "content noselect";
    dropdownContent.id = id + "content";

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

        const p = document.createElement("p");
        p.innerText = item;
        p.style.color = gradient[i];

        if (activeValues.includes(item))
            p.className = "selected";

        p.onclick = () => {
            if (!isMulti)
                for (const element of dropdownContent.children as HTMLCollectionOf<HTMLParagraphElement>)
                    element.className = "";

            p.className = p.className == "selected" ? "" : "selected";
            const selectedItems = updateInput();
            invoke(onchange, [ p.innerText, p.className == "selected", selectedItems ]);
        }
        dropdownContent.appendChild(p);
    }

    updateInput();

    dropDownDiv.appendChild(dropdownContent);

    dropdown.appendChild(dropDownDiv);
 
    element.appendChild(dropdown);
}

function uiCreateSelector(label: string, info: string, state: number, id: string, onchange: string, target: string, items: string[]): void {
    const element = document.getElementById(target);
    if (!element)
        return noTarget(target);

    const selector = document.createElement("div");
    selector.className = "selector element"
    selector.id = id;

    //     <p class="label">${label}</p>
    const p = document.createElement("p");
    p.className = "label";
    p.innerText = label;
    selector.appendChild(p);

    //     <span class="shadow">${label}</span>
    const shadow = document.createElement("span");
    shadow.className = "shadow";
    shadow.innerText = label;
    selector.appendChild(shadow);

    if (info != "")
        selector.appendChild(createInfoBox(info));

    const selectorContent = document.createElement("div");
    selectorContent.id = id + "content"
    selectorContent.className = "content noselect"

    const gradient = colors.generateGradient(items.length);

    for (let i = 0; i < items.length; i++) {
        const item = items[i];

        const p = document.createElement("p");
        p.innerText = item;

        p.style.backgroundColor = gradient[i];

        if (i == state)
            p.className = "selected";

        p.onclick = () => {
            if (p.className == "selected")
                return;

            for (const element of document.getElementById(id + "content")!.children as HTMLCollectionOf<HTMLParagraphElement>)
                element.className = "";

            p.className = "selected";
            
            // state that gets returned is ignored
            invoke(onchange, [ p.innerText ]);
        }
        selectorContent.appendChild(p);
    }

    selector.appendChild(selectorContent);

    element.appendChild(selector);
}

function uiCreateInput(label: string, info: string, id: string, value: string, onchange: string, target: string): void {
    const element = document.getElementById(target);
    if (!element)
        return noTarget(target);

    const input = document.createElement("div");
    input.className = "input element"
    input.id = id;

    //     <p class="label">${label}</p>
    const p = document.createElement("p");
    p.className = "label";
    p.innerText = label;
    input.appendChild(p);

    //     <span class="shadow">${label}</span>
    const shadow = document.createElement("span");
    shadow.className = "shadow";
    shadow.innerText = label;
    input.appendChild(shadow);

    if (info != "")
        input.appendChild(createInfoBox(info));

    const container = document.createElement("div");

    const inputField = document.createElement("input");
    inputField.type = "text";
    inputField.value = value;
    inputField.spellcheck = false;
    inputField.autocapitalize = "false";
    inputField.autocomplete = "off";

    inputField.onblur = debounce(async () => {
        inputField.value = await invoke(onchange, [ inputField.value ]);
    }, 250)

    container.appendChild(inputField);

    input.appendChild(container);

    element.appendChild(input);
}

function uiCreateList(label: string, info: string, id: string, activeValues: string[], validator: string, onchange: string, target: string): void {
    const element = document.getElementById(target);
    if (!element)
        return noTarget(target);

    const list = document.createElement("div");
    list.className = "list element"
    list.id = id;

    //     <p class="label">${label}</p>
    const p = document.createElement("p");
    p.className = "label";
    p.innerText = label;
    list.appendChild(p);

    //     <span class="shadow">${label}</span>
    const shadow = document.createElement("span");
    shadow.className = "shadow";
    shadow.innerText = label;
    list.appendChild(shadow);

    if (info != "")
        list.appendChild(createInfoBox(info));

    const container = document.createElement("div");

    const listView = document.createElement("div");
    listView.className = "listView";
    listView.id = id+"listView";

    const RenderListViewItems = () => {
        listView.innerHTML = "";
        for (const value of activeValues) {
            const item = document.createElement("p");
            item.innerText = value;
            item.onclick = () => {
                activeValues = activeValues.filter(activeValue => value != activeValue);
                listView.removeChild(item);
                invoke(onchange, [ activeValues ]);
            }
            listView.appendChild(item);
        }
    }

    RenderListViewItems();

    container.appendChild(listView);

    const listInput = document.createElement("div");
    listInput.className = "listInput";

    const inputField = document.createElement("input");
    inputField.id = id + "inputFieldId"
    inputField.type = "text";
    inputField.spellcheck = false;
    inputField.autocapitalize = "false";
    inputField.autocomplete = "off";
    listInput.appendChild(inputField);

    const add = document.createElement("button");
    add.innerText = "add";
    add.id = id+"addListItem";
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
    listInput.appendChild(add);

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
            const item = document.createElement("p");
            item.innerText = value;
            item.onclick = () => {
                activeValues = activeValues.filter(activeValue => value != activeValue);
                listView.removeChild(item);
                invoke(onchange, [ activeValues ]);
            }
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
    const notification = document.createElement("div");
    notification.className = "notification noselect";

    const content = document.createElement("div");
    content.className = "content";

    const contentTitle = document.createElement("p");
    contentTitle.className = "contentTitle";
    contentTitle.innerText = title;
    content.appendChild(contentTitle);

    const contentDescription = document.createElement("p");
    contentDescription.className = "contentDescription";
    contentDescription.innerText = description;
    content.appendChild(contentDescription);

    notification.appendChild(content);

    let isRemoving = false;

    const clearThisElement = () => {
        if (!isRemoving) {
            isRemoving = true;

            notification.classList.add("fade");
            setTimeout(() => parent.removeChild(notification), 300);
        }
    }

    let timeout = setTimeout(clearThisElement, 4700);

    notification.onclick = () => {
        clearTimeout(timeout);
        clearThisElement();
    }

    const statusBar = document.createElement("div");
    statusBar.className = "statusbar";
    statusBar.appendChild(document.createElement("div"));
    notification.appendChild(statusBar);

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