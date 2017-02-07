#!/usr/bin/python
#-*- coding:utf-8 -*-

import time
import asyncore
import socket
import threading
import do_msg

class EchoHandler(asyncore.dispatcher_with_send):
    def handle_read(self):
        data = self.recv(1024)
        do_msg.DoMsg(data)

class EchoServer(asyncore.dispatcher):

    def __init__(self, host, port):
        asyncore.dispatcher.__init__(self)
        self.create_socket(socket.AF_INET, socket.SOCK_STREAM)
        self.set_reuse_addr()
        self.bind((host, port))
        self.listen(5)

    def handle_accept(self):
        conn, addr = self.accept()
        print 'Incoming connection from %s' % repr(addr)
        self.handler = EchoHandler(conn)

def main():
    server = EchoServer('127.0.0.1', 9999)
    while(True):
      asyncore.loop(0.01)

if __name__ == "__main__":
  main()

