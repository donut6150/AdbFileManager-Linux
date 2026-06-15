#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QSizePolicy>
#include <QSpacerItem>
#include <QTreeView>
#include <QVBoxLayout>
#include "adb.h"
#include "devicebrowser.h"
#include "centralpanel.h"
#include "localbrowser.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow() override;
  
  public slots:
  void copy(const QString &source, const QString &dest, bool toDevice);
  
  private:
  Ui::MainWindow *ui;
  DeviceBrowser *leftPanel;
  CentralPanel *centralPanel;
  LocalBrowser *rightPanel;
  QLabel *bottomBar;
  AdbHandler *adb;
};
#endif // MAINWINDOW_H
