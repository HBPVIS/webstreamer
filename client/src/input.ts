function parseModifierKeys (event: MouseEvent|KeyboardEvent) {
    let result = 0;
    if (event.altKey) {
        result |= (1 << 0);
    }
    if (event.ctrlKey) {
        result |= (1 << 1);
    }
    if (event.metaKey) {
        result |= (1 << 2);
    }
    if (event.shiftKey) {
        result |= (1 << 3);
    }
    return result;
}

export function parseMouseEvent(width: number, height: number, event: MouseEvent) {
    const MOUSE_ACTIONS = {
        "mousemove": 0,
        "mousedown": 1,
        "mouseup":   2,
        "dblclick":  3,
    };
    
    const timestamp = performance.now();
    const arrayBuffer = new ArrayBuffer(16);
    const dataView = new DataView(arrayBuffer);
    dataView.setUint8 ( 0, 1); // TODO: create constant
    dataView.setUint8 ( 1, parseModifierKeys(event));
    dataView.setUint32( 4, timestamp, true);
    dataView.setUint8 ( 8, MOUSE_ACTIONS[event.type]);
    dataView.setUint8 ( 9, event.button);
    dataView.setUint8 (10, event.buttons);
    dataView.setUint16(12, event.offsetX / width * 65536, true);
    dataView.setUint16(14, event.offsetY / height * 65536, true);
    return arrayBuffer;
}

export function parseKeyEvent(event: KeyboardEvent) {
    const KEY_ACTIONS = {
        "keydown":  0,
        "keyup":    1,
        "keypress": 2,
    };
    
    const KEY_CHARACTERS_OFFSET = 12;
    const KEY_CHARACTERS_COUNT = 16;
    const CODE_CHARACTERS_OFFSET = KEY_CHARACTERS_OFFSET + KEY_CHARACTERS_COUNT;
    const CODE_CHARACTERS_COUNT = 16;

    const timestamp = performance.now();
    const arrayBuffer = new ArrayBuffer(12 + 32);
    const dataView = new DataView(arrayBuffer);
    dataView.setUint8 ( 0, 2); // TODO: create constant
    dataView.setUint8 ( 1, parseModifierKeys(event));
    dataView.setUint32( 4, timestamp, true);
    dataView.setUint8 ( 8, KEY_ACTIONS[event.type]);
    dataView.setUint8 ( 9, event.repeat ? 1 : 0);

    // TODO: handle unicode
    for (let i = 0; i < KEY_CHARACTERS_COUNT; ++i) {
        const offset = KEY_CHARACTERS_OFFSET + i;
        const charCode = event.key.length > i ? event.key.charCodeAt(i) : 0;
        dataView.setUint8(offset, charCode);
    }
    for (let i = 0; i < CODE_CHARACTERS_COUNT; ++i) {
        const offset = CODE_CHARACTERS_OFFSET + i;
        const charCode = event.code.length > i ? event.code.charCodeAt(i) : 0;
        dataView.setUint8(offset, charCode);
    }

    return arrayBuffer;
}