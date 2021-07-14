#ifndef OUTPUTWORKER_H
#define OUTPUTWORKER_H

#include <qmutex.h>
#include <qsettings.h>
#include <qstandardpaths.h>
#include <settings/settingshandler.h>
#include <tchar.h>
#include <windows.h>

#include <QObject>
#include <QThread>
#include <QWaitCondition>
#include <headers/SerialPort.hpp>

#include "headers/SimConnect.h"
#include "output.h"
#include "outputbundle.h"
#include "outputhandler.h"

class OutputWorker : public QThread {
  Q_OBJECT
  void run() override { testDataRequest(); }
 signals:
  void updateLastValUI(QString lastVal);
  void updateLastStatusUI(QString lastStatus);

 public:
  OutputWorker();
  ~OutputWorker();
  QMutex mutex;
  QWaitCondition condition;

  std::string getLastVal() { return lastVal; }
  std::string getLastStatusVal() { return lastStatus; }
  void clearBundles();

  const char* portName;
  const char* valPort;
  bool abort = false;
  void setOutputsToMap(QList<Output*> list) { this->outputsToMap = list; };

  void addBundle(outputBundle* bundle);

 private:
  QList<Output*> outputsToMap;
  QList<outputBundle*>* outputBundles = new QList<outputBundle*>();
  SettingsHandler settingsHandler;
  outputHandler outputHandler;
  QMap<int, Output*> availableSets;

  int updatePerXFrames = 15;
  std::string lastVal;
  std::string lastStatus;
  SerialPort* arduino;

  static void MyDispatchProcRD(SIMCONNECT_RECV* pData, DWORD cbData,
                               void* pContext);
  void testDataRequest();
};

#endif  // OUTPUTWORKER_H