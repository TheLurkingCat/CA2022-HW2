#include "kinematics.h"

#include <algorithm>

#include "utils.h"
void forwardKinematics(const Posture& posture, Bone* bone) {
  // TODO (FK)
  // You should set these variables:
  //     bone->startPosition = Eigen::Vector3f::Zero();
  //     bone->endPosition = Eigen::Vector3f::Zero();
  //     bone->rotation = Eigen::Quaternionf::Identity();
  // The sample above just set everything to initial state
  // Hint:
  //   1. posture.translations, posture.rotations
  // Note:
  //   1. This function will be called with bone == root bone of the skeleton
  while (bone != nullptr) {
    if (bone->parent == nullptr) {
      bone->startPosition = bone->endPosition = posture.translations[0];
      bone->rotation = posture.rotations[0];
    } else {
      bone->rotation = bone->parent->rotation * bone->rotationParentCurrent * posture.rotations[bone->idx];
      bone->startPosition = bone->endPosition = bone->parent->endPosition;
      bone->endPosition += posture.translations[bone->idx];
      bone->endPosition += bone->rotation * bone->direction * bone->length;
    }
    forwardKinematics(posture, bone->child);
    bone = bone->sibling;
  }
}

Motion motionWarp(const Motion& motion, int oldKeyframe, int newKeyframe) {
  Motion newMotion = motion;
  int totalFrames = static_cast<int>(motion.size());
  int totalBones = static_cast<int>(motion.posture(0).rotations.size());
  for (int i = 0; i < totalFrames; ++i) {
    for (int j = 0; j < totalBones; ++j) {
      // TODO (Time warping)
      // original: |--------------|---------------|
      // new     : |------------------|-----------|
      // OR
      // original: |--------------|---------------|
      // new     : |----------|-------------------|
      // You should set these variables:
      //     newMotion.posture(i).translations[j] = Eigen::Vector3f::Zero();
      //     newMotion.posture(i).rotations[j] = Eigen::Quaternionf::Identity();
      // The sample above just set to initial state
      // Hint:
      //   1. Your should scale the frames before and after key frames.
      //   2. You can use linear interpolation with translations.
      //   3. You should use spherical linear interpolation for rotations.
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

Motion motionBlend(const Motion& motionA, const Motion& motionB) {
  Motion newMotion;
  constexpr int blendFrameCount = 20;
  constexpr float blendFactor = 1.0f / blendFrameCount;
  constexpr int matchRange = 10;
  float difference[matchRange] = {};
  // TODO (Bonus)
  // motionA: |--------------|--matchRange--|--blendFrameCount--|
  // motionB:                               |--blendFrameCount--|--------------|
  // The starting frame of `blendFrameCount` can be in `matchRange`
  // Hint:
  //   1. Find motionB's starting posture
  //   2. Match it with the minimum cost posture in `matchRange`
  //   3. Find to translation and rotation offset between matched motionA and motionB's start
  //   4. Begin from the matched frame, blend `blendFrameCount` of frames,
  //      with a blendFactor from 1 / `blendFrameCount` to 1
  //   5. Add remaining motionB to newMotion
  // Note:
  //   1. The offset found in 3 should apply to 4 and 5
  //   2. A simple cost function is translation offsets between two posture.
  //   3. A better one considered both translations and rotations.
  //   4. Your animation should smoothly change from motionA to motionB.
  //   5. You can adjust those `constexpr`s above by yourself if you need.
  newMotion.posture().reserve(motionA.size() + motionB.size());
  const Posture& firstB = motionB.posture(0);
  int boneNum = static_cast<int>(motionA.posture(0).rotations.size());
  int start = motionA.size() - matchRange - blendFrameCount;
  // Unaffected motionA
  for (int i = 0; i < start; ++i) newMotion.posture().emplace_back(motionA.posture(i));
  // Motion matching
  for (int i = 0; i < matchRange; ++i) {
    for (int j = 1; j < boneNum; ++j) {
      difference[i] += (motionA.posture(start + i).translations[j] - firstB.translations[j]).norm();
      difference[i] += 2 * std::acos(std::abs(motionA.posture(start + i).rotations[j].dot(firstB.rotations[j])));
    }
  }
  int blendStart = static_cast<int>(std::min_element(difference, difference + matchRange) - difference);
  // Unaffected motionA
  for (int i = 0; i < blendStart; ++i) newMotion.posture().emplace_back(motionA.posture(start + i));
  const Posture& firstA = motionA.posture(start + blendStart);
  Eigen::Vector3f rootTranslate = firstA.translations[0] - firstB.translations[0];
  Eigen::Quaternionf rootRotation = firstB.rotations[0].inverse() * firstA.rotations[0];
  // Blending
  for (int i = 0; i < blendFrameCount; ++i) {
    Posture& temp = newMotion.posture().emplace_back(boneNum);
    float currentFactor = 0.5f + std::sin(EIGEN_PI * blendFactor * i - EIGEN_PI / 2) / 2;
    // root
    temp.translations[0] = (1 - currentFactor) * motionA.posture(start + blendStart + i).translations[0] +
                           currentFactor * (motionB.posture(i).translations[0] + rootTranslate);
    temp.rotations[0] = motionA.posture(start + blendStart + i)
                            .rotations[0]
                            .slerp(currentFactor, motionB.posture(i).rotations[0] * rootRotation);
    for (int j = 1; j < boneNum; ++j) {
      temp.translations[j] = (1 - currentFactor) * motionA.posture(start + blendStart + i).translations[j] +
                             currentFactor * motionB.posture(i).translations[j];
      temp.rotations[j] =
          motionA.posture(start + blendStart + i).rotations[j].slerp(currentFactor, motionB.posture(i).rotations[j]);
    }
  }
  // Unaffected motionB
  for (int i = blendFrameCount; i < motionB.size(); ++i) {
    Posture& temp = newMotion.posture().emplace_back(motionB.posture(i));
    temp.translations[0] = motionB.posture(i).translations[0] + rootTranslate;
    temp.rotations[0] = motionB.posture(i).rotations[0] * rootRotation;
  }
  return newMotion;
}
