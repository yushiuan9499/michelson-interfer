#ifndef FILE_IO_H
#define FILE_IO_H

#include <QObject>
#include <opencv2/core.hpp>
#include <string>
#include <vector>

class FileIo : public QObject {
  Q_OBJECT;

public:
  explicit FileIo(QObject *parent = nullptr);

  cv::Mat getFrame(std::string filename, int index) const;
  int getFrameCount(std::string filename) const;
  void readFramesAsync(const std::string &filename);
  int writeCsv(const std::string &filename, const std::vector<double> &data);

signals:
  void loadFrame(const cv::Mat &frame, const bool isLastFrame);
};
#endif // FILE_IO_H
