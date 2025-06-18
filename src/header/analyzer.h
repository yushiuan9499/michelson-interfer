#ifndef ANALYZER_H
#define ANALYZER_H

#include <QObject>
#include <opencv2/core.hpp>
#include <vector>

class Analyzer : public QObject {
  Q_OBJECT;

protected:
  cv::Rect *roi;
  std::vector<double> results;

public:
  explicit Analyzer(QObject *parent = nullptr);

  /**
   * 設定ROI中心
   * @param x: ROI中央x座標
   * @param y: ROI中央y座標
   */
  void setRoiCenter(int x, int y);

  /**
   * 設定ROI大小
   * @param size: ROI方形的邊長
   */
  void setRoiSize(int size);

  /**
   * 清空資料
   */
  void clearResults();

  /**
   * 計算圓形變化
   * @param thresholdLow: 低閾值
   * @param thresholdHigh: 高閾值
   * @param startFrame: 開始幀
   * @param endFrame: 結束幀
   * @return: 計數結果
   */
  int calculateCircleChange(double thresholdLow, double thresholdHigh,
                            int startFrame = 0, int endFrame = -1);

  /**
   * 取得結果
   * @return: 結果
   */
  const std::vector<double> &getResults() const;
  /**
   * 取得ROI大小
   * @return: ROI大小，單位為像素
   */
  int getRoiSize() const;
  /**
   * 取得ROI中心
   * @return: ROI中心座標
   */
  const std::pair<int, int> getRoiCenter() const;

public slots:
  /**
   * 計算每一幀在特定方形內的平均值
   * @param frame: 當前幀
   * @param isLastFrame: 是否為最後一幀
   * @emit: updateResults(cv::Mat, double*, int, int):
   * 讓mainWindow更新圖片和圖表
   */
  void calculateMean(const cv::Mat &frame, const bool isLastFrame);
signals:
  // this signal means that the analysis have done 10 frames
  // so the main window have to update
  void updateResults(const cv::Mat &frame, double *meanIntesity, int startFrame,
                     int size);
};

#endif // ANALYZER_H
