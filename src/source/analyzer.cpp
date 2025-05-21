#include "analyzer.h"
#include <opencv2/ximgproc.hpp>

Analyzer::Analyzer(QObject *parent) : QObject(parent) {}

/*
 * 設定ROI區域
 * @param left: ROI左上角x座標
 * @param right: ROI右下角x座標
 * @param top: ROI左上角y座標
 * @param bottom: ROI右下角y座標
 */
void Analyzer::setBound(int left, int right, int top, int bottom) {
  this->roi = new cv::Rect(left, top, right - left, bottom - top);
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
