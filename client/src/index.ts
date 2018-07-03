// import { H264Decoder } from "./h264-decoder";
// import { WebRTCStream } from "./webrtc-stream";
// import { getQueryVariable } from "./getQueryVariable"
// import { WebStreamer } from "./webstreamer";

// const serverLocation = getQueryVariable("server");

// let webStreamer = new WebStreamer(new WebRTCStream(serverLocation),
//                                   new H264Decoder());
// webStreamer.onError = (reason: string) => {
//     if (confirm(reason + "\n\nDo you want to try to reconnect?")) {
//         location.href = "connect.html?server=" +
//                         encodeURIComponent(serverLocation);
//     } else {
//         location.href = "index.html?server=" +
//                         encodeURIComponent(serverLocation);
//     }
// }
// document.body.appendChild(webStreamer.domElement);

import { WebStreamer } from "./webstreamer";
import { H264Decoder } from "./h264-decoder";
import * as $ from 'jquery';
import "jquery-ui/ui/effect";
import "./theme/jquery-ui.css";
import "./theme/jquery-ui.theme.css";
import "./theme/jquery-ui.structure.css";
import "./style.less";

let webstreamer = null;
$(() => {
    webstreamer = new WebStreamer();
});
