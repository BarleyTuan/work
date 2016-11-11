#include "protobuf_channel.h"

ProtobufChannel::ProtobufChannel(ProtobufService* service, int sock_fd)
    :service_(service), sock_fd_(sock_fd) {
  packet_parser_ = new PacketParser();
  service->GetEpoll()->AddDescriptor(service, sock_fd_, EPOLLIN|EPOLLERR);
  service->AddChannel(this, sock_fd_);
}

ProtobufChannel::ProtobufChannel(ProtobufService* service, std::string host, int port)
    :service_(service) {
  packet_parser_ = new PacketParser();
  struct sockaddr_in servaddr;
  memset(&servaddr, 0, sizeof(servaddr));
  inet_pton(AF_INET, host.c_str(), (void *)&(servaddr.sin_addr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(port);
  sock_fd_ = socket(AF_INET, SOCK_STREAM, 0);
  if (connect(sock_fd_, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
    std::cout << sock_fd_ << "  sock connect error" << std::endl;
    return;
  } 
  std::cout << "new protobuf fd:" << sock_fd_ << std::endl;
  service->GetEpoll()->AddDescriptor(service, sock_fd_, EPOLLIN|EPOLLERR);
  service->AddChannel(this, sock_fd_);
}

ProtobufChannel::~ProtobufChannel() {
    service_->GetEpoll()->DeleteDescriptor(sock_fd_);
    service_->RemoveChannel(sock_fd_);
    memset(buf, 0, sizeof(buf));
    service_ = NULL;
    delete packet_parser_;
  }

void ProtobufChannel::HandleRead() {
  int bytesread = read(sock_fd_, buf, sizeof(buf) - 1);
  if (bytesread <= 0) {
    std::cout << "sock fd " << sock_fd_ << "  read data error " << std::endl;
    service_->GetEpoll()->DeleteDescriptor(sock_fd_);
    return;
  }
  buf[bytesread] = 0;
  std::cout << "recv_from " << sock_fd_ << "  : " <<  bytesread << std::endl;
  Packet packet = packet_parser_->DecodePacketFromStr(buf);
  if (packet.method_id  != 0) {
    service_->RunService(packet, sock_fd_);
    return;
  }
  int call_guid = packet.call_guid;
  RequestContext* context = request_context_map_[call_guid];
  context->response->ParseFromString(packet.content);
  context->done->Run();
  request_context_map_.erase(call_guid);
}

