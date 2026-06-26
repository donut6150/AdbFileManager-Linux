#include "ui_mainwindow.h"
#include "headers/mainwindow.h"
#include "headers/adb.h"
#include "headers/console.h"
#include "headers/devicebrowser.h"
#include "headers/settings.h"
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
          [this]() { adb->exec({"devices"}); });

  connect(leftPanel, &DeviceBrowser::pathChanged, this,
          [this](const QString &newPath) {
            adb->exec({"shell", "ls", "-Ll", "\"" + newPath + "\""}, ProcType::LOAD_FILES);
          });

  connect(adb, &AdbHandler::output, this,
          [this](const QString &text, ProcType type) {
            if (type == ProcType::DEVICE_CHECK) {
              QStringList lines =
                  text.split(QRegularExpression("[\r\n]"), Qt::SkipEmptyParts);
              if (lines.size() > 1) {
                leftPanel->deviceStatus = DeviceStatus::CONNECTED;
                QStringList info = lines[1].split(QRegularExpression("\\s+"),
                                                  Qt::SkipEmptyParts);
                leftPanel->deviceSerial->setText(info[0]);
                leftPanel->deviceState->setText(info[1]);
                adb->exec({"shell", "ls", "-Ll", leftPanel->currentPath},
                          ProcType::LOAD_FILES);
              }
            } else if (type == ProcType::LOAD_FILES)
              leftPanel->parseAdbOutput(text);
            leftPanel->deviceStatus = DeviceStatus::CONNECTED;
          });

  connect(centralPanel->refreshBtn, &QPushButton::clicked, this, [this]() {
    adb->exec({"shell", "ls", "-Ll", leftPanel->currentPath},
              ProcType::LOAD_FILES);
  });

  connect(centralPanel->refreshBtn, &QPushButton::clicked, this,
          &MainWindow::listDevices);

  connect(centralPanel->settingBtn, &QPushButton::clicked, this, [this]() {
    SettingsDialog settings(this);
    settings.exec();
  });

  connect(centralPanel->pushBtn, &QPushButton::clicked, this, [this]() {
    QStringList sources = rightPanel->pushPaths();
    QString dest = leftPanel->currentPath;
    copy(sources, dest, FileOp::PUSH);
  });

  connect(centralPanel->pullBtn, &QPushButton::clicked, this, [this]() {
    QStringList sources = leftPanel->pullPaths();
    QString dest = rightPanel->currDir();
    copy(sources, dest, FileOp::PULL);
  });

  connect(adb, &AdbHandler::procStarted, this,
          [this](const QString &cmd) { bottomBar->setText("adb " + cmd); });

  adb->startServer();
  adb->exec({"devices"}, ProcType::DEVICE_CHECK);
}

void MainWindow::copy(const QStringList &sources, const QString &dest,
                      FileOp fileOp) {
  if (sources.isEmpty() || dest.isEmpty())
    return;

  if (fileOp == FileOp::NONE)
    return;

  QStringList args;
  if (fileOp == FileOp::PUSH)
    args << "push";
  else if (fileOp == FileOp::PULL) {
    args << "pull";

    QSettings settings;
    if (settings.value("preserveAttributesOnPull", false).toBool())
      args << "-a";
  }
  args << sources << dest;

  bottomBar->setText("Copying ");
  adb->exec(args);
}

void MainWindow::listDevices() {
  if (leftPanel->deviceStatus == DeviceStatus::CONNECTED &&
      leftPanel->deviceState->text().isEmpty())
    adb->exec({"devices"}, ProcType::DEVICE_CHECK);
}
MainWindow::~MainWindow() { delete ui; }