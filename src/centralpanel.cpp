#include "headers/centralpanel.h"

CentralPanel::CentralPanel(QWidget *parent) : QWidget(parent) {
  mainLayout = new QVBoxLayout(this);

  pullBtn = new QPushButton("Pull ->");
  pushBtn = new QPushButton("<- Push");
  refreshBtn = new QPushButton("Refresh");
  consoleBtn = new QPushButton("Console");
  settingBtn = new QPushButton("Settings");

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