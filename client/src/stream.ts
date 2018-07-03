import * as $ from 'jquery';
import { Event, serializeEvent, deserializeEvent, getEventString } from "./events"

export enum StreamType {
    WebRTC = "webRTC",
    WebSocket = "webSocket",
}

export abstract class Stream {
    public onOpen: () => void;
    public onDisconnect: (reason: string) => void;
    public onReceiveEncodedFrame: (encodedFrame: ArrayBufferView) => void;
    public onReceiveEvent: (event: Event) => void;

    public readonly type: StreamType;

    protected _isConnected: boolean = false;
    public get isConnected(): boolean {
        return this._isConnected;
    }

    public constructor(type: StreamType) {
        this.type = type;
        $(".stream-settings").hide();
        $(".stream-settings[data-stream-type='" + type + "']").show();
    }

    public abstract configure(options: any);

    public sendEvent(event: Event) {
        console.info("Send event: " + getEventString(event));
        this.sendEventData(serializeEvent(event));
    }

    protected receiveEventData(data: ArrayBufferView) {
        if (this.onReceiveEvent) {
            this.onReceiveEvent(deserializeEvent(data));
        }
    }

    protected abstract sendEventData(data: ArrayBuffer): void;
}
