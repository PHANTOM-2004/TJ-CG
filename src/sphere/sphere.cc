
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include "sphere.hpp"
#include "elements.hpp"
// https://www.songho.ca/opengl/gl_sphere.html

void Sphere::fillIndices() {
  int k1, k2;
  for (int i = 0; i < stack_count_; ++i) {
    k1 = i * (sector_count_ + 1); // beginning of current stack
    k2 = k1 + sector_count_ + 1;  // beginning of next stack

    for (int j = 0; j < sector_count_; ++j, ++k1, ++k2) {
      // 2 triangles per sector excluding first and last stacks
      // k1 => k2 => k1+1
      if (i != 0) {
        indices_.push_back(k1);
        indices_.push_back(k2);
        indices_.push_back(k1 + 1);
      }

      // k1+1 => k2 => k2+1
      if (i != (stack_count_ - 1)) {
        indices_.push_back(k1 + 1);
        indices_.push_back(k2);
        indices_.push_back(k2 + 1);
      }

      // store indices for lines
      // vertical lines for all stacks, k1 => k2
      line_indices_.push_back(k1);
      line_indices_.push_back(k2);
      if (i != 0) // horizontal lines except 1st stack, k1 => k+1
      {
        line_indices_.push_back(k1);
        line_indices_.push_back(k1 + 1);
      }
    }
  }
}

void Sphere::fillVertices() {
  const float PI = std::acos(-1.0f);
  float x, y, z, xy;                            // vertex position
  float nx, ny, nz, lengthInv = 1.0f / radius_; // vertex normal

  float sectorStep = 2 * PI / sector_count_;
  float stackStep = PI / stack_count_;
  float sectorAngle, stackAngle;

  for (int i = 0; i <= stack_count_; ++i) {
    stackAngle = PI / 2 - i * stackStep; // starting from pi/2 to -pi/2
    xy = radius_ * cosf(stackAngle);     // r * cos(u)
    z = radius_ * sinf(stackAngle);      // r * sin(u)

    // add (sectorCount+1) vertices per stack
    // first and last vertices have same position and normal, but different tex
    // coords
    for (int j = 0; j <= sector_count_; ++j) {
      sectorAngle = j * sectorStep; // starting from 0 to 2pi

      // vertex position (x, y, z)
      x = xy * cosf(sectorAngle); // r * cos(u) * cos(v)
      y = xy * sinf(sectorAngle); // r * cos(u) * sin(v)
      // normalized vertex normal (nx, ny, nz)
      nx = x * lengthInv;
      ny = y * lengthInv;
      nz = z * lengthInv;
      normal_vertices_.push_back(nx);
      normal_vertices_.push_back(ny);
      normal_vertices_.push_back(nz);
    }
  }
}

void Sphere::prepareBuffer() {
  // prepare data
  fillVertices();
  fillIndices();

  glGenVertexArrays(1, &VAO_);
  glGenBuffers(1, &VBO_);
  glGenBuffers(1, &EBO_);

  // bind VAO
  glBindVertexArray(VAO_);

  // VBO buffer
  glBindBuffer(GL_ARRAY_BUFFER, VBO_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * normal_vertices_.size(),
               normal_vertices_.data(), GL_STATIC_DRAW);

  // EBO buffer
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * indices_.size(),
               indices_.data(), GL_STATIC_DRAW);

  // position attr
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        static_cast<void *>(0));
  glEnableVertexAttribArray(0);

  // unbind  buffer
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void Sphere::prepareShader() {
  static const char *vertexShaderSource =
#include "sphereShader.vert.glsl"
      ;

  static const char *fragShaderSource =
#include "sphereShader.frag.glsl"
      ;
  prepareVertexShader(this->vertex_shader_, vertexShaderSource);
  prepareFragShader(this->frag_shader_, fragShaderSource);
}

void Sphere::linkShader() {
  prepareProgram(this->shader_program_, this->vertex_shader_,
                 this->frag_shader_);
}

void Sphere::shaderClean() {
  glDeleteShader(vertex_shader_);
  glDeleteShader(frag_shader_);
}

void Sphere::programDataSet() {
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

void Sphere::drawMainloop() {
  // mainloop;
  glEnable(GL_DEPTH_TEST);

  Log << "rendering" << std::endl;

  // keep drawing
  while (!glfwWindowShouldClose(window_)) {

    FrameCount();

    processInput(window_);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // clear the screen first
    glClearColor(0.2f, 0.2f, 0.2f, 0.8f);
    glClear(GL_COLOR_BUFFER_BIT |
            GL_DEPTH_BUFFER_BIT); // 这里取决于我们希望clear什么,
    // glUseProgram(shaderProgram);

    programDataSet();

    // seeing as we only have a single VAO there's no need to bind it every
    // time, but we'll do so to keep things a bit more organized
    glBindVertexArray(VAO_);
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    // The first argument specifies the mode we want to draw in, similar to
    // glDrawArrays. The second argument is the count or number of elements
    // we'd like to draw. We specified 6 indices so we want to draw 6
    // vertices in total. The third argument is the type of the indices
    // which is of type GL_UNSIGNED_INT. The last argument allows us to
    // specify an offset in the EBO (or pass in an index array, but that is
    // when you're not using element buffer objects), but we're just going
    // to leave this at 0.
    //  glDrawElements(GL_TRIANGLES, , sizeof(vertices) /
    //  sizeof(vertices[0]));
    glDrawElements(GL_TRIANGLES, static_cast<unsigned>(indices_.size()),
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
