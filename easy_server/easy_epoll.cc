#include <stdio.h>
#include "easy_epoll.h"

bool EasyEpoll::AddDescriptor(ProtobufService *service, int fd, int event) {
  Descriptor *descriptor = new Descriptor(service, fd);
  struct epoll_event ev;
  memset(&ev, 0, sizeof(ev));
  ev.events = event;
  ev.data.ptr = descriptor;;
  if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &ev) < 0) {
    cout << "epoll ctl add error " << fd << " event " << event << endl;
    return false;
  }
  std::cout << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxx add fd " << fd << "  event " << event << std::endl;
  return true;
}

void EasyEpoll::Poll(int ms) {
  int num_events;
  while(true) {
    num_events = epoll_wait(epoll_fd_, events_, event_count_, ms);
    if (num_events <= 0) {
      continue;
    }
    break;
  }
  std::cout << " get num_events " << num_events << std::endl;
  for (int i = 0; i< num_events; ++i) {
    Descriptor* descriptor = reinterpret_cast<Descriptor*>(events_[i].data.ptr);
    int fd = descriptor->socket_fd;
    std::cout << "fd: " << fd << std::endl;
    ProtobufService* service = descriptor->service;
    service->RunEvents(fd);
  }
  memset(events_, 0, sizeof(events_));
}
