#pragma once

#include <QMainWindow>
#include <QStackedWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "ApiClient.hpp"

class WorkerWindow;
class EmployerWindow;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void onWorkerSelected();
    void onEmployerSelected();

private:
    void setupUi();

    QStackedWidget* stack_;
    QWidget*        rolePage_;
    WorkerWindow*   workerWindow_;
    EmployerWindow* employerWindow_;
    ApiClient*      apiClient_;
};
