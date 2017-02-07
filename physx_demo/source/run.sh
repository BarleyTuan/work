#!/bin/bash
g++ gl_manager_wrapper.cpp px_manager.cpp -fPIC -shared  -w -g -pthread \
  -D NDEBUG \
  -L/home/dm63/physx_source/PhysXSDK/Lib/linux64 -Wl,-Bstatic -lPhysX3Extensions \
  -Wl,-Bdynamic \
  -I/usr/include/python2.7 -lpython2.7 -lboost_python \
  -I/home/dm63/physx_source/PhysXSDK/Include \
  -lPhysX3Common_x64 -lPhysX3_x64 -lPhysX3CharacterKinematic_x64 \
  -o glwrapper.so