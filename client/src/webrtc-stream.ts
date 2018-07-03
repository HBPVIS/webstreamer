import { Stream, StreamType } from "./stream";
import { JSONGetValue } from "./json";

export class WebRTCStream extends Stream {
    private websocket: WebSocket;
    private peerConnection: RTCPeerConnection;
    private videoChannel: any;
    private eventChannel: any;

    private currentMessageNumber: number;
    private receivedBytes: number;
    private messageBuffer: Uint8Array;

    public constructor() {
        super(StreamType.WebRTC);
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

            this.websocket.addEventListener("open", () => {
                this.peerConnection = new RTCPeerConnection({});
                this.peerConnection.onicecandidate = (connection) => {
                    const serializedCandidate = JSON.stringify(connection.candidate);
                    console.info("ICE candidate: " + serializedCandidate);
                    this.websocket.send(serializedCandidate);
                };
                this.peerConnection.onsignalingstatechange = (event) => {
                    console.info("onsignalingstatechange: " + this.peerConnection.signalingState);
                };
                (this.peerConnection as any).ondatachannel = (event) => {
                    const channel = event.channel;
                    console.info("ondatachannel: " + channel.label);

                    if (channel.label === "video") {
                        this.videoChannel = channel;
                        this.videoChannel.onmessage = (message) => {
                            WebRTCStream.BlobToArrayBuffer(
                                message.data,
                                (buffer) => {
                                    this.handleVideoMessage(buffer);
                                });
                        };
                        this.videoChannel.onopen = () => {
                            console.info("Video channel open");
                            if (this.eventChannel && this.eventChannel.readyState === "open") {
                                this._isConnected = true;
                                if (this.onOpen) {
                                    this.onOpen();
                                }
                            }
                        }
                        this.videoChannel.onclose = (event) => {
                            console.info("Video channel closed");
                        }
                    } else if (channel.label === "event") {
                        this.eventChannel = channel;
                        this.eventChannel.onmessage = () => {
                            console.info("Received event message");
                        };
                        this.eventChannel.onopen = () => {
                            if (this.videoChannel && this.videoChannel.readyState === "open") {
                                this._isConnected = true;
                                if (this.onOpen) {
                                    this.onOpen();
                                }
                            }
                        }
                    } else {
                        console.error("Unknown data channel: " + channel.label);
                    }
                };
            });

            this.websocket.addEventListener("close", (event) => {
                if (this.onDisconnect) {
                    this.onDisconnect(event.reason);
                }
            });

            this.websocket.addEventListener("error", () => {
                this.websocket.close();
            });

            this.websocket.addEventListener("message", (event) => {
                this.handleSignalingMessage(event.data);
            });
        }
    }

    public sendEventData(event: ArrayBuffer) {
        if (this.eventChannel && this.eventChannel.readyState === "open") {
            this.eventChannel.send(event);
        } else {
            throw new Error("Send event ")
        }
    }

    private handleSignalingMessage(data: string) {
        const object = JSON.parse(data);
        if (object.hasOwnProperty("type") && object.hasOwnProperty("sdp")) {
            console.info("Received remote description: " + data);
            this.peerConnection.setRemoteDescription(
                new RTCSessionDescription(object),
                () => {
                    console.info("setRemoteDescription(): success");
                    this.peerConnection.createAnswer(
                        (localSessionDescription) => {
                            console.info("createAnswer(): sucess");

                            const serializedLocalSessionDescription = JSON.stringify(localSessionDescription);
                            console.info("Local description: " + serializedLocalSessionDescription);

                            this.websocket.send(serializedLocalSessionDescription);

                            this.peerConnection.setLocalDescription(
                                localSessionDescription,
                                () => {
                                    console.info("setLocalDescription(): success");
                                },
                                (error) => {
                                    console.error("setLocalDescription(): failure (" + error.toString() + ")");
                                });
                        },
                        (error) => {
                            console.info("createAnswer(): failure (" + error.toString() + ")");
                        }
                    );
                },
                (error) => {
                    console.error("setRemoteDescription(): failure (" + error.toString() + ")");
                });
        } else if (object.hasOwnProperty("candidate") && object.hasOwnProperty("sdpMid") && object.hasOwnProperty("sdpMLineIndex")) {
            console.info("Received remote candidate: " + data);
            this.peerConnection.addIceCandidate(
                new RTCIceCandidate(object),
                () => {
                    console.info("addIceCandidate(): success");
                },
                (error) => {
                    console.error("addIceCandidate(): failure (" + error.toString() + ")");
                })
        }
    }

    private handleVideoMessage(buffer: ArrayBuffer) {
        const dataView = new DataView(buffer);
        const messageNumber = dataView.getUint32(0, true);
        const messageSize = dataView.getUint32(4, true);
        const dataOffset = dataView.getUint32(8, true);

        const array = new Uint8Array(buffer, 12);

        if (messageNumber === this.currentMessageNumber) {
            if (messageSize !== this.messageBuffer.byteLength) {
                console.warn("Message length changed during transmission. Skip message!");
                this.messageBuffer = new Uint8Array([]);
            } else {
                if (dataOffset + array.byteLength > messageSize) {
                    console.warn("Invalid offset. Skip message!");
                    this.messageBuffer = new Uint8Array([]);
                } else {
                    this.messageBuffer.set(array, dataOffset);
                    this.receivedBytes += array.byteLength;
                }
            }
        } else {
            this.currentMessageNumber = messageNumber;
            this.messageBuffer = new Uint8Array(messageSize);
            if (dataOffset + array.byteLength > messageSize) {
                console.warn("Invalid offset. Skip message!");
                this.messageBuffer = new Uint8Array([]);
            } else {
                this.messageBuffer.set(array, dataOffset);
                this.receivedBytes = array.byteLength;
            }
        }
        if (this.receivedBytes === this.messageBuffer.byteLength) {
            this.onReceiveEncodedFrame(this.messageBuffer);
        }
    }

    private static BlobToArrayBuffer(blob: Blob, callback: (arrayBuffer: ArrayBuffer) => void) {
        const fileReader = new FileReader();
        fileReader.onload = () => {
            callback(fileReader.result);
        };
        fileReader.readAsArrayBuffer(blob);

    }
}
