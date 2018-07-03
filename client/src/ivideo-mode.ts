export interface IVideoMode {
    width: number;
    height: number;
    framerate: number;
}

export function getVideoModeText(displayMode: IVideoMode) {
    return displayMode.height.toFixed(0) + "p" + displayMode.framerate.toFixed(0);
}