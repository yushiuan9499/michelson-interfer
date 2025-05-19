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
}

MainWindow::~MainWindow() {}
