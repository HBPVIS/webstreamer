//------------------------------------------------------------------------------
// Web Streamer
//
// Copyright (c) 2017 RWTH Aachen University, Germany,
// Virtual Reality & Immersive Visualization Group.
//------------------------------------------------------------------------------
//                                 License
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//------------------------------------------------------------------------------

#ifdef WEBSTREAMER_ENABLE_WEBRTC

#include "webstreamer/webrtc_stream_client.hpp"
#include <functional>
#include "log.hpp"
#include "webstreamer/encoder.hpp"
#include "webstreamer/suppress_warnings.hpp"
SUPPRESS_WARNINGS_BEGIN
#include "Poco/JSON/Object.h"
#include "Poco/JSON/Parser.h"
#include "Poco/Net/NetException.h"
#include "webrtc/api/jsep.h"
SUPPRESS_WARNINGS_END

namespace webstreamer {

namespace {
class SetSessionDescriptionObserver
    : public webrtc::SetSessionDescriptionObserver {
 public:
  template <typename T, typename U>
  SetSessionDescriptionObserver(T&& success_callback, U&& failure_callback)
      : success_callback_(success_callback),
        failure_callback_(failure_callback) {}

  void OnSuccess() override { success_callback_(); }

  void OnFailure(const std::string& error) override {
    failure_callback_(error);
  }

  int AddRef() const override { return 1; }
  int Release() const override {
    delete this;
    return 0;
  }

 private:
  std::function<void()> success_callback_;
  std::function<void(const std::string&)> failure_callback_;

  ~SetSessionDescriptionObserver() = default;
};

const std::uint32_t MAX_MESSAGE_SIZE = 16 * 1000;
const std::uint32_t MESSAGE_HEADER_SIZE = 12;
const std::uint32_t MAX_MESSAGE_CONTENT_SIZE =
    MAX_MESSAGE_SIZE - MESSAGE_HEADER_SIZE;

}  // namespace

WebRTCStreamClient::WebRTCStreamClient(
    Poco::Net::WebSocket web_socket,
    webrtc::PeerConnectionFactoryInterface* peer_connection_factory,
    const webrtc::PeerConnectionInterface::RTCConfiguration& configuration)
    : peer_connection_(peer_connection_factory->CreatePeerConnection(
          configuration, nullptr, nullptr, this)),
      web_socket_(std::move(web_socket)),
      receive_thread_(&WebRTCStreamClient::ReceiveThread, this),
      send_buffer_(MAX_MESSAGE_SIZE) {
  webrtc::DataChannelInit event_channel_config;
  event_channel_ =
      peer_connection_->CreateDataChannel("event", &event_channel_config);
  event_channel_->RegisterObserver(this);

  webrtc::DataChannelInit video_channel_config;
  video_channel_config.reliable = false;
  video_channel_config.maxRetransmits = 1;
  video_channel_config.ordered = false;
  video_channel_ =
      peer_connection_->CreateDataChannel("video", &video_channel_config);
  video_channel_->RegisterObserver(this);

  constraints_.AddMandatory("offerToReceiveAudio", true);
  peer_connection_->CreateOffer(this, &constraints_);
}

WebRTCStreamClient::~WebRTCStreamClient() {
  event_channel_->UnregisterObserver();
  video_channel_->UnregisterObserver();
  web_socket_.close();
  Die();
  receive_thread_.join();
  LOGI("WebRTCStreamClient disconnected");
}

void WebRTCStreamClient::OnFrameEncoded(const EncodedFrame& encoded_frame) {
  LOGV("Try to send ", encoded_frame.size_in_bytes, " bytes");
  if (video_channel_->state() == webrtc::DataChannelInterface::kOpen) {
    const std::uint32_t frame_size =
        static_cast<std::uint32_t>(encoded_frame.size_in_bytes);
    assert(frame_size == encoded_frame.size_in_bytes);
    std::uint32_t bytes_sent = 0;

    while (bytes_sent < frame_size) {
      send_buffer_.Clear();

      send_buffer_.AppendData(
          reinterpret_cast<const std::uint8_t*>(&message_counter_),
          sizeof(message_counter_));
      send_buffer_.AppendData(
          reinterpret_cast<const std::uint8_t*>(&frame_size),
          sizeof(frame_size));
      send_buffer_.AppendData(
          reinterpret_cast<const std::uint8_t*>(&bytes_sent),
          sizeof(bytes_sent));
      assert(send_buffer_.size() == MESSAGE_HEADER_SIZE);

      std::uint32_t fragment_size =
          std::min(frame_size - bytes_sent, MAX_MESSAGE_CONTENT_SIZE);
      send_buffer_.AppendData(encoded_frame.data + bytes_sent, fragment_size);
      video_channel_->Send({send_buffer_, true});
      LOGV("Sent ", send_buffer_.size(), " bytes");
      LOGV("Buffered data in video channel: ",
           video_channel_->buffered_amount());

      bytes_sent += fragment_size;
    }
    ++message_counter_;
  } else {
    LOGE("Failed to send video data: video data channel is not open");
  }
}

void WebRTCStreamClient::OnCodecSwitched(Codec codec,
                                         const CodecOptions& options) {
  (void)codec;
  (void)options;
  LOGI("WebRTCStreamClient::OnCodecSwitched");
}

void WebRTCStreamClient::SendEvent(const Event& event) {
  const auto buffer = event.Serialize();
  rtc::CopyOnWriteBuffer data;
  data.SetData(buffer.data(), buffer.size());
  event_channel_->Send({data, true});
}

void WebRTCStreamClient::OnSignalingChange(
    webrtc::PeerConnectionInterface::SignalingState new_state) {
  (void)new_state;
  LOGI("WebRTCStreamClient::OnSignalingChange");
}

void WebRTCStreamClient::OnAddStream(
    rtc::scoped_refptr<webrtc::MediaStreamInterface> stream) {
  (void)stream;
  LOGI("WebRTCStreamClient::OnAddStream");
}

void WebRTCStreamClient::OnRemoveStream(
    rtc::scoped_refptr<webrtc::MediaStreamInterface> stream) {
  (void)stream;
  LOGI("WebRTCStreamClient::OnRemoveStream");
}

void WebRTCStreamClient::OnDataChannel(
    rtc::scoped_refptr<webrtc::DataChannelInterface> data_channel) {
  (void)data_channel;
  LOGI("WebRTCStreamClient::OnDataChannel");
}

void WebRTCStreamClient::OnRenegotiationNeeded() {
  LOGI("WebRTCStreamClient::OnRenegotiationNeeded");
}

void WebRTCStreamClient::OnIceConnectionChange(
    webrtc::PeerConnectionInterface::IceConnectionState new_state) {
  (void)new_state;
  LOGI("WebRTCStreamClient::OnIceConnectionChange");
}

void WebRTCStreamClient::OnIceGatheringChange(
    webrtc::PeerConnectionInterface::IceGatheringState new_state) {
  (void)new_state;
  LOGI("WebRTCStreamClient::OnIceGatheringChange");
}

void WebRTCStreamClient::OnIceCandidate(
    const webrtc::IceCandidateInterface* candidate) {
  std::string candidate_sdp;
  if (candidate->ToString(&candidate_sdp)) {
    LOGI("New ICE candidate: ", candidate_sdp);

    Poco::JSON::Object candidate_object;
    candidate_object.set("candidate", candidate_sdp);
    candidate_object.set("sdpMid", candidate->sdp_mid());
    candidate_object.set("sdpMLineIndex", candidate->sdp_mline_index());

    std::stringstream candidate_stream;
    candidate_object.stringify(candidate_stream);
    const std::string candidate_string = candidate_stream.str();
    web_socket_.sendFrame(candidate_string.c_str(),
                          static_cast<int>(candidate_string.length()));
  } else {
    LOGE("Failed to serialize ICE candidate");
  }
}

void WebRTCStreamClient::OnIceCandidatesRemoved(
    const std::vector<cricket::Candidate>& candidates) {
  (void)candidates;
  LOGI("WebRTCStreamClient::OnIceCandidatesRemoved");
}

void WebRTCStreamClient::OnIceConnectionReceivingChange(bool receiving) {
  LOGI("WebRTCStreamClient::OnIceConnectionReceivingChange: ", receiving);
}

void WebRTCStreamClient::OnAddTrack(
    rtc::scoped_refptr<webrtc::RtpReceiverInterface> receiver,
    const std::vector<rtc::scoped_refptr<webrtc::MediaStreamInterface>>&
        streams) {
  (void)receiver;
  (void)streams;
  LOGI("WebRTCStreamClient::OnAddTrack");
}

void WebRTCStreamClient::OnSuccess(webrtc::SessionDescriptionInterface* desc) {
  std::string sdp;
  desc->ToString(&sdp);
  LOGI("Local session description: ", desc->type(), "\n", sdp);
  peer_connection_->SetLocalDescription(
      new SetSessionDescriptionObserver(
          []() { LOGI("SetLocalDescription(): success"); },
          [](const std::string& error_message) {
            LOGE("SetLocalDescription(): failure (", error_message, ")");
          }),
      desc);

  Poco::JSON::Object offer;
  offer.set("type", desc->type());
  offer.set("sdp", sdp);

  std::stringstream offer_stream;
  offer.stringify(offer_stream);
  const std::string offer_string = offer_stream.str();
  web_socket_.sendFrame(offer_string.c_str(),
                        static_cast<int>(offer_string.length()));
}

void WebRTCStreamClient::OnFailure(const std::string& error) {
  (void)error;
  LOGI("WebRTCStreamClient::OnFailure");
}

void WebRTCStreamClient::OnStateChange() {
  auto state_to_string = [](webrtc::DataChannelInterface::DataState state) {
    switch (state) {
      case webrtc::DataChannelInterface::kConnecting:
        return "connecting";
      case webrtc::DataChannelInterface::kOpen:
        return "open";
      case webrtc::DataChannelInterface::kClosing:
        return "closing";
      case webrtc::DataChannelInterface::kClosed:
        return "closed";
      default:
        return "invalid state";
    }
  };

  LOGI("WebRTCStreamClient::OnStateChange");
  LOGD("Event channel state: ", state_to_string(event_channel_->state()));
  LOGD("Video channel state: ", state_to_string(video_channel_->state()))

  if (event_channel_->state() == webrtc::DataChannelInterface::kClosing ||
      event_channel_->state() == webrtc::DataChannelInterface::kClosed ||
      video_channel_->state() == webrtc::DataChannelInterface::kClosing ||
      video_channel_->state() == webrtc::DataChannelInterface::kClosed) {
    // DebugBreak();
    Die();
  }
}

void WebRTCStreamClient::OnMessage(const webrtc::DataBuffer& buffer) {
  AddEvent(DeserializeEvent(buffer.data.cdata<char>(), buffer.data.size()));
}

void WebRTCStreamClient::ReceiveThread() {
  Poco::Buffer<char> buffer(0);
  int flags;
  Poco::JSON::Parser json_parser;
  std::string buffer_string;

  while (is_alive()) {
    try {
      buffer.resize(0);
      int bytes_received = web_socket_.receiveFrame(buffer, flags);
      if (bytes_received == 0) {
        Die();
      } else {
        if (flags == Poco::Net::WebSocket::FRAME_TEXT) {
          buffer_string.clear();
          buffer_string.insert(buffer_string.begin(), buffer.begin(),
                               buffer.end());
          try {
            auto var = json_parser.parse(buffer_string);
            HandleMessage(buffer_string,
                          var.extract<Poco::JSON::Object::Ptr>());
          } catch (const Poco::Exception& exception) {
            LOGE("Failed to parse message: ", exception.message());
            LOGE(buffer_string);
          }
        } else if (flags & Poco::Net::WebSocket::FRAME_OP_CLOSE) {
          Die();
        } else {
          LOGE("Invalid frame flags");
        }
      }
    } catch (const Poco::Net::NetException&) {
      Die();
    } catch (const Poco::TimeoutException&) {
    }
  }
}

void WebRTCStreamClient::HandleMessage(
    const std::string& message, const Poco::JSON::Object::Ptr& message_data) {
  if (message_data->has("type") && message_data->has("sdp")) {
    LOGI("Received remote session description: ", message);
    try {
      const std::string type = message_data->getValue<std::string>("type");
      const std::string sdp = message_data->getValue<std::string>("sdp");

      webrtc::SdpParseError error;
      std::unique_ptr<webrtc::SessionDescriptionInterface> remote_description(
          webrtc::CreateSessionDescription(type, sdp, &error));
      if (remote_description == nullptr) {
        LOGE("Failed to parse remote SDP. Error on line ", error.line, ": ",
             error.description);
        LOGE(sdp);
      } else {
        peer_connection_->SetRemoteDescription(
            new SetSessionDescriptionObserver(
                []() { LOGI("SetRemoteDescription(): success"); },
                [](const std::string& error_message) {
                  LOGE("SetRemoteDescription(): failure (", error_message, ")");
                }),
            remote_description.get());
        remote_description.release();
      }
    } catch (const Poco::BadCastException& exception) {
      LOGE("Failed to parse remote session description: ", exception.message());
    }
  } else if (message_data->has("candidate") && message_data->has("sdpMid") &&
             message_data->has("sdpMLineIndex")) {
    LOGI("Received candiate: ", message);
    try {
      const std::string sdp = message_data->getValue<std::string>("candidate");
      const std::string sdp_mid = message_data->getValue<std::string>("sdpMid");
      const int sdp_mline_index = message_data->getValue<int>("sdpMLineIndex");

      webrtc::SdpParseError error;
      std::unique_ptr<webrtc::IceCandidateInterface> candidate(
          webrtc::CreateIceCandidate(sdp_mid, sdp_mline_index, sdp, &error));

      if (candidate == nullptr) {
        LOGE("Failed to parse remote candidate. Error on line ", error.line,
             ": ", error.description);
        LOGE(sdp);
      } else {
        LOGI("Candidate added");
        peer_connection_->AddIceCandidate(candidate.get());
      }
    } catch (const Poco::BadCastException& exception) {
      LOGE("Failed to parse remote session description: ", exception.message());
    }
  }
}

}  // namespace webstreamer

#endif
