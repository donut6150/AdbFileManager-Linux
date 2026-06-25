#include "headers/adb.h"
#include <QByteArray>
#include <QCoreApplication>
#include <QStandardPaths>

AdbHandler::AdbHandler(QObject *parent) : QObject(parent) {
  adbProc = new QProcess(this);

  adbBin = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) +
           "/platform-tools/adb";

  connect(adbProc, &QProcess::readyReadStandardOutput, this,
          &AdbHandler::readStdOut);
  connect(adbProc, &QProcess::readyReadStandardError, this,
          &AdbHandler::readStdErr);
  connect(adbProc, &QProcess::finished, this,
          [this](int exitCode) { emit procFinished(exitCode); });
}

void AdbHandler::startServer() {
  adbProc->start(adbBin, {"start-server"});
  adbProc->waitForFinished();
  adbProc->start(adbBin, {"version"});
  adbProc->waitForFinished();
  emit procStarted({"Server started"});
}

void AdbHandler::exec(const QStringList &args, ProcType type) {
  QProcess *worker = new QProcess(this);
  connect(worker, &QProcess::finished, this, [this, worker, type]() {
    QString outText = worker->readAllStandardOutput();
    QString errText = worker->readAllStandardError();
    if (!outText.isEmpty())
      emit output(outText, type);
    if (!errText.isEmpty())
      emit err(errText);
    worker->deleteLater();
  });
  worker->start(adbBin, args);
  emit procStarted({args.join(" ")});
}

void AdbHandler::readStdOut() {
  emit output(QString::fromUtf8(adbProc->readAllStandardOutput()), procType);
}

void AdbHandler::readStdErr() {
  emit err(QString::fromUtf8(adbProc->readAllStandardError()));
}