#pragma once

#include <vector>

#include <Eigen/Core>
#include <Eigen/Geometry>
#include "utils.h"

struct CAHW_API Posture {
  Posture() noexcept = default;
  explicit Posture(const std::size_t size) noexcept;
  bool operator==(const Posture& other) const noexcept;

  std::vector<Eigen::Quaternionf> rotations;
  std::vector<Eigen::Vector3f> translations;
};
