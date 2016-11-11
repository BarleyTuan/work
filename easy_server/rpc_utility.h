#ifndef PLATFORM_RPC_UTILITY_H_
#define PLATFORM_RPC_UTILITY_H_

#include <stdlib.h>
#include <string>

static inline int hash_string(std::string full_name) {
  return 1234;
}

static inline int get_new_guid() {
  return rand() % 10000;
}

#endif // PLATFORM_RPC_UTILITY_H_