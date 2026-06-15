#pragma once
#include <QAbstractItemView>
#include <QFrame>
#include <QHeaderView>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QTreeView>
#include <QVBoxLayout>
#include <QWidget>

class DeviceBrowser : public QWidget {
  Q_OBJECT
public:
  explicit DeviceBrowser(QWidget *parent = nullptr);
  QLabel *deviceIcon, *deviceSerial, *deviceState, *devicePath;
  void parseList(const QString &output);
  QString currPath();

private:
  QFrame *statusFrame, *pathFrame;
  QHBoxLayout *statusLayout, *pathLayout;
  QVBoxLayout *mainLayout;
  QPushButton *pushBtn, *pullBtn, *refreshBtn, *consoleBtn, *settingBtn;
  QTableWidget *androidFileView;
};