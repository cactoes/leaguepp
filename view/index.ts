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


namespace ui {
    const noTarget = (target: string) => console.error(`Error: Failed to find target "${target}"`);

    export function createFrame(name: string, id: string, layout: string, target: string): void {
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
    
    export function createButton(label: string, id: string, onclick: string, target: string): void {
        const element = document.getElementById(target);
        if (!element)
            return noTarget(target);
    
        const button = document.createElement("div");
        button.className = "button element";
        button.id = id;
    
        const btn = document.createElement("button");
        btn.innerText = label;
        btn.onclick = () => { invoke(onclick, []); }
        button.appendChild(btn);
    
        element.appendChild(button);
    }
    
    export function createLabel(text: string, pos: string, id: string, target: string): void {
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

    export function updateLabel(text: string, id: string): void {
        const element = document.getElementById(id) as HTMLDivElement | null;
        if (!element)
            return noTarget(id);

        const p = document.createElement("p");
        p.innerText = text;

        const shadow = document.createElement("span");
        shadow.innerText = text;
        
        element.replaceChildren(p, shadow);
    }
    
    export function createCheckBox(label: string, state: boolean, id: string, onclick: string, target: string): void {
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
    
        const lbl = document.createElement("label");
        const input = document.createElement("input");
        input.type = "checkbox";
        input.checked = state;
        input.onclick = (e: MouseEvent) => {
            // state that gets returned is ignored
            invoke(onclick, [ (e.currentTarget as HTMLInputElement).checked ])
        }
        lbl.appendChild(input);
        const span = document.createElement("span");
        lbl.appendChild(span);
        checkbox.appendChild(lbl);
    
        element.appendChild(checkbox);
    }
    
    export function createSlider(label: string, id: string, onchange: string, min: number, max: number, target: string): void {
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

        const div = document.createElement("div");
    
        const sldr = document.createElement("input");
        sldr.type = "range"
        sldr.min = `${min}`;
        sldr.max = `${max}`;

        const doSliderEvents = (newValue: number) => {
            invoke(onchange, [ newValue ]);
            const span = document.getElementById(id+"sliderTick") as HTMLSpanElement;
            span.innerText = `${newValue}`;
            span.style.left = `${(newValue - min) / (max - min) * 100}%`
        }
    
        sldr.oninput = () => {
            const newValue = parseInt(sldr.value);
            doSliderEvents(newValue);
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
    
    export function createDropDown(label: string, id: string, onchange: string, target: string, items: string[]): void {
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

        const dropDownDiv = document.createElement("div");
    
        const disabledInput = document.createElement("input");
        disabledInput.type = "text";
        disabledInput.id = id + "input";
        disabledInput.disabled = true;
        dropDownDiv.appendChild(disabledInput);
    
        const dropdownContent = document.createElement("div");
        dropdownContent.className = "content noselect";
        dropdownContent.id = id + "content";
    
        for (const item of items) {
            const p = document.createElement("p");
            p.innerText = item;
            p.onclick = () => {
                p.className = p.className == "selected" ? "" : "selected";

                const selectedItems: string[] = [];
                for (const element of document.getElementById(id + "content")!.children as HTMLCollectionOf<HTMLParagraphElement>) {
                    if (element.className == "selected")
                        selectedItems.push(element.innerText);
                }
    
                const a = document.getElementById(id + "input")! as HTMLInputElement;
                a.value = selectedItems.join(", ");
    
                invoke(onchange, [ p.innerText, p.className == "selected", selectedItems ]);
            }
            dropdownContent.appendChild(p);
        }
    
        dropDownDiv.appendChild(dropdownContent);
    
        dropdown.appendChild(dropDownDiv);
     
        element.appendChild(dropdown);
    }

    export function createSelector(label: string, state: number, id: string, onchange: string, target: string, items: string[]): void {
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

        const selectorContent = document.createElement("div");
        selectorContent.id = id + "content"
        selectorContent.className = "noselect"

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

    export function createNotificationBox(title: string, description: string, typeId: number, parent: HTMLDivElement): HTMLDivElement {
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
}; // namespace ui

function uiCreateFrame(name: string, id: string, layout: string, target: string): void {
    ui.createFrame(name, id, layout, target);
}

function uiCreateButton(label: string, id: string, onclick: string, target: string): void {
    ui.createButton(label, id, onclick, target);
}

function uiCreateLabel(text: string, pos: string, id: string, target: string): void {
    ui.createLabel(text, pos, id, target);
}

function uiUpdateLabel(text: string, id: string): void {
    ui.updateLabel(text, id);
}

function uiCreateCheckBox(label: string, state: boolean, id: string, onclick: string, target: string): void {
    ui.createCheckBox(label, state, id, onclick, target);
}

function uiCreateSlider(label: string, id: string, onchange: string, min: number, max: number, target: string): void {
    ui.createSlider(label, id, onchange, min, max, target);
}

function uiCreateDropDown(label: string, id: string, onchange: string, target: string, items: string[]): void {
    ui.createDropDown(label, id, onchange, target, items);
}

function uiCreateSelector(label: string, state: number, id: string, onchange: string, target: string, items: string[]): void {
    ui.createSelector(label, state, id, onchange, target, items);
}

function createNotification(title: string, description: string, typeId: number) {
    const target = document.getElementById("notification_wrapper") as HTMLDivElement;
    const box = ui.createNotificationBox(title, description, typeId, target);
    target.appendChild(box);
}

async function main() {
    register(uiCreateFrame);
    register(uiCreateButton);
    register(uiCreateLabel);
    register(uiUpdateLabel);
    register(uiCreateCheckBox);
    register(uiCreateSlider);
    register(uiCreateDropDown);
    register(uiCreateSelector);

    register(createNotification);

    for (const button of (document.getElementsByClassName("button") as HTMLCollectionOf<HTMLDivElement>))
        button.addEventListener("click", () => invoke("HandleWindowEvent", [ button.id ]))

    await invoke("GetMainLayout", []);
}

main();