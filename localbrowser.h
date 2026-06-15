#pragma once
#include <QTreeView>
#include <QVBoxLayout>
#include <QWidget>
#include <QVBoxLayout>
#include <QWidget>

class LocalBrowser : public QWidget {
  Q_OBJECT
public:
  explicit LocalBrowser(QWidget *parent = nullptr);

private:
  QTreeView *fileView;
  QVBoxLayout *mainLayout;
};