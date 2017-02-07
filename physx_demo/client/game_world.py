#-*- coding:utf-8 -*-
import G
import asyncore

def update(dt):
  #print "len socket map ", len(asyncore.socket_map)
  asyncore.loop(0.01, count = 1)
  return

def on_key_down(key):
  print "get key down key", key
  send_msg = ""
  if key == "c":
    if not G.actor_dict.has_key(1):
      G.add_actor(1, 1, 1, 4)
      send_msg += "C#1#1#1#4$"
    elif not G.actor_dict.has_key(2):
      G.add_actor(2, 3, 1, 4)
      send_msg += "C#2#3#1#4$"
    else:
      pass
  elif key == "p":
    send_msg += "P#0#0#0#0$"
    actor_id = 1 
    pos = G.px_manager.GetPosition(actor_id)
    print "get actor_id %s pos: %s" % (actor_id, pos)
    actor_id = 2 
    pos = G.px_manager.GetPosition(actor_id)
    print "get actor_id %s pos: %s\n============" % (actor_id, pos)
  else:
    actor_id = 0
    add_x = 0
    add_y = 0
    add_z = 0
    if key == "a" or key == "h":
      add_x = -1
    elif key == "s" or key == "j":
      add_z = -1
    elif key == "d" or key == "k":
      add_x = 1
    elif key == "w" or key == "u":
      add_z = 1
    elif key == "x" or key == "m":
      add_y = 3

    if key in ["a", "s", "d", "w", "x"]:
      actor_id = 1
    else:
      actor_id = 2
    if add_x + add_y + add_z == 0:
      return
    print "get cur info  actor_id ", actor_id, add_x, add_y, add_z
    G.px_manager.MovePosition(actor_id, add_x * 1.0, add_y * 1.0, add_z * 1.0, 1)
    send_msg += "M#%s#%s#%s#%s$" % (actor_id, add_x, add_y, add_z)
  if not send_msg:
    return
  print "send_msg ========", send_msg
  if G.client_stub:
    G.client_stub.messages.append(send_msg)
  return








