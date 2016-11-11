#include <iostream>
#include "easy_epoll.h"
#include "echo.pb.h"
#include <tr1/unordered_map>
#include "protobuf_channel.h"
#include "protobuf_service.h"

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
  std::cout << "fuck the client" << std::endl;
  static EasyEpoll* poll;
  poll = EasyEpoll::CreatePoll(MAX_EVENT_COUNT);
  ProtobufService client(poll);
  MyEchoClient* echo_client(new MyEchoClient);
  ProtobufChannel* channel = new ProtobufChannel(&client, "127.0.0.1", 8888);
  //if (true) { sleep(2); return 0; } 
  EchoService_Stub echo_stub(channel);
  EchoRequest request;
  EchoResponse response;
  request.set_res_msg("call helloworld");

  std::cout << request.res_msg() << "   " << channel << std::endl;
  echo_stub.Echo(NULL, &request, &response,
      google::protobuf::NewCallback(
      echo_client, &MyEchoClient::MyEchoCallback, &response));

  while(!echo_client->is_stop) {
    poll->Poll(100);
  }
  std::cout << "fuck client over" << std::endl;
  return 0;
}
