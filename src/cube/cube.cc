#include "cube.hpp"
#include "GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>

Cube::Cube() {
  dynamic_vertices = std::make_unique<float[]>(vertices_size);
  for (int i = 0; i < vertices_size; i++)
    dynamic_vertices[i] = static_vertices[i];
}

void Cube::updateVertices() {
  auto const time = static_cast<float>(glfwGetTime());
  float const t = 0.25f * std::cos(time * 2) + 0.75;

  // VBO buffer
  glBindBuffer(GL_ARRAY_BUFFER, VBO_);

  for (int i = 0; i < 8; i++) {
    int const base_pos = 6 * i + 3;
    if (i & 0x1) {
      dynamic_vertices[base_pos + 0] = t;
    }
    if (i & 0x2) {
      dynamic_vertices[base_pos + 1] = t;
    }
    if (i & 0x4) {
      dynamic_vertices[base_pos + 2] = t;
    }
  }

  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(static_vertices),
                  dynamic_vertices.get());
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Cube::programDataSet() {
  // prepare MVP data
  static auto constexpr I = glm::mat4(1.0f);
  auto const rotate_angle = static_cast<float>(glfwGetTime());
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

void Cube::drawMainloop() {
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
    glBindVertexArray(VAO_);
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    // The first argument specifies the mode we want to draw in, similar to
    // glDrawArrays. The second argument is the count or number of elements we'd
    // like to draw. We specified 6 indices so we want to draw 6 vertices in
    // total. The third argument is the type of the indices which is of type
    // GL_UNSIGNED_INT. The last argument allows us to specify an offset in the
    // EBO (or pass in an index array, but that is when you're not using element
    // buffer objects), but we're just going to leave this at 0.
    //  glDrawElements(GL_TRIANGLES, , sizeof(vertices) / sizeof(vertices[0]));
    glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]),
                   GL_UNSIGNED_INT, 0);

    updateVertices();

    glfwSwapBuffers(window_);
    glfwPollEvents();
  }

  // release VAO, VBO, EBO, program
  glDeleteVertexArrays(1, &VAO_);
  glDeleteBuffers(1, &VBO_);
  glDeleteBuffers(1, &EBO_);
  glDeleteProgram(shader_program_);
}

void Cube::shaderClean() {
  glDeleteShader(vertex_shader_);
  glDeleteShader(frag_shader_);
}

void Cube::prepareBuffer() {
  glGenVertexArrays(1, &VAO_);
  glGenBuffers(1, &VBO_);
  glGenBuffers(1, &EBO_);

  // bind VAO
  glBindVertexArray(VAO_);

  // VBO buffer
  glBindBuffer(GL_ARRAY_BUFFER, VBO_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(static_vertices), dynamic_vertices.get(),
               GL_DYNAMIC_DRAW);

  // EBO buffer
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  // position attr
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        static_cast<void *>(0));
  glEnableVertexAttribArray(0);

  // color attr
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        reinterpret_cast<void *>(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void Cube::linkShader() {
  prepareProgram(this->shader_program_, this->vertex_shader_,
                 this->frag_shader_);
}

void Cube::prepareShader() {
  // vertex shader object
  static const char *vertexShaderSource =
#include "cubeShader.vert.glsl"
      ;

  // fragment shader object
  static const char *fragmentShaderSource =
#include "cubeShader.frag.glsl"
      ;

  prepareVertexShader(this->vertex_shader_, vertexShaderSource);
  prepareFragShader(this->frag_shader_, fragmentShaderSource);
}
