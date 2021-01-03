#include "app.h"

#include "src/dirmonitor.h"
#include <QApplication>

#define TEST

#ifndef TEST

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  App w;
  w.show();
  return a.exec();
}

#else

int main() {
  DirMonitor d("../edu-DirMonitor");
  auto res = d.applyMonitor();
  res = d.applyMonitor();
}

#endif
