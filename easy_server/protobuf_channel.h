/*
一个channel是一个连接, 负责read, write
EchoService_stub 需要对应的channel
*/
#ifndef PLATFORM_RPC_PROTOBUF_CHANNEL_H_
#define PLATFORM_RPC_PROTOBUF_CHANNEL_H_

#include <string>
#include <stdlib.h>
#include <iostream>
#include <tr1/unordered_map>
#include "easy_epoll.h"
#include "echo.pb.h"
#include "packet.h"
#include "google/protobuf/descriptor.h"
#include "google/protobuf/message.h"
#include "google/protobuf/service.h"
#include "protobuf_service.h"
#include "rpc_utility.h"

class ProtobufService;
class PacketParser;
class ResponseContext;
class MethodInfo;

class ProtobufChannel: public google::protobuf::RpcChannel::RpcChannel {
public:
  ProtobufChannel(ProtobufService* service, int sock_fd);
  ProtobufChannel(ProtobufService* service, std::string host, int port);
  virtual ~ProtobufChannel();

  void HandleRead();

  void HandleWrite(Packet packet) {
    std::cout << "============== packet " << packet.method_id << std::endl;
    std::string msg = packet_parser_->EncodePacketToStr(packet);
    int flag = write(sock_fd_, msg.c_str(), msg.length());
    std::cout << "write ok " << flag << " write len " << msg.length() << std::endl;
  }
  
  void CallMethod(const google::protobuf::MethodDescriptor * method, 
      google::protobuf::RpcController *controller,
      const google::protobuf::Message *request,
      google::protobuf::Message *response,
      google::protobuf::Closure *done) {
    int call_guid = get_new_guid();
    Packet packet = Packet(call_guid, method, request);
    RequestContext* request_context = new RequestContext(response, done);
    request_context_map_[call_guid] = request_context;
    std::cout << "============== callmethod " << call_guid << std::endl;
    HandleWrite(packet);
  }
private:
  ProtobufService* service_;
  int sock_fd_;
  char buf[BUFFER_SIZE];
  PacketParser* packet_parser_;
  std::tr1::unordered_map<int, RequestContext*> request_context_map_;
};


#endif // PLATFORM_RPC_PROTOBUF_CHANNEL_H_