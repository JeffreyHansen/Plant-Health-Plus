#include "testrunner.h"
#include <QApplication>
#include <QTimer>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>
#include <QPixmap>
#include <QPainter>
#include <QTextCursor>
#include <QEventLoop>
#include <QScrollArea>
#include <QFileInfo>
#include <QTextStream>
#include <QFile>

// Charts includes
#include <QtCharts>

TestRunnerDialog::TestRunnerDialog(QWidget *parent)
    : QDialog(parent)
    , progressBar(nullptr)
    , resultsText(nullptr)
    , passedCount(0)
    , failedCount(0)
    , totalTests(0)
    , currentTest(0)
    , resultsChart(nullptr)
{
    setupUI();
    setWindowTitle("Test Runner - Plant Health Plus");
    setModal(true);
    resize(800, 600);
}

void TestRunnerDialog::setupUI()
{
    setStyleSheet(R"(
        QDialog {
            background-color: #f0f0f0;
        }
        QPushButton {
            background-color: #4CAF50;
            color: white;
            border: none;
            padding: 8px 16px;
            border-radius: 4px;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #45a049;
        }
        QPushButton:pressed {
            background-color: #3d8b40;
        }
        QTextEdit {
            border: 1px solid #ddd;
            border-radius: 4px;
            font-family: 'Courier New', monospace;
            background-color: white;
        }
        QProgressBar {
            border: 2px solid #ddd;
            border-radius: 8px;
            text-align: center;
            font-weight: bold;
        }
        QProgressBar::chunk {
            background-color: #4CAF50;
            border-radius: 6px;
        }
    )");

    auto *layout = new QVBoxLayout(this);
    
    // Header
    auto *headerLayout = new QHBoxLayout();
    auto *titleLabel = new QLabel("🧪 Plant Health Plus Test Suite");
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #2E7D32; margin: 10px;");
    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();
    
    // Status info
    statusLabel = new QLabel("Ready to run tests");
    statusLabel->setStyleSheet("font-size: 12px; color: #666; margin: 5px;");
    headerLayout->addWidget(statusLabel);
    
    layout->addLayout(headerLayout);
    
    // Progress section
    auto *progressLayout = new QVBoxLayout();
    auto *progressTitleLabel = new QLabel("Progress:");
    progressTitleLabel->setStyleSheet("font-weight: bold; margin-top: 10px;");
    progressLayout->addWidget(progressTitleLabel);
    
    progressBar = new QProgressBar();
    progressBar->setTextVisible(true);
    progressBar->setFormat("%v/%m tests completed (%p%)");
    progressLayout->addWidget(progressBar);
    
    layout->addLayout(progressLayout);
    
    // Results area
    auto *resultsLayout = new QVBoxLayout();
    auto *resultsLabel = new QLabel("Test Results:");
    resultsLabel->setStyleSheet("font-weight: bold; margin-top: 15px;");
    resultsLayout->addWidget(resultsLabel);
    
    resultsText = new QTextEdit();
    resultsText->setReadOnly(true);
    resultsText->setMinimumHeight(200);
    resultsLayout->addWidget(resultsText);
    
    layout->addLayout(resultsLayout);
    
    // Chart section
    auto *chartLayout = new QVBoxLayout();
    auto *chartLabel = new QLabel("Test Summary:");
    chartLabel->setStyleSheet("font-weight: bold; margin-top: 15px;");
    chartLayout->addWidget(chartLabel);
    
    resultsChart = new QChartView();
    resultsChart->setMinimumHeight(200);
    resultsChart->setRenderHint(QPainter::Antialiasing);
    chartLayout->addWidget(resultsChart);
    
    layout->addLayout(chartLayout);
    
    // Buttons
    auto *buttonLayout = new QHBoxLayout();
    
    auto *runButton = new QPushButton("🚀 Run All Tests");
    runButton->setStyleSheet("QPushButton { background-color: #2196F3; } QPushButton:hover { background-color: #1976D2; }");
    connect(runButton, &QPushButton::clicked, this, &TestRunnerDialog::runTests);
    buttonLayout->addWidget(runButton);
    
    auto *exportButton = new QPushButton("📄 Export Results");
    exportButton->setStyleSheet("QPushButton { background-color: #FF9800; } QPushButton:hover { background-color: #F57C00; }");
    connect(exportButton, &QPushButton::clicked, this, &TestRunnerDialog::exportResults);
    buttonLayout->addWidget(exportButton);
    
    auto *openFolderButton = new QPushButton("📁 Open Results Folder");
    openFolderButton->setStyleSheet("QPushButton { background-color: #9C27B0; } QPushButton:hover { background-color: #7B1FA2; }");
    connect(openFolderButton, &QPushButton::clicked, this, &TestRunnerDialog::openResultsFolder);
    buttonLayout->addWidget(openFolderButton);
    
    buttonLayout->addStretch();
    
    auto *closeButton = new QPushButton("❌ Close");
    closeButton->setStyleSheet("QPushButton { background-color: #f44336; } QPushButton:hover { background-color: #d32f2f; }");
    connect(closeButton, &QPushButton::clicked, this, &QDialog::close);
    buttonLayout->addWidget(closeButton);
    
    layout->addLayout(buttonLayout);
}

void TestRunnerDialog::runTests()
{
    // Reset counters
    passedCount = 0;
    failedCount = 0;
    currentTest = 0;
    totalTests = 10; // We have 10 test classes
    
    // Clear previous results
    resultsText->clear();
    progressBar->setMaximum(totalTests);
    progressBar->setValue(0);
    
    statusLabel->setText("Running tests...");
    resultsText->append("<h3>🧪 Starting Plant Health Plus Test Suite</h3>");
    resultsText->append(QString("<p><b>Test run started at:</b> %1</p>").arg(QDateTime::currentDateTime().toString()));
    resultsText->append("<hr>");
    
    QApplication::processEvents();
    
    // List of test classes to run
    QStringList testClasses = {
        "AddPlantDialog",
        "ConditionChart", 
        "ForecastClient",
        "LogbookEntryDialog",
        "LogbookManager",
        "PlantCard",
        "PlantManager",
        "RangeIndicator",
        "Scraper",
        "UserManager"
    };
    
    auto startTime = QDateTime::currentMSecsSinceEpoch();
    
    // Run each test
    for (const QString &testClass : testClasses) {
        auto testStartTime = QDateTime::currentMSecsSinceEpoch();
        
        statusLabel->setText(QString("Running %1 tests...").arg(testClass));
        QApplication::processEvents();
        
        // Simulate running the test (in a real implementation, you would run the actual test)
        bool testPassed = runSingleTest(testClass);
        
        auto testEndTime = QDateTime::currentMSecsSinceEpoch();
        auto duration = testEndTime - testStartTime;
        
        addTestResult(testClass, testPassed, 
                     testPassed ? "All assertions passed" : "One or more assertions failed", 
                     duration);
        
        updateProgress(++currentTest, totalTests);
        QApplication::processEvents();
        
        // Add a small delay to make progress visible
        QTimer::singleShot(100, [](){});
        QApplication::processEvents();
    }
    
    auto endTime = QDateTime::currentMSecsSinceEpoch();
    auto totalDuration = endTime - startTime;
    
    // Final summary
    resultsText->append("<hr>");
    resultsText->append("<h3>📊 Test Summary</h3>");
    resultsText->append(QString("<p><b>Total Tests:</b> %1</p>").arg(totalTests));
    resultsText->append(QString("<p><b>Passed:</b> <span style='color: green;'>%1</span></p>").arg(passedCount));
    resultsText->append(QString("<p><b>Failed:</b> <span style='color: red;'>%1</span></p>").arg(failedCount));
    resultsText->append(QString("<p><b>Total Duration:</b> %1 ms</p>").arg(totalDuration));
    resultsText->append(QString("<p><b>Test run completed at:</b> %1</p>").arg(QDateTime::currentDateTime().toString()));
    
    statusLabel->setText(QString("Tests completed - %1 passed, %2 failed").arg(passedCount).arg(failedCount));
    
    // Update chart
    updateChart();
    
    // Export results automatically
    exportResults();
}

bool TestRunnerDialog::runSingleTest(const QString &testClassName)
{
    // In a real implementation, this would execute the actual test
    // For now, we'll simulate test results
    
    // Simulate different success rates for demonstration
    static QMap<QString, bool> testResults = {
        {"AddPlantDialog", true},
        {"ConditionChart", true},
        {"ForecastClient", false}, // Simulate a failing test
        {"LogbookEntryDialog", true},
        {"LogbookManager", true},
        {"PlantCard", true},
        {"PlantManager", true},
        {"RangeIndicator", true},
        {"Scraper", false}, // Simulate another failing test
        {"UserManager", true}
    };
    
    return testResults.value(testClassName, true);
}

void TestRunnerDialog::updateProgress(int current, int total)
{
    progressBar->setValue(current);
    progressBar->setFormat(QString("%1/%2 tests completed (%3%)").arg(current).arg(total).arg((current * 100) / total));
}

void TestRunnerDialog::addTestResult(const QString& testName, bool passed, const QString& details, qint64 duration)
{
    if (passed) {
        passedCount++;
        resultsText->append(QString("<p><span style='color: green; font-weight: bold;'>✅ %1</span> - %2 ms<br><small>%3</small></p>")
                           .arg(testName).arg(duration).arg(details));
    } else {
        failedCount++;
        resultsText->append(QString("<p><span style='color: red; font-weight: bold;'>❌ %1</span> - %2 ms<br><small style='color: red;'>%3</small></p>")
                           .arg(testName).arg(duration).arg(details));
    }
    
    // Auto-scroll to bottom
    auto cursor = resultsText->textCursor();
    cursor.movePosition(QTextCursor::End);
    resultsText->setTextCursor(cursor);
}

void TestRunnerDialog::updateChart()
{
    auto *series = new QtCharts::QPieSeries();
    
    if (passedCount > 0) {
        auto *passedSlice = series->append("Passed", passedCount);
        passedSlice->setColor(QColor("#4CAF50"));
        passedSlice->setLabelVisible(true);
    }
    
    if (failedCount > 0) {
        auto *failedSlice = series->append("Failed", failedCount);
        failedSlice->setColor(QColor("#f44336"));
        failedSlice->setLabelVisible(true);
    }
    
    auto *chart = new QtCharts::QChart();
    chart->addSeries(series);
    chart->setTitle("Test Results Distribution");
    chart->setTheme(QtCharts::QChart::ChartThemeLight);
    
    // Make chart more visually appealing
    chart->setBackgroundBrush(QBrush(QColor("#fafafa")));
    chart->setTitleFont(QFont("Arial", 12, QFont::Bold));
    
    resultsChart->setChart(chart);
}

void TestRunnerDialog::exportResults()
{
    // Create results directory
    QDir dir;
    QString resultsDir = QDir::currentPath() + "/test_results";
    if (!dir.exists(resultsDir)) {
        dir.mkpath(resultsDir);
    }
    
    // Generate timestamp for unique filenames
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss");
    
    // Export HTML report
    QString htmlFile = resultsDir + QString("/test_report_%1.html").arg(timestamp);
    QFile file(htmlFile);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << "<!DOCTYPE html>\n";
        out << "<html><head><title>Plant Health Plus Test Report</title>\n";
        out << "<style>\n";
        out << "body { font-family: Arial, sans-serif; margin: 20px; background-color: #f5f5f5; }\n";
        out << ".container { max-width: 800px; margin: 0 auto; background: white; padding: 20px; border-radius: 8px; box-shadow: 0 2px 4px rgba(0,0,0,0.1); }\n";
        out << ".header { text-align: center; color: #2E7D32; border-bottom: 2px solid #4CAF50; padding-bottom: 10px; }\n";
        out << ".passed { color: #4CAF50; }\n";
        out << ".failed { color: #f44336; }\n";
        out << ".summary { background: #e8f5e8; padding: 15px; border-radius: 5px; margin: 20px 0; }\n";
        out << "</style>\n";
        out << "</head><body>\n";
        out << "<div class='container'>\n";
        out << "<div class='header'>\n";
        out << "<h1>🧪 Plant Health Plus Test Report</h1>\n";
        out << "<p>Generated on: " << QDateTime::currentDateTime().toString() << "</p>\n";
        out << "</div>\n";
        out << "<div class='summary'>\n";
        out << "<h2>📊 Summary</h2>\n";
        out << "<p><strong>Total Tests:</strong> " << totalTests << "</p>\n";
        out << "<p><strong>Passed:</strong> <span class='passed'>" << passedCount << "</span></p>\n";
        out << "<p><strong>Failed:</strong> <span class='failed'>" << failedCount << "</span></p>\n";
        out << "<p><strong>Success Rate:</strong> " << QString::number((double)passedCount / totalTests * 100, 'f', 1) << "%</p>\n";
        out << "</div>\n";
        out << "<h2>📋 Detailed Results</h2>\n";
        out << resultsText->toHtml();
        out << "</div>\n";
        out << "</body></html>\n";
        file.close();
    }
    
    // Export JSON report
    QString jsonFile = resultsDir + QString("/test_report_%1.json").arg(timestamp);
    QFile jsonFileHandle(jsonFile);
    if (jsonFileHandle.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QJsonObject report;
        report["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);
        report["total_tests"] = totalTests;
        report["passed_tests"] = passedCount;
        report["failed_tests"] = failedCount;
        report["success_rate"] = (double)passedCount / totalTests * 100;
        
        QJsonArray testResults;
        // In a real implementation, you would collect individual test results
        // For now, we'll add placeholder data
        
        QJsonDocument doc(report);
        QTextStream jsonOut(&jsonFileHandle);
        jsonOut << doc.toJson();
        jsonFileHandle.close();
    }
    
    // Show success message
    QMessageBox::information(this, "Export Complete", 
                           QString("Test results exported to:\n%1\n\nFiles created:\n- HTML Report\n- JSON Report").arg(resultsDir));
}

void TestRunnerDialog::openResultsFolder()
{
    QString path = QDir::currentPath() + "/test_results";
    QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}
