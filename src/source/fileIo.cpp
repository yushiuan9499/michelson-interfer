
#include "fileIo.h"
#include <QThread>
#include <QtConcurrent/QtConcurrent>
#include <fstream>
#ifdef _WIN32
#include <codecvt>
#include <locale>
#endif // _WIN32

FileIo::FileIo(QObject *parent) : QObject(parent) {}

void FileIo::readFramesAsync() {
  cap.set(cv::CAP_PROP_POS_FRAMES, 0);
  QtConcurrent::run([this]() {
    if (!cap.isOpened())
      return;
    cv::Mat frame;
    const int preAllocRows =
        static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
    const int preAllocCols =
        static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    const int preAllocType = CV_8UC3; // 假設為彩色影像
    frame.create(preAllocRows, preAllocCols, preAllocType);
    while (cap.read(frame)) {
      if (frame.empty())
        break;
      bool isLastFrame = (cap.get(cv::CAP_PROP_POS_FRAMES) ==
                          cap.get(cv::CAP_PROP_FRAME_COUNT));
      emit loadFrame(frame.clone(), isLastFrame ? 1 : 0);
      QThread::msleep(3); // simulate processing delay
    }
  });
}

cv::Mat FileIo::getFrame(int index) {
  if (!cap.isOpened())
    return cv::Mat();
  cap.set(cv::CAP_PROP_POS_FRAMES, index);
  cv::Mat frame;
  cap >> frame;
  return frame;
}

int FileIo::getFrameCount() const {
  if (!cap.isOpened())
    return 0;
  return static_cast<int>(cap.get(cv::CAP_PROP_FRAME_COUNT));
}

void FileIo::openVideo(const std::string &filename) {
  if (cap.isOpened()) {
    cap.release();
  }
  cap.open(filename);
  if (!cap.isOpened()) {
    throw std::runtime_error("Cannot open video file: " + filename);
  }
}

int writeCsv(const std::string &filename, const std::vector<double> &data) {
  int count = data.size();
#ifdef _WIN32
  std::wofstream ofs(std::wstring(filename.begin(), filename.end()));
  ofs.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
#else
  std::ofstream ofs(filename);
#endif // _WIN32
  for (const auto &val : data) {
    ofs << val << "\n";
  }
  return count;
}
