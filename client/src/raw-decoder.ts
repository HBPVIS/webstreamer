import { Decoder, Codec } from "./decoder";

import { Player } from "broadway-player";
import { IVideoMode } from "./ivideo-mode";

interface Latencies {
    client: number;
    server: number;
    frameIndex: number;
}

export class RawDecoder extends Decoder {
    public readonly domElement: HTMLCanvasElement;

    private context: CanvasRenderingContext2D;
    private buffer: ImageData;

    public constructor() {
        super(Codec.Raw);

        this.domElement = document.createElement("canvas");
        this.context = this.domElement.getContext("2d");

        this.setAvailableVideoModes([
            {
                width: 1280,
                height: 720,
                framerate: 30
            }
        ]);
    }

    public configure(configuration: any) {
        throw new Error("Method not implemented.");
    }

    public decodeFrame(frameData: ArrayBufferView): void {
        if (!this.buffer || this.buffer.width * this.buffer.height * 3 !== frameData.byteLength) {
            console.info("Invalid byte length of encoded frame.");
            return;
        }

        const width = this.buffer.width;
        const height = this.buffer.height;
        const sourceData = new Uint8Array(frameData.buffer, frameData.byteOffset);

        for (let y = 0; y < height; ++y) {
            for (let x = 0; x < width; ++x) {
                for (let c = 0; c < 3; ++c) {
                    this.buffer.data[(y * width + x) * 4 + c] = sourceData[(y * width + x) * 3 + c];
                }
                this.buffer.data[(y * width + x) * 4 + 3] = 255;
            }
        }
        this.context.putImageData(this.buffer, 0, 0);
    }

    public changeVideoMode(videoMode: IVideoMode) {
        super.changeVideoMode(videoMode);
        if (!this.buffer || this.buffer.width != videoMode.width || this.buffer.height != videoMode.height) {
            this.buffer = this.context.createImageData(videoMode.width, videoMode.height);
            this.domElement.width = videoMode.width;
            this.domElement.height = videoMode.height;
            console.log("Switched to " + videoMode.width + "x" + videoMode.height);

            this.changeVideoMode(videoMode);
        }
    }
}
