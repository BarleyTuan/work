#-*- coding:utf-8 -*-

import time
import asyncore
import socket


class EchoClient(asyncore.dispatcher):

    def __init__(self, host, port):
        asyncore.dispatcher.__init__(self)
        self.messages = ["testmsg"]
        self.create_socket(socket.AF_INET, socket.SOCK_STREAM)
        self.connect((host, port))

    def handle_connect(self):
        pass

    def handle_close(self):
        self.close()

    def handle_read(self):
        print self.recv(1024)

    def writable(self):
        return (len(self.messages) > 0)

    def handle_write(self):
        if len(self.messages) > 0: 
            self.send(self.messages.pop(0))
            



def main():
  client = EchoClient('127.0.0.1', 9999)
  while(True):
    asyncore.loop(0.01)

if __name__ == "__main__":
  main()