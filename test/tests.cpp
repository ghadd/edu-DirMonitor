/* Generated file, do not edit */

#ifndef CXXTEST_RUNNING
#define CXXTEST_RUNNING
#endif

#define _CXXTEST_HAVE_STD
#define _CXXTEST_HAVE_EH
#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/TestRunner.h>
#include <cxxtest/RealDescriptions.h>
#include <cxxtest/TestMain.h>
#include <cxxtest/ErrorPrinter.h>

int main( int argc, char *argv[] ) {
 int status;
    CxxTest::ErrorPrinter tmp;
    CxxTest::RealWorldDescription::_worldName = "cxxtest";
    status = CxxTest::Main< CxxTest::ErrorPrinter >( tmp, argc, argv );
    return status;
}
bool suite_DirMonitorTest_init = false;
#include "/home/ghadd/Development/CPP/edu-DirMonitor/test/dirmonitor_test.h"

static DirMonitorTest suite_DirMonitorTest;

static CxxTest::List Tests_DirMonitorTest = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_DirMonitorTest( "test/dirmonitor_test.h", 11, "DirMonitorTest", suite_DirMonitorTest, Tests_DirMonitorTest );

static class TestDescription_suite_DirMonitorTest_testApplyMonitor : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_DirMonitorTest_testApplyMonitor() : CxxTest::RealTestDescription( Tests_DirMonitorTest, suiteDescription_DirMonitorTest, 13, "testApplyMonitor" ) {}
 void runTest() { suite_DirMonitorTest.testApplyMonitor(); }
} testDescription_suite_DirMonitorTest_testApplyMonitor;

static class TestDescription_suite_DirMonitorTest_testApplyMonitorInvalidPath : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_DirMonitorTest_testApplyMonitorInvalidPath() : CxxTest::RealTestDescription( Tests_DirMonitorTest, suiteDescription_DirMonitorTest, 42, "testApplyMonitorInvalidPath" ) {}
 void runTest() { suite_DirMonitorTest.testApplyMonitorInvalidPath(); }
} testDescription_suite_DirMonitorTest_testApplyMonitorInvalidPath;

#include <cxxtest/Root.cpp>
const char* CxxTest::RealWorldDescription::_worldName = "cxxtest";
