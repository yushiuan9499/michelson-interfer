#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H
#include <QChartView>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QLineSeries>
#include <QPushButton>
#include <QWidget>

class MainWindow : public QWidget {
  Q_OBJECT

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
};

#endif // MAIN_WINDOW_H
