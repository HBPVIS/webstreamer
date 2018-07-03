import { Stream, StreamType } from "./stream";
import { JSONGetValue } from "./json";

export class WebSocketStream extends Stream {
    private websocket: WebSocket;

    public constructor() {
        super(StreamType.WebSocket);
    }

    public configure(options: any) {
        const url = JSONGetValue(options, "url", window.location.href.match(/^(?:.+:\/\/)?([^:\/]+)(?::\d+)?(?:\/.*)?/)[1]);
        const serverAddress = "ws://" + url + ":" + JSONGetValue(options, "port");

        if (this.websocket && this.websocket.url !== serverAddress) {
            this.websocket.close();
            this.websocket = undefined;
        }
        if (!this.websocket) {
            this.websocket = new WebSocket(serverAddress);
            this.websocket.binaryType = "arraybuffer";

            this.websocket.addEventListener("open", () => {
                this._isConnected = true;
                if (this.onOpen) {
                    this.onOpen();
                }
            });

            this.websocket.addEventListener("close", () => {
                if (this.onDisconnect) {
                    this.onDisconnect("");
                }
            });

            this.websocket.addEventListener("error", () => {
                this.websocket.close();
            });

            this.websocket.addEventListener("message", (event) => {
                // alert("WebSocket: message(" + event.data + ")");
                this.onData(event.data);
            });
        }
    }

    public sendEventData(event: ArrayBuffer) {
        this.websocket.send(event);
    }

    private onData(data: ArrayBuffer) {
        const data_type = new Uint32Array(data, 0, 1)[0];
        if (data_type === 1) {
            this.receiveEventData(new Uint8Array(data, 4));
        } else if (data_type === 2) {
            if (this.onReceiveEncodedFrame) {
                this.onReceiveEncodedFrame(new Uint8Array(data, 4));
            }
        } else {
            alert("Invalid data type!");
        }
    }
}
