#include "mainWindow.h"
#include "qnamespace.h"
#include <QFileDialog>
#include <QHBoxLayout>
#include <QImage>
#include <QInputDialog>
#include <QLabel>
#include <QPixmap>
#include <QVBoxLayout>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
  this->showMaximized();
  // Buttons
  btnSelectVideo = new QPushButton("選擇影片", this);
  btnAnalyze = new QPushButton("分析", this);
  btnExport = new QPushButton("匯出", this);
  btnSetRoiSize = new QPushButton("設定ROI大小", this);

  // Inputs
  auto *labelThresholdLow = new QLabel("閾值（低）", this);
  editThresholdLow = new QDoubleSpinBox(this);
  auto *labelThresholdHigh = new QLabel("閾值（高）", this);
  editThresholdHigh = new QDoubleSpinBox(this);
  auto *labelRoiSize = new QLabel("ROI大小", this);
  editThresholdLow->setRange(0, 1000);
  editThresholdHigh->setRange(0, 1000);

  // Chart
  lineSeries = new QtCharts::QLineSeries();
  chart = new QtCharts::QChart();
  chart->addSeries(lineSeries);
  chart->createDefaultAxes();
  auto axesY = chart->axes(Qt::Vertical, lineSeries);
  if (!axesY.isEmpty()) {
    auto *valueAxisY = qobject_cast<QtCharts::QValueAxis *>(axesY.first());
    if (valueAxisY)
      valueAxisY->setRange(0, 255);
  }
  chartView = new QtCharts::QChartView(chart, this);

  // Image display
  graphicsView = new QGraphicsView(this);
  graphicsScene = new QGraphicsScene(this);
  graphicsView->setScene(graphicsScene);
  graphicsView->setMinimumSize(480, 320);
  graphicsView->setStyleSheet(
      "background: #eee; border: 1px solid #ccc; color: #888;");
  imageItem = nullptr;
  roiCrossItem = nullptr;
  roiRectItem = nullptr;
  roiCenter = QPoint(-1, -1);

  // Layouts
  auto *buttonLayout = new QHBoxLayout();
  buttonLayout->addWidget(btnSelectVideo);
  buttonLayout->addWidget(btnAnalyze);
  buttonLayout->addWidget(btnExport);

  auto *leftLayout = new QVBoxLayout();
  leftLayout->addLayout(buttonLayout);
  leftLayout->addWidget(labelThresholdLow);
  leftLayout->addWidget(editThresholdLow);
  leftLayout->addWidget(labelThresholdHigh);
  leftLayout->addWidget(editThresholdHigh);
  leftLayout->addWidget(btnSetRoiSize);
  leftLayout->addStretch();

  auto *rightLayout = new QVBoxLayout();
  rightLayout->addWidget(graphicsView);

  auto *mainLayout = new QVBoxLayout(this);
  auto *topLayout = new QHBoxLayout();
  topLayout->addLayout(leftLayout);
  topLayout->addLayout(rightLayout);
  mainLayout->addLayout(topLayout);

  auto *contentLayout = new QHBoxLayout();
  contentLayout->addWidget(chartView, 1);
  mainLayout->addLayout(contentLayout);

  setLayout(mainLayout);

  qRegisterMetaType<cv::Mat>("cv::Mat");
  // File I/O & Analyzer
  fileIo = new FileIo(this);
  analyzer = new Analyzer(this);
  connect(fileIo, &FileIo::loadFrame, analyzer, &Analyzer::calculateMean);
  connect(analyzer, &Analyzer::updateResults, this, &MainWindow::updateResults);
  connect(btnSelectVideo, &QPushButton::clicked, this,
          &MainWindow::selectVideo);
  connect(btnAnalyze, &QPushButton::clicked, this, &MainWindow::analyze);
  connect(btnExport, &QPushButton::clicked, this, &MainWindow::exportCsv);
  connect(btnSetRoiSize, &QPushButton::clicked, this,
          &MainWindow::showRoiSizeDialog);
}

MainWindow::~MainWindow() {}

void MainWindow::updateResults(const cv::Mat &frame, double *meanIntesity,
                               int startFrame, int size) {
  for (int i = 0; i < size; i++) {
    lineSeries->append(startFrame + i, meanIntesity[i]);
  }
  QImage img(frame.data, frame.cols, frame.rows, frame.step[0],
             QImage::Format_BGR888);
  if (imageItem)
    graphicsScene->removeItem(imageItem);
  imageItem = graphicsScene->addPixmap(QPixmap::fromImage(img));
  graphicsScene->setSceneRect(img.rect());
  // 若有 ROI，重畫標記
  if (roiCenter.x() >= 0 && roiCenter.y() >= 0)
    setRoiCenter(roiCenter);
}

void MainWindow::selectVideo() {
  this->fileName = QFileDialog::getOpenFileName(this, tr("選擇影片"), "",
                                                tr("影片檔案 (*.mp4)"))
                       .toStdString();
  if (fileName.empty()) {
    return;
  }
  // set the image label to the first frame
  cv::Mat firstFrame = fileIo->getFrame(fileName, 0);
  QImage img(firstFrame.data, firstFrame.cols, firstFrame.rows,
             firstFrame.step[0], QImage::Format_BGR888);
  if (imageItem)
    graphicsScene->removeItem(imageItem);
  imageItem = graphicsScene->addPixmap(QPixmap::fromImage(img));
  graphicsScene->setSceneRect(img.rect());
  if (roiCenter.x() >= 0 && roiCenter.y() >= 0)
    setRoiCenter(roiCenter);
  lineSeries->clear();
  int frameCount = fileIo->getFrameCount(fileName);
  // Set the range of the x-axis
  auto axesX = chart->axes(Qt::Horizontal, lineSeries);
  if (!axesX.isEmpty()) {
    auto *valueAxisX = qobject_cast<QtCharts::QValueAxis *>(axesX.first());
    if (valueAxisX)
      valueAxisX->setRange(0, frameCount);
  }
}

void MainWindow::analyze() {
  if (fileName.empty()) {
    return;
  }

  lineSeries->clear();
  // Get threshold values
  double thresholdLow = editThresholdLow->value();
  double thresholdHigh = editThresholdHigh->value();

  // Load video and start analysis
  fileIo->readFramesAsync(fileName);
}

void MainWindow::exportCsv() {
  // Export the results to a CSV file
  QString fileName = QFileDialog::getSaveFileName(this, tr("匯出結果"), "",
                                                  tr("CSV檔案 (*.csv)"));
  if (!fileName.isEmpty()) {
    fileIo->writeCsv(fileName.toStdString(), this->analyzer->getResults());
  }
}
void MainWindow::updateRoi() {
  // 移除舊的 ROI 標記
  if (roiCrossItem) {
    graphicsScene->removeItem(roiCrossItem);
    delete roiCrossItem;
    roiCrossItem = nullptr;
  }
  if (roiRectItem) {
    graphicsScene->removeItem(roiRectItem);
    delete roiRectItem;
    roiRectItem = nullptr;
  }
  // 畫紅色 X
  QPainterPath crossPath;
  crossPath.moveTo(roiCenter.x() - 10, roiCenter.y() - 10);
  crossPath.lineTo(roiCenter.x() + 10, roiCenter.y() + 10);
  crossPath.moveTo(roiCenter.x() - 10, roiCenter.y() + 10);
  crossPath.lineTo(roiCenter.x() + 10, roiCenter.y() - 10);
  roiCrossItem = graphicsScene->addPath(crossPath, QPen(Qt::red, 2));
  if (roiRectItem) {
    graphicsScene->removeItem(roiRectItem);
    delete roiRectItem;
    roiRectItem = nullptr;
  }
  // 畫紅色矩形
  roiRectItem = graphicsScene->addRect(QRectF(roiCenter.x() - roiSize / 2,
                                              roiCenter.y() - roiSize / 2,
                                              roiSize, roiSize),
                                       QPen(Qt::red, 2), QBrush(Qt::NoBrush));
  analyzer->setBound(roiCenter.x() - roiSize / 2, roiCenter.x() + roiSize / 2,
                     roiCenter.y() - roiSize / 2, roiCenter.y() + roiSize / 2);
}
void MainWindow::setRoiCenter(const QPoint &pt) {
  roiCenter = pt;
  updateRoi();
}
void MainWindow::showRoiSizeDialog() {
  bool ok = false;
  int value = QInputDialog::getInt(
      this, tr("設定ROI大小"), tr("請輸入ROI邊長："), roiSize, 1, 1000, 1, &ok);
  if (ok) {
    roiSize = value;
    updateRoi();
  }
}
void MainWindow::mousePressEvent(QMouseEvent *event) {
  if (graphicsView->underMouse()) {
    if ((event->button() == Qt::LeftButton) &&
        (event->modifiers() & Qt::ShiftModifier)) {
      QPoint viewPos = graphicsView->mapFromGlobal(event->globalPos());
      QPointF scenePos = graphicsView->mapToScene(viewPos);
      if (imageItem && imageItem->contains(scenePos)) {
        setRoiCenter(scenePos.toPoint());
      }
    }
  }
  QWidget::mousePressEvent(event);
}
