1 client.cc
  �ͻ���, rpc��reponse callback:  MyEchoClient::MyEchoCallback
2 server.cc
  �����. �յ�echo_service��Ӧ�Ĵ�����. MyEchoServer::Echo()
3 easy_epoll
  ����epoll. AddDescriptor, DeleteDescriptor, Poll��ѯ
4 packet
  ���弸����������
5 protobuf_channel
  �̳��� google::protobuf::RpcChannel::RpcChannel
  HandleRead, HandleWrite, CallMethod.
6 protobuf_service
  google::protobuf::Service��Register.
  HandleAccept
  Poll���ź�,������RunEvents(),��channel���֮���RunService
7 rpc_utility
  һЩ��������, �� hash_string, get_new_guid��.