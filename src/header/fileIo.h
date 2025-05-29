#ifndef FILE_IO_H
#define FILE_IO_H

#include <QObject>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

class FileIo : public QObject {
  Q_OBJECT;

private:
  cv::VideoCapture cap;

public:
  explicit FileIo(QObject *parent = nullptr);

  void openVideo(const std::string &filename);
  cv::Mat getFrame(int index);
  int getFrameCount() const;
  void readFramesAsync();
  int writeCsv(const std::string &filename, const std::vector<double> &data);

signals:
  void loadFrame(const cv::Mat &frame, const bool isLastFrame);
};
#endif // FILE_IO_H
