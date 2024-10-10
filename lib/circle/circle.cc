
#include "circle.hpp"
#include <stdexcept>
#include <cmath>

Circle::Circle(float const radius, float const x, float const y, float const z,
               unsigned const count)
    : radius_(radius), x0((x + radius) / radius), y0(y / radius), z0(z),
      count_(count) {
  if (count_ <= 10) {
    std::runtime_error("count should at least be 10");
  }
  if (z < -1 || z > 1) {
    std::runtime_error("z should be NDC");
  }
}

void Circle::PrepareData() {
  vertices_.reserve(count_ * 3);

  vertices_.emplace_back(x0);
  vertices_.emplace_back(y0);
  vertices_.emplace_back(z0);

  static const float PI = std::acos(-1);

  float const theta = 2 * PI / count_;

  for (unsigned i = 1; i < count_; i++) {
    float const di = 2 * radius_ * std::sin(i * theta);
    float const xi = x0 - 2 * di * std::sin(theta) * std::sin(theta);
    float const yi = y0 + di * std::sin(2 * theta);
    vertices_.emplace_back(xi);
    vertices_.emplace_back(yi);
    vertices_.emplace_back(z0);
    // TODO: add indices
  }
}
