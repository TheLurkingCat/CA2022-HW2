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
#include "test.h"

int alignSize = 256;

int uboAlign(int i) { return ((i + 1 * (alignSize - 1)) / alignSize) * alignSize; }

int test(FK_Func fk, MW_Func mw) {
  // Initialize OpenGL context.
  OpenGLContext& context = OpenGLContext::getContext();
  GLFWwindow* window = context.createWindow("HW2", 1, 1, GLFW_OPENGL_CORE_PROFILE);

  Skeleton skeleton(findPath("skeleton.asf"), 0.4f);

  Skeleton skeletonStudent(skeleton);
  Skeleton skeletonTA(skeleton);
  constexpr int motionWarpParam[3][2] = {{160, 150}, {175, 135}, {200, 100}};
  Motion motions[3] = {Motion(findPath("punch_kick.amc"), skeleton), Motion(findPath("walk.amc"), skeleton),
                       Motion(findPath("running.amc"), skeleton)};
  // Test FK
  for (int i = 0; i < 3; ++i) {
    bool pass = true;
    for (int j = 0; pass && j < motions[i].size(); ++j) {
      forwardKinematicsTA(motions[i].posture(j), skeletonTA.bone(0));
      fk(motions[i].posture(j), skeletonStudent.bone(0));
      pass &= (skeletonTA == skeletonStudent);
    }
    if (pass)
      std::cerr << ", 1";
    else
      std::cerr << ", 0";
  }
  // Test time warp
  for (int i = 0; i < 3; ++i) {
    bool pass = (motionWarpTA(motions[0], motionWarpParam[i][0], motionWarpParam[i][1]) ==
                 mw(motions[0], motionWarpParam[i][0], motionWarpParam[i][1]));
    if (pass)
      std::cerr << ", 1";
    else
      std::cerr << ", 0";
  }
  std::cerr << std::endl;
  glfwDestroyWindow(window);
  return 0;
}
