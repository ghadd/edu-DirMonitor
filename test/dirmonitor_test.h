#include "../src/dirmonitor.h"
#include <cxxtest/TestSuite.h>
#include <thread>

using namespace std::chrono_literals;

void createFile(QString filePath) {
  QFile file(filePath);
  if (!file.open(QIODevice::WriteOnly))
    throw std::runtime_error("Could not create file.");
  file.close();
}

class DirMonitorTest : public CxxTest::TestSuite {
public:
  void testApplyMonitor() {
    // preparing test dir
    QDir testDir("testApplyMonitorDir");
    if (!testDir.exists()) {
      testDir.mkpath(".");
      createFile(testDir.filePath("file1"));
      testDir.mkdir("subdir");
      createFile(testDir.filePath("subdir/file2"));
    }

    // begin testing
    DirMonitor monitor(testDir);
    TS_ASSERT_THROWS_NOTHING(monitor.validatePath());

    auto result = monitor.applyMonitor();
    auto infos = result.first;

    QVector<QString> wantFilenames = {"file1", "subdir/file2"};

    TS_ASSERT(infos.size() == wantFilenames.size());

    for (auto &fname : wantFilenames) {
      auto res = std::find_if(infos.begin(), infos.end(),
                              [&fname](const FileInfo &finfo) {
                                return finfo.filename.endsWith(fname);
                              });
      TS_ASSERT(res != std::end(infos));
    }
  }

  void testApplyMonitorInvalidPath() {
    DirMonitor monitor("|\\invalid_path");
    TS_ASSERT_THROWS(monitor.validatePath(), PathError);
  }

  void testCachingPositive() {
    DirMonitor monitor("..");
    auto res = monitor.applyMonitor();
    
    std::this_thread::sleep_for(3s);
    TS_ASSERT_THROWS_NOTHING(monitor.loadCachedResult());
  }

  void testCachingNegative() {
    DirMonitor monitor("..");
    auto res = monitor.applyMonitor();
    
    std::this_thread::sleep_for(7s);
    TS_ASSERT_THROWS_ANYTHING(monitor.loadCachedResult());
  }

};
