#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H
#include "analyzer.h"
#include "fileIo.h"
#include <QChartView>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QLineSeries>
#include <QPushButton>
#include <QSlider>
#include <QWidget>

class MainWindow : public QWidget {
  Q_OBJECT;

public:
  MainWindow(QWidget *parent = nullptr);
  virtual ~MainWindow();
  void mousePressEvent(QMouseEvent *event) override;

private:
  // Buttons
  QPushButton *btnSelectVideo;
  QPushButton *btnAnalyze;
  QPushButton *btnExport;
  QPushButton *btnSetRoiSize;

  // Inputs
  QDoubleSpinBox *editThresholdLow;
  QDoubleSpinBox *editThresholdHigh;

  // Chart
  QtCharts::QChartView *chartView;
  QtCharts::QLineSeries *lineSeries;
  QtCharts::QChart *chart;

  // Image display
  QGraphicsView *graphicsView;
  QGraphicsScene *graphicsScene;
  QGraphicsPixmapItem *imageItem;
  QGraphicsItem *roiCrossItem;
  QGraphicsRectItem *roiRectItem;
  QPoint roiCenter;
  int roiSize = 20;
  // Sliders
  QSlider *frameSlider;
  QSlider *rangeSliderMin;
  QSlider *rangeSliderMax;
  // SpinBox for frame selection
  QSpinBox *spinRangeMin;
  QSpinBox *spinRangeMax;
  QSpinBox *spinFrame;

  // Result display
  QLabel *labelCircleChange;

  // File I/O & Analyzer
  FileIo *fileIo;
  Analyzer *analyzer;

  std::string fileName;

  void selectVideo();
  void analyze();
  void exportCsv();
  void setRoiCenter(const QPoint &pt);
  void updateRoi();
  void updateSlider(int min, int max, int value);
public slots:
  void updateResults(const cv::Mat &frame, double *meanIntesity, int startFrame,
                     int size);
  void showRoiSizeDialog();
};

#endif // MAIN_WINDOW_H
