#pragma once

#include "elements.hpp"
#include <vector>

class Sphere : public mygldraw::Elements {
public:
  Sphere(float radius) : radius_(radius){};

protected:
  void drawMainloop() override;

  void prepareShader() override;

  void prepareTexture() override { Log << "unused" << std::endl; };

  void linkShader() override;

  void programDataSet() override;

  void prepareBuffer() override;

  void shaderClean() override;

  void updateVertices() override {};

private:
  unsigned VAO_lines_ = -1;
  unsigned VBO_lines_ = -1;
  float const radius_;
  static constexpr int sector_count_ = 50;
  static constexpr int stack_count_ = 30;

  void fillVertices();

  void fillIndices();

  std::vector<float> normal_vertices_;
  std::vector<unsigned> indices_;
  std::vector<unsigned> line_indices_;
};
