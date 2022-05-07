#pragma once
#include "utils.h"

class Posture;
class Motion;
struct Bone;

using FK_Func = auto (*)(const Posture&, Bone*) -> void;
using MW_Func = auto (*)(const Motion&, int, int) -> Motion;

CAHW_API int test(FK_Func, MW_Func);
