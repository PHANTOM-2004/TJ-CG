#pragma once

#include "elements.hpp"
#include "circle.hpp"

#include <vector>

class SolarSystem : public mygldraw::Elements {
public:
  struct Planet {
    float radius;
    float x, y, z;
    float r, g, b, a = 1.0f;
    float w_rate = 0.0f;
  };

  SolarSystem() = delete;

  SolarSystem(std::vector<Planet> const &planets);

  void prepareShader() override;

  void linkShader() override;

  void programDataSet() override;

  void updateVertices() override {};

  void prepareBuffer() override;

  void drawMainloop() override;

  void shaderClean() override;

  void programSetMVP(float w_rate_);

private:
  std::vector<Circle> planets_;
  std::vector<float> w_rate_;

  unsigned vertex_shader_ = 0, frag_shader_ = 0, shader_program_ = 0;
};
