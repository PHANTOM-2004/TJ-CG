#include "common.hpp"
#include "circle.hpp"
#include "glad/glad.h"
#include <exception>
#include <stdexcept>
#include <cmath>

static const float PI = std::acos(-1);

static bool check(float const t) {
  if (t < -1.0f || t > 1.0f) {
    std::cerr << "[ERROR] NOT NDC" << std::endl;
    std::runtime_error("not NDC");
    std::terminate();
  }
  return true;
}

Circle::Circle(float const radius, float const x, float const y, float const z,
               unsigned const count, bool const data)
    : radius_(radius), x0(x), y0(y), z0(z), count_(count) {
  if (count_ <= 10) {
    std::runtime_error("count should at least be 10");
  }
  check(x);
  check(y);
  check(z);
  check(radius);
  if (data) {
    PrepareData();
  }
}

void Circle::Clear() {
  glDeleteVertexArrays(1, &VAO_);
  glDeleteBuffers(1, &VBO_);
  glDeleteBuffers(1, &EBO_);
}

void Circle::Draw() {
  glBindVertexArray(VertexArray());
  // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  // The first argument specifies the mode we want to draw in, similar to
  // glDrawArrays. The second argument is the count or number of elements we'd
  // like to draw. We specified 6 indices so we want to draw 6 vertices in
  // total. The third argument is the type of the indices which is of type
  // GL_UNSIGNED_INT. The last argument allows us to specify an offset in the
  // EBO (or pass in an index array, but that is when you're not using element
  // buffer objects), but we're just going to leave this at 0.
  //  glDrawElements(GL_TRIANGLES, , sizeof(vertices) / sizeof(vertices[0]));

  glDrawElements(GL_TRIANGLES, IndiceCount(), GL_UNSIGNED_INT, 0);
}

void Circle::PreprareBuffer() {

  glGenVertexArrays(1, &VAO_);
  glGenBuffers(1, &VBO_);
  glGenBuffers(1, &EBO_);

  glBindVertexArray(VAO_);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_);
  glBufferData(GL_ARRAY_BUFFER, VerticeSize(), VerticeData(), GL_STATIC_DRAW);
  // EBO
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndiceSize(), IndiceData(),
               GL_STATIC_DRAW);

  // 只有一个存放坐标的
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Stride(),
                        static_cast<void *>(0));
  glEnableVertexAttribArray(0);
  // unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void Circle::PrepareData() {
  // vertices_.clear();
  // indices_.clear();
  vertices_.reserve(count_ * 3);
  indices_.reserve(count_ * 3);

  static float const theta = 2 * PI / count_;

  for (unsigned i = 0; i < count_; i++) {
    float const xi = x0 + radius_ * std::cos(i * theta);
    float const yi = y0 + radius_ * std::sin(i * theta);
    vertices_.emplace_back(xi);
    vertices_.emplace_back(yi);
    vertices_.emplace_back(z0);
    // std::cout << xi << ' ' << yi << std::endl;

    if (i > 0 && i + 1 < count_) {
      indices_.emplace_back(0);
      indices_.emplace_back(i);
      indices_.emplace_back(i + 1);
    }
  }
}

CircleBound::CircleBound(float const radius, float const x, float const y,
                         float const z, unsigned const count)
    : Circle(radius, x, y, z, count, false) {
  PrepareData();
}

void CircleBound::PrepareData() {
  vertices_.reserve(count_ * 2);
  static float const theta = 2 * PI / count_;

  for (unsigned i = 0; i < count_; i++) {
    float const xi = x0 + radius_ * std::cos(i * theta);
    float const yi = y0 + radius_ * std::sin(i * theta);
    vertices_.emplace_back(xi);
    vertices_.emplace_back(yi);
    vertices_.emplace_back(z0);

    if (i + 1 < count_) {
      indices_.emplace_back(i);
      indices_.emplace_back(i + 1);
    }
  }
}
