#include <common.hpp>
#include "cube.hpp"

int main() {
  Log << "This is my neon-cube!" << std::endl;
  Status();

  if constexpr (1) {
    Cube c;
    c.Draw();
  }
}
