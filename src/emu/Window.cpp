#include "Window.h"

#include <string>

#include <GL/gl.h>
#include <GLFW/glfw3.h>

Window::Window(std::string title, int width, int height) {
  if (!glfwInit()) {
  }

  // Set basic window hints
  glfwWindowHint(GLFW_DOUBLEBUFFER, 1);
  glfwWindowHint(GLFW_DEPTH_BITS, 1);
  glfwWindowHint(GLFW_STENCIL_BITS, 8);

  // Set OpenGL version information and profile
  std::string glsl_version;
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glsl_version = "#version 150";

  // Required on macOS
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#elif __linux__
  glsl_version = "#version 150";

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#elif _WIN32
  glsl_version = "#version 130";

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
#endif

  // HiDPI scaling
#ifdef _WIN32
  GLFWmonitor *monitor = glfwGetPrimaryMonitor();
  float scale_x;
  float scale_y;
  glfwGetMonitorContentScale(monitor, &scale_x, &scale_y);

  if (scale_x > 1 || scale_y > 1) {
    hidpi_scale_factor = scale_x;
    glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
  }
#elif __APPLE__
  // Prevent framebuffer size doubling for Retina screens
  glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);
#endif

  // Create window
  window_ = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
  if (!window_) {
  }

  // Watch window resizing
  glfwSetFramebufferSizeCallback(window_, framebufferResizeCallback);

  // Set context
  glfwMakeContextCurrent(window_);

  // Enable VSync
  glfwSwapInterval(1);

  // Set initial GL viewport
  int actual_width;
  int actual_height;
  glfwGetWindowSize(window_, &actual_width, &actual_height);
  this->framebufferResizeCallback(window_, actual_width, actual_height);
}

Window::~Window() {
  if (window_ != nullptr) {
    glfwDestroyWindow(window_);
  }
  glfwTerminate();
}

bool Window::running() { return !glfwWindowShouldClose(window_); }

void Window::framebufferResizeCallback(GLFWwindow *window, int width,
                                       int height) {
  glViewport(0, 0, width, height);
}
