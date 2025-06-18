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

  /*
   * 開啟影片檔案
   * @param filename: 影片檔名
   */
  void openVideo(const std::string &filename);
  /**
   * 取得特定幀
   * @param index: 幀數編號，0-indexed
   * @return: 取得的幀
   */
  cv::Mat getFrame(int index);

  /**
   * 取得影片幀數
   * @return: 影片幀數
   */
  int getFrameCount() const;

  /**
   * 讀取影片
   */
  void readFramesAsync();

signals:
  void loadFrame(const cv::Mat &frame, const bool isLastFrame);
};
/**
 * 寫入CSV檔案
 * @param filename: CSV檔名
 * @param data: 寫入的資料
 * @return: 寫入的資料筆數
 */
int writeCsv(const std::string &filename, const std::vector<double> &data);
#endif // FILE_IO_H
