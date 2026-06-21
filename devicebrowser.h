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

enum class DeviceStatus { DISCONNECTED, CONNECTED };

class DeviceBrowser : public QWidget {
  Q_OBJECT

public:
  explicit DeviceBrowser(QWidget *parent = nullptr);

  QLabel *deviceIcon, *deviceSerial, *deviceState, *devicePath;
  void parseAdbOutput(const QString &output);
  
  QStringList pullPaths() const;
  QString currentPath = "/sdcard";

  DeviceStatus deviceStatus;

signals:
  void pathChanged(const QString &newPath);

private:
  QFrame *statusFrame, *pathFrame;
  QHBoxLayout *statusLayout, *pathLayout;
  QVBoxLayout *mainLayout;
  QPushButton *pushBtn, *pullBtn, *refreshBtn, *consoleBtn, *settingBtn;
  QTableWidget *androidFileView;
  bool isDir(const QString &type);
};