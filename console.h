#pragma once
#include <QPlainTextEdit>
#include <QPushButton>
#include <QScrollBar>
#include <QVBoxLayout>
#include <QWidget>

class ConsoleLog : public QWidget {
  Q_OBJECT
public:
  explicit ConsoleLog(QWidget *parent = nullptr);

signals:
  void showVersion();
  void listDevices();

public slots:
  void appendStdOut(const QString &text);
  void appendStdErr(const QString &text);
  void clearOutput();

private:
  QPlainTextEdit *conOut;
  QPushButton *versionBtn, *devicesBtn;
};