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
        p.innerText = text;
        label.appendChild(p);
    
        element.appendChild(label);
    }

    export function updateLabel(text: string, id: string): void {
        const element = document.getElementById(id) as HTMLDivElement | null;
        if (!element)
            return noTarget(id);

        const p = document.createElement("p");
        p.innerText = text;
        element.replaceChildren(p);
    }
    
    export function createCheckBox(label: string, state: boolean, id: string, onclick: string, target: string): void {
        const element = document.getElementById(target);
        if (!element)
            return noTarget(target);
    
        const checkbox = document.createElement("div");
        checkbox.className = "checkbox element";
        checkbox.id = id;
    
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
    
        const p = document.createElement("p");
        p.innerText = label;
        checkbox.appendChild(p);
    
        element.appendChild(checkbox);
    }
    
    export function createSlider(label: string, id: string, onchange: string, min: number, max: number, target: string): void {
        const element = document.getElementById(target);
        if (!element)
            return noTarget(target);
    
        const slider = document.createElement("div");
        slider.className = "slider element";
        slider.id = id;
    
        const sldr = document.createElement("input");
        sldr.type = "range"
        sldr.min = `${min}`;
        sldr.max = `${max}`;
    
        sldr.oninput = (e: Event) => {
            const newValue = parseInt((e.currentTarget as HTMLInputElement).value);
            invoke(onchange, [ newValue ]);
            const span = document.getElementById(id+"sliderTick") as HTMLSpanElement;
            span.innerText = `${newValue}`;
            span.style.left = `${(newValue - min) / (max - min) * 100}px`
        }
    
        slider.appendChild(sldr);
    
        const span = document.createElement("span");
        span.id = id+"sliderTick";
        const mid = Math.round((min + max) / 2);
        span.innerText = `${mid}`;
        span.style.left = `${(mid - min) / (max - min) * 100}px`
        slider.appendChild(span);
    
        const p = document.createElement("p");
        p.innerText = label;
        slider.appendChild(p);
    
        element.appendChild(slider);
    }
    
    export function createDropDown(label: string, id: string, onchange: string, target: string, items: string[]): void {
        const element = document.getElementById(target);
        if (!element)
            return noTarget(target);

        const dropdown = document.createElement("div");
        dropdown.className = "dropdown element"
        dropdown.id = id;
    
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
    
    
        const lbl = document.createElement("p");
        lbl.innerText = label;
        dropdown.appendChild(lbl);
     
        element.appendChild(dropdown);
    }

    export function createSelector(label: string, state: number, id: string, onchange: string, target: string, items: string[]): void {
        const element = document.getElementById(target);
        if (!element)
            return noTarget(target);
    
        const selector = document.createElement("div");
        selector.className = "selector element"
        selector.id = id;

        const selectorContent = document.createElement("div");
        selectorContent.id = id + "content"
        selectorContent.className = "noselect"

        for (let i = 0; i < items.length; i++) {
            const item = items[i];

            const p = document.createElement("p");
            p.innerText = item;

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
            
        const lbl = document.createElement("p");
        lbl.innerText = label;
        selector.appendChild(lbl);

        element.appendChild(selector);
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

function createNotification(text: string, typeId: number) {
    console.log(text, typeId);
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