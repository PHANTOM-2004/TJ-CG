#include <cstddef>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "solar-sys.hpp"
#include "GLFW/glfw3.h"
#include "elements.hpp"
#include "circle.hpp"

auto SolarSystem::Model(float x, float y, float z, float w_rate) {
  auto constexpr I = glm::mat4(1.0f);
  auto model = I;
  auto const rotate_angle = static_cast<float>(glfwGetTime() * w_rate);
  static auto const rotate_axle = glm::vec3(0.0f, 0.0f, 1.0f);
  model = glm::translate(model, glm::vec3(x, y, z));
  model = glm::rotate(model, rotate_angle, rotate_axle);
  // NOTE:默认x右边
  model = glm::translate(model, glm::vec3(-x, -y, -z));

  if (x > 1e-4f) {
    // std::cout << x << ' ' << y << ' ' << z << '\n';
  }
  return model;
}

auto SolarSystem::View() {
  auto constexpr I = glm::mat4(1.0f);
  // V we do not transform
  static auto const view = glm::translate(I, glm::vec3(0.0f, 0.0f, -4.0f));
  return view;
}

auto SolarSystem::Projection() {
  static float const aspect = static_cast<float>(1.0 * width_ / height_);
  // P unchanged
  static auto const projection =
      glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
  return projection;
}

void SolarSystem::shaderClean() {
  glDeleteShader(vertex_shader_);
  glDeleteShader(frag_shader_);
}

void SolarSystem::drawMainloop() {
  // mainloop;
  glEnable(GL_DEPTH_TEST);

  Log << "rendering" << std::endl;

  // keep drawing
  while (!glfwWindowShouldClose(window_)) {

    processInput(window_);
    FrameCount();

    // clear the screen first
    glClearColor(0.2f, 0.2f, 0.2f, 0.8f);
    glClear(GL_COLOR_BUFFER_BIT |
            GL_DEPTH_BUFFER_BIT); // 这里取决于我们希望clear什么,
                                  // 这里我们只关心 color

    // 只需设置一次即可
    programSetView();
    programSetProjection();

    // 对于多个星星
    glm::mat4 last_model;
    for (size_t i = 0; i < planets_.size(); i++) {
      programSetColor(planets_[i].r_, planets_[i].g_, planets_[i].b_,
                      planets_[i].a_);
      // 但是model需要多次设置
      if (i < planets_.size() - 1) {
        last_model = programSetModel(planets_.front().X(), planets_.front().Y(),
                                     planets_.front().Z(), w_rate_[i]);
      } else if (planets_.size() >= 3) {
        // 为月球额外设置
        glUseProgram(shader_program_);
        auto model =
            last_model * Model(planets_[i - 1].X(), planets_[i - 1].Y(),
                               planets_[i - 1].Z(), w_rate_[i]);
        glUniformMatrix4fv(glGetUniformLocation(this->shader_program_, "model"),
                           1, GL_FALSE, glm::value_ptr(model));
      }
      planets_[i].Draw();
    }

    glfwSwapBuffers(window_);
    glfwPollEvents();
  }

  for (auto &p : planets_) {
    p.Clear();
  }
  glDeleteProgram(shader_program_);
}

glm::mat4 SolarSystem::programSetModel(float x, float y, float z,
                                       float w_rate) {
  static auto const model_location =
      glGetUniformLocation(this->shader_program_, "model");

  auto const model = Model(x, y, z, w_rate);
  glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model));
  return model;
}

void SolarSystem::programSetView() {
  static auto const view_location =
      glGetUniformLocation(this->shader_program_, "view");
  glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(View()));
}

void SolarSystem::programSetProjection() {
  // remeber to use
  glUseProgram(this->shader_program_);

  static auto const projection_location =
      glGetUniformLocation(this->shader_program_, "projection");

  // note: currently we set the projection matrix each frame, but since the
  // projection matrix rarely changes it's often best practice to set it
  // outside the main loop only once.
  glUniformMatrix4fv(projection_location, 1, GL_FALSE,
                     glm::value_ptr(Projection()));
}

void SolarSystem::programDataSet() {}

void SolarSystem::prepareBuffer() {
  // 首先prepare Sun的buffer用于测试

  for (auto &p : planets_) {
    p.PreprareBuffer();
  }
}

void SolarSystem::programSetColor(float const r, float const g, float const b,
                                  float const a) {
  glUseProgram(this->shader_program_);
  static auto const vcolor_location =
      glGetUniformLocation(this->shader_program_, "vColor");
  glUniform4f(vcolor_location, r, g, b, a);
}

void SolarSystem::prepareShader() {
  // vertex shader object
  static const char *vertexShaderSource =
#include "circleShader.vert.glsl"
      ;

  // fragment shader object
  static const char *fragmentShaderSource =
#include "circleShader.frag.glsl"
      ;

  prepareVertexShader(this->vertex_shader_, vertexShaderSource);
  prepareFragShader(this->frag_shader_, fragmentShaderSource);
}

void SolarSystem::linkShader() {
  prepareProgram(this->shader_program_, this->vertex_shader_,
                 this->frag_shader_);
}
