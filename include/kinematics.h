#pragma once

#include "bone.h"
#include "motion.h"
#include "posture.h"
void forwardKinematics(const Posture& posture, Bone* root);
Motion motionWarp(const Motion& motion, int oldKeyframe, int newKeyframe);
Motion motionBlend(const Motion& motionA, const Motion& motionB);

void forwardKinematicsTA(const Posture& posture, Bone* root);
Motion motionWarpTA(const Motion& motion, int oldKeyframe, int newKeyframe);
Motion motionBlendTA(const Motion& motionA, const Motion& motionB);
