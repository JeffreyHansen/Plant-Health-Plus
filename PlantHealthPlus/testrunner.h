#ifndef TESTRUNNER_H
#define TESTRUNNER_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QProgressBar>
#include <QTextEdit>
#include <QDateTime>

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
    void updateChart();
    bool runSingleTest(const QString &testClassName);

    // UI components
    QProgressBar* progressBar;
    QTextEdit* resultsText;
    QLabel* statusLabel;
    QWidget* resultsChart; // Placeholder for future chart implementation
    
    // Test tracking
    int passedCount;
    int failedCount;
    int totalTests;
    int currentTest;
};

#endif // TESTRUNNER_H
