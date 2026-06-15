#pragma once
#include <QCheckBox>
#include <QDialog>
#include <QNetworkAccessManager>
#include <QProgressBar>
#include <QPushButton>
#include <QVBoxLayout>

class SettingsDialog : public QDialog {
  Q_OBJECT
public:
  explicit SettingsDialog(QWidget *parent = nullptr);
  QString appData, toolsDir;
private slots:
  void downloadTools();

private:
  QVBoxLayout *mainLayout;
  QCheckBox *preserveAttr;
  QPushButton *downloadBtn;
  QProgressBar *dlProgress;
  QNetworkAccessManager *qNetMan;

  void extractTools(const QString &path);
  bool toolsExist();
};