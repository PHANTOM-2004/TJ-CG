#include "elements.hpp"
#include <exception>

using mygldraw::Elements;

void Elements::FrameCount() {
  static unsigned frame_count = 0;
  static float last_time = 0;

  float const current_time = static_cast<float>(glfwGetTime());
  frame_count++;

  if (current_time - last_time >= 1.0) {
    std::clog << "[INFO]FPS: " << frame_count << '\r';
    frame_count = 0;
    last_time = current_time;
  }
}


static void framebuffer_size_callback(GLFWwindow *w, int const width,
                                      int const height) {
  glViewport(0, 0, width, height);
}

void Elements::processInput(GLFWwindow *w) {
  // 如如果没有按下, 这里会返回RELEASE
  int const ret = glfwGetKey(w, GLFW_KEY_Q);
  if (ret) {
    glfwSetWindowShouldClose(w, true);
    std::cout << "got quit signal" << std::endl;
  }
}

Elements::Elements(int const w, int const h) : width_(w), height_(h) {}

Elements::~Elements() { glfwTerminate(); }

void Elements::Draw() {

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

void Elements::prepareGLAD() {
  // initialize GLAD
  // NOTE: 这实际上通过glad, 用于加载函数指针, gl的实现实际上是在运行时确定的
  // 函数地址, 相当于开头的一个loader可以方便后续的流程, 否则需要一个一个typedef
  // 再加上调用wglgetprocaddress去得到指针
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    Log << "Failed to initialize GLAD" << std::endl;
    std::terminate();
  }
}

void Elements::prepareWindow() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // create a window object
  window_ = glfwCreateWindow(width_, height_, "LearnOpenGL", nullptr, nullptr);
  if (!window_) {
    Log << "Failed to create GLFW window" << std::endl;
    throw std::runtime_error(
        "An error occurred! refer to the log for more information");
  }
  // This function makes the OpenGL or OpenGL ES context of the specified window
  // current on the calling thread. It can also detach the current context from
  // the calling thread without making a new one current by passing in NULL.
  // NOTE: 从这里其实可以看出opengl的设计, attach与dettach往往是共用同一个接口的
  glfwMakeContextCurrent(window_);
  glfwSetFramebufferSizeCallback(window_, framebuffer_size_callback);
}
