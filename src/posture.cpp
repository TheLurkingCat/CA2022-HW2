#include "posture.h"

#include <utility>

Posture::Posture(const std::size_t size) noexcept :
    rotations(size, Eigen::Quaternionf::Identity()), translations(size, Eigen::Vector3f::Zero()) {}

bool Posture::operator==(const Posture& other) const noexcept {
  bool pass = true;
  for (int i = 0; pass && i < rotations.size(); ++i) {
    pass &= ((translations[i] - other.translations[i]).norm() < 1e-3f);
    float rotDiff = std::acos(std::abs(rotations[i].dot(other.rotations[i])));
    pass &= (rotDiff < 1e-3f || rotDiff != rotDiff);
  }
  return pass;
}
