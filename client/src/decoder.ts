import * as $ from 'jquery';
import { IVideoMode, getVideoModeText } from './ivideo-mode';

export enum Codec {
    Raw = "raw",
    H264 = "h264",
}

export function getCodecId(codec: Codec) {
    switch (codec) {
        case Codec.Raw: return 0;
        case Codec.H264: return 1;
    }
}

export abstract class Decoder {
    public readonly codec: Codec;
    public abstract readonly domElement: HTMLElement;


    private _options: any = {};
    public get options(): any {
        return this._options;
    }
    public onOptionsChanged: () => void;

    private _availableVideoModes: IVideoMode[] = [];
    public get availableVideoModes(): ReadonlyArray<Readonly<IVideoMode>> {
        return this._availableVideoModes;
    }
    public onAvailableVideoModesChange: () => void;

    public constructor(codec: Codec) {
        this.codec = codec;
        $(".decoder-settings").hide();
        $(".decoder-settings[data-codec='" + codec + "']").show();
    }

    public abstract configure(configuration: any);
    public abstract decodeFrame(encodedFrame: ArrayBufferView);

    public changeVideoMode(videoMode: IVideoMode) {
        $("#current-video-mode").text(getVideoModeText(videoMode));
    }

    protected changeOptions(options: any) {
        this._options = options;
        if (this.onOptionsChanged) {
            this.onOptionsChanged();
        }
    }

    protected setAvailableVideoModes(videoModes: ReadonlyArray<IVideoMode>) {
        this._availableVideoModes = [];

        for (let videoMode of videoModes) {
            const videoModeCopy = {
                width: videoMode.width,
                height: videoMode.height,
                framerate: videoMode.framerate,
            };
            this._availableVideoModes.push(videoModeCopy);

            const div = document.createElement("div");
            div.innerHTML = getVideoModeText(videoMode);
            $(div).click(() => {
                this.changeVideoMode(videoModeCopy);
            });

            const li = document.createElement("li");
            li.appendChild(div);

            $("#quality-settings-menu").append(li);
        }
        $("#quality-settings-menu").menu("refresh");
        if (this.onAvailableVideoModesChange) {
            this.onAvailableVideoModesChange();
        }
    }
}
