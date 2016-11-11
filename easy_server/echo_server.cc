#include <iostream>
#include "easy_epoll.h"
#include "echo.pb.h"
#include <tr1/unordered_map>
#include "protobuf_service.h"

class MyEchoServer: public EchoService {
 public:
  virtual void Echo(google::protobuf::RpcController* controller,
      const EchoRequest* request, EchoResponse* response,
      google::protobuf::Closure* done) {
    std::cout << "server receive xxxx : " << request->res_msg() << std::endl;
    string msg = "server:" + request->res_msg();
    response->set_resp_msg(msg);
    if (done) {
      done->Run();
    }
  }
};

class MyEchoClient {
public:
  MyEchoClient() {
    is_stop = false;
  }
  void MyEchoCallback(EchoResponse *response) {
    cout << "response word: " << response->resp_msg() << endl;
    is_stop = true;
  }
  bool is_stop;
};


int main() {
  // server
  std::cout << "fuck the server" << std::endl;
  static EasyEpoll* poll;
  poll = EasyEpoll::CreatePoll(MAX_EVENT_COUNT);
  ProtobufService server(poll);
  server.StartService("127.0.0.1", 8888);
  MyEchoServer* echo_service(new MyEchoServer);
  server.Register(echo_service);

  while(1) {
    poll->Poll(100);
  }
  std::cout << "fuck server over" << std::endl;
  return 0;
}
