#include "localbrowser.h"
#include <QDir>
#include <QFileSystemModel>
#include <QVBoxLayout>

LocalBrowser::LocalBrowser(QWidget *parent) : QWidget(parent) {
  mainLayout = new QVBoxLayout(this);
  mainLayout->setContentsMargins(0, 0, 0, 0);

  fileView = new QTreeView(this);

  mainLayout->addWidget(fileView);

  linuxExplorer = new QFileSystemModel(this);
  linuxExplorer->setRootPath(QDir::homePath());

  fileView->setModel(linuxExplorer);
  fileView->setRootIndex(linuxExplorer->index(QDir::homePath()));
  fileView->setSelectionMode(QAbstractItemView::ExtendedSelection);
}

QStringList LocalBrowser::pushPaths() const {

  QStringList paths;
  QModelIndexList selectedItems = fileView->selectionModel()->selectedRows();
  for (const QModelIndex &index : selectedItems)
    paths << linuxExplorer->fileInfo(index).absoluteFilePath();

  return paths;
}

QString LocalBrowser::currDir() const {
  return linuxExplorer->filePath(fileView->currentIndex());
}