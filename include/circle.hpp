#pragma once

#include <vector>

// 用于获得circle的buffer
// 暂定只存放坐标，颜色由uniform变量决定
class Circle {
public:
  Circle(float radius, float x, float y, float z, unsigned count,
         bool data = true);

  void Clear();

  unsigned VertexArray() const { return VAO_; }

  unsigned VerticeSize() const {
    return VerticeCount() * static_cast<unsigned>(sizeof(float));
  };

  unsigned VerticeCount() const {
    return static_cast<unsigned>(vertices_.size());
  };

  unsigned IndiceCount() const {
    return static_cast<unsigned>(indices_.size());
  };

  unsigned IndiceSize() const {
    return IndiceCount() * static_cast<unsigned>(sizeof(float));
  };

  float *VerticeData() { return vertices_.data(); };

  unsigned *IndiceData() { return indices_.data(); };

  virtual unsigned Stride() const {
    return static_cast<unsigned>(3 * sizeof(float));
  }

  void PreprareBuffer();

  void Draw() const;

  void SetColor(float r, float g, float b, float a) {
    r_ = r, g_ = g, b_ = b, a_ = a;
  }

  float X() const { return x0; }
  float Y() const { return y0; }
  float Z() const { return z0; }

  float r_ = 0.5f, g_ = 0.5f, b_ = 0.5f, a_ = 1.0f;

protected:
  void PrepareData();

  float const radius_;
  float const x0;
  float const y0;
  float const z0;
  unsigned const count_;

  std::vector<float> vertices_;
  std::vector<unsigned> indices_;

  unsigned VAO_, VBO_, EBO_;
};

class CircleBound : public Circle {
public:
  CircleBound(float radius, float x, float y, float z, unsigned count);

  unsigned Stride() const override {
    return static_cast<unsigned>(2 * sizeof(float));
  }

private:
  void PrepareData();
};
