#include "webrtc/base/ssladapter.h"
#include "webrtc/base/win32socketinit.h"
#include "webrtc/api/peerconnectioninterface.h"
#include "webrtc/api/test/fakeconstraints.h"

int main() {
  rtc::WinsockInitializer winsock_init;
  rtc::InitializeSSL();
  // 创建PeerConnectionFactory
  rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface> peer_connection_factory =
      webrtc::CreatePeerConnectionFactory();

  // 配置ice服务器
  webrtc::PeerConnectionInterface::IceServer ice_server;
  ice_server.uri = "stun:stun.l.google.com:19302";
  std::vector<webrtc::PeerConnectionInterface::IceServer> ice_servers;
  ice_servers.push_back(ice_server);

  // 配置SDP约束
  webrtc::FakeConstraints constraints;
  constraints.AddOptional(webrtc::MediaConstraintsInterface::kEnableDtlsSrtp, "true");
  constraints.AddOptional(webrtc::MediaConstraintsInterface::kOfferToReceiveAudio, "true");
  constraints.AddOptional(webrtc::MediaConstraintsInterface::kOfferToReceiveVideo, "true");

  // 创建PeerConnection
  rtc::scoped_refptr<webrtc::PeerConnectionInterface> peer_connection =
      peer_connection_factory->CreatePeerConnection(ice_servers, &constraints, nullptr, nullptr, nullptr);

  // 创建DataChannel
  webrtc::DataChannelInit data_channel_config;
  rtc::scoped_refptr<webrtc::DataChannelInterface> data_channel =
      peer_connection->CreateDataChannel("data_channel", &data_channel_config);

  // 设置本地SDP
  webrtc::SdpParseError error;
  std::unique_ptr<webrtc::SessionDescriptionInterface> session_description =
      peer_connection->CreateOffer(&constraints, &error);
  peer_connection->SetLocalDescription(DummySetSessionDescriptionObserver::Create(), session_description.get());


    // 建立 WebSocket 连接
    auto ws = std::make_shared<WebSocket>("wss://your.websocket.server.com");

    // 发送本地 SDP 到远程端
    std::unique_ptr<webrtc::SessionDescriptionInterface> local_sdp;
    peer_connection->CreateOffer(&constraints, &error);
    peer_connection->SetLocalDescription(DummySetSessionDescriptionObserver::Create(), local_sdp.get());
    std::string local_sdp_str;
    local_sdp->ToString(&local_sdp_str);
    ws->send(local_sdp_str);

    // 接收远程 SDP 并设置到 PeerConnection
    ws->setOnMessageCallback([&](const std::string& message) {
      webrtc::SdpParseError error;
      std::unique_ptr<webrtc::SessionDescriptionInterface> remote_sdp = webrtc::CreateSessionDescription("offer", message, &error);
      peer_connection->SetRemoteDescription(DummySetSessionDescriptionObserver::Create(), remote_sdp.get());
    });

    // 等待ICE建立连接
    // ...
    
    // 通话过程中发送音视频数据
    // 创建并发送音视频数据
    // ...

  // 关闭连接
  peer_connection->Close();

  rtc::CleanupSSL();
  return 0;
}
