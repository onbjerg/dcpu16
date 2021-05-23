#pragma once
#include <string>

#include <GL/gl.h>
#include <GLFW/glfw3.h>

class Window {
public:
  Window(const Window &) = delete;
  Window(std::string title, int width, int height);
  ~Window();

  bool running();

private:
  static void framebufferResizeCallback(GLFWwindow *window, int width,
                                        int height);

  GLFWwindow *window_ = nullptr;
  float hidpi_scale_factor = 1;
};
