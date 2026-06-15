#include "localbrowser.h"
#include <QDir>
#include <QFileSystemModel>
#include <QVBoxLayout>

LocalBrowser::LocalBrowser(QWidget *parent) : QWidget(parent) {
  mainLayout = new QVBoxLayout(this);
  mainLayout->setContentsMargins(0, 0, 0, 0);

  fileView = new QTreeView(this);

  mainLayout->addWidget(fileView);

  QFileSystemModel *linuxExplorer = new QFileSystemModel(this);
  linuxExplorer->setRootPath(QDir::homePath());

  fileView->setModel(linuxExplorer);
  fileView->setRootIndex(linuxExplorer->index(QDir::homePath()));
}