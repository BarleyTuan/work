#-*- coding:utf-8 -*-

import server
import glwrapper
import asyncore
import server_global as G

IS_INIT_PX = True

def main():
  server_stub = server.EchoServer('0.0.0.0', 9999)
  if IS_INIT_PX:
    G.px_manager = glwrapper.PxManager()
    G.px_manager.InitializePhysX()

  while(True):
    asyncore.loop(0.01, count = 1)
    if G.px_manager:
      G.px_manager.UpdatePhysX(0.01)


if __name__ == "__main__":
  main()