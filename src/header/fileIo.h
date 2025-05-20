#ifndef FILE_IO_H
#define FILE_IO_H

#include <QObject>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

class FileIo : public QObject {
  Q_OBJECT;

public:
  explicit FileIo(QObject *parent = nullptr);

  void readFramesAsync(const std::string &filename);
  int writeCsv(const std::string &filename, const std::vector<double> &data);

signals:
  void loadFrame(const cv::Mat &frame, const bool isLastFrame);
};
#endif // FILE_IO_H
