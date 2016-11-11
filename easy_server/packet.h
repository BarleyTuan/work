#ifndef PACKET_H_
#define PACKET_H_

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "google/protobuf/descriptor.h"
#include "google/protobuf/message.h"
#include "rpc_utility.h"

using std::string;

// sizeof (magic_num) + sizeof (call_guid) + sizeof (method_id) + sizeof (size) = 21
static const int kMetaSize = 21;
static const int kPacketMagicNum = 11;

struct RequestContext {
  // client_call , use for at rpc end callback
  google::protobuf::Message* response;
  google::protobuf::Closure* done;
  RequestContext(google::protobuf::Message*  re, 
      google::protobuf::Closure* d)
  : response(re), done(d) {}
};

struct ResponseContext {
  // service runservice then use this to sendback rpc
  google::protobuf::Message* response;
  int call_guid;
  int channel_guid;
  ResponseContext(int channel_gd, int call_gd, 
    google::protobuf::Message* res)
  :channel_guid(channel_gd), call_guid(call_gd), response(res) {}
};

class MethodInfo {
public:
  MethodInfo(google::protobuf::Service* service,
    const google::protobuf::MethodDescriptor* method_descriptor) :
    service(service), method_descriptor(method_descriptor) {
  }
  google::protobuf::Service* GetService() {
    // this show not be a const, because the service will change to specific class: like EchoService
    return service;
  }
  const google::protobuf::MethodDescriptor& GetMethodDescriptor() const{
    return *method_descriptor;
  }
  const google::protobuf::Message& GetRequestPrototype() const{
    return service->GetRequestPrototype(method_descriptor);
  }
  const google::protobuf::Message& GetResponsePrototype() const{
    return service->GetResponsePrototype(method_descriptor);
  }
private:
  google::protobuf::Service* service;
  const google::protobuf::MethodDescriptor* method_descriptor;
};


struct Packet {
  // 只是作为测试用
  int magic_num; // 2位
  int call_guid; // 4位
  // method_id为0表示这个packet是response消息,不为0表示这是request消息
  int method_id; // 4位
  int size;  // 4位
  std::string content;

  Packet(): magic_num(kPacketMagicNum), call_guid(0), method_id(0), size(0) {
  }

  Packet(int call_guid,
      const google::protobuf::MethodDescriptor *method,
      const google::protobuf::Message *request)
  : magic_num(kPacketMagicNum), call_guid(call_guid), method_id(0) {
    content = request->SerializeAsString();
    size = content.length();
    std::cout << "packet size " << size << std::endl;
    if (method) {
      method_id = hash_string(method->full_name());
    }
  }
};

class PacketParser {
public:
  PacketParser() {
  }
  ~PacketParser() {}
  string EncodePacketToStr(Packet packet) {
    memset(tmp, 0, sizeof(tmp));
    int len = 0;
    len += sprintf(tmp + len, "%02d", packet.magic_num);
    len += sprintf(tmp + len, "%04d", packet.call_guid);
    len += sprintf(tmp + len, "%04d", packet.method_id);
    len += sprintf(tmp + len, "%04d", packet.size);
    std::cout << "tmp encode " << tmp << std::endl;
    string out = string(tmp) + packet.content;
    return out;
  }
  Packet DecodePacketFromStr(char *buf) {
    char kkk[100];
    memset(kkk, 0, sizeof(kkk));
    memcpy(kkk, buf, 14);
    std::cout << kkk << std::endl;
    memset(tmp, 0, sizeof(tmp));
    memcpy(tmp, buf, 2);
    packet_.magic_num = atoi(tmp);
    memcpy(tmp, buf + 2, 4);
    packet_.call_guid = atoi(tmp);
    memcpy(tmp, buf + 6, 4);
    packet_.method_id = atoi(tmp);
    memcpy(tmp, buf + 10, 4);
    packet_.size = atoi(tmp);
    packet_.content = string(buf + 14);

    std::cout << "decode packet method_id:" << packet_.method_id 
        << " lenth:" << packet_.size << std::endl;
    return packet_;
  }
private:
  char tmp[20];
  Packet packet_;
};
#endif  // PACKET_H_
