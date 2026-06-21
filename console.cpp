#include "console.h"
#include <QHBoxLayout>

ConsoleLog::ConsoleLog(QWidget *parent) : QWidget(parent) {

  this->setWindowFlags(Qt::Window);
  this->setWindowTitle("Console output");
  this->resize(1000, 600);

  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  QHBoxLayout *mainHLayout = new QHBoxLayout();

  versionBtn = new QPushButton();
  devicesBtn = new QPushButton();

  versionBtn->setIcon(QIcon::fromTheme("help-about"));
  versionBtn->setText("ADB version");

  devicesBtn->setIcon(QIcon::fromTheme("phone"));
  devicesBtn->setText("List devices");

  connect(versionBtn, &QPushButton::clicked, this, &ConsoleLog::showVersion);
  connect(devicesBtn, &QPushButton::clicked, this, &ConsoleLog::listDevices);

  conOut = new QPlainTextEdit(this);
  conOut->setReadOnly(true);
  conOut->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));

  conOut->setStyleSheet("background-color: #1e1e1e; color: #d4d4d4");

  mainHLayout->addWidget(devicesBtn);
  mainHLayout->addStretch();
  mainHLayout->addWidget(versionBtn);

  mainLayout->addLayout(mainHLayout);
  mainLayout->addWidget(conOut);
}

void ConsoleLog::appendStdOut(const QString &text) {
  conOut->insertPlainText(text);

  QScrollBar *bar = conOut->verticalScrollBar();
  bar->setValue(bar->maximum());
}

void ConsoleLog::appendStdErr(const QString &text) {
  QString err = QString("<span style=\"color: #dddd00;\">%1</span>")
                    .arg(text.toHtmlEscaped().replace('\n', "<br>"));
  conOut->appendHtml(err);
}

void ConsoleLog::clearOutput() { conOut->clear(); }