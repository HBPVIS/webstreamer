// Type definitions take from http://dev.w3.org/2011/webrtc/editor/getusermedia.html

interface MediaStreamConstraints {
    audio: boolean;
    video: boolean;
}
declare var MediaStreamConstraints: {
    prototype: MediaStreamConstraints;
    new (): MediaStreamConstraints;
}

interface MediaTrackConstraints {
    mandatory: MediaTrackConstraintSet;
    optional: MediaTrackConstraint[];
}
declare var MediaTrackConstraints: {
    prototype: MediaTrackConstraints;
    new (): MediaTrackConstraints;
}

// ks - Not defined in the source doc.
interface MediaTrackConstraintSet {
}
declare var MediaTrackConstraintSet: {
    prototype: MediaTrackConstraintSet;
    new (): MediaTrackConstraintSet;
}

// ks - Not defined in the source doc.
interface MediaTrackConstraint {
}
declare var MediaTrackConstraint: {
    prototype: MediaTrackConstraint;
    new (): MediaTrackConstraints;
}

interface Navigator {
    getUserMedia(constraints: MediaStreamConstraints, successCallback: (stream: LocalMediaStream) => void , errorCallback: (error: Error) => void );
    webkitGetUserMedia(constraints: MediaStreamConstraints, successCallback: (stream: LocalMediaStream) => void , errorCallback: (error: Error) => void );
}

interface EventHandler { (event: Event): void; }

interface NavigatorUserMediaSuccessCallback { (stream: LocalMediaStream): void; }

interface NavigatorUserMediaError {
    PERMISSION_DENIED: number; // = 1;
    code: number;
}
declare var NavigatorUserMediaError: {
    prototype: NavigatorUserMediaError;
    new (): NavigatorUserMediaError;
    PERMISSION_DENIED: number; // = 1;
}

interface NavigatorUserMediaErrorCallback { (error: NavigatorUserMediaError): void; }

interface MediaStreamTrackList {
    length: number;
    item: MediaStreamTrack;
    add(track: MediaStreamTrack): void;
    remove(track: MediaStreamTrack): void;
    onaddtrack: (event: Event) => void;
    onremovetrack: (event: Event) => void;
}
declare var MediaStreamTrackList: {
    prototype: MediaStreamTrackList;
    new (): MediaStreamTrackList;
}
declare var webkitMediaStreamTrackList: {
    prototype: MediaStreamTrackList;
    new (): MediaStreamTrackList;
}

interface MediaStream {
    label: string;
    audioTracks: MediaStreamTrackList;
    videoTracks: MediaStreamTrackList;
    ended: boolean;
    onended: (event: Event) => void;
}
declare var MediaStream: {
    prototype: MediaStream;
    new (): MediaStream;
    new (trackContainers: MediaStream[]): MediaStream;
    new (trackContainers: MediaStreamTrackList[]): MediaStream;
    new (trackContainers: MediaStreamTrack[]): MediaStream;
}
declare var webkitMediaStream: {
    prototype: MediaStream;
    new (): MediaStream;
    new (trackContainers: MediaStream[]): MediaStream;
    new (trackContainers: MediaStreamTrackList[]): MediaStream;
    new (trackContainers: MediaStreamTrack[]): MediaStream;
}

interface LocalMediaStream extends MediaStream {
    stop(): void;
}


interface MediaStreamTrack {
    kind: string;
    label: string;
    enabled: boolean;
    LIVE: number; // = 0;
    MUTED: number; // = 1;
    ENDED: number; // = 2;
    readyState: number;
    onmute: (event: Event) => void;
    onunmute: (event: Event) => void;
    onended: (event: Event) => void;
}
var MediaStramTrack: {
    prototype: MediaStreamTrack;
    new (): MediaStreamTrack;
    LIVE: number; // = 0;
    MUTED: number; // = 1;
    ENDED: number; // = 2;
}

interface URL {
    createObjectURL(stream: MediaStream): string;
}
var URL: {
    prototype: MediaStreamTrack;
    new (): URL;
    createObjectURL(stream: MediaStream): string;
}

interface WebkitURL extends URL {
}
var webkitURL: {
    prototype: WebkitURL;
    new (): URL;
    createObjectURL(stream: MediaStream): string;
}

// These are TypeScript definitions to support static typing in TypeScript when interacting with WebRtc.
// Definitions taken from http://dev.w3.org/2011/webrtc/editor/webrtc.html

interface RTCConfiguration {
    iceServers: RTCIceServer[];
}
var RTCConfiguration: {
    prototype: RTCConfiguration;
    new (): RTCConfiguration;
}

interface RTCIceServer {
    url: string;
    credential?: string;
}
var RTCIceServer: {
    prototype: RTCIceServer;
    new (): RTCIceServer;
}

interface webkitRTCPeerConnection extends RTCPeerConnection {
}
var webkitRTCPeerConnection: {
    prototype: webkitRTCPeerConnection;
    new (settings: RTCPeerConnectionConfig, constraints?:MediaConstraints): webkitRTCPeerConnection;
}

interface IceState {
}
var IceState: {
    prototype: IceState;
    new (): IceState;
}

// ks 12/20/12 - There's more here that doesn't seem to be documented very well yet.
interface MediaConstraints {
    mandatory: MediaOfferConstraints;
}

interface MediaOfferConstraints {
    OfferToReceiveAudio: boolean;
    OfferToReceiveVideo: boolean;
}

interface RTCSessionDescription {
    type?: RTCSdpType;
    sdp?: string;
}
var RTCSessionDescription: {
    prototype: RTCSessionDescription;
    new (descriptionInitDict?: RTCSessionDescriptionInit): RTCSessionDescription;
}

interface RTCSessionDescriptionInit {
    type: RTCSdpType;
    sdp: string;
}
var RTCSessionDescriptionInit: {
    prototype: RTCSessionDescriptionInit;
    new (): RTCSessionDescriptionInit;
}

interface SdpType {
}

interface RTCPeerState {
}

interface RTCDataChannelInit {
    reliable: boolean;
}

enum RTCSdpType {
    offer,
    pranswer,
    answer
}

enum RTCDataChannelState {
    connecting,
    open,
    closing,
    closed
}

interface RTCDataChannel extends EventTarget {
    label: string;
    reliable: boolean;
    readyState: RTCDataChannelState;
    bufferedAmount: number;
    onopen: (event: Event)=> void;
    onerror: (event: Event)=> void;
    onclose: (event: Event)=> void;
    close(): void;
    onmessage: (event: Event)=> void;
    binaryType: string;
    send(data: string);
    send(data: ArrayBuffer);
    send(data: Blob);
}
declare var RTCDataChannel: {
    prototype: RTCDataChannel;
    new (): RTCDataChannel;
}

interface RTCDataChannelEvent extends Event {
    constructor (eventInitDict: RTCDataChannelEventInit);
    channel: RTCDataChannel;
}
var RTCDataChannelEvent: {
    prototype: RTCDataChannelEvent;
    new (eventInitDict: RTCDataChannelEventInit);
}

interface RTCIceCandidateEvent extends Event{
    candidate: RTCIceCandidate;
}

interface RTCMediaStreamEvent extends Event {
    stream: MediaStream;
}

interface EventInit {
}

interface RTCDataChannelEventInit extends EventInit {
    channel: RTCDataChannel;
}

interface RTCVoidCallback {
    (): void;
}
interface RTCSessionDescriptionCallback {
    (sdp: RTCSessionDescription): void;
}
interface RTCPeerConnectionErrorCallback {
    (errorInformation: string): void;
}

// This should be an enum
interface RTCGatheringState {
    string;
}

// This should be an enum
interface RTCIceState {
    string;
}

interface RTCPeerConnection {
    createOffer(successCallback: RTCSessionDescriptionCallback, failureCallback?: RTCPeerConnectionErrorCallback, constraints?: MediaConstraints): void;
    createAnswer(successCallback: RTCSessionDescriptionCallback, failureCallback?: RTCPeerConnectionErrorCallback, constraints?: MediaConstraints): void;
    setLocalDescription(description: RTCSessionDescription, successCallback?: RTCVoidCallback, failureCallback?: RTCPeerConnectionErrorCallback): void;
    localDescription: RTCSessionDescription;
    setRemoteDescription(description: RTCSessionDescription, successCallback?: RTCVoidCallback, failureCallback?: RTCPeerConnectionErrorCallback): void;
    remoteDescription: RTCSessionDescription;
    readyState: RTCPeerState;
    updateIce(configuration?: RTCConfiguration, constraints?: MediaConstraints): void;
    addIceCandidate(candidate: RTCIceCandidate, 
        successCallback?: RTCVoidCallback, failureCallback?: RTCPeerConnectionErrorCallback): void;
    iceGatheringState: RTCGatheringState;
    iceState: RTCIceState;
    localStreams: MediaStream[];
    remoteStreams: MediaStream[];
    createDataChannel(label?: string, dataChannelDict?: RTCDataChannelInit): RTCDataChannel;
    ondatachannel: (event: Event)=> void;
    addStream(stream: MediaStream, constraints?: MediaConstraints): void;
    removeStream(stream: MediaStream): void;
    close(): void;
    onnegotiationneeded: (event: Event)=> void;
    onconnecting: (event: Event)=> void;
    onsignalingstatechange: (event: Event)=> void;
    onopen: (event: Event)=> void;
    onaddstream: (event: RTCMediaStreamEvent)=> void;
    onremovestream: (event: RTCMediaStreamEvent)=> void;
    onstatechange: (event: Event)=> void;
    onicechange: (event: Event)=> void;
    onicecandidate: (event: RTCIceCandidateEvent)=> void;
    onidentityresult: (event: Event)=> void;
}
var RTCPeerConnection: {
    prototype: RTCPeerConnection;
    new (configuration: RTCConfiguration, constraints?: MediaConstraints): RTCPeerConnection;
}

interface RTCIceCandidate {
    candidate?: string;
    sdpMid?: string;
    sdpMLineIndex?: number;
}
var RTCIceCandidate: {
    prototype: RTCIceCandidate;
    new (candidateInitDict?: RTCIceCandidate);
}

interface RTCIceCandidateInit {
    candidate: string;
    sdpMid: string;
    sdpMLineIndex: number;
}
var RTCIceCandidateInit:{
    prototype: RTCIceCandidateInit;
    new (): RTCIceCandidateInit;
}

interface PeerConnectionIceEvent {
    peer: RTCPeerConnection;
    candidate: RTCIceCandidate;
}
var PeerConnectionIceEvent: {
    prototype: PeerConnectionIceEvent;
    new (): PeerConnectionIceEvent;
}

interface RTCPeerConnectionConfig {
    iceServers: RTCIceServer[];
}
var RTCPeerConnectionConfig: {
    prototype: RTCPeerConnectionConfig;
    new (): RTCPeerConnectionConfig;
}