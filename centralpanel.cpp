#include "centralpanel.h"

CentralPanel::CentralPanel(QWidget *parent) : QWidget(parent) {
  mainLayout = new QVBoxLayout(this);

  pullBtn = new QPushButton();
  pushBtn = new QPushButton();
  refreshBtn = new QPushButton();
  consoleBtn = new QPushButton();
  settingBtn = new QPushButton();

  pullBtn->setIcon(QIcon::fromTheme("go-next"));
  pushBtn->setIcon(QIcon::fromTheme("go-previous"));
  refreshBtn->setIcon(QIcon::fromTheme("view-refresh"));
  consoleBtn->setIcon(QIcon::fromTheme("utilities-terminal"));
  settingBtn->setIcon(QIcon::fromTheme("preferences-system"));
  
  pullBtn->setToolTip("Pull from device");
  pushBtn->setToolTip("Push to device");
  refreshBtn->setToolTip("Refresh");
  consoleBtn->setToolTip("Show console");
  settingBtn->setToolTip("Settings");

  mainLayout->addSpacerItem(
      new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));
  mainLayout->addWidget(pullBtn);
  mainLayout->addWidget(pushBtn);
  mainLayout->addWidget(refreshBtn);
  mainLayout->addSpacerItem(
      new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));
  mainLayout->addWidget(consoleBtn);
  mainLayout->addWidget(settingBtn);
}