#!/bin/bash

# 檢查套件
if ! dpkg -s libqt5charts5-dev qtbase5-dev qt5-qmake >/dev/null 2>&1; then
  echo "缺少 Qt5 相關套件，請執行：sudo apt install libqt5charts5-dev qtbase5-dev qt5-qmake"
fi
if ! dpkg -s libopencv-dev >/dev/null 2>&1; then
  echo "缺少 OpenCV，請執行：sudo apt install libopencv-dev"
fi

# 創建必要的資料夾
mkdir -p build bin
cd build
mkdir -p obj moc

# 編譯
qmake ../project.pro
if ! command -v bear >/dev/null 2>&1; then
  make
else
  bear -- make
fi
