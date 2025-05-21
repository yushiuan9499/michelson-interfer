#include "analyzer.h"
#include <opencv2/ximgproc.hpp>

Analyzer::Analyzer(QObject *parent) : QObject(parent) {}

void Analyzer::setBound(int left, int right, int top, int bottom) {
  this->roi = new cv::Rect(left, top, right - left, bottom - top);
}
void Analyzer::clearResults() { this->results.clear(); }
const std::vector<double> &Analyzer::getResults() const {
  return this->results;
}
void Analyzer::calculateMean(const cv::Mat &frame, const bool isLastFrame) {
  // Example: calculate mean pixel intensity as a simple analysis
  if (frame.empty())
    return;
  // Calculate the mean intensity of the specified region
  cv::Mat roiFrame = frame(*roi);
  cv::Scalar meanIntensity = cv::mean(roiFrame);
  results.push_back(meanIntensity[0]);
  if (results.size() % 10 == 0 || isLastFrame) {
    // Emit the signal with the current frame and mean intensity
    int size = (results.size() - 1) % 10 + 1;
    double *meanIntesity = &this->results[results.size() - size];
    emit updateResults(frame, meanIntesity, results.size() - size, size);
  }
}

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
