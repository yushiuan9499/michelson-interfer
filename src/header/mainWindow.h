#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H
#include "analyzer.h"
#include "fileIo.h"
#include <QChartView>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QLineSeries>
#include <QPushButton>
#include <QWidget>

class MainWindow : public QWidget {
  Q_OBJECT;

public:
  MainWindow(QWidget *parent = nullptr);
  virtual ~MainWindow();

private:
  // Buttons
  QPushButton *btnSelectVideo;
  QPushButton *btnAnalyze;
  QPushButton *btnExport;

  // Inputs
  QDoubleSpinBox *editThresholdLow;
  QDoubleSpinBox *editThresholdHigh;

  // Chart
  QtCharts::QChartView *chartView;
  QtCharts::QLineSeries *lineSeries;

  // Image display
  QLabel *imageLabel;

  // File I/O & Analyzer
  FileIo *fileIo;
  Analyzer *analyzer;

  std::string fileName;

  void selectVideo();
  void analyze();
  void exportCsv();
public slots:
  void updateResults(const cv::Mat &frame, double *meanIntesity, int size);
};

#endif // MAIN_WINDOW_H
