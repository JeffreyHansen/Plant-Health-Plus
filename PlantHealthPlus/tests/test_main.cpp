#include <QtTest/QtTest>
#include <QApplication>
#include <QDebug>
#include <QTextStream>
#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>

// Include test classes
#include "test_plantmanager.h"
#include "test_usermanager.h"
#include "test_logbookmanager.h"
#include "test_plantcard.h"
#include "test_rangeindicator.h"
#include "test_conditionchart.h"
#include "test_forecastclient.h"
#include "test_scraper.h"
#include "test_addplantdialog.h"
#include "test_logbookentrydialog.h"

class TestRunner : public QObject
{
    Q_OBJECT

public:
    TestRunner() : m_totalTests(0), m_passedTests(0), m_failedTests(0) {}
    
    struct TestResult {
        QString testName;
        bool passed;
        QString details;
        int functionsRun;
        int functionsPassed;
        int functionsFailed;
        qint64 duration; // in milliseconds
    };

    bool runAllTests() {
        qDebug() << "=== Plant Health Plus Unit Test Suite ===";
        qDebug() << "Starting test execution at" << QDateTime::currentDateTime().toString();
        
        QDateTime startTime = QDateTime::currentDateTime();
        
        // Run each test class
        runTest<TestPlantManager>("Plant Manager Tests");
        runTest<TestUserManager>("User Manager Tests");
        runTest<TestLogbookManager>("Logbook Manager Tests");
        runTest<TestPlantCard>("Plant Card Tests");
        runTest<TestRangeIndicator>("Range Indicator Tests");
        runTest<TestConditionChart>("Condition Chart Tests");
        runTest<TestForecastClient>("Forecast Client Tests");
        runTest<TestScraper>("Web Scraper Tests");
        runTest<TestAddPlantDialog>("Add Plant Dialog Tests");
        runTest<TestLogbookEntryDialog>("Logbook Entry Dialog Tests");
        
        QDateTime endTime = QDateTime::currentDateTime();
        qint64 totalDuration = startTime.msecsTo(endTime);
        
        // Print summary
        qDebug() << "\n=== Test Summary ===";
        qDebug() << QString("Total Tests: %1").arg(m_totalTests);
        qDebug() << QString("Passed: %1").arg(m_passedTests);
        qDebug() << QString("Failed: %1").arg(m_failedTests);
        qDebug() << QString("Success Rate: %1%").arg(m_totalTests > 0 ? (m_passedTests * 100 / m_totalTests) : 0);
        qDebug() << QString("Total Duration: %1 ms").arg(totalDuration);
        
        // Export results
        exportResults(totalDuration);
        
        return m_failedTests == 0;
    }

private:
    template<typename TestClass>
    void runTest(const QString& testName) {
        TestClass test;
        TestResult result;
        result.testName = testName;
        
        qDebug() << QString("\n--- Running %1 ---").arg(testName);
        
        QDateTime testStart = QDateTime::currentDateTime();
        int exitCode = QTest::qExec(&test);
        QDateTime testEnd = QDateTime::currentDateTime();
        
        result.duration = testStart.msecsTo(testEnd);
        result.passed = (exitCode == 0);
        
        if (result.passed) {
            result.details = "All test functions passed";
            m_passedTests++;
            qDebug() << QString("✓ %1 PASSED (%2 ms)").arg(testName).arg(result.duration);
        } else {
            result.details = QString("Test failed with exit code %1").arg(exitCode);
            m_failedTests++;
            qDebug() << QString("✗ %1 FAILED (%2 ms)").arg(testName).arg(result.duration);
        }
        
        m_totalTests++;
        m_testResults.append(result);
    }
    
    void exportResults(qint64 totalDuration) {
        // Create test results directory
        QDir().mkpath("test_results");
        
        // Export to JSON
        exportToJson(totalDuration);
        
        // Export to text
        exportToText(totalDuration);
        
        // Export to HTML
        exportToHtml(totalDuration);
        
        qDebug() << "\nTest results exported to:";
        qDebug() << "- test_results/test_results.json";
        qDebug() << "- test_results/test_results.txt";
        qDebug() << "- test_results/test_results.html";
    }
    
    void exportToJson(qint64 totalDuration) {
        QJsonObject root;
        root["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);
        root["total_duration_ms"] = totalDuration;
        root["total_tests"] = m_totalTests;
        root["passed_tests"] = m_passedTests;
        root["failed_tests"] = m_failedTests;
        root["success_rate"] = m_totalTests > 0 ? (m_passedTests * 100.0 / m_totalTests) : 0.0;
        
        QJsonArray testsArray;
        for (const auto& result : m_testResults) {
            QJsonObject testObj;
            testObj["name"] = result.testName;
            testObj["passed"] = result.passed;
            testObj["details"] = result.details;
            testObj["duration_ms"] = result.duration;
            testsArray.append(testObj);
        }
        root["tests"] = testsArray;
        
        QJsonDocument doc(root);
        QFile file("test_results/test_results.json");
        if (file.open(QIODevice::WriteOnly)) {
            file.write(doc.toJson());
        }
    }
    
    void exportToText(qint64 totalDuration) {
        QFile file("test_results/test_results.txt");
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << "Plant Health Plus - Unit Test Results\n";
            out << "=====================================\n\n";
            out << "Timestamp: " << QDateTime::currentDateTime().toString() << "\n";
            out << "Total Duration: " << totalDuration << " ms\n\n";
            
            out << "Summary:\n";
            out << "--------\n";
            out << "Total Tests: " << m_totalTests << "\n";
            out << "Passed: " << m_passedTests << "\n";
            out << "Failed: " << m_failedTests << "\n";
            out << "Success Rate: " << (m_totalTests > 0 ? (m_passedTests * 100 / m_totalTests) : 0) << "%\n\n";
            
            out << "Detailed Results:\n";
            out << "-----------------\n";
            for (const auto& result : m_testResults) {
                out << result.testName << ": " << (result.passed ? "PASSED" : "FAILED") 
                    << " (" << result.duration << " ms)\n";
                if (!result.passed) {
                    out << "  Details: " << result.details << "\n";
                }
            }
        }
    }
    
    void exportToHtml(qint64 totalDuration) {
        QFile file("test_results/test_results.html");
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << "<!DOCTYPE html>\n<html>\n<head>\n";
            out << "<title>Plant Health Plus - Test Results</title>\n";
            out << "<style>\n";
            out << "body { font-family: Arial, sans-serif; margin: 20px; }\n";
            out << ".header { background-color: #4CAF50; color: white; padding: 20px; text-align: center; }\n";
            out << ".summary { background-color: #f9f9f9; padding: 15px; margin: 20px 0; border-radius: 5px; }\n";
            out << ".test-passed { background-color: #d4edda; padding: 10px; margin: 5px 0; border-radius: 3px; }\n";
            out << ".test-failed { background-color: #f8d7da; padding: 10px; margin: 5px 0; border-radius: 3px; }\n";
            out << ".timestamp { color: #666; font-size: 0.9em; }\n";
            out << "</style>\n</head>\n<body>\n";
            
            out << "<div class='header'>\n";
            out << "<h1>Plant Health Plus - Unit Test Results</h1>\n";
            out << "<p class='timestamp'>Generated: " << QDateTime::currentDateTime().toString() << "</p>\n";
            out << "</div>\n";
            
            out << "<div class='summary'>\n";
            out << "<h2>Summary</h2>\n";
            out << "<p><strong>Total Tests:</strong> " << m_totalTests << "</p>\n";
            out << "<p><strong>Passed:</strong> " << m_passedTests << "</p>\n";
            out << "<p><strong>Failed:</strong> " << m_failedTests << "</p>\n";
            out << "<p><strong>Success Rate:</strong> " << (m_totalTests > 0 ? (m_passedTests * 100 / m_totalTests) : 0) << "%</p>\n";
            out << "<p><strong>Total Duration:</strong> " << totalDuration << " ms</p>\n";
            out << "</div>\n";
            
            out << "<h2>Test Details</h2>\n";
            for (const auto& result : m_testResults) {
                QString cssClass = result.passed ? "test-passed" : "test-failed";
                QString status = result.passed ? "✓ PASSED" : "✗ FAILED";
                out << "<div class='" << cssClass << "'>\n";
                out << "<strong>" << result.testName << ":</strong> " << status 
                    << " (" << result.duration << " ms)<br>\n";
                if (!result.passed) {
                    out << "<em>Details: " << result.details << "</em>\n";
                }
                out << "</div>\n";
            }
            
            out << "</body>\n</html>\n";
        }
    }

private:
    QList<TestResult> m_testResults;
    int m_totalTests;
    int m_passedTests;
    int m_failedTests;
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    TestRunner runner;
    bool success = runner.runAllTests();
    
    return success ? 0 : 1;
}

#include "test_main.moc"
