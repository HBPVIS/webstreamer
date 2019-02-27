import * as $ from 'jquery';
import { Stream, StreamType } from "./stream"
import { Decoder, Codec } from "./decoder";
import { StatGraph } from "./stat-graph";
import { parseMouseEvent, parseKeyEvent } from "./input";
import { deserializeEvent, EventType, serializeEvent } from "./events";
import { H264Decoder } from "./h264-decoder";
import { WebSocketStream } from "./websocket-stream";
import { RawDecoder } from "./raw-decoder";
import { AJAXGetTextRequest } from "./ajax";
import { JSONGetValue } from "./json";
import { InputBar } from "./input-bar";
import { AdvancedSettingsOverlay } from "./advanced-settings-overlay";
import { IVideoMode } from "./ivideo-mode";
import { WebRTCStream } from './webrtc-stream';
import { InputCapturer } from './input-capturer';
import { Clipboard } from './clipboard';

export class WebStreamer {
    private streamConfig: any;
    private decoder: Decoder;
    private stream: Stream;
    private inputBar: InputBar;
    private advancedSettingsOverlay: AdvancedSettingsOverlay;
    private isStopped = false;
    private currentVideoMode: IVideoMode = null;
    private inputCapturer: InputCapturer = new InputCapturer();

    constructor() {
        AJAXGetTextRequest("stream_config.json",
            (response) => {
                this.streamConfig = JSON.parse(response);
                this.selectStreamAndCodec();
            }, (status, statusText) => {
                alert("Failed to connect to the server, error code: " + status);
            });

        this.inputBar = new InputBar();
        this.inputBar.onStop = () => {
            this.isStopped = true;
            if (this.stream) {
                this.stream.sendEvent({ type: EventType.Stop });
            }
        };
        this.inputBar.onPlay = () => {
            this.isStopped = false;
            if (this.stream) {
                this.stream.sendEvent({ type: EventType.Play });
            }
        };
        this.inputBar.onAquireInput = () => {
            if (this.stream) {
                this.stream.sendEvent({
                    type: EventType.AquireInput
                });
            }
        };
        this.inputBar.onReleaseInput = () => {
            if (this.stream) {
                this.stream.sendEvent({
                    type: EventType.ReleaseInput
                });
            }
        };

        this.advancedSettingsOverlay = new AdvancedSettingsOverlay();
        $(window).resize(() => {
            if (this.decoder) {
                this.chooseVideoSize();
                this.chooseVideoMode();
            }
        });
    }

    public isStreamTypeSupported(streamType: StreamType): boolean {
        return JSONGetValue(this.streamConfig, "streams." + streamType + ".supported") === true
    }

    public selectStream(streamType: StreamType) {
        const options = JSONGetValue(this.streamConfig, "streams." + streamType);

        if (this.stream && this.stream.type !== streamType) {
            this.stream.onOpen = undefined;
            this.stream.onReceiveEncodedFrame = undefined;
            this.stream = undefined;
        }
        if (!this.stream) {
            switch (streamType) {
                case StreamType.WebRTC:
                    this.stream = new WebRTCStream();
                    break;

                case StreamType.WebSocket:
                    this.stream = new WebSocketStream();
                    break;

                default:
                    throw new Error("Failed to select the stream");
            }
            this.stream.onReceiveEncodedFrame = this.onVideoData.bind(this);
            this.stream.onOpen = () => {
                $("#input-button").prop("checked", false);
                if (this.decoder) {
                    this.stream.sendEvent({
                        type: EventType.ChangeCodec,
                        codec: this.decoder.codec,
                        options: this.decoder.options
                    });
                }
            };
        }
        this.stream.configure(options);

        this.inputCapturer.stream = this.stream;
				Clipboard.STREAM = this.stream;
    }

    public isCodecSupported(codec: Codec): boolean {
        return JSONGetValue(this.streamConfig, "codecs." + codec + ".supported") === true;
    }

    public selectCodec(codec: Codec) {
        const options = JSONGetValue(this.streamConfig, "codecs." + codec);

        if (this.decoder && this.decoder.codec !== codec) {
            this.decoder.onOptionsChanged = undefined;
            this.decoder.onAvailableVideoModesChange = undefined;
            $(this.decoder.domElement).remove();
            this.decoder = undefined;
        }
        if (!this.decoder) {
            switch (codec) {
                case Codec.Raw:
                    this.decoder = new RawDecoder();
                    break;

                case Codec.H264:
                    this.decoder = new H264Decoder();
                    break;

                default:
                    alert('Failed to select the codec');
            }
        }

        if (this.decoder) {
            this.decoder.onAvailableVideoModesChange = () => this.chooseVideoMode();
            this.decoder.onOptionsChanged = () => {
                if (this.stream && this.stream.isConnected) {
                    this.stream.sendEvent({
                        type: EventType.ChangeCodec,
                        codec: this.decoder.codec,
                        options: this.decoder.options
                    });
                }
            }
            this.chooseVideoSize();
            this.chooseVideoMode();
            $("#webstream").append(this.decoder.domElement);
        }

        this.decoder.configure(options);
        this.inputCapturer.domElement = this.decoder.domElement;
				Clipboard.setup(this.decoder.domElement);
    }

    private selectStreamAndCodec() {
		
      if (this.stream && this.isStreamTypeSupported(this.stream.type)) {
          this.selectStream(this.stream.type);
      } else {
          if (this.isStreamTypeSupported(StreamType.WebRTC)) {
              this.selectStream(StreamType.WebRTC);
          } else if (this.isStreamTypeSupported(StreamType.WebSocket)) {
              this.selectStream(StreamType.WebSocket);
          } else {
              throw new Error("Failed to find a supported stream type");
          }
      }

      if (this.isCodecSupported(Codec.H264)) {
          this.selectCodec(Codec.H264);
      } else if (this.isCodecSupported(Codec.Raw)) {
          this.selectCodec(Codec.Raw);
      } else {
          alert("Failed to find a supported codec");
      }
			
			//this.selectStream(StreamType.WebRTC);
    }

    private onVideoData(encodedFrame: ArrayBufferView) {
        if (!this.isStopped && this.decoder) {
            this.decoder.decodeFrame(encodedFrame);
        }
    }

    private chooseVideoSize() {
        interface Size {
            width: number;
            height: number;
        };

        const sizes: Size[] = [
            {
                width: 3840,
                height: 2160,
            },
            {
                width: 2560,
                height: 1440,
            },
            {
                width: 1920,
                height: 1080,
            },
            {
                width: 1280,
                height: 720,
            },
            {
                width: 854,
                height: 480,
            },
            {
                width: 640,
                height: 360,
            },
            {
                width: 426,
                height: 240,
            },
        ];

        for (let size of sizes) {
            if (size.width < window.innerWidth) {
                $(this.decoder.domElement).css("width", size.width + "px");
                $(this.decoder.domElement).css("height", size.height + "px");
                break;
            }
        }
    }

    private chooseVideoMode() {
        const windowWidth = window.innerWidth;
        let bestVideoMode: IVideoMode = null;

        for (let videoMode of this.decoder.availableVideoModes) {
            if (!bestVideoMode) {
                bestVideoMode = videoMode;
            } else {
                if (bestVideoMode.width > windowWidth) {
                    if (videoMode.width < bestVideoMode.width) {
                        bestVideoMode = videoMode;
                    }
                } else {
                    if (videoMode.width > bestVideoMode.width &&
                        videoMode.width <= windowWidth) {
                        bestVideoMode = videoMode;
                    }
                }
            }
        }

        if (bestVideoMode && (!this.currentVideoMode ||
            bestVideoMode.width != this.currentVideoMode.width ||
            bestVideoMode.height != this.currentVideoMode.height ||
            bestVideoMode.framerate != this.currentVideoMode.framerate)) {
            this.currentVideoMode = bestVideoMode;
            this.decoder.changeVideoMode(bestVideoMode);
        }
    }
}
