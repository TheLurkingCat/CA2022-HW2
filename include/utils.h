#pragma once
#include <stdexcept>
#include <string>

#include <Eigen/Core>
#include <Eigen/Geometry>

#ifndef DELETE_COPY
#define DELETE_COPY(ClassName)           \
  ClassName(const ClassName &) = delete; \
  ClassName &operator=(const ClassName &) = delete;
#endif

#ifndef DEFAULT_COPY
#define DEFAULT_COPY(ClassName)           \
  ClassName(const ClassName &) = default; \
  ClassName &operator=(const ClassName &) = default;
#endif

#ifndef DELETE_MOVE
#define DELETE_MOVE(ClassName)      \
  ClassName(ClassName &&) = delete; \
  ClassName &operator=(ClassName &&) = delete;
#endif

#ifndef DEFAULT_MOVE
#define DEFAULT_MOVE(ClassName)      \
  ClassName(ClassName &&) = default; \
  ClassName &operator=(ClassName &&) = default;
#endif

#ifndef MOVE_ONLY
#define MOVE_ONLY(ClassName) \
  DELETE_COPY(ClassName)     \
  DEFAULT_MOVE(ClassName)
#endif

// Some useful C++ 20 feature
#if __cplusplus >= 202002L && !defined(__APPLE__)
#ifndef CONSTEXPR_VIRTUAL
#define CONSTEXPR_VIRTUAL constexpr
#endif
#endif

// Fallbacks
#ifndef CONSTEXPR_VIRTUAL
#define CONSTEXPR_VIRTUAL
#endif

// From https://gcc.gnu.org/wiki/Visibility
#ifdef CAHW_DLL
#if defined _WIN32 || defined __CYGWIN__
#ifdef CAHW_BUILDING_DLL
#ifdef __GNUC__
#define CAHW_API __attribute__((dllexport))
#else
#define CAHW_API __declspec(dllexport)  // Note: actually gcc seems to also supports this syntax.
#endif
#else
#ifdef __GNUC__
#define CAHW_API __attribute__((dllimport))
#else
#define CAHW_API __declspec(dllimport)  // Note: actually gcc seems to also supports this syntax.
#endif
#endif
#else
#if __GNUC__ >= 4
#define CAHW_API __attribute__((visibility("default")))
#else
#define CAHW_API
#endif
#endif
#endif

Eigen::Quaternionf CAHW_API rotateZYX(const Eigen::Ref<const Eigen::Vector3f> &rotation);
Eigen::Quaternionf CAHW_API rotateXYZ(const Eigen::Ref<const Eigen::Vector3f> &rotation);

constexpr float toRadians(double x) { return static_cast<float>(x * EIGEN_PI / 180); }
Eigen::Matrix4f lookAt(const Eigen::Ref<const Eigen::Vector3f> &position,
                       const Eigen::Ref<const Eigen::Vector3f> &front,
                       const Eigen::Ref<const Eigen::Vector3f> &up);
Eigen::Matrix4f perspective(float fov, float aspect, float zNear, float zFar);
Eigen::Matrix4f ortho(float left, float right, float bottom, float top, float zNear, float zFar);
std::string findPath(const std::string &filename);
