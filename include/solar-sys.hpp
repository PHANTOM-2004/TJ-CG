#pragma once

#include "elements.hpp"
#include <glm/glm.hpp>
#include "circle.hpp"
#include <vector>

class SolarSystem : public mygldraw::Elements {
public:
  struct Planet {
    float radius;
    glm::vec3 pos;
    glm::vec4 color;
    float w_rate = 0.0f;
  };

  SolarSystem() = delete;

  SolarSystem(std::vector<Planet> const &planets);

protected:
  void prepareShader() override;

  void linkShader() override;

  void programDataSet() override;

  void updateVertices() override {};

  void prepareBuffer() override;

  void drawMainloop() override;

  void shaderClean() override;

  auto Model(float x, float y, float z, float w_rate);

  auto View();

  auto Projection();

  glm::mat4 programSetModel(float x, float y, float z, float w_rate);

  void programSetView();

  void programSetProjection();

  void programSetColor(float r, float g, float b, float a);

private:
  std::vector<Circle> planets_;
  std::vector<float> w_rate_;

  unsigned vertex_shader_ = 0, frag_shader_ = 0, shader_program_ = 0;
};
