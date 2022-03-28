#!/bin/bash
if [[ !(-d src) ]]; then
  cd ..
fi

if [[ !(-d src) ]]; then
  echo "Please run this script in project root folder."
  exit 1
fi

git apply --ignore-whitespace script/homework.patch \
&& zip -qr9 HW2.zip \
extern \
cmake \
HW2 \
include \
assets \
src \
.clang-format \
CMakeLists.txt \
HW2.sln \
README.md \
&& git apply -R --ignore-whitespace script/homework.patch
