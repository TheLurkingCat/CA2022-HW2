#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <GLFW/glfw3.h>
#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#undef GLAD_GL_IMPLEMENTATION

#include "hw2.h"
int alignSize = 256;
bool isWindowSizeChanged = true;
bool mouseBinded = false;

int uboAlign(int i) { return ((i + 1 * (alignSize - 1)) / alignSize) * alignSize; }

void keyCallback(GLFWwindow* window, int key, int, int action, int) {
  // There are three actions: press, release, hold
  if (action != GLFW_PRESS) return;
  // Press ESC to close the window.
  if (key == GLFW_KEY_ESCAPE) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
    return;
  } else if (key == GLFW_KEY_F9) {
    // Disable / enable mouse cursor.
    if (mouseBinded)
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    else
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    mouseBinded = !mouseBinded;
  }
}

void framebufferSizeCallback(GLFWwindow*, int width, int height) {
  // Minimize event guard
  if (width == 0 && height == 0) return;
  windowWidth = width;
  windowHeight = height;
  glViewport(0, 0, width, height);
  isWindowSizeChanged = true;
}

int main() {
  // Initialize OpenGL context.
  OpenGLContext& context = OpenGLContext::getContext();
  GLFWwindow* window = context.createWindow("HW2", 1280, 720, GLFW_OPENGL_CORE_PROFILE);
  glfwSetKeyCallback(window, keyCallback);
  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

#ifndef NDEBUG
  context.printSystemInfo();
  context.enableDebugCallback();
#endif

  glfwSwapInterval(1);
  glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
  glViewport(0, 0, windowWidth, windowHeight);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glClearColor(0, 0, 0, 1);
  glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &alignSize);

  speedMultiplier = (240 / context.getRefreshRate());
  GUI gui(window, context.getOpenGLVersion());
  // Initialize shaders
  ShaderProgram renderer;
  {
    VertexShader vs;
    FragmentShader fs;
    vs.fromFile(findPath("render.vert"));
    fs.fromFile(findPath("render.frag"));
    renderer.attach(&vs, &fs);
    renderer.link();
    renderer.detach(&vs, &fs);
    renderer.use();
    renderer.uniformBlockBinding("model", 0);
    renderer.uniformBlockBinding("camera", 1);
  }

  Camera camera(Eigen::Vector4f(0, 2, -10, 1));
  UniformBuffer cameraUBO;
  cameraUBO.allocate(uboAlign(20 * sizeof(GLfloat)));
  cameraUBO.load(0, 16 * sizeof(GLfloat), camera.viewProjectionMatrix().data());
  cameraUBO.load(16 * sizeof(GLfloat), 4 * sizeof(GLfloat), camera.position().data());
  cameraUBO.bindUniformBlockIndex(1, 0, uboAlign(20 * sizeof(GLfloat)));

  while (!glfwWindowShouldClose(window)) {
    // Polling events.
    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    bool cameraChanged = mouseBinded ? camera.move(window) : false;
    if (isWindowSizeChanged) {
      isWindowSizeChanged = false;
      camera.updateProjection();
      cameraChanged = true;
    }
    if (cameraChanged) {
      cameraUBO.load(0, 16 * sizeof(GLfloat), camera.viewProjectionMatrix().data());
      cameraUBO.load(16 * sizeof(GLfloat), 4 * sizeof(GLfloat), camera.position().data());
    }
    gui.render();
#ifdef __APPLE__
    glFlush();
#endif
    glfwSwapBuffers(window);
  }
  glfwDestroyWindow(window);
  return 0;
}
