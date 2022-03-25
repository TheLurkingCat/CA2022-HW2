#pragma once
#include <string>
#include <vector>

#include "bone.h"

class Skeleton final {
 public:
  explicit Skeleton(const std::string &filename, float scale_ = 0.2f) noexcept;
  /**
   * @brief Get skeleton's scale
   */
  float scale() const { return _scale; }
  /**
   * @brief Get total bones in the skeleton
   */
  int size() const { return static_cast<int>(bones.size()); }
  // get total movable bones in the skeleton
  int movableBoneNum() const { return _movableBones; }
  // get specific bone by its index
  Bone *bone(int bone_idx) { return &bones[bone_idx]; }
  const Bone *const bone(int bone_idx) const { return &bones[bone_idx]; }
  Bone *bone(const std::string &name);
  const Bone *const bone(const std::string &name) const;
  // set bone's model matrices (for rendering)
  void setModelMatrix(Eigen::Ref<Eigen::Matrix4Xf> modelMatrix);

 private:
  bool readASFFile(const std::string &file_name);
  // This function sets sibling or child for parent bone
  // If parent bone does not have a child,
  // then pChild is set as parent's child
  // else pChild is set as a sibling of parents already existing child
  int setChildrenSibling(Bone *parent, Bone *child);
  // Transform the direction vector (dir),
  // which is defined in character's global coordinate system in the ASF file,
  // to local coordinate
  void rotateLocalCoordinate();
  void computeRotationParent2Child(Bone *parent, Bone *child);
  void computeRotation2Parent(Bone *bone);

  float _scale = 0.2;
  int _movableBones = 1;
  std::vector<Bone> bones = std::vector<Bone>(1);
};
