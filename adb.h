#pragma once
#include <QObject>
#include <QProcess>
#include <QString>

enum class ProcType { NONE, DEVICE_CHECK, LOAD_FILES };

class AdbHandler : public QObject {
  Q_OBJECT
public:
  explicit AdbHandler(QObject *parent = nullptr);

  void startServer();
  void exec(const QStringList &args, ProcType type = ProcType::NONE);

signals:
  void output(const QString &outStr, ProcType type);
  void err(const QString &errStr);
  void procFinished(int exitCode);
  void procStarted(const QString &cmd);

private slots:
  void readStdOut();
  void readStdErr();

private:
  QProcess *adbProc;
  QString adbBin;
  ProcType procType;
};