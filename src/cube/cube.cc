#include "cube.hpp"
#include "GLFW/glfw3.h"
#include <exception>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>

static constexpr int BUFFER_SIZE = 512;

static char infoLog[BUFFER_SIZE];

static void framebuffer_size_callback(GLFWwindow *w, int const width,
                                      int const height) {
  glViewport(0, 0, width, height);
}

void Cube::processInput(GLFWwindow *w) {
  // 如如果没有按下, 这里会返回RELEASE
  int const ret = glfwGetKey(w, GLFW_KEY_Q);
  if (ret) {
    glfwSetWindowShouldClose(w, true);
    std::cout << "got quit signal" << std::endl;
  }
}

Cube::~Cube() { glfwTerminate(); }

Cube::Cube() {
  dynamic_vertices = std::make_unique<float[]>(vertices_size);
  for (int i = 0; i < vertices_size; i++)
    dynamic_vertices[i] = static_vertices[i];
}

void Cube::updateVertices() {
  auto const time = static_cast<float>(glfwGetTime());
  float const t = 0.25f * std::cos(time * 2) + 0.75;

  // VBO buffer
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

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
  static auto constexpr aspect = static_cast<float>(1.0 * SCR_W / SCR_H);

  // M this changes
  auto const model = glm::rotate(I, rotate_angle, rotate_axle);

  // V we do not transform
  static auto const view = glm::translate(I, glm::vec3(0.0f, 0.0f, -3.0f));

  // P unchanged
  static auto const projection =
      glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);

  // remeber to use
  glUseProgram(this->shaderProgram);

  static auto const model_location =
      glGetUniformLocation(this->shaderProgram, "model");
  static auto const view_location =
      glGetUniformLocation(this->shaderProgram, "view");
  static auto const projection_location =
      glGetUniformLocation(this->shaderProgram, "projection");

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
  while (!glfwWindowShouldClose(window)) {

    float const current_time = static_cast<float>(glfwGetTime());
    frame_count++;

    if (current_time - last_time >= 1.0) {
      std::clog << "[INFO]FPS: " << frame_count << '\r';
      frame_count = 0;
      last_time = current_time;
    }

    processInput(window);

    // clear the screen first
    glClearColor(0.2f, 0.2f, 0.2f, 0.8f);
    glClear(GL_COLOR_BUFFER_BIT |
            GL_DEPTH_BUFFER_BIT); // 这里取决于我们希望clear什么,
                                  // 这里我们只关心 color
    // glUseProgram(shaderProgram);

    programDataSet();

    // seeing as we only have a single VAO there's no need to bind it every
    // time, but we'll do so to keep things a bit more organized
    glBindVertexArray(VAO);
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

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // release VAO, VBO, EBO, program
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteProgram(shaderProgram);
}

void Cube::Draw() {

  prepareWindow();

  prepareGLAD();

  prepareShader();

  linkShader();

  //  Oh yeah, and don't forget to delete the shader objects once we've linked
  //  them into the program object; we no longer need them anymore:
  shaderClean();

  prepareBuffer();

  prepareTexture();

  drawMainloop();
}

void Cube::shaderClean() {
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

void Cube::prepareBuffer() {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  // bind VAO
  glBindVertexArray(VAO);

  // VBO buffer
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(static_vertices), dynamic_vertices.get(),
               GL_DYNAMIC_DRAW);

  // EBO buffer
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
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

void Cube::prepareGLAD() {
  // initialize GLAD
  // NOTE: 这实际上通过glad, 用于加载函数指针, gl的实现实际上是在运行时确定的
  // 函数地址, 相当于开头的一个loader可以方便后续的流程, 否则需要一个一个typedef
  // 再加上调用wglgetprocaddress去得到指针
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    Log << "Failed to initialize GLAD" << std::endl;
    exit(-1);
  }
}

void Cube::prepareWindow() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // create a window object
  window = glfwCreateWindow(SCR_W, SCR_H, "LearnOpenGL", nullptr, nullptr);
  if (!window) {
    Log << "Failed to create GLFW window" << std::endl;
    throw std::runtime_error(
        "An error occurred! refer to the log for more information");
  }
  // This function makes the OpenGL or OpenGL ES context of the specified window
  // current on the calling thread. It can also detach the current context from
  // the calling thread without making a new one current by passing in NULL.
  // NOTE: 从这里其实可以看出opengl的设计, attach与dettach往往是共用同一个接口的
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

void Cube::prepareProgram(unsigned &p, unsigned const vshader,
                          unsigned const fshader) {
  int success;
  // 这里是为了把所有的shader串联起来
  p = glCreateProgram();
  glAttachShader(p, vshader);
  glAttachShader(p, fshader);
  glLinkProgram(p);

  glGetProgramiv(p, GL_LINK_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(p, BUFFER_SIZE, nullptr, infoLog);
    Log << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    throw std::runtime_error(
        "An error occurred! refer to the log for more information");
  }
  Log << "INFO: shader link successfully" << std::endl;
}

void Cube::linkShader() {
  prepareProgram(this->shaderProgram, this->vertexShader, this->fragmentShader);
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

  prepareVertexShader(this->vertexShader, vertexShaderSource);
  prepareFragShader(this->fragmentShader, fragmentShaderSource);
}

void Cube::prepareFragShader(unsigned &f, char const *src) {
  f = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(f, 1, &src, nullptr);
  glCompileShader(f);

  int success = -1;
  glGetShaderiv(f, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(f, 512, nullptr, infoLog);
    Log << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
        << infoLog << std::endl;
    throw std::runtime_error(
        "An error occurred! refer to the log for more information");
  }
  Log << "INFO: fragment shader compiled successfully" << std::endl;
}

void Cube::prepareVertexShader(unsigned &v, char const *src) {
  // vertex shader object
  v = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(v, 1, &src, nullptr);
  // compile vertex shader
  glCompileShader(v);

  int success = -1;
  glGetShaderiv(v, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(v, BUFFER_SIZE, nullptr, infoLog);
    Log << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
        << infoLog << std::endl;
    throw std::runtime_error(
        "An error occurred! refer to the log for more information");
  }
  Log << "INFO: vertex shader compiled successfully" << std::endl;
}
