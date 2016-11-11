1 client.cc
  客户端, rpc的reponse callback:  MyEchoClient::MyEchoCallback
2 server.cc
  服务端. 收到echo_service对应的处理函数. MyEchoServer::Echo()
3 easy_epoll
  建立epoll. AddDescriptor, DeleteDescriptor, Poll轮询
4 packet
  定义几个基本的类
5 protobuf_channel
  继承自 google::protobuf::RpcChannel::RpcChannel
  HandleRead, HandleWrite, CallMethod.
6 protobuf_service
  google::protobuf::Service的Register.
  HandleAccept
  Poll有信号,进来的RunEvents(),在channel解包之后的RunService
7 rpc_utility
  一些公共函数, 如 hash_string, get_new_guid等.