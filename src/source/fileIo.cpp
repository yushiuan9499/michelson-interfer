
#include "fileIo.h"
#include <QThread>
#include <QtConcurrent/QtConcurrent>
#include <fstream>
#include <opencv2/opencv.hpp>

FileIo::FileIo(QObject *parent) : QObject(parent) {}

/*
 * 讀取影片
 * @param filename: 影片檔名
 * @emit: loadFrame(cv::Mat, int): 讓anaylzer分析影片
 */
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

/*
 * 寫入CSV檔案
 * @param filename: CSV檔名
 * @param data: 寫入的資料
 * @return: 寫入的資料筆數
 */
int FileIo::writeCsv(const std::string &filename,
                     const std::vector<double> &data) {
  int count = data.size();
  std::ofstream ofs(filename);
  for (const auto &val : data) {
    ofs << val << "\n";
  }
  return count;
}

/*
 * 取得特定幀
 * @param filename: 影片檔名
 * @param index: 幀數編號，0-indexed
 * @return: 取得的幀
 */
cv::Mat FileIo::getFrame(std::string filename, int index) const {
  cv::VideoCapture cap(filename);
  if (!cap.isOpened())
    return cv::Mat();
  cap.set(cv::CAP_PROP_POS_FRAMES, index);
  cv::Mat frame;
  cap >> frame;
  return frame;
}
/*
 * 取得影片幀數
 * @param filename: 影片檔名
 * @return: 影片幀數
 */
int FileIo::getFrameCount(std::string filename) const {
  cv::VideoCapture cap(filename);
  if (!cap.isOpened())
    return 0;
  return static_cast<int>(cap.get(cv::CAP_PROP_FRAME_COUNT));
}
