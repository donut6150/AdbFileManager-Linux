#pragma once
#include <QTreeView>
#include <QVBoxLayout>
#include <QWidget>
#include <QVBoxLayout>
#include <QWidget>
#include <QFileSystemModel>

class LocalBrowser : public QWidget {
  Q_OBJECT
public:
  explicit LocalBrowser(QWidget *parent = nullptr);

  QStringList pushPaths() const;
  QString currDir() const;

private:
  QTreeView *fileView;
  QVBoxLayout *mainLayout;
  QFileSystemModel *linuxExplorer;
};