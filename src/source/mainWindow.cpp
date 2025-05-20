#include "mainWindow.h"
#include <QFileDialog>
#include <QHBoxLayout>
#include <QImage>
#include <QLabel>
#include <QPixmap>
#include <QVBoxLayout>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
  // Buttons
  btnSelectVideo = new QPushButton("選擇影片", this);
  btnAnalyze = new QPushButton("分析", this);
  btnExport = new QPushButton("匯出", this);

  // Inputs
  auto *labelThresholdLow = new QLabel("閾值（低）", this);
  editThresholdLow = new QDoubleSpinBox(this);
  auto *labelThresholdHigh = new QLabel("閾值（高）", this);
  editThresholdHigh = new QDoubleSpinBox(this);
  editThresholdLow->setRange(0, 1000);
  editThresholdHigh->setRange(0, 1000);

  // Chart
  lineSeries = new QtCharts::QLineSeries();
  auto *chart = new QtCharts::QChart();
  chart->addSeries(lineSeries);
  chart->createDefaultAxes();
  chartView = new QtCharts::QChartView(chart, this);

  // Image display
  imageLabel = new QLabel(this);
  imageLabel->setFixedSize(1080, 720);
  imageLabel->setStyleSheet("background: #eee; border: 1px solid #ccc;");

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

  auto *rightLayout = new QVBoxLayout();
  rightLayout->addWidget(imageLabel);
  rightLayout->addStretch();

  auto *mainLayout = new QVBoxLayout(this);
  auto *topLayout = new QHBoxLayout();
  topLayout->addLayout(leftLayout);
  topLayout->addLayout(rightLayout);
  mainLayout->addLayout(topLayout);

  auto *contentLayout = new QHBoxLayout();
  contentLayout->addWidget(chartView, 1);
  contentLayout->addLayout(rightLayout);
  mainLayout->addLayout(contentLayout);

  setLayout(mainLayout);

  // File I/O & Analyzer
  fileIo = new FileIo(this);
  analyzer = new Analyzer(this);
  connect(fileIo, &FileIo::loadFrame, analyzer, &Analyzer::calculateMean);
  connect(analyzer, &Analyzer::updateResults, this, &MainWindow::updateResults);
  connect(btnSelectVideo, &QPushButton::clicked, this,
          &MainWindow::selectVideo);
  connect(btnAnalyze, &QPushButton::clicked, this, &MainWindow::analyze);
  connect(btnExport, &QPushButton::clicked, this, &MainWindow::exportCsv);
}

MainWindow::~MainWindow() {}

void MainWindow::updateResults(const cv::Mat &frame, double *meanIntesity,
                               int size) {
  for (int i = 0; i < size; i++) {
    lineSeries->append(i, meanIntesity[i]);
  }

  // Update the image label with the current frame
  QImage img(frame.data, frame.cols, frame.rows, frame.step[0],
             QImage::Format_RGB888);
  imageLabel->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::selectVideo() {
  this->fileName = QFileDialog::getOpenFileName(this, tr("選擇影片"), "",
                                                tr("影片檔案 (*.mp4)"))
                       .toStdString();
}

void MainWindow::analyze() {
  if (fileName.empty()) {
    return;
  }

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
