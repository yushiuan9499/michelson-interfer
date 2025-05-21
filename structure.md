# 架構說明
~~code很多是copilot寫的，所以順便寫個架構說明給自己看~~
## 目錄結構
```
SRC/
├── HEADER/
│   ├── analyzer.h      // 分析器類別宣告
│   ├── fileIo.h        // 檔案 I/O 類別宣告
│   └── mainWindow.h    // 主視窗類別宣告
└── SOURCE/
    ├── analyzer.cpp    // 分析器類別實作
    ├── fileIo.cpp      // 檔案 I/O 類別實作
    └── mainWindow.cpp  // 主視窗類別實作
```
## 模組說明
### 1. MainWindow（主視窗）

- 檔案：`mainWindow.cpp`
- 功能：
  - 提供圖形化介面，包含影片選擇、ROI 設定、分析、匯出等按鈕。
  - 顯示影片畫面、分析結果圖表、滑桿控制分析範圍與影格。
  - 處理滑鼠事件以設定 ROI 中心點。
  - 負責與 FileIo、Analyzer 進行訊號與槽（signal/slot）連接。
  - 根據分析結果更新圖表與畫面。

### 2. FileIo（檔案 I/O）

- 檔案：`fileIo.cpp`
- 功能：
  - 非同步讀取影片逐幀，並透過 signal 傳送給 Analyzer。
  - 提供取得指定影格、影片總幀數的功能。
  - 支援將分析結果寫入 CSV 檔案。

### 3. Analyzer（分析器）

- 檔案：`analyzer.cpp`
- 功能：
  - 設定 ROI 區域。
  - 計算每一幀在 ROI 內的平均像素強度，並儲存結果。
  - 根據設定的閾值計算「圓形變化」次數。
  - 提供清空與取得分析結果的介面。
  - 分析完成時發送 signal 供 UI 更新。
