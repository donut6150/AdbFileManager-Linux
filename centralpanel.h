#pragma once
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

class CentralPanel : public QWidget {
  Q_OBJECT
public:
  explicit CentralPanel(QWidget *parent = nullptr);
  QPushButton *pullBtn, *pushBtn, *refreshBtn, *consoleBtn, *settingBtn;

private:
  QVBoxLayout *mainLayout;
};