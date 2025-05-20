
#include "fileIo.h"
#include <QThread>
#include <QtConcurrent/QtConcurrent>
#include <fstream>
#include <opencv2/opencv.hpp>

FileIo::FileIo(QObject *parent) : QObject(parent) {}

void FileIo::readFramesAsync(const std::string &filename) {
  QtConcurrent::run([this, filename]() {
    cv::VideoCapture cap(filename);
    if (!cap.isOpened())
      return;
    cv::Mat frame;
    while (cap.read(frame)) {
      if (frame.empty())
        break;
      bool isLastFrame = (cap.get(cv::CAP_PROP_POS_FRAMES) ==
                          cap.get(cv::CAP_PROP_FRAME_COUNT));
      emit loadFrame(frame.clone(), isLastFrame ? 1 : 0);
      QThread::msleep(10); // simulate processing delay
    }
  });
}

int FileIo::writeCsv(const std::string &filename,
                     const std::vector<double> &data) {
  int count = data.size();
  std::ofstream ofs(filename);
  for (const auto &val : data) {
    ofs << val << "\n";
  }
  return count;
}

cv::Mat FileIo::getFrame(std::string filename, int index) const {
  cv::VideoCapture cap(filename);
  if (!cap.isOpened())
    return cv::Mat();
  cap.set(cv::CAP_PROP_POS_FRAMES, index);
  cv::Mat frame;
  cap >> frame;
  return frame;
}
int FileIo::getFrameCount(std::string filename) const {
  cv::VideoCapture cap(filename);
  if (!cap.isOpened())
    return 0;
  return static_cast<int>(cap.get(cv::CAP_PROP_FRAME_COUNT));
}
