#include "settings.h"
#include <QDir>
#include <QNetworkReply>
#include <QProcess>
#include <QSettings>
#include <QStandardPaths>

SettingsDialog::SettingsDialog(QWidget *parent) : QDialog(parent) {
  QSettings settings;

  appData = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
  toolsDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) +
             "/platform-tools";

  this->setWindowTitle("Settings");
  this->setFixedSize(600, 225);

  mainLayout = new QVBoxLayout(this);

  preserveAttr =
      new QCheckBox("Preserve file attributes on pull? (Adds -a flag)", this);
  preserveAttr->setChecked(
      settings.value("preserveAttributes", false).toBool());

  connect(preserveAttr, &QCheckBox::toggled, [](bool checked) {
    QSettings().setValue("preserveAttributesOnPull", checked);
  });

  downloadBtn = new QPushButton("Download ADB", this);
  dlProgress = new QProgressBar(this);
  dlProgress->setValue(0);
  dlProgress->hide();

  mainLayout->addWidget(preserveAttr);
  mainLayout->addStretch();
  mainLayout->addWidget(dlProgress);
  mainLayout->addWidget(downloadBtn);

  qNetMan = new QNetworkAccessManager(this);
  
  downloadBtn->setEnabled(!toolsExist());
  downloadBtn->setText(toolsExist() ? "ADB location: " + toolsDir : "Download ADB");

  connect(downloadBtn, &QPushButton::clicked, this,
          &SettingsDialog::downloadTools);
}

void SettingsDialog::downloadTools() {
  downloadBtn->setEnabled(false);
  downloadBtn->setText("Downloading...");
  dlProgress->show();

  QDir().mkpath(appData);
  QString zip = appData + "/platform-tools.zip";

  QNetworkRequest req((QUrl("https://dl.google.com/android/repository/"
                            "platform-tools-latest-linux.zip")));
  req.setAttribute(QNetworkRequest::RedirectPolicyAttribute,
                   QNetworkRequest::NoLessSafeRedirectPolicy);
  QNetworkReply *reply = qNetMan->get(req);

  dlProgress->setMaximum(0);
  dlProgress->setMinimum(0);

  connect(reply, &QNetworkReply::finished, [this, reply, zip]() {
    if (reply->error() == QNetworkReply::NoError) {
      QFile zipFile(zip);
      if (zipFile.open(QIODevice::WriteOnly)) {
        zipFile.write(reply->readAll());
        zipFile.close();
        downloadBtn->setText("Extracting...");
        extractTools(zip);
      }
    } else {
      downloadBtn->setText("Download failed.");
      downloadBtn->setEnabled(true);
    }
  });
}

void SettingsDialog::extractTools(const QString &path) {

  QProcess *unzip = new QProcess(this);
  unzip->start("unzip", {"-o", path, "-d", appData});

  connect(unzip, &QProcess::finished, [this, path](int exitCode) {
    if (exitCode == 0) {
      QFile adbBin(toolsDir + "/adb");
      adbBin.setPermissions(adbBin.permissions() | QFile::ExeUser |
                            QFile::ExeGroup | QFile::ExeOther);

      QFile::remove(path);

      downloadBtn->setText("Downloaded to " + toolsDir);
      dlProgress->hide();
    } else {
      downloadBtn->setText("Extraction failed.");
      downloadBtn->setEnabled(true);
    }
  });
}

bool SettingsDialog::toolsExist() { 
  return QFile::exists(toolsDir + "/adb");
}