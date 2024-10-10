#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "solar-sys.hpp"
#include "GLFW/glfw3.h"
#include "elements.hpp"
#include "circle.hpp"

SolarSystem::SolarSystem() : Sun(0.5f, 0, 0, 0.0f, 50) {}

void SolarSystem::shaderClean() {
  glDeleteShader(vertex_shader_);
  glDeleteShader(frag_shader_);
}

void SolarSystem::drawMainloop() {
  // mainloop;
  glEnable(GL_DEPTH_TEST);

  static unsigned frame_count = 0;
  static float last_time = 0;
  Log << "rendering" << std::endl;

  // keep drawing
  while (!glfwWindowShouldClose(window_)) {

    float const current_time = static_cast<float>(glfwGetTime());
    frame_count++;

    if (current_time - last_time >= 1.0) {
      std::clog << "[INFO]FPS: " << frame_count << '\r';
      frame_count = 0;
      last_time = current_time;
    }

    processInput(window_);

    // clear the screen first
    glClearColor(0.2f, 0.2f, 0.2f, 0.8f);
    glClear(GL_COLOR_BUFFER_BIT |
            GL_DEPTH_BUFFER_BIT); // 这里取决于我们希望clear什么,
                                  // 这里我们只关心 color
    // glUseProgram(shaderProgram);

    programDataSet();

    // seeing as we only have a single VAO there's no need to bind it every
    // time, but we'll do so to keep things a bit more organized
    glBindVertexArray(VAO_sun_);
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    // The first argument specifies the mode we want to draw in, similar to
    // glDrawArrays. The second argument is the count or number of elements we'd
    // like to draw. We specified 6 indices so we want to draw 6 vertices in
    // total. The third argument is the type of the indices which is of type
    // GL_UNSIGNED_INT. The last argument allows us to specify an offset in the
    // EBO (or pass in an index array, but that is when you're not using element
    // buffer objects), but we're just going to leave this at 0.
    //  glDrawElements(GL_TRIANGLES, , sizeof(vertices) / sizeof(vertices[0]));
    glDrawElements(GL_TRIANGLES, Sun.IndiceCount(), GL_UNSIGNED_INT, 0);

    updateVertices();

    glfwSwapBuffers(window_);
    glfwPollEvents();
  }

  // release VAO, VBO, EBO, program
  glDeleteVertexArrays(1, &VAO_sun_);
  glDeleteBuffers(1, &VBO_sun_);
  glDeleteBuffers(1, &EBO_sun_);
  glDeleteProgram(shader_program_);
}

void SolarSystem::programDataSet() {
  // prepare MVP data
  static auto constexpr I = glm::mat4(1.0f);
  auto const rotate_angle = static_cast<float>(0);
  auto constexpr rotate_axle = glm::vec3(0.5f, 1.0f, 0.0f);
  static float const aspect = static_cast<float>(1.0 * width_ / height_);

  // M this changes
  auto const model = glm::rotate(I, rotate_angle, rotate_axle);

  // V we do not transform
  static auto const view = glm::translate(I, glm::vec3(0.0f, 0.0f, -3.0f));

  // P unchanged
  static auto const projection =
      glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);

  // remeber to use
  glUseProgram(this->shader_program_);

  static auto const model_location =
      glGetUniformLocation(this->shader_program_, "model");
  static auto const view_location =
      glGetUniformLocation(this->shader_program_, "view");
  static auto const projection_location =
      glGetUniformLocation(this->shader_program_, "projection");

  glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view));
  // note: currently we set the projection matrix each frame, but since the
  // projection matrix rarely changes it's often best practice to set it
  // outside the main loop only once.
  glUniformMatrix4fv(projection_location, 1, GL_FALSE,
                     glm::value_ptr(projection));
}

void SolarSystem::prepareBuffer() {
  // 首先prepare Sun的buffer用于测试

  glGenVertexArrays(1, &VAO_sun_);
  glGenBuffers(1, &VBO_sun_);
  glGenBuffers(1, &EBO_sun_);

  // VBO
  glBindVertexArray(VAO_sun_);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_sun_);
  glBufferData(GL_ARRAY_BUFFER, Sun.VerticeSize(), Sun.VerticeData(),
               GL_STATIC_DRAW);
  // EBO
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_sun_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, Sun.IndiceSize(), Sun.IndiceData(),
               GL_STATIC_DRAW);

  // 只有一个存放坐标的
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Sun.Stride(),
                        static_cast<void *>(0));
  glEnableVertexAttribArray(0);
  // unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
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
