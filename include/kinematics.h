#pragma once

#include "bone.h"
#include "motion.h"
#include "posture.h"

void forwardKinematics(const Posture& posture, Bone* root);
Motion timewarping(const Motion& motion, int oldKeyframe, int newKeyframe);
