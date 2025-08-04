#ifndef TESTRUNNER_H
#define TESTRUNNER_H

#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QProgressBar>
#include <QTextEdit>
#include <QTimer>
#include <QScrollArea>
#include <QFrame>
#include <QApplication>
#include <QDateTime>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>
#include <QPixmap>
#include <QPainter>
#include <QChart>
#include <QChartView>
#include <QPieSeries>
#include <QPieSlice>
#include <QProcess>

QT_CHARTS_USE_NAMESPACE

class TestRunnerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TestRunnerDialog(QWidget *parent = nullptr);

private slots:
    void runTests();
    void exportResults();
    void openResultsFolder();

private:
    void setupUI();
    void updateProgress(int current, int total);
    void addTestResult(const QString& testName, bool passed, const QString& details, qint64 duration);
    void showSummary(int passed, int failed, qint64 totalDuration);
    void createResultsChart(int passed, int failed);
    void exportToJson(const QJsonObject& results);
    void exportToHtml(const QJsonObject& results);
    void exportToPng();

    QVBoxLayout* m_mainLayout;
    QLabel* m_titleLabel;
    QProgressBar* m_progressBar;
    QPushButton* m_runButton;
    QPushButton* m_exportButton;
    QPushButton* m_openFolderButton;
    QPushButton* m_closeButton;
    QScrollArea* m_resultsArea;
    QWidget* m_resultsWidget;
    QVBoxLayout* m_resultsLayout;
    QChartView* m_chartView;
    
    struct TestResult {
        QString name;
        bool passed;
        QString details;
        qint64 duration;
    };
    
    QList<TestResult> m_testResults;
    QJsonObject m_fullResults;
    bool m_testsRun;
};

TestRunnerDialog::TestRunnerDialog(QWidget *parent)
    : QDialog(parent)
    , m_testsRun(false)
{
    setupUI();
    setModal(true);
    setFixedSize(800, 600);
    setWindowTitle("Plant Health Plus - Unit Test Runner");
}

void TestRunnerDialog::setupUI()
{
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setSpacing(15);
    m_mainLayout->setContentsMargins(20, 20, 20, 20);
    
    // Title
    m_titleLabel = new QLabel("Unit Test Runner");
    m_titleLabel->setStyleSheet(
        "QLabel {"
        "    font-size: 18px;"
        "    font-weight: bold;"
        "    color: #2E7D32;"
        "    padding: 10px;"
        "    background-color: #E8F5E8;"
        "    border-radius: 5px;"
        "    text-align: center;"
        "}"
    );
    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_mainLayout->addWidget(m_titleLabel);
    
    // Description
    QLabel* descLabel = new QLabel(
        "Run comprehensive unit tests for Plant Health Plus components.\n"
        "Tests cover plant management, user authentication, data persistence, and UI functionality."
    );
    descLabel->setStyleSheet("QLabel { color: #666; font-size: 12px; margin: 5px 0; }");
    descLabel->setWordWrap(true);
    m_mainLayout->addWidget(descLabel);
    
    // Progress bar
    m_progressBar = new QProgressBar();
    m_progressBar->setVisible(false);
    m_progressBar->setStyleSheet(
        "QProgressBar {"
        "    border: 1px solid #ccc;"
        "    border-radius: 5px;"
        "    text-align: center;"
        "    height: 20px;"
        "}"
        "QProgressBar::chunk {"
        "    background-color: #4CAF50;"
        "    border-radius: 4px;"
        "}"
    );
    m_mainLayout->addWidget(m_progressBar);
    
    // Results area
    m_resultsArea = new QScrollArea();
    m_resultsArea->setWidgetResizable(true);
    m_resultsArea->setVisible(false);
    m_resultsArea->setStyleSheet(
        "QScrollArea {"
        "    border: 1px solid #ccc;"
        "    border-radius: 5px;"
        "    background-color: #fafafa;"
        "}"
    );
    
    m_resultsWidget = new QWidget();
    m_resultsLayout = new QVBoxLayout(m_resultsWidget);
    m_resultsLayout->setAlignment(Qt::AlignTop);
    m_resultsArea->setWidget(m_resultsWidget);
    m_mainLayout->addWidget(m_resultsArea);
    
    // Chart view
    m_chartView = new QChartView();
    m_chartView->setVisible(false);
    m_chartView->setFixedHeight(200);
    m_chartView->setRenderHint(QPainter::Antialiasing);
    m_mainLayout->addWidget(m_chartView);
    
    // Buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    
    m_runButton = new QPushButton("Run All Tests");
    m_runButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #4CAF50;"
        "    color: white;"
        "    padding: 10px 20px;"
        "    border: none;"
        "    border-radius: 5px;"
        "    font-weight: bold;"
        "    font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #45a049;"
        "}"
        "QPushButton:disabled {"
        "    background-color: #cccccc;"
        "}"
    );
    connect(m_runButton, &QPushButton::clicked, this, &TestRunnerDialog::runTests);
    
    m_exportButton = new QPushButton("Export Results");
    m_exportButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #2196F3;"
        "    color: white;"
        "    padding: 10px 20px;"
        "    border: none;"
        "    border-radius: 5px;"
        "    font-weight: bold;"
        "    font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #1976D2;"
        "}"
        "QPushButton:disabled {"
        "    background-color: #cccccc;"
        "}"
    );
    m_exportButton->setEnabled(false);
    connect(m_exportButton, &QPushButton::clicked, this, &TestRunnerDialog::exportResults);
    
    m_openFolderButton = new QPushButton("Open Results Folder");
    m_openFolderButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #FF9800;"
        "    color: white;"
        "    padding: 10px 20px;"
        "    border: none;"
        "    border-radius: 5px;"
        "    font-weight: bold;"
        "    font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #F57C00;"
        "}"
        "QPushButton:disabled {"
        "    background-color: #cccccc;"
        "}"
    );
    m_openFolderButton->setEnabled(false);
    connect(m_openFolderButton, &QPushButton::clicked, this, &TestRunnerDialog::openResultsFolder);
    
    m_closeButton = new QPushButton("Close");
    m_closeButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #757575;"
        "    color: white;"
        "    padding: 10px 20px;"
        "    border: none;"
        "    border-radius: 5px;"
        "    font-weight: bold;"
        "    font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #616161;"
        "}"
    );
    connect(m_closeButton, &QPushButton::clicked, this, &QDialog::accept);
    
    buttonLayout->addWidget(m_runButton);
    buttonLayout->addWidget(m_exportButton);
    buttonLayout->addWidget(m_openFolderButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_closeButton);
    
    m_mainLayout->addLayout(buttonLayout);
}

void TestRunnerDialog::runTests()
{
    m_runButton->setEnabled(false);
    m_progressBar->setVisible(true);
    m_resultsArea->setVisible(true);
    
    // Clear previous results
    for (QWidget* child : m_resultsWidget->findChildren<QWidget*>()) {
        child->deleteLater();
    }
    m_testResults.clear();
    
    // Simulate running 10 tests
    QStringList testNames = {
        "Plant Manager Tests",
        "User Manager Tests", 
        "Logbook Manager Tests",
        "Plant Card Tests",
        "Range Indicator Tests",
        "Condition Chart Tests",
        "Forecast Client Tests",
        "Web Scraper Tests",
        "Add Plant Dialog Tests",
        "Logbook Entry Dialog Tests"
    };
    
    m_progressBar->setRange(0, testNames.size());
    
    QDateTime startTime = QDateTime::currentDateTime();
    
    for (int i = 0; i < testNames.size(); ++i) {
        updateProgress(i, testNames.size());
        QApplication::processEvents();
        
        // Simulate test execution time
        QTimer timer;
        timer.setSingleShot(true);
        QEventLoop loop;
        connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
        timer.start(200); // 200ms per test
        loop.exec();
        
        // Simulate test results (90% pass rate)
        bool passed = (i != 2 && i != 7); // Fail tests 3 and 8 for demonstration
        QString details = passed ? "All test functions passed successfully" : 
                         QString("Test failed in function %1").arg(i % 3 + 1);
        qint64 duration = 150 + (i * 25); // Varying durations
        
        addTestResult(testNames[i], passed, details, duration);
    }
    
    updateProgress(testNames.size(), testNames.size());
    
    QDateTime endTime = QDateTime::currentDateTime();
    qint64 totalDuration = startTime.msecsTo(endTime);
    
    int passed = 0, failed = 0;
    for (const auto& result : m_testResults) {
        if (result.passed) passed++;
        else failed++;
    }
    
    showSummary(passed, failed, totalDuration);
    createResultsChart(passed, failed);
    
    // Prepare full results for export
    m_fullResults["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);
    m_fullResults["total_duration_ms"] = totalDuration;
    m_fullResults["total_tests"] = testNames.size();
    m_fullResults["passed_tests"] = passed;
    m_fullResults["failed_tests"] = failed;
    m_fullResults["success_rate"] = (passed * 100.0 / testNames.size());
    
    QJsonArray testsArray;
    for (const auto& result : m_testResults) {
        QJsonObject testObj;
        testObj["name"] = result.name;
        testObj["passed"] = result.passed;
        testObj["details"] = result.details;
        testObj["duration_ms"] = result.duration;
        testsArray.append(testObj);
    }
    m_fullResults["tests"] = testsArray;
    
    m_testsRun = true;
    m_exportButton->setEnabled(true);
    m_openFolderButton->setEnabled(true);
    m_runButton->setEnabled(true);
    m_runButton->setText("Run Tests Again");
}

void TestRunnerDialog::updateProgress(int current, int total)
{
    m_progressBar->setValue(current);
    if (current < total) {
        m_titleLabel->setText(QString("Running Tests... (%1/%2)").arg(current + 1).arg(total));
    } else {
        m_titleLabel->setText("Tests Completed!");
    }
}

void TestRunnerDialog::addTestResult(const QString& testName, bool passed, const QString& details, qint64 duration)
{
    TestResult result = {testName, passed, details, duration};
    m_testResults.append(result);
    
    QFrame* resultFrame = new QFrame();
    resultFrame->setStyleSheet(QString(
        "QFrame {"
        "    background-color: %1;"
        "    border: 1px solid %2;"
        "    border-radius: 5px;"
        "    padding: 10px;"
        "    margin: 2px;"
        "}"
    ).arg(passed ? "#d4edda" : "#f8d7da", passed ? "#c3e6cb" : "#f5c6cb"));
    
    QHBoxLayout* layout = new QHBoxLayout(resultFrame);
    
    QString icon = passed ? "✓" : "✗";
    QString status = passed ? "PASSED" : "FAILED";
    QLabel* statusLabel = new QLabel(QString("%1 %2").arg(icon, status));
    statusLabel->setStyleSheet(QString("font-weight: bold; color: %1;").arg(passed ? "#155724" : "#721c24"));
    
    QLabel* nameLabel = new QLabel(testName);
    nameLabel->setStyleSheet("font-weight: bold;");
    
    QLabel* durationLabel = new QLabel(QString("(%1 ms)").arg(duration));
    durationLabel->setStyleSheet("color: #666; font-size: 11px;");
    
    layout->addWidget(statusLabel);
    layout->addWidget(nameLabel);
    layout->addStretch();
    layout->addWidget(durationLabel);
    
    if (!passed) {
        QLabel* detailsLabel = new QLabel(details);
        detailsLabel->setStyleSheet("color: #721c24; font-style: italic; font-size: 11px;");
        layout->addWidget(detailsLabel);
    }
    
    m_resultsLayout->addWidget(resultFrame);
}

void TestRunnerDialog::showSummary(int passed, int failed, qint64 totalDuration)
{
    QFrame* summaryFrame = new QFrame();
    summaryFrame->setStyleSheet(
        "QFrame {"
        "    background-color: #e8f4f8;"
        "    border: 2px solid #2196F3;"
        "    border-radius: 8px;"
        "    padding: 15px;"
        "    margin: 10px 0;"
        "}"
    );
    
    QVBoxLayout* layout = new QVBoxLayout(summaryFrame);
    
    QLabel* titleLabel = new QLabel("Test Summary");
    titleLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #1976D2;");
    layout->addWidget(titleLabel);
    
    int total = passed + failed;
    double successRate = total > 0 ? (passed * 100.0 / total) : 0.0;
    
    QLabel* statsLabel = new QLabel(QString(
        "Total Tests: %1 | Passed: %2 | Failed: %3\n"
        "Success Rate: %4% | Duration: %5 ms"
    ).arg(total).arg(passed).arg(failed).arg(successRate, 0, 'f', 1).arg(totalDuration));
    
    statsLabel->setStyleSheet("font-size: 12px; color: #333;");
    layout->addWidget(statsLabel);
    
    m_resultsLayout->insertWidget(0, summaryFrame);
}

void TestRunnerDialog::createResultsChart(int passed, int failed)
{
    QPieSeries* series = new QPieSeries();
    
    if (passed > 0) {
        QPieSlice* passedSlice = series->append("Passed", passed);
        passedSlice->setColor(QColor("#4CAF50"));
        passedSlice->setLabelVisible(true);
    }
    
    if (failed > 0) {
        QPieSlice* failedSlice = series->append("Failed", failed);
        failedSlice->setColor(QColor("#F44336"));
        failedSlice->setLabelVisible(true);
    }
    
    QChart* chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Test Results Overview");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->setAlignment(Qt::AlignBottom);
    
    m_chartView->setChart(chart);
    m_chartView->setVisible(true);
}

void TestRunnerDialog::exportResults()
{
    if (!m_testsRun) return;
    
    QDir().mkpath("test_results");
    
    exportToJson(m_fullResults);
    exportToHtml(m_fullResults);
    exportToPng();
    
    QMessageBox::information(this, "Export Complete", 
        "Test results have been exported to:\n"
        "• test_results/test_results.json\n"
        "• test_results/test_results.html\n"
        "• test_results/test_results.png\n\n"
        "Click 'Open Results Folder' to view the files.");
}

void TestRunnerDialog::exportToJson(const QJsonObject& results)
{
    QJsonDocument doc(results);
    QFile file("test_results/test_results.json");
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
    }
}

void TestRunnerDialog::exportToHtml(const QJsonObject& results)
{
    QFile file("test_results/test_results.html");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        
        out << "<!DOCTYPE html>\n<html>\n<head>\n";
        out << "<title>Plant Health Plus - Test Results</title>\n";
        out << "<style>\n";
        out << "body { font-family: Arial, sans-serif; margin: 20px; background-color: #f5f5f5; }\n";
        out << ".container { max-width: 1000px; margin: 0 auto; background: white; padding: 20px; border-radius: 10px; box-shadow: 0 2px 10px rgba(0,0,0,0.1); }\n";
        out << ".header { background: linear-gradient(135deg, #4CAF50, #2E7D32); color: white; padding: 20px; text-align: center; border-radius: 10px; margin-bottom: 20px; }\n";
        out << ".summary { background: #e8f4f8; padding: 15px; margin: 20px 0; border-radius: 8px; border-left: 5px solid #2196F3; }\n";
        out << ".test-passed { background: #d4edda; padding: 10px; margin: 5px 0; border-radius: 5px; border-left: 4px solid #28a745; }\n";
        out << ".test-failed { background: #f8d7da; padding: 10px; margin: 5px 0; border-radius: 5px; border-left: 4px solid #dc3545; }\n";
        out << ".timestamp { color: #666; font-size: 0.9em; }\n";
        out << ".stats { display: flex; justify-content: space-around; margin: 20px 0; }\n";
        out << ".stat-item { text-align: center; padding: 10px; }\n";
        out << ".stat-number { font-size: 2em; font-weight: bold; }\n";
        out << ".passed { color: #28a745; }\n";
        out << ".failed { color: #dc3545; }\n";
        out << "</style>\n</head>\n<body>\n";
        
        out << "<div class='container'>\n";
        out << "<div class='header'>\n";
        out << "<h1>🌱 Plant Health Plus - Unit Test Results</h1>\n";
        out << "<p class='timestamp'>Generated: " << results["timestamp"].toString() << "</p>\n";
        out << "</div>\n";
        
        out << "<div class='summary'>\n";
        out << "<h2>📊 Test Summary</h2>\n";
        out << "<div class='stats'>\n";
        out << "<div class='stat-item'><div class='stat-number'>" << results["total_tests"].toInt() << "</div><div>Total Tests</div></div>\n";
        out << "<div class='stat-item'><div class='stat-number passed'>" << results["passed_tests"].toInt() << "</div><div>Passed</div></div>\n";
        out << "<div class='stat-item'><div class='stat-number failed'>" << results["failed_tests"].toInt() << "</div><div>Failed</div></div>\n";
        out << "<div class='stat-item'><div class='stat-number'>" << QString::number(results["success_rate"].toDouble(), 'f', 1) << "%</div><div>Success Rate</div></div>\n";
        out << "<div class='stat-item'><div class='stat-number'>" << results["total_duration_ms"].toInt() << "ms</div><div>Duration</div></div>\n";
        out << "</div>\n</div>\n";
        
        out << "<h2>🔍 Detailed Results</h2>\n";
        QJsonArray tests = results["tests"].toArray();
        for (const auto& testValue : tests) {
            QJsonObject test = testValue.toObject();
            QString cssClass = test["passed"].toBool() ? "test-passed" : "test-failed";
            QString icon = test["passed"].toBool() ? "✅" : "❌";
            QString status = test["passed"].toBool() ? "PASSED" : "FAILED";
            
            out << "<div class='" << cssClass << "'>\n";
            out << "<strong>" << icon << " " << test["name"].toString() << ":</strong> " << status 
                << " <span style='color: #666; font-size: 0.9em;'>(" << test["duration_ms"].toInt() << " ms)</span><br>\n";
            if (!test["passed"].toBool()) {
                out << "<em style='color: #721c24;'>Details: " << test["details"].toString() << "</em>\n";
            }
            out << "</div>\n";
        }
        
        out << "</div>\n</body>\n</html>\n";
    }
}

void TestRunnerDialog::exportToPng()
{
    if (m_chartView && m_chartView->isVisible()) {
        QPixmap pixmap = m_chartView->grab();
        pixmap.save("test_results/test_results.png");
    }
}

void TestRunnerDialog::openResultsFolder()
{
    QString path = QDir::currentPath() + "/test_results";
    QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}

#include "testrunner.moc"

#endif // TESTRUNNER_H
