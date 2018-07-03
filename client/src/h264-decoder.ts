import { Decoder, Codec } from "./decoder";

import { Player } from "broadway-player";
import { IVideoMode } from "./ivideo-mode";

interface Latencies {
    client: number;
    server: number;
    frameIndex: number;
}

export class H264Decoder extends Decoder {
    public readonly domElement: HTMLCanvasElement;

    private avc: Player;
    private currentWidth: number;
    private currentHeight: number;
    private lastReceivedSequenceNumber: number;
    private currentFrameIndex: number;
    private close: boolean = false;

    private latencies: Latencies[] = [];

    public constructor() {
        super(Codec.H264);

        this.avc = new Player({
            useWorker: false
        });
        this.domElement = this.avc.canvas;
    }

    public decodeFrame(frameData: ArrayBufferView): void {
        this.avc.decode(new Uint8Array(frameData.buffer, frameData.byteOffset));
    }

    public changeVideoMode(videoMode: IVideoMode) {
        super.changeVideoMode(videoMode);
        this.changeOptions(videoMode);
    }

    public configure(options: any) {
        this.setAvailableVideoModes(options.availableDisplayModes);
    }
}
