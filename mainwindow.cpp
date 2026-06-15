#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "adb.h"
#include "console.h"
#include "devicebrowser.h"
#include "settings.h"
#include <QSettings>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  QWidget *centralWidget;
  QVBoxLayout *mainLayout;
  QHBoxLayout *mainHLayout;

  centralWidget = new QWidget(this);

  mainLayout = new QVBoxLayout(centralWidget);
  mainHLayout = new QHBoxLayout();

  leftPanel = new DeviceBrowser(this);
  centralPanel = new CentralPanel(this);
  rightPanel = new LocalBrowser(this);
  bottomBar = new QLabel(this);

  bottomBar->setWordWrap(false);
  bottomBar->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
  bottomBar->setContentsMargins(10, 5, 10, 5);
  bottomBar->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));

  leftPanel->deviceSerial->setText("(no device connected)");

  ConsoleLog *console = new ConsoleLog();
  console->setAttribute(Qt::WA_QuitOnClose, false);

  adb = new AdbHandler(this);

  mainLayout->addLayout(mainHLayout, 9);
  mainLayout->addWidget(bottomBar, 1);

  this->setCentralWidget(centralWidget);

  mainHLayout->addWidget(leftPanel, 6);
  mainHLayout->addWidget(centralPanel, 1);
  mainHLayout->addWidget(rightPanel, 6);

  connect(adb, &AdbHandler::output, console, &ConsoleLog::appendStdOut);
  connect(adb, &AdbHandler::err, console, &ConsoleLog::appendStdErr);

  connect(centralPanel->consoleBtn, &QPushButton::clicked, this, [console]() {
    console->show();
    console->raise();
    console->activateWindow();
  });

  connect(console, &ConsoleLog::showVersion, this,
          [this]() { adb->exec({"version"}); });

  connect(console, &ConsoleLog::listDevices, this,
          [this]() { adb->exec({"devices"}, ProcType::DeviceCheck); });

  connect(
      adb, &AdbHandler::output, this,
      [this](const QString &text, ProcType type) {
        if (type == ProcType::DeviceCheck) {
          QStringList lines =
              text.split(QRegularExpression("[\r\n]"), Qt::SkipEmptyParts);
          if (lines.size() > 1) {
            QStringList info =
                lines[1].split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
            leftPanel->deviceSerial->setText(info[0]);
            leftPanel->deviceState->setText(info[1]);
            adb->exec({"shell", "ls", "-Ll", "/sdcard"}, ProcType::LoadFiles);
          }
        } else if (type == ProcType::LoadFiles)
          leftPanel->parseList(text);
      });

  connect(centralPanel->refreshBtn, &QPushButton::clicked, this, [this]() {
    adb->exec({"shell", "ls", "-Ll", "/sdcard"}, ProcType::LoadFiles);
  });

  connect(centralPanel->settingBtn, &QPushButton::clicked, this, [this]() {
    SettingsDialog settings(this);
    settings.exec();
  });

  connect(adb, &AdbHandler::procStarted, this,
          [this](const QString &cmd) { bottomBar->setText(cmd); });

  adb->startServer();
  adb->exec({"devices"}, ProcType::DeviceCheck);
}

void MainWindow::copy(const QString &source, const QString &dest,
                      bool toDevice) {
  if (source.isEmpty() || dest.isEmpty())
    return;

  QStringList args;

  if (toDevice)
    args << "push";
  else {
    args << "pull";

    QSettings settings;
    if (settings.value("preserveAttributes", false).toBool())
      args << "-a";
  }
  args << source << dest;

  bottomBar->setText("Copying " + source + " to " + dest);
  adb->exec(args);
}

MainWindow::~MainWindow() { delete ui; }