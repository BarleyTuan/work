#!/usr/bin/python
#-*- coding:utf-8 -*-

'''
C#2#500#300#400$:  create actor 2 as pos(500, 300, 400)/  100.0
M#1#500#300#400$:  move actor 1 to pos(500, 300, 400) / 100.0
P#1#0#0#0$
'''
import server_global as G

def DoMsg(data):
  print "recv data : ", data
  if not G.px_manager:
    return
  msg_list = data.split("$")
  for msg in msg_list:
    op = msg.split("#")
    if len(op) != 5:
      continue
    if op[0] != "C" and op[0] != "M" and op[0] != "P":
      print "error op for msg %s" % msg 
      continue
    actor_id = int(op[1])
    pos_x = int(op[2]) / 1.0 
    pos_y = int(op[3]) / 1.0 
    pos_z = int(op[4]) / 1.0 
    if op[0] == "M":
      G.px_manager.MovePosition(actor_id, pos_x, pos_y, pos_z, 1)
    if op[0] == "C":
      G.px_manager.AddController(actor_id, pos_x, pos_y, pos_z)
    if op[0] == "P":
      actor_id = 1 
      pos = G.px_manager.GetPosition(actor_id)
      print "get actor_id %s pos: %s" % (actor_id, pos)
      actor_id = 2 
      pos = G.px_manager.GetPosition(actor_id)
      print "get actor_id %s pos: %s\n============" % (actor_id, pos)
  return