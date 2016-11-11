/*
一个TAccepter和一个change_dict:key: TSocket, 目前就对是int
*/
#ifndef PLATFORM_RPC_PROTOBUF_SERVICE_H_
#define PLATFORM_RPC_PROTOBUF_SERVICE_H_

#include <map>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <sys/time.h>
#include "protobuf_channel.h"
#include "easy_epoll.h"
#include "echo.pb.h"
#include "google/protobuf/descriptor.h"
#include "google/protobuf/message.h"
#include "google/protobuf/service.h"
#include "protobuf_channel.h"
#include "rpc_utility.h"

class EasyEpoll;
class ProtobufChannel;
class PacketParser;
class Packet;
class ResponseContext;
class MethodInfo;

class ProtobufService {
public:
  ProtobufService(EasyEpoll* poll) {
    poll_ = poll;
    accept_fd_ = 0;
  }
  ~ProtobufService() {
  }

  void StartService(std::string host, int port);
  void Register(google::protobuf::Service* service);
  void RunEvents(int fd);
  void HandleAccept();
  void AddChannel(ProtobufChannel *channel, int fd) {
    channel_map_[fd] = channel;
  }
  void RemoveChannel(int fd) {
    channel_map_.erase(fd);
  }
  void RunService(Packet packet, int channel_guid);
  void ResponseHandler(ResponseContext *context);
  EasyEpoll* GetEpoll() {
    return poll_;
  }
private:
  int accept_fd_;
  EasyEpoll* poll_;
  // just for test, so fd as its key
  std::tr1::unordered_map<int, ProtobufChannel *> channel_map_;
  std::tr1::unordered_map<int, MethodInfo*> method_map_;
};

#endif // PLATFORM_RPC_PROTOBUF_SERVICE_H_
