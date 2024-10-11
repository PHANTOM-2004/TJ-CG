#include "solar-sys.hpp"

int main() {
  std::vector<SolarSystem::Planet> P{
      {0.22f, {0.0f, 0.0f, 0.0f}, {0.5f, 0.0f, 0.2f, 1.0f}, 0.0f},
      {0.08f, {0.45f, 0.0f, 0.0f}, {0.5f, 0.0f, 0.5f, 1.0f}, 1.0f},
      {
          0.11f,
          {0.75f, 0.0f, 0.0f},
          {0.3f, 0.9f, 0.4f, 1.0f},
          1.5f,
      },
      {0.05f, {0.9f, 0.0f, 0.0f}, {0.0f, 0.5f, 0.5f, 1.0f}, 3.0f},

  };

  SolarSystem S{P};
  S.Draw();
}
