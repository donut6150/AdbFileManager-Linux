#include "devicebrowser.h"
#include <QFontDatabase>
#include <qdebug.h>

DeviceBrowser::DeviceBrowser(QWidget *parent) : QWidget(parent) {
  mainLayout = new QVBoxLayout(this);

  statusFrame = new QFrame();
  statusLayout = new QHBoxLayout(statusFrame);

  deviceSerial = new QLabel();
  deviceState = new QLabel();

  deviceSerial->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
  deviceState->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));

  statusLayout->addWidget(deviceSerial);
  statusLayout->addStretch();
  statusLayout->addWidget(deviceState);

  pathFrame = new QFrame();
  pathFrame->setFrameShape(QFrame::StyledPanel);
  pathLayout = new QHBoxLayout(pathFrame);

  devicePath = new QLabel();
  pathLayout->addWidget(devicePath);

  androidFileView = new QTableWidget(this);

  androidFileView->setColumnCount(5);
  androidFileView->setHorizontalHeaderLabels(
      {"Name", "Size", "Type", "Date", "Permissions"});

  androidFileView->setSelectionBehavior(QAbstractItemView::SelectRows);
  androidFileView->setSelectionMode(QAbstractItemView::SingleSelection);
  androidFileView->setEditTriggers(QAbstractItemView::NoEditTriggers);
  androidFileView->setShowGrid(false);
  androidFileView->verticalHeader()->setVisible(false);
  androidFileView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
  androidFileView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
  androidFileView->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);

  

  mainLayout->addWidget(statusFrame);
  mainLayout->addWidget(androidFileView);
}

void DeviceBrowser::parseList(const QString &output) {
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

    androidFileView->insertRow(row);

    androidFileView->setItem(row, 0, new QTableWidgetItem(name));
    androidFileView->setItem(row, 1, new QTableWidgetItem(size));
    androidFileView->setItem(row, 2, new QTableWidgetItem(type));
    androidFileView->setItem(row, 3, new QTableWidgetItem(datetime));
    androidFileView->setItem(row, 4, new QTableWidgetItem(permissions));
  }
  androidFileView->insertRow(row);
  androidFileView->setItem(row, 0, new QTableWidgetItem(".."));
}

QString DeviceBrowser::currPath() {
  QString path = "";
  return path;
}