import { Codec, getCodecId } from "./decoder";
import { Clipboard } from "./clipboard"

export enum EventType {
    Unknown = 0x0,

    // 0x10 - 0x1F: Stream control events
    StreamConfigChanged = 0x10,
    ChangeCodec = 0x11,

    // 0x30 - 0x3F: User control events
    Play = 0x30,
    Stop = 0x31,
    AquireInput = 0x32,
    ReleaseInput = 0x33,

    // 0x50 - 0x5F: Input events
    MouseInput = 0x50,
    KeyboardInput = 0x51,
		ClipboardInput = 0x52,
		RequestClipboard = 0x53
}

interface IEvent {
    type: EventType;
}

export interface IInvalidEvent extends IEvent {
    type: EventType.Unknown;
}

export interface IEmptyEvent extends IEvent {
    type: EventType.AquireInput | EventType.ReleaseInput | EventType.Play | EventType.Stop;
}

export interface IChangeCodecEvent extends IEvent {
    type: EventType.ChangeCodec;
    codec: Codec;
    options: any;
}

export enum MouseAction {
    Move = 0,
    ButtonDown = 1,
    ButtonUp = 2,
    DoubleClick = 3,
}

export interface IMouseInputEvent extends IEvent {
    type: EventType.MouseInput;
    action: MouseAction;
    x: number;
    y: number;
    button: number;
    buttons: number;
}

export enum KeyboardAction {
    KeyDown = 0,
    KeyPress = 1,
    KeyUp = 2,
}

export interface IKeyboardInputEvent extends IEvent {
    type: EventType.KeyboardInput;
    action: KeyboardAction;
    code: string;
    key: string;
}

export interface IClipboardEvent extends IEvent {
	type: EventType.ClipboardInput;
	size: number;
	content: string;
}

export type Event = IInvalidEvent | IEmptyEvent | IChangeCodecEvent | IMouseInputEvent | IKeyboardInputEvent | IClipboardEvent;

export function getEventString(event: Event) {
    switch (event.type) {
        case EventType.AquireInput:
        case EventType.ReleaseInput:
        case EventType.Play:
        case EventType.Stop:
            return EventType[event.type];

        case EventType.ChangeCodec:
            return "ChangeCodec(" + event.codec + "," + JSON.stringify(event.options) + ")";

        case EventType.MouseInput:
            return "MouseInput(" + MouseAction[event.action] + "," + event.x + "," + event.y + "," + event.button + "," + event.buttons + ")";

        case EventType.KeyboardInput:
            return "KeyboardInput(" + KeyboardAction[event.action] + "," + event.code + "," + event.key + ")";

		case EventType.ClipboardInput:
			return "ClipboardInput(" + event.size + ","+event.content+")";

        case EventType.Unknown:
            return null;
    }
}

export function serializeEvent(event: Event) {
    switch (event.type) {
        case EventType.AquireInput:
        case EventType.ReleaseInput:
        case EventType.Play:
        case EventType.Stop: {
            const buffer = new ArrayBuffer(1);
            const view = new DataView(buffer);
            view.setUint8(0, event.type);
            return buffer;
        }

        case EventType.ChangeCodec: {
            let options = "";
            if (event.options) {
                options = JSON.stringify(event.options);
                if (options.length > 1022) {
                    throw new Error("Options too long");
                }
            }

            const buffer = new ArrayBuffer(2 + options.length);
            const view = new DataView(buffer);
            view.setUint8(0, event.type);
            view.setUint8(1, getCodecId(event.codec));
            for (let i = 0; i < options.length; ++i) {
                const charCode = options.charCodeAt(i);
                if (charCode >= 128) {
                    throw new Error("Non ASCII characters are not allowed in the encoder options");
                }
                view.setUint8(2 + i, charCode);
            }
            return buffer;
        }

        case EventType.MouseInput: {
            const buffer = new ArrayBuffer(8);
            const view = new DataView(buffer);

            view.setUint8(0, event.type);
            view.setUint8(1, event.action);
            view.setUint16(2, event.x * 0xffff, true);
            view.setUint16(4, event.y * 0xffff, true);
            view.setUint8(6, event.button);
            view.setUint8(7, event.buttons);

            return buffer;
        }

        case EventType.KeyboardInput: {

            const buffer = new ArrayBuffer(4 + event.code.length + event.key.length);
            const view = new DataView(buffer);

            view.setUint8(0, event.type);
            view.setUint8(1, event.action);

            if (event.code.length >= 256) {
                throw new Error("Code too long");
            }
            view.setUint8(2, event.code.length);

            if (event.key.length >= 256) {
                throw new Error("Key too long");
            }
            view.setUint8(3, event.key.length);

            for (let i = 0; i < event.code.length; ++i) {
                const charCode = event.code.charCodeAt(i);
                if (charCode >= 128) {
                    throw new Error("Non ASCII character in key code");
                }
                view.setUint8(4 + i, charCode);
            }

            for (let i = 0; i < event.key.length; ++i) {
                const charCode = event.key.charCodeAt(i);
                if (charCode >= 128) {
                    throw new Error("Non ASCII character in key name");
                }
                view.setUint8(4 + event.code.length + i, charCode);
            }

            return buffer;
        }

        case EventType.ClipboardInput:
        {
            var clipBoardContent = event.content;

            var size = clipBoardContent.length;

            const buffer = new ArrayBuffer(1 + size);
            const view = new DataView(buffer);
            
            view.setUint8(0, event.type);
            
            for(let i = 0; i < size; i++)
            {
                const charCode = clipBoardContent.charCodeAt(i);
                if (charCode >= 128) 
                {
                    view.setUint8(1 + i, 100);
                }
                else
                {
                    view.setUint8(1 + i, charCode);
                }
            }

            return buffer;
        }

        case EventType.Unknown:
            return null;
    }
}

export function deserializeEvent(buffer: ArrayBufferView): Event {
    const view = new DataView(buffer.buffer, buffer.byteOffset);

    const type = view.getUint8(0);
    switch (type) {
        case EventType.AquireInput:
        case EventType.ReleaseInput:
        case EventType.Play:
        case EventType.Stop:
            if (buffer.byteLength != 1) {
                return { type: EventType.Unknown };
            }
            return { type };
    }
    return { type: EventType.Unknown }
}
