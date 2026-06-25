#include "headers/devicebrowser.h"
#include <QApplication>
#include <QFontDatabase>
#include <qdebug.h>

DeviceBrowser::DeviceBrowser(QWidget *parent) : QWidget(parent) {
  mainLayout = new QVBoxLayout(this);
  deviceStatus = DeviceStatus::DISCONNECTED;

  statusFrame = new QFrame();
  statusLayout = new QHBoxLayout(statusFrame);

  deviceSerial = new QLabel();
  deviceState = new QLabel();
  devicePath = new QLabel();

  deviceSerial->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
  deviceState->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
  devicePath->setText(currentPath);

  statusLayout->addWidget(deviceSerial);
  statusLayout->addStretch();
  statusLayout->addWidget(deviceState);

  pathFrame = new QFrame();
  pathFrame->setFrameShape(QFrame::StyledPanel);
  pathLayout = new QHBoxLayout(pathFrame);

  pathLayout->addWidget(devicePath);

  androidFileView = new QTableWidget(this);

  androidFileView->setColumnCount(5);
  androidFileView->setHorizontalHeaderLabels(
      {"Name", "Size", "Type", "Date", "Permissions"});

  androidFileView->setSelectionBehavior(QAbstractItemView::SelectRows);
  androidFileView->setSelectionMode(QAbstractItemView::ExtendedSelection);
  androidFileView->setEditTriggers(QAbstractItemView::NoEditTriggers);
  androidFileView->setShowGrid(false);
  androidFileView->verticalHeader()->setVisible(false);
  androidFileView->horizontalHeader()->setSectionResizeMode(
      0, QHeaderView::ResizeToContents);
  androidFileView->horizontalHeader()->setSectionResizeMode(
      1, QHeaderView::ResizeToContents);
  androidFileView->horizontalHeader()->setSectionResizeMode(
      3, QHeaderView::ResizeToContents);

  connect(androidFileView, &QTableWidget::cellDoubleClicked, this,
          [this](int row, int column) {
            QTableWidgetItem *entryType = androidFileView->item(row, 2),
                             *entryName = androidFileView->item(row, 0);

            if (entryName->text() == "..") {
              if (currentPath == "/")
                return;

              int lastSlash = currentPath.lastIndexOf('/');
              currentPath = lastSlash ? currentPath.left(lastSlash) : "/";

              devicePath->setText(currentPath);
              emit pathChanged(currentPath);
              return;
            }

            if (isDir(entryType->text())) {
              QString folderName = androidFileView->item(row, 0)->text();
              if (!currentPath.endsWith("/"))
                currentPath += "/";
              currentPath += folderName;

              devicePath->setText(currentPath);
              emit pathChanged(currentPath);
            }
          });

  mainLayout->addWidget(statusFrame);
  mainLayout->addWidget(pathFrame);
  mainLayout->addWidget(androidFileView);
}

void DeviceBrowser::parseAdbOutput(const QString &output) {
  androidFileView->setRowCount(0);

  QStringList lines =
      output.split(QRegularExpression("[\r\n]"), Qt::SkipEmptyParts);

  if (lines.size() <= 1)
    return;

  int row = 0;
  for (int i = lines.size() - 1; i > 0; i--) {
    QString line = lines[i].trimmed();

    if (!line.startsWith("-") && !line.startsWith("d") && !line.startsWith("l"))
      continue;

    QString permissions = line.section(QRegularExpression("\\s+"), 0, 0);
    QString size = line.section(QRegularExpression("\\s+"), 4, 4) + " bytes";
    QString datetime = line.section(QRegularExpression("\\s+"), 5, 5) + " " +
                       line.section(QRegularExpression("\\s+"), 6, 6);
    QString name = line.section(QRegularExpression("\\s+"), 7);
    QString type = permissions.startsWith("d") ? "Folder" : "File";
    QStyle::StandardPixmap icon =
        isDir(type) ? QStyle::SP_DirIcon : QStyle::SP_FileIcon;

    androidFileView->insertRow(row);
    androidFileView->setItem(
        row, 0,
        new QTableWidgetItem(QApplication::style()->standardIcon(icon), name));
    androidFileView->setItem(row, 1, new QTableWidgetItem(size));
    androidFileView->setItem(row, 2, new QTableWidgetItem(type));
    androidFileView->setItem(row, 3, new QTableWidgetItem(datetime));
    androidFileView->setItem(row, 4, new QTableWidgetItem(permissions));
  }
  androidFileView->insertRow(row);
  androidFileView->setItem(
      row, 0,
      new QTableWidgetItem(
          QApplication::style()->standardIcon(QStyle::SP_DirIcon), ".."));
}

QStringList DeviceBrowser::pullPaths() const {
  QStringList paths;
  for (int i = 0; i < androidFileView->rowCount(); ++i) {
    QTableWidgetItem *entry = androidFileView->item(i, 0);
    if(entry && entry->isSelected()) {
      QString fileName = entry->text();
      if (fileName == "..") continue;
      QString path = (currentPath == "/") ? "/" + fileName : currentPath + "/" + fileName;
      paths << path;
    }
  }
  return paths;
}

bool DeviceBrowser::isDir(const QString &type) { return type == "Folder"; }