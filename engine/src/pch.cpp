#include "pch.h"

namespace fuse {
  static std::random_device randomizer;
  static std::mt19937_64 generator(randomizer());
  static std::uniform_int_distribution<uuid64> distribution;

  uuid64 generate_uuid() {
    uuid64 uuid = INVALID_ID;
    do {
      uuid = distribution(generator); 
    } while (uuid == INVALID_ID);
    return uuid;
  }
}
