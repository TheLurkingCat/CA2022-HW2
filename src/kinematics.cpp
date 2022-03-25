#include "kinematics.h"

#include "utils.h"
void forwardKinematics(const Posture& posture, Bone* bone) {
  // TODO
  // This function will be called with bone == root bone of the skeleton
  // You should set these variables:
  //     bone->startPosition = Eigen::Vector3f::Zero();
  //     bone->endPosition = Eigen::Vector3f::Zero();
  //     bone->rotation = Eigen::Quaternionf::Identity();
  // The sample above just set everything to initial state
  while (bone != nullptr) {
    if (bone->parent == nullptr) {
      bone->startPosition = bone->endPosition = posture.translations[0];
      bone->rotation = posture.rotations[0];
    } else {
      bone->rotation = bone->parent->rotation * bone->rotationParentCurrent.rotation() * posture.rotations[bone->idx];
      bone->startPosition = bone->endPosition = bone->parent->endPosition;
      bone->endPosition += posture.translations[bone->idx];
      bone->endPosition += bone->rotation * bone->direction * bone->length;
    }
    forwardKinematics(posture, bone->child);
    bone = bone->sibling;
  }
}

Motion timewarping(const Motion& motion, int oldKeyframe, int newKeyframe) {
  Motion newMotion = motion;
  int totalFrames = static_cast<int>(motion.size());
  int totalBones = static_cast<int>(motion.posture(0).rotations.size());
  for (int i = 0; i < totalFrames; ++i) {
    for (int j = 0; j < totalBones; ++j) {
      // TODO
      // You should set these variables:
      // newMotion.posture(i).translations[j] = Eigen::Vector3f::Zero();
      // newMotion.posture(i).rotations[j] = Eigen::Quaternionf::Identity();
      // The sample above just set to initial state
    }
  }

  float beforeMid = static_cast<float>(oldKeyframe) / newKeyframe;
  float afterMid = static_cast<float>(totalFrames - oldKeyframe) / (totalFrames - newKeyframe);
  for (int i = 0; i < totalFrames; ++i) {
    float index = i <= newKeyframe ? i * beforeMid : oldKeyframe + afterMid * (i - newKeyframe);

    int left = std::max(0, static_cast<int>(std::floor(index)));
    int right = std::min(totalFrames - 1, static_cast<int>(std::ceil(index)));
    float step = index - left;
    for (int j = 0; j < totalBones; ++j) {
      newMotion.posture(i).translations[j] =
          (1 - step) * newMotion.posture(left).translations[j] + step * newMotion.posture(right).translations[j];

      Eigen::Quaternionf first = newMotion.posture(left).rotations[j];
      Eigen::Quaternionf secend = newMotion.posture(right).rotations[j];
      newMotion.posture(i).rotations[j] = first.slerp(step, secend);
    }
  }

  return newMotion;
}
