import { Stream } from "./stream";
import { EventType, MouseAction, KeyboardAction } from "./events";
import { KeyMapper } from "./key-mapper";

function getMouseAction(eventType: string) {
    switch (eventType) {
        case "mousemove":
            return MouseAction.Move;

        case "mousedown":
            return MouseAction.ButtonDown;

        case "mouseup":
            return MouseAction.ButtonUp;

        case "dblclick":
            return MouseAction.DoubleClick;

        default:
            throw new Error("Invalid event type");
    }
}

function getKeyboardAction(eventType: string) {
    switch (eventType) {
        case "keydown":
            return KeyboardAction.KeyDown;

        case "keypress":
            return KeyboardAction.KeyPress;

        case "keyup":
            return KeyboardAction.KeyUp;

        default:
            throw new Error("Invalid event type");
    }
}

export class InputCapturer {
    private _domElement: HTMLElement;
    public get domElement(): HTMLElement {
        return this._domElement;
    }
    public set domElement(v: HTMLElement) {
        this.deregisterInputHandlers();
        this._domElement = v;
        this.registerInputHandlers();
    }

    private _stream: Stream;
    public get stream(): Stream {
        return this._stream;
    }
    public set stream(v: Stream) {
        this._stream = v;
    }

    private contextMenuEvent = this.onContextMenu.bind(this);
    private mouseEvent = this.onMouseInput.bind(this);
    private keyboardEvent = this.onKeyboardInput.bind(this);

    private registerInputHandlers() {
        if (this._domElement) {
						this._domElement.addEventListener("contextmenu", this.contextMenuEvent);
            this._domElement.addEventListener("mousemove", this.mouseEvent);
            this._domElement.addEventListener("mousedown", this.mouseEvent);
            this._domElement.addEventListener("mouseup", this.mouseEvent);
            this._domElement.addEventListener("dblclick", this.mouseEvent);
            this._domElement.addEventListener("keydown", this.keyboardEvent);
            //this._domElement.addEventListener("keypress", this.keyboardEvent);
            this._domElement.addEventListener("keyup", this.keyboardEvent);
						if (this._domElement.tabIndex < 0) {
                this._domElement.tabIndex = -1;
            }
            this._domElement.focus();

						KeyMapper.initializeKeymap();
        }
    }

    private deregisterInputHandlers() {
        if (this._domElement) {
            this._domElement.removeEventListener("contextmenu", this.contextMenuEvent);
            this._domElement.removeEventListener("mousemove", this.mouseEvent);
            this._domElement.removeEventListener("mousedown", this.mouseEvent);
            this._domElement.removeEventListener("mouseup", this.mouseEvent);
            this._domElement.removeEventListener("dblclick", this.mouseEvent);
            this._domElement.removeEventListener("keydown", this.keyboardEvent);
            //this._domElement.removeEventListener("keypress", this.keyboardEvent);
            this._domElement.removeEventListener("keyup", this.keyboardEvent);
        }
    }

    private onContextMenu(event: PointerEvent) {
				event.preventDefault();
    }

    private onMouseInput(event: MouseEvent) {
        this._domElement.focus();
        if (this._stream) {
            this._stream.sendEvent({
                type: EventType.MouseInput,
                action: getMouseAction(event.type),
                x: event.offsetX / (this._domElement.offsetWidth - 1),
                y: event.offsetY / (this._domElement.offsetHeight - 1),
                button: event.button,
                buttons: event.buttons
            });
        }
        event.preventDefault();
    }

    private onKeyboardInput(event: KeyboardEvent) {
        if (this._stream) {
						
						var mappedKey : string = KeyMapper.onKeyboardEvent(event.key);

						if(mappedKey != '')
						{
		          this._stream.sendEvent({
		              type: EventType.KeyboardInput,
		              action: getKeyboardAction(event.type),
		              code: event.code,
		              key: mappedKey
		          });
							//event.preventDefault();
						}
        }
    }
}
