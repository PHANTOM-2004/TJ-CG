#pragma once

#include "elements.hpp"
#include "circle.hpp"

class SolarSystem : public mygldraw::Elements {
public:
  SolarSystem();

  void prepareShader() override;

  void linkShader() override;

  void programDataSet() override;

  void updateVertices() override {};

  void prepareBuffer() override;

  void drawMainloop() override;

  void shaderClean() override;

private:
  Circle Sun;


  unsigned vertex_shader_ = 0, frag_shader_ = 0, shader_program_ = 0;
};
