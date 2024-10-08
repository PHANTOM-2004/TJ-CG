#include "elements.hpp"

constexpr int BUFFER_SIZE = 512;
static char infoLog[BUFFER_SIZE];

using mygldraw::Elements;

void Elements::prepareFragShader(unsigned &f, char const *src) {
  f = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(f, 1, &src, nullptr);
  glCompileShader(f);

  int success = -1;
  glGetShaderiv(f, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(f, BUFFER_SIZE, nullptr, infoLog);
    Log << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
        << infoLog << std::endl;
    throw std::runtime_error(
        "An error occurred! refer to the log for more information");
  }
  Log << "INFO: fragment shader compiled successfully" << std::endl;
}

void Elements::prepareVertexShader(unsigned &v, char const *src) {
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

void Elements::prepareProgram(unsigned &p, unsigned const vshader,
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
