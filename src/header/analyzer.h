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

  void setRoiCenter(int x, int y);
  void setRoiSize(int size);
  void clearResults();
  int calculateCircleChange(double thresholdLow, double thresholdHigh,
                            int startFrame = 0, int endFrame = -1);

  const std::vector<double> &getResults() const;
  int getRoiSize() const;
  const std::pair<int, int> getRoiCenter() const;

public slots:
  void calculateMean(const cv::Mat &frame, const bool isLastFrame);
signals:
  // this signal means that the analysis have done 10 frames
  // so the main window need to update
  void updateResults(const cv::Mat &frame, double *meanIntesity, int startFrame,
                     int size);
};

#endif // ANALYZER_H
