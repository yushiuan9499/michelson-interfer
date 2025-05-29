#include "analyzer.h"
#include <opencv2/ximgproc.hpp>

Analyzer::Analyzer(QObject *parent)
    : QObject(parent), roi(new cv::Rect{-1, -1, 20, 20}) {}

/*
 * 設定ROI中心
 * @param x: ROI中央x座標
 * @param y: ROI中央y座標
 */
void Analyzer::setRoiCenter(int x, int y) {
  roi->x = x - roi->width / 2;
  roi->y = y - roi->height / 2;
}
/*
 * 設定ROI大小
 * @param size: ROI方形的邊長
 */
void Analyzer::setRoiSize(int size) {
  if (size <= 0) {
    size = 20; // Default size if invalid
  }
  // Update the existing ROI rectangle size
  roi->width = size;
  roi->height = size;
}
/*
 * 清空資料
 */
void Analyzer::clearResults() { this->results.clear(); }
/*
 * 取得結果
 * @return: 結果
 */
const std::vector<double> &Analyzer::getResults() const {
  return this->results;
}

/*
 * 計算每一幀在特定方形內的平均值
 * @param frame: 當前幀
 * @param isLastFrame: 是否為最後一幀
 * @emit: updateResults(cv::Mat, double*, int, int):
 * 讓mainWindow更新圖片和圖表
 */
void Analyzer::calculateMean(const cv::Mat &frame, const bool isLastFrame) {
  // Example: calculate mean pixel intensity as a simple analysis
  if (frame.empty())
    return;
  // Calculate the mean intensity of the specified region
  cv::Mat roiFrame = frame(*roi);
  cv::Scalar meanIntensity = cv::mean(roiFrame);
  results.push_back(meanIntensity[0]);
  if (results.size() % 10 == 0 || isLastFrame) {
    // Emit the signal with the current frame and mean intensitys
    int size = (results.size() - 1) % 10 + 1;
    double *meanIntesity = &this->results[results.size() - size];
    emit updateResults(frame, meanIntesity, results.size() - size, size);
  }
}

/*
 * 計算圓形變化
 * @param thresholdLow: 低閾值
 * @param thresholdHigh: 高閾值
 * @param startFrame: 開始幀
 * @param endFrame: 結束幀
 * @return: 計數結果
 */
int Analyzer::calculateCircleChange(double thresholdLow, double thresholdHigh,
                                    int startFrame, int endFrame) {
  /*
   * 設定兩個閾值，當值從低於閾值變為高於閾值時，計數器加一
   */
  int cnt = 0;
  bool reachThreshold = false, low;
  for (int i = startFrame; i < results.size() && i != endFrame; i++) {
    const double result = results[i];
    if (result < thresholdLow) {
      reachThreshold = true;
      low = true;
    } else if (result > thresholdHigh) {
      if (reachThreshold) {
        cnt += low;
      } else
        reachThreshold = true;
      low = false;
    }
  }
  return cnt;
}

int Analyzer::getRoiSize() const {
  return roi->width; // Return -1 if ROI is not set
}
const std::pair<int, int> &Analyzer::getRoiCenter() const {
  static std::pair<int, int> center(-1, -1);
  if (roi && roi->width > 0 && roi->height > 0) {
    center.first = roi->x + roi->width / 2;
    center.second = roi->y + roi->height / 2;
  }
  return center;
}
