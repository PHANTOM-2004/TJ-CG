
#include "solar-sys.hpp"

int main() {

  std::vector<SolarSystem::Planet> P{
      {0.2f, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0},
      {0.05f, 0.4f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 1},
      {
          0.13f,
          0.8f,
          0.0f,
          0.0f,
          0.5f,
          0.0f,
          0.0f,
          1.0f,
          1.5f,
      }};

  SolarSystem S{P};
  S.Draw();
}
