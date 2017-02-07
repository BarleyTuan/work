#-*- coding:utf-8 -*-

px_manager = None
actor_dict = {}
client_stub = None

def add_actor(actor_id, x, y, z):
  global px_manager
  if px_manager == None:
    print "none px_manager"
    return
  print "python add actor ", actor_id, x, y, z
  px_manager.AddController(actor_id, x * 1.0 , y * 1.0, z * 1.0)
  actor_dict[actor_id] = 1
  return

