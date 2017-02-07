#-*- coding:utf-8 -*-
import glwrapper
import game_world
import G
import client

def main():
  print "hello world"
  #G.client_stub = client.EchoClient('127.0.0.1', 9999)
  G.client_stub = client.EchoClient('192.168.1.1', 9999)

  G.px_manager = glwrapper.PxManager()
  G.px_manager.InitializePhysX()
  glwrapper.logic_main(G.px_manager)
  return

if __name__ == "__main__":
  main()