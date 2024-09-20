#include <common.hpp>
#include "cube.hpp"

int main() {
  Log << "This is my neon-cube!" << std::endl;
  Log << "fs base path[" << FS_BASE_PATH << "]" << std::endl;
#ifdef BUILD_PLATFORM_WINDOWS
  Log << "Build platform: " << ANSI_FMT("Windows", ANSI_FG_RED) << std::endl;
#elif defined(BUILD_PLATFORM_LINUX)
  Log << "Build platform: " << ANSI_FMT("Linux", ANSI_FG_RED) << std::endl;
#endif

  if constexpr (1) {
    Cube c;
    c.Draw();
  }
}
