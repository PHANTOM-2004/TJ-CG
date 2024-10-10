#include "common.hpp"
#include "circle.hpp"
#include <exception>
#include <stdexcept>
#include <cmath>

static bool check(float const t) {
  if (t < -1.0f || t > 1.0f) {
    std::runtime_error("not NDC");
    std::terminate();
  }
  return true;
}

Circle::Circle(float const radius, float const x, float const y, float const z,
               unsigned const count)
    : radius_(radius), x0(x), y0(y), z0(z), count_(count) {
  if (count_ <= 10) {
    std::runtime_error("count should at least be 10");
  }
  check(x);
  check(y);
  check(z);
  check(radius);
  PrepareData();
}

void Circle::PrepareData() {
  // vertices_.clear();
  // indices_.clear();
  vertices_.reserve(count_ * 3);
  indices_.reserve(count_ * 3);

  static const float PI = std::acos(-1);

  static float const theta = 2 * PI / count_;

  for (unsigned i = 0; i < count_; i++) {
    float const xi = x0 + radius_ * std::cos(i * theta);
    float const yi = x0 + radius_ * std::sin(i * theta);
    vertices_.emplace_back(xi);
    vertices_.emplace_back(yi);
    vertices_.emplace_back(z0);

    std::cout << "[" << i << "]";
    std::cout << xi << ' ' << yi << ' ' << z0 << std::endl;

    if (i > 0 && i + 1 < count_) {
      indices_.emplace_back(0);
      indices_.emplace_back(i);
      indices_.emplace_back(i + 1);
    }
  }
  Log << VerticeSize() << std::endl;
  Log << IndiceSize() << std::endl;

  int i = 0;
  for (auto const f : indices_) {
    std::cout << f << ' ';
    i++;
    if (i % 3 == 0) {
      std::cout << std::endl;
    }
  }
}
