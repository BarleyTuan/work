#ifndef PLATFORM_NET_POLL_H_
#define PLATFORM_NET_POLL_H_

#define MAX_EVENT_COUNT 128
#define BUFFER_SIZE 2500
#define ECHO_PORT 8888

#include <arpa/inet.h>
#include <sys/epoll.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <iostream>
#include <string>
#include "protobuf_service.h"
#include <string.h>
using namespace std;

class ProtobufService;
struct Descriptor {
  ProtobufService *service;
  int socket_fd;
  Descriptor(ProtobufService *ser, int fd) {
    service = ser;
    socket_fd = fd;
  }
};
class EasyEpoll {
public:
  static EasyEpoll* CreatePoll(int event_count) {
    int epoll_fd = epoll_create(event_count);
    EasyEpoll* p(new EasyEpoll());
    p->epoll_fd_ = epoll_fd;
    p->event_count_ = event_count;
    return p;
  }
  ~EasyEpoll() {epoll_fd_ = 0;}
  bool AddDescriptor(ProtobufService *service, int fd, int event);
  bool DeleteDescriptor(int fd) {
    struct epoll_event ev;
    memset(&ev, 0, sizeof(ev));
    ev.data.fd = fd;
    if (epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, fd, &ev) < 0) {
      cout << "epoll ctl del error " << fd << endl;
      return false;
    }
    return true;
  }
  void Poll(int ms);
  int GetEventCount() {
    return event_count_;
  }
private:
  int epoll_fd_;
  int event_count_;
  struct epoll_event events_[MAX_EVENT_COUNT];
};

#endif // PLATFORM_NET_POLL_H_
