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

  /**
   * 選擇影片檔案
   */
  void selectVideo();

  /**
   * 分析影片
   */
  void analyze();

  /**
   * 匯出結果
   */
  void exportCsv();

  /**
   * 設定 ROI 位置
   * @param pt: ROI 中心點座標
   */
  void setRoiCenter(const QPoint &pt);

  /**
   * 更新 ROI 標記
   */
  void updateRoi();

  /**
   * 更新滑桿的值
   * @param min: 最小值, -1表示不更新
   * @param max: 最大值, -1表示不更新
   * @param value: 當前值, -1表示不更新
   */
  void updateSlider(int min, int max, int value);
public slots:
  /**
   * 更新圖表和圖片
   * @param frame: 當前幀
   * @param meanIntesity: 平均強度值的陣列
   * @param startFrame: 開始幀
   * @param size: 平均強度陣列的大小
   */
  void updateResults(const cv::Mat &frame, double *meanIntesity, int startFrame,
                     int size);

  /**
   * 顯示 ROI 大小設定對話框
   */
  void showRoiSizeDialog();
};

#endif // MAIN_WINDOW_H
