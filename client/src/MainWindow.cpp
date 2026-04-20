#include "MainWindow.hpp"
#include "WorkerWindow.hpp"
#include "EmployerWindow.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFont>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , apiClient_(new ApiClient("http://localhost:8080", this))
{
    setWindowTitle("Job Service");
    resize(900, 650);
    setupUi();
}

void MainWindow::setupUi() {
    stack_ = new QStackedWidget(this);
    setCentralWidget(stack_);

    rolePage_ = new QWidget;
    auto* vbox = new QVBoxLayout(rolePage_);
    vbox->setAlignment(Qt::AlignCenter);
    vbox->setSpacing(20);

    auto* title = new QLabel("Job Service");
    QFont titleFont;
    titleFont.setPointSize(28);
    titleFont.setBold(true);
    title->setFont(titleFont);
    title->setAlignment(Qt::AlignCenter);

    auto* subtitle = new QLabel("Выберите режим работы");
    subtitle->setAlignment(Qt::AlignCenter);
    QFont subFont;
    subFont.setPointSize(14);
    subtitle->setFont(subFont);

    auto* workerBtn   = new QPushButton("👷  Я — Работник");
    auto* employerBtn = new QPushButton("🏢  Я — Работодатель");

    for (auto* btn : {workerBtn, employerBtn}) {
        btn->setMinimumSize(260, 60);
        QFont f;
        f.setPointSize(13);
        btn->setFont(f);
    }

    vbox->addStretch();
    vbox->addWidget(title);
    vbox->addWidget(subtitle);
    vbox->addSpacing(30);
    vbox->addWidget(workerBtn,   0, Qt::AlignCenter);
    vbox->addWidget(employerBtn, 0, Qt::AlignCenter);
    vbox->addStretch();

    connect(workerBtn,   &QPushButton::clicked, this, &MainWindow::onWorkerSelected);
    connect(employerBtn, &QPushButton::clicked, this, &MainWindow::onEmployerSelected);

    workerWindow_   = new WorkerWindow(apiClient_);
    employerWindow_ = new EmployerWindow(apiClient_);

    connect(workerWindow_,   &WorkerWindow::backRequested,
            this, [this]() { stack_->setCurrentWidget(rolePage_); });
    connect(employerWindow_, &EmployerWindow::backRequested,
            this, [this]() { stack_->setCurrentWidget(rolePage_); });

    stack_->addWidget(rolePage_);
    stack_->addWidget(workerWindow_);
    stack_->addWidget(employerWindow_);
    stack_->setCurrentWidget(rolePage_);
}

void MainWindow::onWorkerSelected() {
    stack_->setCurrentWidget(workerWindow_);
}

void MainWindow::onEmployerSelected() {
    stack_->setCurrentWidget(employerWindow_);
}
