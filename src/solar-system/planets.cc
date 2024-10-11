#include "solar-sys.hpp"
#include "common.hpp"

SolarSystem::SolarSystem(std::vector<Planet> const &planets)
    : Elements(1000, 750) {
  Log << "Initializing" << std::endl;

  for (auto const &p : planets) {
    planets_.emplace_back(p.radius, p.pos.x, p.pos.y, p.pos.z, 40);
    planets_.back().SetColor(p.color.x, p.color.y, p.color.z, p.color.w);
  }

  Log << "Initialized" << std::endl;

  // 角速度倍率
  for (auto const &p : planets) {
    w_rate_.emplace_back(p.w_rate);
  }
}
