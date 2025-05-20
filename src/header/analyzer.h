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

  void setBound(int left, int right, int top, int bottom);
  void clearResults();

  const std::vector<double> &getResults() const;

public slots:
  void calculateMean(const cv::Mat &frame, const bool isLastFrame);
signals:
  // this signal means that the analysis have done 10 frames
  // so the main window need to update
  void updateResults(const cv::Mat &frame, double *meanIntesity, int startFrame,
                     int size);
};

#endif // ANALYZER_H
