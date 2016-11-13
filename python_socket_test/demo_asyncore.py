#-*- coding:utf-8 -*-
import time
import asyncore
import socket
import threading

class EchoHandler(asyncore.dispatcher_with_send):
  def handle_read(self):
    data = self.recv(1024)
    print "server receive data: %s" % data
    if data:
      self.send(data)

class EchoServer(asyncore.dispatcher):
  def __init__(self, host, port):
    asyncore.dispatcher.__init__(self)
    self.create_socket(socket.AF_INET, socket.SOCK_STREAM)
    self.set_reuse_addr()
    self.bind((host, port))
    self.listen(5)
    
  def handle_accept(self):
    conn_sock, addr = self.accept()
    print 'Incoming connection from %s' % repr(addr)
    self.handler = EchoHandler(conn_sock)

class EchoClient(asyncore.dispatcher):
  def __init__(self, host, port):
    asyncore.dispatcher.__init__(self)
    self.messages = ['1', '2', '3', '4', '5', '6', '7', '8', '9', '10']
    self.create_socket(socket.AF_INET, socket.SOCK_STREAM)
    self.connect((host, port))

  def handle_connect(self):
    print "socket_map is ", asyncore.socket_map

  def handle_close(self):
    self.close()

  def handle_read(self):
    data = self.recv(1024)
    print "data : %s" % data

  def writable(self):
    return (len(self.messages) > 0)

  def handle_write(self):
    if len(self.messages) > 0: 
      self.send(self.messages.pop(0))

class EchoServerThread(threading.Thread):
  def __init__(self):
    threading.Thread.__init__(self)

  def run(self):
    server = EchoServer('localhost', 9999)
    asyncore.loop(0.1)

class EchoClientThread(threading.Thread):
  def __init__(self):
    threading.Thread.__init__(self)

  def run(self):
    client = EchoClient('localhost', 9999)
    #同进程共享asyncore的map
    #asyncore.loop(1)

def main():
  print "server start"
  EchoServerThread().start()
  time.sleep(0.1)
  EchoClientThread().start()

if __name__ == "__main__":
  main()