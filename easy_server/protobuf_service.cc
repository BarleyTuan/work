#include "protobuf_service.h"


void ProtobufService::StartService(string host, int port) {
  struct sockaddr_in servaddr;
  accept_fd_ = socket(AF_INET, SOCK_STREAM, 0);
  poll_->AddDescriptor(this, accept_fd_, EPOLLIN|EPOLLERR);
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = INADDR_ANY;
  servaddr.sin_port = htons(port);
  if (bind(accept_fd_, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
    std::cout << "bind error" << std::endl;
    return;
  }
  if (listen(accept_fd_, poll_->GetEventCount()) < 0) {
    std::cout << "listen error" << std::endl;
  }
}

void ProtobufService::Register(google::protobuf::Service* service) {
  const google::protobuf::ServiceDescriptor* service_descriptor = 
       service->GetDescriptor();
  for (int i = 0; i< service_descriptor->method_count(); ++i) {
    const google::protobuf::MethodDescriptor* method_descriptor = 
        service_descriptor->method(i);
    int method_id = hash_string(method_descriptor->full_name());
    MethodInfo* method_info(new MethodInfo(service, method_descriptor));
    method_map_[method_id] = method_info;
  }
}

void ProtobufService::RunEvents(int fd) {
  std::cout << "fd:" << fd << "  accept_fd_" << accept_fd_ << std::endl;
  if (fd == accept_fd_) {
    HandleAccept();
  } else {
    ProtobufChannel *channel = channel_map_.find(fd)->second;
    channel->HandleRead();
  }
}
void ProtobufService::HandleAccept() {
  struct sockaddr_in remote;
  uint32_t addr_len = sizeof(remote);
  int new_sock = accept(accept_fd_, (struct sockaddr *) &remote, &addr_len);
  if (new_sock < 0) {
    std::cout << "accept sock error" << std::endl;
    return;
  }
  std::cout << "connection from host " << inet_ntoa(remote.sin_addr)
      << ", port " << ntohs(remote.sin_port)
      << ", socket " << new_sock << std::endl; 
  ProtobufChannel* channel = new ProtobufChannel(this, new_sock);
}

void ProtobufService::RunService(Packet packet, int channel_guid) {
  MethodInfo* method_info = method_map_[packet.method_id];
  google::protobuf::Message* request = method_info->GetRequestPrototype().New();
  google::protobuf::Message* response = method_info->GetResponsePrototype().New();
  request->ParseFromString(packet.content);
  ResponseContext* context = new ResponseContext(channel_guid, packet.call_guid, response);
  google::protobuf::Closure* done = google::protobuf::NewPermanentCallback(
      this, &ProtobufService::ResponseHandler, context);

  method_info->GetService()->CallMethod(
      &(method_info->GetMethodDescriptor()), 
      reinterpret_cast<google::protobuf::RpcController*>(NULL),
      request, response, 
      //reinterpret_cast<google::protobuf::Closure*>(NULL));
      done);
  delete request;
}

void ProtobufService::ResponseHandler(ResponseContext *context) {
  Packet response_packet = Packet(context->call_guid, NULL, context->response);
  ProtobufChannel* channel = channel_map_[context->channel_guid];
  channel->HandleWrite(response_packet);
  delete context->response;
}
