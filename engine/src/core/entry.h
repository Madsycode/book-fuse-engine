#pragma once
#include "application.h"

extern fuse::app_config fuse::create_application();  

int main(int argc, char** argv) {   
  auto config = fuse::create_application();
  fuse::run_application(config);
  return 0;
}