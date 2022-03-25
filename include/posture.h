#pragma once

#include <vector>

#include <Eigen/Core>
#include <Eigen/Geometry>

class Posture final {
 public:
  Posture() noexcept = default;
  explicit Posture(const std::size_t size) noexcept;

  std::vector<Eigen::Quaternionf> rotations;
  std::vector<Eigen::Vector3f> translations;
};
