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

#ifndef WEBSTREAMER_INCLUDE_WEBSTREAMER_WEBRTC_STREAM_HPP_
#define WEBSTREAMER_INCLUDE_WEBSTREAMER_WEBRTC_STREAM_HPP_

#ifdef WEBSTREAMER_ENABLE_WEBRTC

#include <cstdint>
#include "webstreamer/client.hpp"
#include "webstreamer/export.hpp"
#pragma warning(push, 0)
#pragma warning(disable: 4702)
#include "Poco/Net/WebSocket.h"
#include "webrtc/api/test/fakeconstraints.h"
#include "webrtc/pc/peerconnectionfactory.h"
#pragma warning(pop)

namespace webstreamer {

class WEBSTREAMER_EXPORT WebRTCStreamClient
    : public Client,
      public webrtc::PeerConnectionObserver,
      public webrtc::CreateSessionDescriptionObserver,
      public webrtc::DataChannelObserver {
 public:
  WebRTCStreamClient(
      Poco::Net::WebSocket web_socket,
      webrtc::PeerConnectionFactoryInterface* peer_connection_factory,
      const webrtc::PeerConnectionInterface::RTCConfiguration& configuration);

  ~WebRTCStreamClient() override;

  void OnFrameEncoded(const EncodedFrame& encoded_frame) override;
  void OnCodecSwitched(Codec codec, const CodecOptions& options) override;
  void SendEvent(const Event& event) override;

  // From webrtc::PeerConnectionObserver
  void OnSignalingChange(
      webrtc::PeerConnectionInterface::SignalingState new_state) override;
  void OnAddStream(
      rtc::scoped_refptr<webrtc::MediaStreamInterface> stream) override;
  void OnRemoveStream(
      rtc::scoped_refptr<webrtc::MediaStreamInterface> stream) override;
  void OnDataChannel(
      rtc::scoped_refptr<webrtc::DataChannelInterface> data_channel) override;
  void OnRenegotiationNeeded() override;
  void OnIceConnectionChange(
      webrtc::PeerConnectionInterface::IceConnectionState new_state) override;
  void OnIceGatheringChange(
      webrtc::PeerConnectionInterface::IceGatheringState new_state) override;
  void OnIceCandidate(const webrtc::IceCandidateInterface* candidate) override;
  void OnIceCandidatesRemoved(
      const std::vector<cricket::Candidate>& candidates) override;
  void OnIceConnectionReceivingChange(bool receiving) override;
  void OnAddTrack(
      rtc::scoped_refptr<webrtc::RtpReceiverInterface> receiver,
      const std::vector<rtc::scoped_refptr<webrtc::MediaStreamInterface>>&
          streams) override;

  // From webrtc::CreateSessionDescriptionObserver
  void OnSuccess(webrtc::SessionDescriptionInterface* desc) override;
  void OnFailure(const std::string& error) override;

  // From webrtc::DataChannelObserver
  void OnStateChange() override;
  void OnMessage(const webrtc::DataBuffer& buffer) override;

  // From rtc::RefCountInterface
  // TODO(Simon): This is somewhat hacky? check with the documentation of
  // rtc::RefCountInterface and change the implementation if it violates
  // documentation.
  int AddRef() const override { return 1; }
  int Release() const override { return 0; }

 private:
  rtc::scoped_refptr<webrtc::PeerConnectionInterface> peer_connection_;
  rtc::scoped_refptr<webrtc::DataChannelInterface> event_channel_;
  rtc::scoped_refptr<webrtc::DataChannelInterface> video_channel_;
  Poco::Net::WebSocket web_socket_;
  webrtc::FakeConstraints constraints_;
  std::thread receive_thread_;

  rtc::CopyOnWriteBuffer send_buffer_;
  std::uint32_t message_counter_ = 0;

  void ReceiveThread();
  void HandleMessage(const std::string& message,
                     const Poco::JSON::Object::Ptr& message_data);
};

}  // namespace webstreamer

#endif

#endif  // WEBSTREAMER_INCLUDE_WEBSTREAMER_WEBRTC_STREAM_HPP_
