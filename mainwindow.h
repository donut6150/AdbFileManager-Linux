#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "adb.h"
#include "centralpanel.h"
#include "devicebrowser.h"
#include "localbrowser.h"
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QSizePolicy>
#include <QSpacerItem>
#include <QTreeView>
#include <QVBoxLayout>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

enum class FileOp { NONE, PUSH, PULL };

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow() override;

public slots:
  void copy(const QStringList &sources, const QString &dest, FileOp fileOp);

private:
  Ui::MainWindow *ui;
  DeviceBrowser *leftPanel;
  CentralPanel *centralPanel;
  LocalBrowser *rightPanel;
  QLabel *bottomBar;
  AdbHandler *adb;
  void listDevices();
};
#endif // MAINWINDOW_H
